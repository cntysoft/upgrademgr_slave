#ifndef UMS_COMMAND_GLOBAL_COMMAND_H
#define UMS_COMMAND_GLOBAL_COMMAND_H

#include "corelib/command/abstract_command.h"

namespace upgrademgr{
namespace slave{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class GlobalHelpCommand : public AbstractCommand 
{
public:
   GlobalHelpCommand(AbstractCommandRunner &runner, const CommandMeta &invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalHelpCommand();
};


class GlobalVersionCommand : public AbstractCommand 
{
public:
   GlobalVersionCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalVersionCommand();
};

}//command
}//slave
}//upgrademgr

#endif // UMS_COMMAND_GLOBAL_COMMAND_H
