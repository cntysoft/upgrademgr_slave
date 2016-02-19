#include "initializers/initializer_cleanup_funcs.h"

namespace upgrademgr{
namespace slave{

void global_initializer()
{
   init_service_provider();
}

void global_cleanup()
{
   cleanup_service_provider();   
}

}//slave
}//upgrademgr
