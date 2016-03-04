#ifndef UMS_SERVICE_UPGRADE_UPGRADEMGR_SLAVE_WRAPPER_H
#define UMS_SERVICE_UPGRADE_UPGRADEMGR_SLAVE_WRAPPER_H

#include <QString>
#include <QSharedPointer>
#include <QTcpSocket>

#include "umsservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

#include "umsservicelib/common/download_client.h"

namespace umsservice{
namespace upgrader{

UMS_USING_SERVICE_NAMESPACES

using umsservice::common::DownloadClient;

class UMS_SERVICE_EXPORT UpgradeUpgradeMgrSlaveWrapper : public AbstractService
{
   Q_OBJECT
   const static QString RPM_FILENAME_TPL;
public:
   UpgradeUpgradeMgrSlaveWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest &request);
protected:
   bool installRpmPackage(const QString &filename, QString &errorString);
protected:
   const QString m_softwareRepoDir;
};

}//upgrader
}//umsservice

#endif // UMS_SERVICE_UPGRADE_UPGRADEMGR_SLAVE_WRAPPER_H



