#include <QDir>

#include "umslib/global/const.h"
#include "kernel/settings.h"

namespace upgrademgr{
namespace slave{

using sn::corelib::Settings;

static void init_global_cfg(Settings &settings);

void init_defualt_cfg(Settings& settings)
{
   init_global_cfg(settings);
}

static void init_global_cfg(Settings& settings)
{
   QString runtimeDir = QDir::tempPath()+QDir::separator()+"upgrademgrslave";
   settings.setValue("runtimeDir", runtimeDir, UMS_CFG_GROUP_GLOBAL);
   settings.setValue("baseDataDir", "/cntysoft/upgrademgrslave", UMS_CFG_GROUP_GLOBAL);
   settings.setValue("cloudControllerWebRootDir", "/srv/www/cloudcontroller", UMS_CFG_GROUP_GLOBAL);
   settings.setValue("upgrademgrMasterHost", "127.0.0.1", UMS_CFG_GROUP_GLOBAL);
   settings.setValue("upgrademgrMasterPort", UMM_LISTEN_PORT, UMS_CFG_GROUP_GLOBAL);
   settings.setValue("dbHost", "localhost", UMS_CFG_GROUP_GLOBAL);
   settings.setValue("dbUser", "root", UMS_CFG_GROUP_GLOBAL);
   settings.setValue("dbPassword", "cntysoft", UMS_CFG_GROUP_GLOBAL);
   settings.setValue("cloudControllerDb", "cloudcontroller", UMS_CFG_GROUP_GLOBAL);
}


}//master
}//upgrademgr