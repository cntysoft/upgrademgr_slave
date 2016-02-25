#include "upgrade_cloudcontroller.h"

#include "corelib/kernel/settings.h"
#include "corelib/kernel/application.h"
#include "umslib/global/const.h"
#include "umslib/kernel/stddir.h"
#include "io/filesystem.h"
#include "corelib/global/common_funcs.h"
#include "corelib/kernel/errorinfo.h"

#include <QDebug>

namespace umsservice{
namespace upgrader{

using sn::corelib::Settings;
using sn::corelib::Application;
using umslib::kernel::StdDir;
using sn::corelib::Filesystem;
using sn::corelib::throw_exception;
using sn::corelib::ErrorInfo;

const QString UpgradeCloudControllerWrapper::CC_UPGRADE_PKG_NAME_TPL = "cloudcontrollerweb_patch_%1_%2.tar.gz";

UpgradeCloudControllerWrapper::UpgradeCloudControllerWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
   Settings& settings = Application::instance()->getSettings();
   m_deployDir = settings.getValue("cloudControllerWebRootDir", UMS_CFG_GROUP_GLOBAL, "/srv/www/cloudcontroller").toString();
}

ServiceInvokeResponse UpgradeCloudControllerWrapper::init(const ServiceInvokeRequest &request)
{
   if(m_step != STEP_PREPARE){
      throw_exception(ErrorInfo("状态错误"), getErrorContext());
   }
   m_context.reset(new UpgradeContext);
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"fromVersion", "toVersion"});
   QString softwareRepoDir = StdDir::getSoftwareRepoDir();
   m_context->fromVersion = args.value("fromVersion").toString();
   m_context->toVersion = args.value("toVersion").toString();
   QString baseFilename = QString(CC_UPGRADE_PKG_NAME_TPL).arg(m_context->fromVersion, m_context->toVersion);
   QString upgradePkgFilename = softwareRepoDir + '/' + baseFilename;
   m_context->pkgFilename = upgradePkgFilename;
   m_context->request = request;
   ServiceInvokeResponse response("Upgrade/UpgradeCloudController/init", true);
   response.setSerial(request.getSerial());
   m_context->response = response;
//   if(!Filesystem::fileExist(upgradePkgFilename)){
//      //下载升级文件到本地
//      downloadUpgradePkg(baseFilename);
//   }  
   downloadUpgradePkg(baseFilename);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   return response;
}

void UpgradeCloudControllerWrapper::downloadUpgradePkg(const QString &filename)
{
   //获取相关的配置信息
   Settings& settings = Application::instance()->getSettings();
   QSharedPointer<DownloadClient> downloader = getDownloadClient(settings.getValue("upgrademgrMasterHost").toString(), 
                                                                 settings.getValue("upgrademgrMasterPort").toInt());
   connect(downloader.data(), &DownloadClient::beginDownload, this, [&](){
      m_context->response.setDataItem("step", STEP_DOWNLOAD_PKG);
      m_context->response.setDataItem("msg", "开始下载软件包");
      writeInterResponse(m_context->request, m_context->response);
   });
   QObject::connect(downloader.data(), &DownloadClient::downloadError, this, [&](int errorCode, const QString &errorMsg){
      m_context->response.setDataItem("step", STEP_DOWNLOAD_PKG);
      m_context->response.setStatus(false);
      m_context->response.setError({errorCode, errorMsg});
      writeInterResponse(m_context->request, m_context->response);
      m_eventLoop.exit();
      m_isInAction = false;
      m_step = STEP_PREPARE;
   });
   connect(downloader.data(), &DownloadClient::downloadComplete, this, [&](){
      
   });
   downloader->download(filename);
   m_eventLoop.exec();
}

void UpgradeCloudControllerWrapper::clearState()
{
   m_context.clear();
}

QSharedPointer<DownloadClient> UpgradeCloudControllerWrapper::getDownloadClient(const QString &host, quint16 port)
{
   if(m_downloadClient.isNull()){
      m_downloadClient.reset(new DownloadClient(getServiceInvoker(host, port)));
   }
   return m_downloadClient;
}

}//upgrader
}//ummservice
