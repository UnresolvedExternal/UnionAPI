#pragma once
#ifndef __UNION_INCLUDE_DETOURS_H__
#define __UNION_INCLUDE_DETOURS_H__
#include "RTVersion.h"
#include "Detours/Detours.h"
#ifndef _WIN64
  #ifndef _DEBUG
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs/v143/detours.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs/v142/detours.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs/v141/detours.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs/v140/detours.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs/v100/detours.lib")
    #else
      #error No 'detours' static libraries for the current toolset
    #endif
  #else
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs/v143d/detours.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs/v142d/detours.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs/v141d/detours.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs/v140d/detours.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs/v100d/detours.lib")
    #else
      #error No 'detours' static libraries for the current toolset
    #endif
  #endif
#else
  #ifndef _DEBUG
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs_x64/v143/detours.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs_x64/v142/detours.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs_x64/v141/detours.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs_x64/v140/detours.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs_x64/v100/detours.lib")
    #else
      #error No 'detours' static libraries for the current toolset
    #endif
  #else
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs_x64/v143d/detours.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs_x64/v142d/detours.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs_x64/v141d/detours.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs_x64/v140d/detours.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs_x64/v100d/detours.lib")
    #else
      #error No 'detours' static libraries for the current toolset
    #endif
  #endif
#endif
#endif // __UNION_INCLUDE_DETOURS_H__