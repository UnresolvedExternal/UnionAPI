#pragma once
#ifndef __UNION_HOOK_H__
#define __UNION_HOOK_H__

#include "HookDetours.h"
#include "HookPatch.h"
#include "HookPartial.h"

namespace Union {
  template<typename Signature>
  inline Hook<Signature> CreateHook( void* originPtr, Signature destPtr, HookType type = HookType::Hook_Auto ) {
    HookProvider* provider = nullptr;

    if( provider == nullptr && type == HookType::Hook_Auto || type == HookType::Hook_CallPatch ) {
      provider = new HookProviderPatch();
      if( !provider->Enable( originPtr, *(void**)&destPtr ) ) {
        delete provider;
        provider = nullptr;
      }
    }

    if( provider == nullptr && type == HookType::Hook_Auto || type == HookType::Hook_Detours ) {
      provider = new HookProviderDetours();
      if( !provider->Enable( originPtr, *(void**)&destPtr ) ) {
        delete provider;
        provider = nullptr;
      }
    }

    return Hook<Signature>( provider );
  }


  inline Hook<PartialHookProc> CreatePartialHook( void* whereFrom, PartialHookProc whereTo ) {
    HookProviderPartial* provider = new HookProviderPartial();
    provider->Enable( whereFrom, whereTo );
    return Hook<PartialHookProc>( provider );
  }


  inline Hook<PartialHookProc> CreatePartialHook( void* whereFrom, PartialHookNoRegsProc whereTo ) {
    return CreatePartialHook( whereFrom, (PartialHookProc)whereTo );
  }
}

#endif // __UNION_HOOK_H__