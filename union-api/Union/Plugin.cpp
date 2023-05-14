#include "Memory.h"
#include "Plugin.h"
#include <stdio.h>

namespace Union {
  static int find_char( const char* str, char c ) {
    size_t index = strlen( str );
    while( --index >= 0 && str[index] != c );
    return index;
  }


  Plugin::Plugin( HMODULE module ) {
    Module = module;
    DWORD pathLen = GetModuleFileNameW( Module, NULL, 0 );
    if( pathLen == 0 )
      pathLen = 2048;

    FullFileName = new char[pathLen + 1];
    GetModuleFileNameA( Module, FullFileName, pathLen + 1 );
    int index = find_char( FullFileName, '\\' );
    FileName = &FullFileName[index + 1];
    printf( "Plugin initialized: %s\n", FileName );
  }


  HMODULE Plugin::GetCurrentModule() {
    HMODULE module = NULL;
    GetModuleHandleEx(
      GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
      (LPCTSTR)GetCurrentModule,
      &module );

    return module;
  }


  Plugin& Plugin::GetInstance() {
    static Plugin* instance = new Plugin( GetCurrentModule() );
    return *instance;
  }


  Plugin& Plugin::GetMainInstance() {
    static Plugin* instance =
      (Plugin*)CreateSharedSingleton( "MainPluginsModule", []() -> void* { return new Plugin( GetModuleHandle( nullptr ) ); } );
    return *instance;
  }

  
  const char* Plugin::GetModuleName() const {
    return FileName;
  }


  const char* Plugin::GetModuleFullName() const {
    return FullFileName;
  }


  _export_c_ Plugin& CurrentPlugin = Plugin::GetInstance();
}