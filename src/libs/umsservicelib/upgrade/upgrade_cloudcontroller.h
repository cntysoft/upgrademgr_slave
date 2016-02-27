#ifndef UMS_SERVICE_UPGRADE_CLOUDCONTROLLER_H
#define UMS_SERVICE_UPGRADE_CLOUDCONTROLLER_H

#include <QString>
#include <QSharedPointer>
#include <QTcpSocket>

#include "umsservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

#include "umsservicelib/common/download_client.h"
#include "corelib/upgrade/upgradeenv.h"

namespace umsservice{
namespace upgrader{

UMS_USING_SERVICE_NAMESPACES

using umsservice::common::DownloadClient;
using UpgradeEnvEngine = sn::corelib::upgrade::UpgradeEnv;

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
      QStringList deleteFiles;
      QStringList modifyFiles;
      QStringList senchaChangedProjects;
      QString upgradeDir;
      QString backupDir;
      QString dbHost;
      QString dbUser;
      QString dbPassword;
      QString ccDbName;
   };
   const static int STEP_PREPARE = -1;
   const static int STEP_INIT_CONTEXT = 0;
   const static int STEP_DOWNLOAD_PKG = 1;
   const static int STEP_DOWNLOAD_COMPLETE = 2;
   const static int STEP_EXTRA_PKG = 3;
   const static int STEP_BACKUP_FILES = 4;
   const static int STEP_UPGRADE_FILES = 5;
   const static int STEP_BACKUP_DB = 6;
   const static int STEP_RUN_UPGRADE_SCRIPT = 7;
   const static int STEP_CLEANUP = 8;
   const static int STEP_FINISH = 9;
   
   const static QString CC_UPGRADE_PKG_NAME_TPL;
   const static QString CC_UPGRADE_DB_META_NAME_TPL;
   const static QString CC_UPGRADE_SCRIPT_NAME_TPL;
public:
   UpgradeCloudControllerWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest &request);
protected:
   void downloadUpgradePkg(const QString &filename);
   void backupScriptFiles();
   void upgradeFiles();
   void backupDatabase();
   void runUpgradeScript();
   void upgradeComplete();
   
   QSharedPointer<DownloadClient> getDownloadClient(const QString &host, quint16 port);
   void clearState();
   QString getBackupDir();
   QString getUpgradeTmpDir();
   void unzipPkg(const QString &pkgFilename);
   QSharedPointer<UpgradeEnvEngine> getUpgradeScriptEngine();
protected:
   virtual void notifySocketDisconnect(QTcpSocket *socket);
protected:
   bool m_isInAction = false;
   QSharedPointer<UpgradeContext> m_context;
   int m_step = STEP_PREPARE;
   QString m_deployDir;
   QSharedPointer<DownloadClient> m_downloadClient;
   QSharedPointer<UpgradeEnvEngine> m_upgradeScriptEngine;
};

}//upgrader
}//ummservice

#endif // UMS_SERVICE_UPGRADE_CLOUDCONTROLLER_H
