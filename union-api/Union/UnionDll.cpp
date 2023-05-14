
#include "Memory.h"
#include "ModuleImm32Collection.h"
#include "HookPatch.h"
#include "UnionDll.h"

namespace Union {
  static Array<UnionDll*> GetUnionDllCollection() {
    static Array<UnionDll*>* collection =
      (Array<UnionDll*>*)CreateSharedSingleton( "UnionDllCollection", []() -> void* { return new Array<UnionDll*>(); } );
    return collection->Share();
  }


  Array<UnionDll*> UnionDll::GetDllCollection() {
    return GetUnionDllCollection();
  }


  UnionDll::UnionDll() {
    Module = GetCurrentDll();
    HookProviderPatch::ModuleInitialized( Module );
    GetUnionDllCollection().Insert( this );
  }


  HMODULE UnionDll::GetCurrentDll() {
    HMODULE module = nullptr;
    GetModuleHandleEx(
      GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
      (LPCTSTR)&GetCurrentDll,
      &module );

    return module;
  }


  UnionDll& UnionDll::GetInstance() {
    static UnionDll* instance = new UnionDll();
    return *instance;
  }


  UnionDll& GlobalUnionDllInstance = UnionDll::GetInstance();
}