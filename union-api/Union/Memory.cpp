#include <Windows.h>
#include <malloc.h>
#include <psapi.h>
#include "Types.h"
#include "Memory.h"
#include "Plugin.h"
#pragma comment(lib, "psapi.lib")

namespace Union {
  _export_c_ Plugin& CurrentPlugin;


  typedef void* (*MallocFunction)(size_t size);
  typedef void* (*CallocFunction)(size_t count, size_t size);
  typedef void* (*ReallocFunction)(void* memory, size_t size);
  typedef void (*FreeFunction)(void* memory);
  typedef size_t( *MsizeFunction )(void* memory);


  struct SharedSingleton {
    char* Name;
    void* Address;
    int Shares;
    SharedSingleton( const char* name, void* address );
    void* operator new(size_t size);
    void operator delete(void* memory);
    ~SharedSingleton();
  };


  struct SharedMemory {
    Plugin* Owner;
    SharedSingleton** Singletons;
    int SingletonsCount;

    MallocFunction Malloc;
    CallocFunction Calloc;
    ReallocFunction Realloc;
    FreeFunction Free;
    MsizeFunction Msize;

    SharedMemory();
    virtual int SearchSingleton( const char* singletonName );
    virtual void Insert( const char* singletonName, void* address );
    virtual void* Share( const char* singletonName );
    virtual void* Release( const char* singletonName );
    void* operator new(size_t size);
    void operator delete(void* memory);
    static SharedMemory* InitializeInstance();
    static SharedMemory& GetInstance();
  };


  SharedMemory::SharedMemory() {
    Singletons = nullptr;
    SingletonsCount = 0;
    Malloc = &malloc;
    Calloc = &calloc;
    Realloc = &realloc;
    Free = &free;
    Msize = &_msize;
    Owner = &CurrentPlugin;
  }


  SharedSingleton::SharedSingleton( const char* name, void* address ) {
    size_t nameLength = strlen( name ) + 1;
    Name = (char*)::malloc(nameLength);
    memcpy( Name, name, nameLength );
    Address = address;
    Shares = 1;
  }


  SharedSingleton::~SharedSingleton() {
    ::free( Name );
  }


  void* SharedSingleton::operator new(size_t size) {
    return ::malloc( size );
  }


  void SharedSingleton::operator delete(void* memory) {
    ::free( memory );
  }


  int SharedMemory::SearchSingleton( const char* singletonName ) {
    for( int i = 0; i < SingletonsCount; i++ )
      if( strcmp( Singletons[i]->Name, singletonName ) == 0 )
        return i;
    
    return -1;
  }


  void SharedMemory::Insert( const char* singletonName, void* address ) {
    int index = SearchSingleton( singletonName );
    if( index == -1 ) {
      index = SingletonsCount++;
      Singletons = (SharedSingleton**)::realloc( Singletons, SingletonsCount * sizeof( SharedSingleton* ) );
      Singletons[index] = new SharedSingleton( singletonName, address );
    }
    else
      Singletons[index]->Shares++;
  }


  void* SharedMemory::Share( const char* singletonName ) {
    int index = SearchSingleton( singletonName );
    if( index == -1 )
      return nullptr;

    Singletons[index]->Shares++;
    return Singletons[index]->Address;
  }


  void* SharedMemory::Release( const char* singletonName ) {
    int index = SearchSingleton( singletonName );
    if( index == -1 )
      return nullptr;

    if( --Singletons[index]->Shares == 0 ) {
      void* address = Singletons[index]->Address;
      delete Singletons[index];
      memcpy( &Singletons[index], &Singletons[--SingletonsCount], sizeof( SharedSingleton* ) );
      Singletons = (SharedSingleton**)::realloc( Singletons, SingletonsCount * sizeof( SharedSingleton* ) );
      return address;
    }

    return nullptr;
  }


  void* SharedMemory::operator new(size_t size) {
    return ::malloc( size );
  }


  void SharedMemory::operator delete(void* memory) {
    ::free( memory );
  }


  _export_c_ SharedMemory* UnionSharedMemoryInstance = nullptr;


  SharedMemory* SharedMemory::InitializeInstance() {
    DWORD processId = GetCurrentProcessId();
    HMODULE modules[1024];
    HANDLE process;
    DWORD cbNeeded;

    process = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId );
    if( process != nullptr ) {
      if( EnumProcessModules( process, modules, sizeof( modules ), &cbNeeded ) ) {
        DWORD length = cbNeeded / sizeof( HMODULE );
        for( DWORD i = 0; i < length; i++ ) {
          auto address = GetProcAddress( modules[i], "UnionSharedMemoryInstance");
          if( address ) {
            auto object = *(SharedMemory**)address;
            if( object ) {
              CloseHandle( process );
              return object;
            }
          }
        }
      }

      CloseHandle( process );
    }

    UnionSharedMemoryInstance = new SharedMemory();
    return UnionSharedMemoryInstance;
  }


  SharedMemory& SharedMemory::GetInstance() {
    static SharedMemory* instance = InitializeInstance();
    return *instance;
  }


  __declspec(noinline) void* MemAlloc( size_t size ) {
    static auto proc = SharedMemory::GetInstance().Malloc;
    return proc( size );
  }


  __declspec(noinline) void* MemCalloc( size_t count, size_t size ) {
    static auto proc = SharedMemory::GetInstance().Calloc;
    return proc( count, size );
  }


  __declspec(noinline) void* MemRealloc( void* memory, size_t size ) {
    static auto proc = SharedMemory::GetInstance().Realloc;
    return proc( memory, size );
  }


  __declspec(noinline) void MemFree( void* memory ) {
    static auto proc = SharedMemory::GetInstance().Free;
    return proc( memory );
  }


  __declspec(noinline) void MemDelete( void* memory ) {
    static auto proc = SharedMemory::GetInstance().Free;
    return proc( memory );
  }


  __declspec(noinline) size_t MemSize( void* memory ) {
    static auto proc = SharedMemory::GetInstance().Msize;
    return proc( memory );
  }


  void* CreateSharedSingleton( const char* globalName, void* (*allocation)() ) {
    auto& memory = SharedMemory::GetInstance();
    int index = memory.SearchSingleton( globalName );
    if( index != -1 )
      return memory.Share( globalName );

    void* address = allocation();
    if( address )
      memory.Insert( globalName, address );

    return address;
  }


  void FreeSharedSingleton( const char* globalName, void(*destructor)(void*) ) {
    auto& memory = SharedMemory::GetInstance();
    int index = memory.SearchSingleton( globalName );
    if( index == -1 )
      return;
    
    void* address = memory.Release( globalName );
    if( address )
      destructor( address );
  }
}


__declspec(noinline) void* operator new (size_t size) {
  return Union::MemAlloc( size );
}


__declspec(noinline) void* operator new []( size_t size ) {
  return Union::MemAlloc( size );
}


__declspec(noinline) void operator delete (void* memory) {
  Union::MemFree( memory );
}


__declspec(noinline) void operator delete []( void* memory ) {
  Union::MemFree( memory );
}