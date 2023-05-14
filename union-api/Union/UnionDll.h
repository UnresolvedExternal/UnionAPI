#pragma once
#ifndef __UNION_DLL_H__
#define __UNION_DLL_H__

#include "Array.h"

#ifndef _MINWINDEF_
typedef void* HMODULE;
#endif

namespace Union {
#if defined(_UNION_API_LIB) && !defined(_UNION_API_BUILD)
#pragma comment(lib,"UnionApi.lib")
#endif
  class UNION_API UnionDll {
    HMODULE Module;
    UnionDll();
  public:
    Array<UnionDll*> GetDllCollection();
    static HMODULE GetCurrentDll();
    static UnionDll& GetInstance();
  };
}

#endif // __UNION_DLL_H__