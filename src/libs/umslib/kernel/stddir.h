#ifndef UMS_LIB_KERNEL_STDDIR_H
#define UMS_LIB_KERNEL_STDDIR_H

#include <QString>

#include "umslib/global/global.h"

#include "corelib/kernel/stddir.h"
#include "corelib/kernel/application.h"

namespace umslib{
namespace kernel{

using BaseStdDir = sn::corelib::StdDir;
using sn::corelib::Application;

class UMS_LIB_EXPORT StdDir : public BaseStdDir
{
public:
   static QString getBaseDataDir();
   static QString getSoftwareRepoDir();
   static QString getMetaDir();
};

}//network
}//umslib

#endif // UMS_LIB_KERNEL_STDDIR_H
