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

namespace umsservice{
namespace upgrader{

using umslib::kernel::StdDir;

using sn::corelib::Filesystem;
using sn::corelib::utils::Version;
using sn::corelib::Application;

const QString UpgradeUpgradeMgrSlaveWrapper::RPM_FILENAME_TPL = "upgrademgr_slave-%1-1.x86_64.rpm";

UpgradeUpgradeMgrSlaveWrapper::UpgradeUpgradeMgrSlaveWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_softwareRepoDir(StdDir::getSoftwareRepoDir())
{
}

ServiceInvokeResponse UpgradeUpgradeMgrSlaveWrapper::upgrade(const ServiceInvokeRequest &request)
{
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
   QString filename = m_softwareRepoDir + "/" +QString(RPM_FILENAME_TPL).arg(targetVersionStr);
   if(!Filesystem::fileExist(filename)){
      response.setError({1, QString("软件包:%1不存在").arg(filename)});
      response.setStatus(false);
      return response;
   }
   response.setDataItem("msg", "开始安装RPM包");
   writeInterResponse(request, response);
   QString errorString;
   int status = installRpmPackage(filename, errorString);
   if(!status){
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
   return response;
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

}//upgrader
}//umsservice
