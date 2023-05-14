#pragma once
#ifndef __UNION_HOOK_PROTO_H__
#define __UNION_HOOK_PROTO_H__
#include "Types.h"

namespace Union {
  struct AnyPtr {
    void* VoidPtr;

    template<typename T>
    AnyPtr( T ptr ) {
      VoidPtr = *(void*)&ptr;
    }

    operator void* () {
      return VoidPtr;
    }
  };


  enum class HookType {
    Hook_Auto,
    Hook_Detours,
    Hook_CallPatch
  };


  class UNION_API HookProvider {
  public:
    virtual bool IsEnabled() = 0;
    virtual bool Enable( void* originPtr, void* destPtr ) = 0;
    virtual bool Enable() = 0;
    virtual bool Disable() = 0;
    virtual void* GetReturnAddress() = 0;
  };


  template<typename Signature>
  class Hook {
    HookProvider* Provider;
    Hook() { }
  public:
    Hook( HookProvider* provider );
    Hook( const Hook& other );
    bool Enable( void* originPtr, void* destPtr );
    bool Enable();
    bool Disable();
    operator Signature();
  };


  template<typename Signature>
  Hook<Signature>::Hook( HookProvider* provider ) {
    Provider = provider;
  }


  template<typename Signature>
  Hook<Signature>::Hook( const Hook& other ) {
    Provider = other.Provider;
  }


  template<typename Signature>
  bool Hook<Signature>::Enable( void* originPtr, void* destPtr ) {
    return Provider->Enable( originPtr, destPtr );
  }


  template<typename Signature>
  bool Hook<Signature>::Enable() {
    return Provider->Enable();
  }


  template<typename Signature>
  bool Hook<Signature>::Disable() {
    return Provider->Disable();
  }


  template<typename Signature>
  Hook<Signature>::operator Signature() {
    // Hack for the x64 addresses on x86 arc
    uint64 ptr = (uint64)Provider->GetReturnAddress();
    return *(Signature*)&ptr;
  }
}

#endif // __UNION_HOOK_PROTO_H__