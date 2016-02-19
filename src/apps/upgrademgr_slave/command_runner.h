#ifndef UMS_COMMAND_RUNNER_H
#define UMS_COMMAND_RUNNER_H

#include "corelib/command/abstract_command_runner.h"

namespace upgrademgr{
namespace slave{

using sn::corelib::AbstractCommandRunner;
class Application;

class CommandRunner : public AbstractCommandRunner
{
public:
   CommandRunner(Application &app);
private:
   void initCommandPool();
   void initRouteItems();
};

}//slave
}//upgrademgr

#endif // UMS_COMMAND_RUNNER_H