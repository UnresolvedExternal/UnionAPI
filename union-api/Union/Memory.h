#pragma once
#ifndef __UNION_MEMORY_H__
#define __UNION_MEMORY_H__

namespace Union {
#if defined(_UNION_API_LIB) && !defined(_UNION_API_BUILD)
#pragma comment(lib,"UnionApi.lib")
#endif
  void* CreateSharedSingleton( const char* globalName, void* (*allocation)() );
  void* FreeSharedSingleton( const char* globalName, void* (*allocation)() );

  void* MemAlloc( size_t size );
  void* MemCalloc( size_t count, size_t size );
  void* MemRealloc( void* memory, size_t size );
  void MemFree( void* memory );
  void MemDelete( void* memory );
  size_t MemSize( void* memory );
}

void* operator new (size_t size);
void* operator new[]( size_t size );
void operator delete (void* memory);
void operator delete[]( void* memory );
#endif // __UNION_MEMORY_H__