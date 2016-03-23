#ifndef UMS_SERVICE_SERVER_STATUS_SERVER_INFO_H
#define UMS_SERVICE_SERVER_STATUS_SERVER_INFO_H

#include <QSharedPointer>
#include <QString>
#include <QMap>

#include "umsservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace umsservice{
namespace serverstatus{

UMS_USING_SERVICE_NAMESPACES

class UMS_SERVICE_EXPORT ServerInfoWrapper : public AbstractService
{
   Q_OBJECT
public:
   ServerInfoWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse getVersionInfo(const ServiceInvokeRequest &request);
};

}//serverstatus
}//umsservice

#endif // UMS_SERVICE_SERVER_STATUS_SERVER_INFO_H