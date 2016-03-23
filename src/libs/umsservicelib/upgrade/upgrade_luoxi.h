#ifndef UMS_SERVICE_UPGRADE_UPGRADE_LUOXI
#define UMS_SERVICE_UPGRADE_UPGRADE_LUOXI

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

using umsservice::common::DownloadClientWrapper;

class UMS_SERVICE_EXPORT UpgradeLuoXiWrapper : public AbstractService
{
   Q_OBJECT
public:
   const static QString RPM_FILENAME_TPL;
   const static int STEP_PREPARE = -1;
   const static int STEP_INIT_CONTEXT = 0;
   const static int STEP_DOWNLOAD_PKG = 1;
   const static int STEP_DOWNLOAD_COMPLETE = 2;
   const static int STEP_INSTALL_RPM = 3;
   const static int STEP_RESTART_SERVER = 4;
   const static int STEP_FINISH = 5;
   const static int STEP_ERROR = 6;
   
   struct UpgradeContext
   {
      QString targetVersion;
      QString pkgFilename;
      ServiceInvokeRequest request;
      ServiceInvokeResponse response;
      bool upgradeStatus;
      QString upgradeErrorString;
   };
public:
   UpgradeLuoXiWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest &request);
protected:
   void downloadUpgradePkg(const QString &filename);
   bool installRpmPackage(const QString &filename, QString &errorString);
   void clearState();
   QSharedPointer<DownloadClientWrapper> getDownloadClient(const QString &host, quint16 port);
   int getLuoXiPid();
protected:
   virtual void notifySocketDisconnect(QTcpSocket *socket);
protected:
   const QString m_softwareRepoDir;
   bool m_isInAction = false;
   QSharedPointer<UpgradeContext> m_context;
   int m_step = STEP_PREPARE;
   QString m_pidFilename;
   QSharedPointer<DownloadClientWrapper> m_downloadClient;
};

}//upgrader
}//umsservice

#endif // UMS_SERVICE_UPGRADE_UPGRADE_LUOXI

