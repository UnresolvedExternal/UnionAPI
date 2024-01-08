#pragma once
#ifndef __UNION_MEMORY_H__
#define __UNION_MEMORY_H__
#pragma warning(push)
#pragma warning(disable:4595)

#include "Types.h"
#include <iostream>

namespace Union {
#if (defined(_UNION_API_LIB) || defined(_UNION_API_DLL)) && !defined(_UNION_API_BUILD)
#pragma comment(lib,"UnionApi.lib")
#endif
  void* CreateSharedSingleton( const char* globalName, void* (*allocation)() );
  void FreeSharedSingleton( const char* globalName, void (*destructor)(void*) );

  void* MemAlloc( size_t size );
  void* MemCalloc( size_t count, size_t size );
  void* MemRealloc( void* memory, size_t size );
  void MemFree( void* memory );
  void MemDelete( void* memory );
  size_t MemSize( void* memory );
}


#if 1
inline void* operator new (size_t size) {
  return Union::MemAlloc( size );
}


inline void* operator new []( size_t size ) {
  return Union::MemAlloc( size );
}


inline void operator delete (void* memory) {
  Union::MemFree( memory );
}


inline void operator delete []( void* memory ) {
  Union::MemFree( memory );
}
#pragma warning(pop)
#endif
#endif // __UNION_MEMORY_H__