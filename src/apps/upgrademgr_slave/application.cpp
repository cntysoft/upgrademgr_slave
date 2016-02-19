#include "application.h"

namespace upgrademgr{
namespace slave{

extern void init_defualt_cfg(Settings &settings);

Application::Application(int &argc, char **argv)
   : BaseApplication(argc, argv)
{
   setApplicationName("upgrademgrslave");
}

Settings::CfgInitializerFnType Application::getDefaultCfgInitializerFn()
{
   return init_defualt_cfg;
}

Application::~Application()
{}

}//slave
}//upgrademgr
