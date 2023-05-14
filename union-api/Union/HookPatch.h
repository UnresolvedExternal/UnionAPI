#pragma once
#ifndef __UNION_HOOK_PATCH_H__
#define __UNION_HOOK_PATCH_H__

#ifdef _COMPILE_STATIC_LIB_
#define DETOURS_INTERNAL
#endif
#include <Windows.h>
#include "Array.h"
#include "Memory.h"
#include "HookProto.h"
#include "ModuleImm32Collection.h"

#if !defined(_UNION_API_DLL) || defined(_UNION_API_BUILD)
#include "Thirdparty/Detours.h"
#endif

namespace Union {
  class UNION_API HookProviderPatch : public HookProvider {
    friend class UnionDll;

    void* OriginalPtr;    // Hook from
    void* DestinationPtr; // Hook to
    void* DetoursPtr;     // Where to return
    Array<void*> Addresses;
    Array<void*> Offsets;
    HookProviderPatch* Prev;
    HookProviderPatch* Next;

    static Array<HookProviderPatch*> GetHookList();
    static HookProviderPatch* GetHookTree( void* ptr );
    HookProviderPatch* GetFirst();
    HookProviderPatch* GetLast();
    void Attach();
    void Detach();
    static void PatchAddress( void* where, void* what );
    static void PatchOffset( void* where, void* what );
    static void ModuleInitialized( HMODULE module );
  public:
    HookProviderPatch();
    virtual bool IsEnabled();
    virtual bool Enable( void* originPtr, void* destPtr );
    virtual bool Enable();
    virtual bool Disable();
    virtual void* GetReturnAddress();
  };


#if !defined(_UNION_API_DLL) || defined(_UNION_API_BUILD)
  inline HookProviderPatch::HookProviderPatch() {
    OriginalPtr = nullptr;
    DestinationPtr = nullptr;
    DetoursPtr = nullptr;
    Prev = nullptr;
    Next = nullptr;
  }


  inline Array<HookProviderPatch*> HookProviderPatch::GetHookList() {
    static Array<HookProviderPatch*>* hooks =
      (Array<HookProviderPatch*>*)CreateSharedSingleton( "PatchHookList", []() -> void* { return new Array<HookProviderPatch*>(); } );
    return hooks->Share();
  }


  inline HookProviderPatch* HookProviderPatch::GetFirst() {
    return Prev ? Prev->GetFirst() : this;
  }


  inline HookProviderPatch* HookProviderPatch::GetLast() {
    return Next ? Next->GetLast() : this;
  }


  inline void HookProviderPatch::Attach() {
    for( auto address : Addresses )
      PatchAddress( address, DestinationPtr );

    for( auto offset : Offsets )
      PatchOffset( offset, DestinationPtr );
  }


  inline void HookProviderPatch::Detach() {
    if( Prev ) {
      Addresses.Swap( Prev->Addresses );
      Offsets.Swap( Prev->Offsets );
      Prev->Attach();
    }
    else {
      for( auto address : Addresses )
        PatchAddress( address, OriginalPtr );

      for( auto offset : Offsets )
        PatchOffset( offset, OriginalPtr );
    }
  }


  inline void HookProviderPatch::PatchAddress( void* where, void* what ) {
    ulong protection = PAGE_EXECUTE_READWRITE;
    VirtualProtect( where, 4, protection, &protection );
    *(void**)where = what;
    VirtualProtect( where, 4, protection, &protection );
  }


  inline void HookProviderPatch::PatchOffset( void* where, void* what ) {
    ulong protection = PAGE_EXECUTE_READWRITE;
    VirtualProtect( where, 4, protection, &protection );
    uint& from   =  (uint&)where;
    uint& offset = *(uint*)where;
    uint& to     =  (uint&)what;
    offset = to - from - 4;
    VirtualProtect( where, 4, protection, &protection );
  }


  inline void HookProviderPatch::ModuleInitialized( HMODULE module ) {
    auto& pcc = ProcessImm32Collection::GetInstance();
    auto hooks = GetHookList();
    for( auto hook : hooks ) {
      Array<void*> addresses, offsets;
      auto node = hook->GetLast();
      pcc.GetImm32For( node->OriginalPtr, addresses, offsets );
      
      for( auto address : addresses ) {
        PatchAddress( address, node->DestinationPtr );
        node->Addresses.Insert( address );
      }

      for( auto offset : offsets ) {
        PatchOffset( offset, node->DestinationPtr );
        node->Offsets.Insert( offset );
      }
    }
  }


  inline HookProviderPatch* HookProviderPatch::GetHookTree( void* ptr ) {
    auto hooks = GetHookList();
    for( auto hook : hooks )
      if( hook->OriginalPtr == ptr )
        return hook;

    return nullptr;
  }


  inline bool HookProviderPatch::IsEnabled() {
    return DetoursPtr != nullptr;
  }


  inline bool HookProviderPatch::Enable( void* from, void* to ) {
    if( from == nullptr || to == nullptr )
      return false;

    if( IsEnabled() )
      return true;

    OriginalPtr = from;
    DestinationPtr = to;
    DetoursPtr = from;

    HookProviderPatch* tree = GetHookTree( from );
    if( tree ) {
      tree = tree->GetLast();
      tree->Next = this;
      this->Prev = tree;
      DetoursPtr = tree->DestinationPtr;
      Addresses.Swap( tree->Addresses );
      Offsets.Swap( tree->Offsets );
    }
    else {
      GetHookList().Insert( this );
      Addresses.Clear(); // Just in case
      Offsets.Clear();   // Just in case
      ProcessImm32Collection::GetInstance().GetImm32For( from, Addresses, Offsets );
    }

    Attach();
    return true;
  }


  inline bool HookProviderPatch::Enable() {
    return Enable( OriginalPtr, DestinationPtr );
  }


  inline bool HookProviderPatch::Disable() {
    if( !IsEnabled() )
      return false;

    if( !Prev ) {
      GetHookList().Remove( this );
      if( Next )
        GetHookList().Insert( Next );
    }
    else
      Prev->Next = Next;
    

    if( Next ) {
      Next->Prev = Prev;
      Next->DetoursPtr = DetoursPtr;
    }
    else
      Detach();

    // Clear a hook information
    DetoursPtr = nullptr;
    Prev = nullptr;
    Next = nullptr;
    return true;
  }


  inline void* HookProviderPatch::GetReturnAddress() {
    return DetoursPtr;
  }
#endif
}

#endif // __UNION_HOOK_PATCH_H__