#ifndef UMS_COMMAND_START_SERVER_COMMAND_H
#define UMS_COMMAND_START_SERVER_COMMAND_H

#include "corelib/command/abstract_command.h"

namespace upgrademgr{
namespace slave{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class StartServerCommand : public AbstractCommand
{
public:
   StartServerCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

}//command
}//slave
}//upgrademgr

#endif // UMS_COMMAND_START_SERVER_COMMAND_H
