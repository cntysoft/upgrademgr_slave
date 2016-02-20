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
//   settings.setValue("dbHost", "localhost");
//   settings.setValue("dbUser", "cntysoft");
//   settings.setValue("dbPassword", "cntysoft");
//   settings.setValue("dbName", "upgrademgrslave");
}


}//master
}//upgrademgr