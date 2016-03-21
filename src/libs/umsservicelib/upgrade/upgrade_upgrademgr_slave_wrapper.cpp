#include "upgrade_upgrademgr_slave.h"

#include <QProcess>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "upgrade_upgrademgr_slave.h"

#include "corelib/io/filesystem.h"

#include "umslib/network/multi_thread_server.h"
#include "umslib/kernel/stddir.h"
#include "umslib/global/common_funcs.h"

#include "corelib/kernel/application.h"
#include "corelib/utils/version.h"
#include "corelib/kernel/settings.h"

namespace umsservice{
namespace upgrader{

using umslib::kernel::StdDir;

using sn::corelib::Filesystem;
using sn::corelib::utils::Version;
using sn::corelib::Application;
using sn::corelib::Settings;

const QString UpgradeUpgradeMgrSlaveWrapper::RPM_FILENAME_TPL = "upgrademgr_slave-%1-1.x86_64.rpm";

UpgradeUpgradeMgrSlaveWrapper::UpgradeUpgradeMgrSlaveWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_softwareRepoDir(StdDir::getSoftwareRepoDir())
{
}

ServiceInvokeResponse UpgradeUpgradeMgrSlaveWrapper::upgrade(const ServiceInvokeRequest &request)
{
   m_context.reset(new UpgradeContext);
   m_context->upgradeStatus = true;
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"targetVersion"});
   ServiceInvokeResponse response("Upgrader/UpgradeUpgradeMgrSlave/upgrade", true);
   response.setSerial(request.getSerial());
   response.setDataItem("msg", "开始准备更新");
   writeInterResponse(request, response);
   response.setDataItem("msg", "比较当前服务器的版本");
   writeInterResponse(request, response);
   QString umsVerStr = umslib::global::get_upgrademgr_slave_version();
   response.setDataItem("msg", QString("当前版本号 %1").arg(umsVerStr));
   writeInterResponse(request, response);
   QString targetVersionStr = args.value("targetVersion").toString();
   Version targetVersion(targetVersionStr);
   Version currentVersion(umsVerStr);
   if(targetVersion <= currentVersion){
      response.setStatus(false);
      response.setError({-1, "目标版本比当前版本低，无须更新"});
      return response;
   }
   response.setDataItem("msg", "开始检查指定版本的服务器RPM包");
   m_context->response = response;
   QString filename = m_softwareRepoDir + "/" +QString(RPM_FILENAME_TPL).arg(targetVersionStr);
   m_context->pkgFilename = filename;
   m_step = STEP_DOWNLOAD_PKG;
   response.setDataItem("msg", "开始下载软件包");
   writeInterResponse(request, response);
   downloadUpgradePkg(QString(RPM_FILENAME_TPL).arg(targetVersionStr));
   if(!m_context->upgradeStatus){
      response.setDataItem("step", STEP_DOWNLOAD_PKG);
      response.setDataItem("msg", m_context->upgradeErrorString);
      writeInterResponse(request, response);
      response.setStatus(false);
      response.setDataItem("step", STEP_ERROR);
      response.setError({-1, "升级失败"});
      clearState();
      return response;
   }
   m_step = STEP_INSTALL_RPM;
   response.setDataItem("msg", "开始安装RPM包");
   writeInterResponse(request, response);
   QString errorString;
   int status = installRpmPackage(filename, errorString);
   if(!status){
      response.setDataItem("step", STEP_INSTALL_RPM);
      response.setStatus(false);
      response.setError({1, errorString});
      return response;
   }
   response.setDataItem("msg", "更新成功,请手动重新启动服务器");
   writeInterResponse(request, response);
   m_serviceProvider.disconnectUnderlineSockets();
   umslib::network::MultiThreadServer *& server = umslib::network::get_global_server();
   server->close();
   Application::instance()->exit(1);
   response.setDataItem("step", STEP_FINISH);
   response.setDataItem("msg", "升级成功");
   clearState();
   return response;
}

void UpgradeUpgradeMgrSlaveWrapper::downloadUpgradePkg(const QString &filename)
{
   //获取相关的配置信息
   Settings& settings = Application::instance()->getSettings();
   QSharedPointer<DownloadClientWrapper> downloader = getDownloadClient(settings.getValue("upgrademgrMasterHost").toString(), 
                                                                 settings.getValue("upgrademgrMasterPort").toInt());
   downloader->download(filename);
   m_eventLoop.exec();
}

QSharedPointer<DownloadClientWrapper> UpgradeUpgradeMgrSlaveWrapper::getDownloadClient(const QString &host, quint16 port)
{
   if(m_downloadClient.isNull()){
      m_downloadClient.reset(new DownloadClientWrapper(getServiceInvoker(host, port)));
      connect(m_downloadClient.data(), &DownloadClientWrapper::beginDownload, this, [&](){
         m_context->response.setDataItem("step", STEP_DOWNLOAD_PKG);
         m_context->response.setDataItem("msg", "开始下载软件包");
         writeInterResponse(m_context->request, m_context->response);
      });
      QObject::connect(m_downloadClient.data(), &DownloadClientWrapper::downloadError, this, [&](int, const QString &errorMsg){
         m_eventLoop.exit();
         m_isInAction = false;
         m_step = STEP_PREPARE;
         m_context->upgradeStatus = false;
         m_context->upgradeErrorString = errorMsg;
      });
      connect(m_downloadClient.data(), &DownloadClientWrapper::downloadComplete, this, [&](){
         m_context->response.setDataItem("step", STEP_DOWNLOAD_COMPLETE);
         m_context->response.setStatus(true);
         m_context->response.setDataItem("msg", "下载软件包完成");
         writeInterResponse(m_context->request, m_context->response);
         m_eventLoop.exit();
         m_step = STEP_DOWNLOAD_COMPLETE;
      });
   }
   return m_downloadClient;
}


bool UpgradeUpgradeMgrSlaveWrapper::installRpmPackage(const QString &filename, QString &errorString)
{
   QProcess process;
   QStringList args;
   args << "-i" << "--force" << "--nodeps" << filename;
   process.start("rpm", args);
   process.waitForFinished();
   if(0 != process.exitCode()){
      errorString = process.readAllStandardError();
      errorString.remove('\n');
      return false;
   }
   return true;
}

void UpgradeUpgradeMgrSlaveWrapper::clearState()
{
   m_context.clear();
   m_step = STEP_PREPARE;
   if(!m_downloadClient.isNull()){
      m_downloadClient->clearState();
   }
   //清除残余文件
   if(!m_serviceInvoker.isNull()){
      m_serviceInvoker->disconnectFromServer();
   }
}

void UpgradeUpgradeMgrSlaveWrapper::notifySocketDisconnect(QTcpSocket*)
{
   clearState();
}


}//upgrader
}//umsservice
