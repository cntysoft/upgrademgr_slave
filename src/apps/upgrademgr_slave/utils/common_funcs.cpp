#include "common_funcs.h"

namespace upgrademgr{
namespace slave{

UpgrademgrSlaveApplication* get_app_ref()
{
   return static_cast<UpgrademgrSlaveApplication*>(UpgrademgrSlaveApplication::instance());
}

QLatin1String get_upgrademgr_slave_version()
{
   return QLatin1String(UMS_VERSION);
}

QString get_application_filepath()
{
#ifdef UMS_DEBUG_BUILD
   return Application::applicationFilePath();
#else
   return QString("/usr/local/bin/upgrademgr_slave");
#endif
}

}//slave
}//upgrademgr