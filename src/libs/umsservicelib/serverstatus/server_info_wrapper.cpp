#include <QMap>
#include <QString>
#include <QVariant>
#include "serverstatus/server_info.h"
#include "umslib/global/common_funcs.h"
#include "umslib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace umsservice{
namespace serverstatus{

using umslib::kernel::StdDir;
using sn::corelib::Filesystem;

ServerInfoWrapper::ServerInfoWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse ServerInfoWrapper::getVersionInfo(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("ServerStatus/ServerInfo/getVersionInfo", true);
   response.setSerial(request.getSerial());
   response.setDataItem("version", umslib::global::get_upgrademgr_slave_version());
   response.setIsFinal(true);
   return response;
}

}//serverstatus
}//msservice