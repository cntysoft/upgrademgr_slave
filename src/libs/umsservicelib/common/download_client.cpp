#include "download_client.h"

namespace umsservice{
namespace common{

DownloadClient::DownloadClient(QSharedPointer<ServiceInvoker> serviceInvoker)
   :m_serviceInvoker(serviceInvoker)
{
}

void DownloadClient::download(const QString &filename)
{
   emit beginDownload();
}

}//common
}//umsservice