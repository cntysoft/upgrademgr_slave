#include "deploy_system_runtime.h"

#include <QProcess>
#include <QThread>
#include <signal.h>

#include "corelib/io/filesystem.h"
#include "umslib/global/const.h"

namespace umsservice{
namespace serverstatus{

using sn::corelib::Filesystem;

DeploySystemLuoXiServerRuntimeWrapper::DeploySystemLuoXiServerRuntimeWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse DeploySystemLuoXiServerRuntimeWrapper::startServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemLuoXiServerRuntime/startServer", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   int pid = getLuoXiServerPid();
   if(-1 != pid){
      response.setDataItem("msg", "服务器正在运行");
      return response;
   }
   if(!QProcess::startDetached(LUOXI_SBIN_NAME, {"start"})){
      response.setDataItem("msg", "启动服务器失败, 请重新尝试");
      return response;
   }
   response.setDataItem("msg", "启动服务器成功");
   return response;
}

ServiceInvokeResponse DeploySystemLuoXiServerRuntimeWrapper::stopServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemLuoXiServerRuntime/stopServer", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   int pid = getLuoXiServerPid();
   if(-1 == pid){
      response.setDataItem("msg", "服务器已经停止");
      return response;
   }
   kill(pid, SIGINT);
   response.setDataItem("msg", "服务器关闭成功");
   return response;
}

ServiceInvokeResponse DeploySystemLuoXiServerRuntimeWrapper::restartServer(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/DeploySystemLuoXiServerRuntime/startServer", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   int pid = getLuoXiServerPid();
   if(-1 != pid){
      kill(pid, SIGINT);
   }
   QThread::msleep(500);
   if(Filesystem::fileExist(m_pidFilename)){
      Filesystem::deleteFile(m_pidFilename);
   }
   if(!QProcess::startDetached(LUOXI_SBIN_NAME, {"start"})){
      response.setDataItem("msg", "启动服务器失败, 请重新尝试");
      return response;
   }
   response.setDataItem("msg", "重新启动服务器成功");
   return response;
}

int DeploySystemLuoXiServerRuntimeWrapper::getLuoXiServerPid()
{
   if(m_pidFilename.isEmpty()){
      QProcess process;
      QStringList args;
      args << "pidfilename";
      process.start(LUOXI_SBIN_NAME, args);
      process.waitForFinished(-1);
      if(0 != process.exitCode()){
         return -1;
      }
      m_pidFilename = process.readAllStandardOutput();
   }
   if(!Filesystem::fileExist(m_pidFilename)){
      return -1; 
   }
   return Filesystem::fileGetContents(m_pidFilename).toInt();
}

}//serverstatus
}//umsservice
