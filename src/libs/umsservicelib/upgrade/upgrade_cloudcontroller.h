#ifndef UMS_SERVICE_UPGRADE_CLOUDCONTROLLER_H
#define UMS_SERVICE_UPGRADE_CLOUDCONTROLLER_H

#include <QString>
#include <QSharedPointer>

#include "umsservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

#include "umsservicelib/common/download_client.h"

namespace umsservice{
namespace upgrader{

UMS_USING_SERVICE_NAMESPACES

using umsservice::common::DownloadClient;

class UMS_SERVICE_EXPORT UpgradeCloudControllerWrapper : public AbstractService
{
   Q_OBJECT
public:
   struct UpgradeContext
   {
      QString fromVersion;
      QString toVersion;
      QString pkgFilename;
      ServiceInvokeRequest request;
      ServiceInvokeResponse response;
   };
   const static int STEP_PREPARE = -1;
   const static int STEP_INIT_CONTEXT = 0;
   const static int STEP_DOWNLOAD_PKG = 1;
   const static int STEP_BACKUP_FILES = 2;
   const static int STEP_UPGRADE_FILES = 3;
   const static int STEP_BACKUP_DB = 4;
   const static int STEP_RUN_UPGRADE_SCRIPT = 5;
   const static int STEP_CLEANUP = 6;
   const static int STEP_FINISH = 7;
   
   const static QString CC_UPGRADE_PKG_NAME_TPL;
public:
   UpgradeCloudControllerWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse init(const ServiceInvokeRequest &request);
//   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest &request);
protected:
   void downloadUpgradePkg(const QString &filename);
   QSharedPointer<DownloadClient> getDownloadClient(const QString &host, quint16 port);
   void clearState();
protected:
   bool m_isInAction = false;
   QSharedPointer<UpgradeContext> m_context;
   int m_step = STEP_PREPARE;
   QString m_deployDir;
   QSharedPointer<DownloadClient> m_downloadClient;
};

}//upgrader
}//ummservice

#endif // UMS_SERVICE_UPGRADE_CLOUDCONTROLLER_H
