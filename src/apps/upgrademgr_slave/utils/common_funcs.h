#ifndef UMS_UTILS_COMMON_FUNCS_H
#define UMS_UTILS_COMMON_FUNCS_H

#include <QLatin1String>

#include "application.h"

namespace upgrademgr{
namespace slave{

using UpgrademgrSlaveApplication = upgrademgr::slave::Application;

UpgrademgrSlaveApplication* get_app_ref();

QLatin1String get_upgrademgr_slave_version();

QString get_application_filepath();

}//slave
}//upgrademgr

#endif // UMS_UTILS_COMMON_FUNCS_H
