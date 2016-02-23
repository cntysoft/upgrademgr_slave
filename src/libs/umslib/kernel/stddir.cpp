#include "stddir.h"

#include "umslib/global/const.h"
#include "corelib/kernel/settings.h"

namespace umslib{
namespace kernel{

using sn::corelib::Settings;

QString StdDir::getBaseDataDir()
{
   static QString dir;
   if(dir.isEmpty()){
#ifdef AUTOTEST_BUILD
      dir = "/cntysoft/upgrademgrslave";
#else
      Settings& settings = Application::instance()->getSettings();
      dir = settings.getValue("baseDataDir", UMS_CFG_GROUP_GLOBAL, "/cntysoft/upgrademgrslave").toString();
#endif
   }
   return dir;
}

QString StdDir::getSoftwareRepoDir()
{
   return StdDir::getBaseDataDir()+"/softwarerepo";
}

QString StdDir::getMetaDir()
{
   return StdDir::getBaseDataDir()+"/meta";
}

}//network
}//umslib