#include "initializers/initializer_cleanup_funcs.h"
#include "umsservicelib/service_repo.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/abstract_service.h"

namespace upgrademgr{
namespace slave{

using sn::corelib::network::ServiceProvider;
using sn::corelib::network::AbstractService;

void init_service_provider()
{
   ServiceProvider& provider = ServiceProvider::instance();
   provider.addServiceToPool("ServerStatus/DeploySystemLuoXiRuntime", [](ServiceProvider& provider)-> AbstractService*{
      return new umsservice::serverstatus::DeploySystemLuoXiServerRuntimeWrapper(provider);
   });
   provider.addServiceToPool("ServerStatus/ServerInfo", [](ServiceProvider& provider)-> AbstractService*{
      return new umsservice::serverstatus::ServerInfoWrapper(provider);
   });
   provider.addServiceToPool("Upgrader/UpgradeCloudController", [](ServiceProvider& provider)-> AbstractService*{
      return new umsservice::upgrader::UpgradeCloudControllerWrapper(provider);
   });
   provider.addServiceToPool("Upgrader/UpgradeUpgradeMgrSlave", [](ServiceProvider& provider)-> AbstractService*{
      return new umsservice::upgrader::UpgradeUpgradeMgrSlaveWrapper(provider);
   });
   provider.addServiceToPool("Upgrader/UpgradeLuoXi", [](ServiceProvider& provider)-> AbstractService*{
      return new umsservice::upgrader::UpgradeLuoXiWrapper(provider);
   });
}

void cleanup_service_provider()
{
   ServiceProvider &provider = ServiceProvider::instance();
   delete &provider;
}

}//slave
}//upgrademgr
