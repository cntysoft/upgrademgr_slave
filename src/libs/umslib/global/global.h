#ifndef UMS_LIB_GLOBAL_GLOBAL_H
#define UMS_LIB_GLOBAL_GLOBAL_H

#include "corelib/global/global.h"

#ifdef UMS_STATIC_LIB
   #define UMS_LIB_EXPORT 
#else
   #ifdef UMS_LIBRARY
      #define UMS_LIB_EXPORT Q_DECL_EXPORT
   #else
      #define UMS_LIB_EXPORT Q_DECL_IMPORT
   #endif
#endif

#endif // UMS_LIB_GLOBAL_GLOBAL_H
