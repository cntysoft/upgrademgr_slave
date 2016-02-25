#include "download_client.h"

namespace umsservice{
namespace common{

void init_download_handler(const ServiceInvokeResponse &response, void* args)
{
   DownloadClient *self = static_cast<DownloadClient*>(args);
   if(!response.getStatus()){
      self->emitDownloadError(response.getError().first, response.getError().second);
   }
}

void DownloadClient::emitDownloadError(int errorCode, const QString &errorMsg)
{
   emit downloadError(errorCode, errorMsg);
}

DownloadClient::DownloadClient(QSharedPointer<ServiceInvoker> serviceInvoker)
   :m_serviceInvoker(serviceInvoker)
{
}

void DownloadClient::download(const QString &filename)
{
   emit beginDownload();
   m_serviceInvoker->connectToServer();
   connect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, [&](){
      ServiceInvokeRequest request("Common/DownloadServer", "init", {
                                      {"filename", filename}
                                   });
      m_serviceInvoker->request(request, init_download_handler, static_cast<void*>(this));
   });
}

}//common
}//umsservice