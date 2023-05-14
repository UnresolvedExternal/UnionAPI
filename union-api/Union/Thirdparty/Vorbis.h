#pragma once
#ifndef __UNION_INCLUDE_VORBIS_H__
#define __UNION_INCLUDE_VORBIS_H__
#include "RTVersion.h"
#include "Vorbis/vorbisfile.h"
#ifndef _WIN64
  #ifndef _DEBUG
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs/v143/libogg.lib")
      #pragma comment(lib, "Libs/v143/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v143/libvorbisfile_static.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs/v142/libogg.lib")
      #pragma comment(lib, "Libs/v142/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v142/libvorbisfile_static.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs/v141/libogg.lib")
      #pragma comment(lib, "Libs/v141/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v141/libvorbisfile_static.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs/v140/libogg.lib")
      #pragma comment(lib, "Libs/v140/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v140/libvorbisfile_static.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs/v100/libogg.lib")
      #pragma comment(lib, "Libs/v100/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v100/libvorbisfile_static.lib")
    #else
      #error No 'vorbis' static libraries for the current toolset
    #endif
  #else
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs/v143d/libogg.lib")
      #pragma comment(lib, "Libs/v143d/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v143d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs/v142d/libogg.lib")
      #pragma comment(lib, "Libs/v142d/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v142d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs/v141d/libogg.lib")
      #pragma comment(lib, "Libs/v141d/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v141d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs/v140d/libogg.lib")
      #pragma comment(lib, "Libs/v140d/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v140d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs/v100d/libogg.lib")
      #pragma comment(lib, "Libs/v100d/libvorbis_static.lib")
      #pragma comment(lib, "Libs/v100d/libvorbisfile_static.lib")
    #else
      #error No 'vorbis' static libraries for the current toolset
    #endif
  #endif
#else
  #ifndef _DEBUG
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs_x64/v143/libogg.lib")
      #pragma comment(lib, "Libs_x64/v143/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v143/libvorbisfile_static.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs_x64/v142/libogg.lib")
      #pragma comment(lib, "Libs_x64/v142/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v142/libvorbisfile_static.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs_x64/v141/libogg.lib")
      #pragma comment(lib, "Libs_x64/v141/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v141/libvorbisfile_static.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs_x64/v140/libogg.lib")
      #pragma comment(lib, "Libs_x64/v140/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v140/libvorbisfile_static.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs_x64/v100/libogg.lib")
      #pragma comment(lib, "Libs_x64/v100/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v100/libvorbisfile_static.lib")
    #else
      #error No 'vorbis' static libraries for the current toolset
    #endif
  #else
    #if defined(_VCRT_V143)
      #pragma comment(lib, "Libs_x64/v143d/libogg.lib")
      #pragma comment(lib, "Libs_x64/v143d/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v143d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V142)
      #pragma comment(lib, "Libs_x64/v142d/libogg.lib")
      #pragma comment(lib, "Libs_x64/v142d/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v142d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V141)
      #pragma comment(lib, "Libs_x64/v141d/libogg.lib")
      #pragma comment(lib, "Libs_x64/v141d/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v141d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V140)
      #pragma comment(lib, "Libs_x64/v140d/libogg.lib")
      #pragma comment(lib, "Libs_x64/v140d/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v140d/libvorbisfile_static.lib")
    #elif defined(_VCRT_V100)
      #pragma comment(lib, "Libs_x64/v100d/libogg.lib")
      #pragma comment(lib, "Libs_x64/v100d/libvorbis_static.lib")
      #pragma comment(lib, "Libs_x64/v100d/libvorbisfile_static.lib")
    #else
      #error No 'vorbis' static libraries for the current toolset
    #endif
  #endif
#endif
#endif // __UNION_INCLUDE_VORBIS_H__