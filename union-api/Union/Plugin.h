#pragma once
#ifndef __UNION_PLUGIN_H__
#define __UNION_PLUGIN_H__

#include <Windows.h>
#include "Types.h"

namespace Union {
  class UNION_API Plugin {
    char* FileName;
    char* FullFileName;
    HMODULE Module;
    Plugin( HMODULE module );
  public:
    static HMODULE GetCurrentModule();
    static Plugin& GetInstance();
    static Plugin& GetMainInstance();
    const char* GetModuleName() const;
    const char* GetModuleFullName() const;
  };
}

#endif // __UNION_STRING_H__