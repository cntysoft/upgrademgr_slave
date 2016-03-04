#include "upgrade_upgrademgr_slave.h"

#include <QProcess>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>

#include "upgrade_upgrademgr_slave.h"

#include "corelib/io/filesystem.h"

#include "umslib/network/multi_thread_server.h"
#include "umslib/kernel/stddir.h"
#include "umslib/global/common_funcs.h"

#include "corelib/kernel/application.h"
#include "corelib/utils/version.h"

namespace umsservice{
namespace upgrader{

using umslib::kernel::StdDir;

using sn::corelib::Filesystem;
using sn::corelib::utils::Version;
using sn::corelib::Application;

const QString UpgradeUpgradeMgrSlaveWrapper::RPM_FILENAME_TPL = "upgrademgr_slave-%1-1.x86_64.rpm";

UpgradeUpgradeMgrSlaveWrapper::UpgradeUpgradeMgrSlaveWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_softwareRepoDir(StdDir::getSoftwareRepoDir())
{
}

ServiceInvokeResponse UpgradeUpgradeMgrSlaveWrapper::upgrade(const ServiceInvokeRequest &request)
{
   
}

}//upgrader
}//umsservice