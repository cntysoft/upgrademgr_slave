#ifndef UMS_APPLICATION_H
#define UMS_APPLICATION_H

#include "corelib/kernel/application.h"
#include "corelib/kernel/settings.h"

namespace upgrademgr{
namespace slave{

using BaseApplication = sn::corelib::Application;
using sn::corelib::Settings;

class Application : public BaseApplication
{
public:
   Application(int &argc, char **argv);
public:
   virtual Settings::CfgInitializerFnType getDefaultCfgInitializerFn();
   virtual ~Application();
};

}//slave
}//upgrademgr

#endif // UMS_APPLICATION_H
