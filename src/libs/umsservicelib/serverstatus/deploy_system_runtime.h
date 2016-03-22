#ifndef UMS_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H
#define UMS_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H

#include <QSharedPointer>
#include <QString>
#include <QMap>

#include "umsservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace umsservice{
namespace serverstatus{

using sn::corelib::network::ServiceInvoker;

UMS_USING_SERVICE_NAMESPACES

class UMS_SERVICE_EXPORT DeploySystemLuoXiServerRuntimeWrapper : public AbstractService
{
   Q_OBJECT
public:
   DeploySystemLuoXiServerRuntimeWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse startServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse stopServer(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse restartServer(const ServiceInvokeRequest &request);
protected:
   int getLuoXiServerPid();
protected:
   QString m_pidFilename;
};

}//serverstatus
}//umsservice


#endif // UMS_SERVICE_SERVER_STATUS_DEPLOY_SYSTEM_RUNTIME_H
