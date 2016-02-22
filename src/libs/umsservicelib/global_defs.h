#ifndef UMS_SERVICE_GLOBAL_DEFS_H
#define UMS_SERVICE_GLOBAL_DEFS_H

#include <qglobal.h>

#ifdef UMS_SERVICE_STATIC_LIB
   #define UMS_SERVICE_EXPORT 
#else
   #ifdef UMS_SERVICE_LIBRARY
      #define UMS_SERVICE_EXPORT Q_DECL_EXPORT
   #else
      #define UMS_SERVICE_EXPORT Q_DECL_IMPORT
   #endif
#endif

#include "macros.h"

#endif // UMS_SERVICE_GLOBAL_DEFS_H
