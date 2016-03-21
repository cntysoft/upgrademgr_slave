#include "upgrade_luoxi.h"

#include <QProcess>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>
#include <signal.h>

#include "upgrade_upgrademgr_slave.h"

#include "umslib/network/multi_thread_server.h"
#include "umslib/kernel/stddir.h"
#include "umslib/global/common_funcs.h"
#include "umslib/global/const.h"

#include "corelib/io/filesystem.h"
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

const QString UpgradeLuoXiWrapper::RPM_FILENAME_TPL = "luoxi-%1-1.x86_64.rpm";

UpgradeLuoXiWrapper::UpgradeLuoXiWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_softwareRepoDir(StdDir::getSoftwareRepoDir())
{
}

ServiceInvokeResponse UpgradeLuoXiWrapper::upgrade(const ServiceInvokeRequest &request)
{
   m_context.reset(new UpgradeContext);
   m_context->upgradeStatus = true;
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"targetVersion"});
   m_context->targetVersion = args.value("targetVersion").toString();
   QString targetVersionStr = m_context->targetVersion;
   m_context->request = request;
   ServiceInvokeResponse response("Upgrader/UpgradeLuoXi/upgrade", true);
   response.setSerial(request.getSerial());
   response.setDataItem("msg", "开始准备更新");
   writeInterResponse(request, response);
   response.setDataItem("msg", "开始检查指定版本的服务器RPM包");
   m_context->response = response;
   QString filename = m_softwareRepoDir + "/" +QString(RPM_FILENAME_TPL).arg(targetVersionStr);
   m_context->pkgFilename = filename;
   if(!Filesystem::fileExist(filename)){
      m_step = STEP_DOWNLOAD_PKG;
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
   }
   m_step = STEP_INSTALL_RPM;
   response.setDataItem("step", STEP_INSTALL_RPM);
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
   response.setDataItem("msg", "更新成功,正在重启服务器");
   writeInterResponse(request, response);
   int pid = getLuoXiPid();
   if(pid != -1){
      kill(pid, SIGINT);
   }
   if(!QProcess::startDetached(LUOXI_SBIN_NAME, {"start"})){
      response.setStatus(false);
      response.setError({1, "重启服务器失败, 升级失败"});
      return response;
   }
   response.setDataItem("step", STEP_FINISH);
   response.setDataItem("msg", "升级成功");
   clearState();
   return response;
}

void UpgradeLuoXiWrapper::downloadUpgradePkg(const QString &filename)
{
   //获取相关的配置信息
   Settings& settings = Application::instance()->getSettings();
   QSharedPointer<DownloadClientWrapper> downloader = getDownloadClient(settings.getValue("upgrademgrMasterHost").toString(), 
                                                                 settings.getValue("upgrademgrMasterPort").toInt());
   downloader->download(filename);
   m_eventLoop.exec();
}

void UpgradeLuoXiWrapper::clearState()
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


bool UpgradeLuoXiWrapper::installRpmPackage(const QString &filename, QString &errorString)
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

QSharedPointer<DownloadClientWrapper> UpgradeLuoXiWrapper::getDownloadClient(const QString &host, quint16 port)
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

int UpgradeLuoXiWrapper::getLuoXiPid()
{
   QProcess process;
   QStringList args;
   args << "pidfilename";
   process.start(LUOXI_SBIN_NAME, args);
   process.waitForFinished(-1);
   if(0 != process.exitCode()){
      return -1;
   }
   QString pidFilename = process.readAllStandardOutput();
   if(!Filesystem::fileExist(pidFilename)){
      return -1;
   }
   return Filesystem::fileGetContents(pidFilename).toInt();
}

void UpgradeLuoXiWrapper::notifySocketDisconnect(QTcpSocket*)
{
   clearState();
}

}//upgrader
}//umsservice
