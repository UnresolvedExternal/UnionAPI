#pragma once
#ifndef __UNION_INCLUDE_ZLIB_H__
#define __UNION_INCLUDE_ZLIB_H__
#include "RTVersion.h"
#include "Zlib/zlib.h"
#ifndef _WIN64
  #ifndef _DEBUG
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs/v143/zlibstat.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs/v142/zlibstat.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs/v141/zlibstat.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs/v140/zlibstat.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs/v100/zlibstat.lib")
    #else
      #error No 'zlib' static libraries for the current toolset
    #endif
  #else
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs/v143d/zlibstat.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs/v142d/zlibstat.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs/v141d/zlibstat.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs/v140d/zlibstat.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs/v100d/zlibstat.lib")
    #else
      #error No 'zlib' static libraries for the current toolset
    #endif
  #endif
#else
  #ifndef _DEBUG
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs_x64/v143/zlibstat.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs_x64/v142/zlibstat.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs_x64/v141/zlibstat.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs_x64/v140/zlibstat.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs_x64/v100/zlibstat.lib")
    #else
      #error No 'zlib' static libraries for the current toolset
    #endif
  #else
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs_x64/v143d/zlibstat.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs_x64/v142d/zlibstat.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs_x64/v141d/zlibstat.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs_x64/v140d/zlibstat.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs_x64/v100d/zlibstat.lib")
    #else
      #error No 'zlib' static libraries for the current toolset
    #endif
  #endif
#endif
#endif // __UNION_INCLUDE_ZLIB_H__