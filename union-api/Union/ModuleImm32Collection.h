#pragma once
#ifndef __UNION_PROCESS_IMM32_COLLECTION_H__
#define __UNION_PROCESS_IMM32_COLLECTION_H__
#pragma warning(push)
#pragma warning(disable:4251)

#ifdef _UNION_API_LIB
#define DETOURS_INTERNAL
#endif
#include "Array.h"
#include "String.h"
#include "Memory.h"
#if !defined(_UNION_API_DLL) || defined(_UNION_API_BUILD)
#include "Thirdparty/Detours.h"
#endif

#include <Windows.h>
#include <dbghelp.h>
#include <Psapi.h>
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "psapi.lib")

namespace Union {
  class UNION_API ProcessImm32Collection {
    struct SegmentInfo {
      char Name[8];
      void* BaseAddress;
      size_t Size;
      bool Is( const char* name ) {
        return strcmp( Name, name ) == 0;
      }
    };

    struct ModuleImm32Collection {
      HMODULE Module;
      void* BaseAddress;
      size_t Size;
      Array<SegmentInfo*> Segments;
      Array<void*> Addresses;
      Array<void*> Offsets;
      Array<void*> XCalls;
    };

    Array<ModuleImm32Collection*> ModuleImm32Collections;

    ProcessImm32Collection();
    void AnalizeText( SegmentInfo* segment, OUT Array<void*>& addresses, OUT Array<void*>& offsets );
    void AnalizeData( SegmentInfo* segment, OUT Array<void*>& addresses );
    void AnalizeTextXCalls( SegmentInfo* segment, OUT Array<void*>& xcalls );
    void CollectSegments( ModuleImm32Collection* moduleImm32 );
    void FillModuleInfo( ModuleImm32Collection* moduleImm32 );
    void GetImm32For( void* address, ModuleImm32Collection* moduleImm32, OUT Array<void*>& addresses, OUT Array<void*>& offsets );
  public:
    void AnalizeModule( HMODULE module );
    void GetImm32For( void* address, HMODULE module, OUT Array<void*>& addresses, OUT Array<void*>& offsets );
    void GetImm32For( void* address, OUT Array<void*>& addresses, OUT Array<void*>& offsets );
    static ProcessImm32Collection& GetInstance();
    static ProcessImm32Collection* CreateSpecificCollection();
  };
  

#if !defined(_UNION_API_DLL) || defined(_UNION_API_BUILD)
  inline ProcessImm32Collection::ProcessImm32Collection() {
    StringANSI( "Creating new ProcessImm32Collection . . ." ).StdPrintLine();
    AnalizeModule( GetModuleHandle( nullptr ) );
  }


  inline void ProcessImm32Collection::AnalizeText( SegmentInfo* segment, OUT Array<void*>& addresses, OUT Array<void*>& offsets ) {
    byte* it = (byte*)segment->BaseAddress;
    byte* end = it + segment->Size - 5;
    StringANSI::Format( "Analizing %t[%x]: %x-%x", segment->Name, segment->Size, it, end ).StdPrintLine();

    while( it < end ) {
      byte instruction = *it;

           if( instruction == 0xFF ) addresses.Insert( it + 2 ); // Call
      else if( instruction == 0x8B ) addresses.Insert( it + 2 ); // Mov
      else if( instruction == 0xA1 ) addresses.Insert( it + 1 ); // Mov
      else if( instruction == 0xB8 ) addresses.Insert( it + 1 ); // Jmp
      else if( instruction == 0x68 ) addresses.Insert( it + 1 ); // Push
      else if( instruction >= 0xB8 &&
               instruction <= 0xBF ) addresses.Insert( it + 1 ); // Mov

      else if( instruction == 0xE8 ) offsets.Insert( it + 1 ); // Call
      else if( instruction == 0xE9 ) offsets.Insert( it + 1 ); // Jmp
      else if( instruction == 0x68 ) offsets.Insert( it + 1 ); // Push

      it = (byte*)DetourCopyInstruction( nullptr, nullptr, it, nullptr, nullptr );
    }
  }


  inline void ProcessImm32Collection::AnalizeData( SegmentInfo* segment, OUT Array<void*>& addresses ) {
    byte* it = (byte*)segment->BaseAddress;
    byte* end = (byte*)segment->BaseAddress + segment->Size - 4;
    StringANSI::Format( "Analizing %t[%x]: %x-%x", segment->Name, segment->Size, it, end ).StdPrintLine();
    while( it < end ) {
      addresses.Insert( it );
      it += sizeof( void* );
    }
  }


  inline bool IsXCallPtr( const byte* mem ) {
    const byte XCallPtr[] = { 0x8B, 0xE5, 0x5D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };

    for( uint i = 0; i < 4; i++ )
      if( mem[i] != XCallPtr[i] )
        return false;

    for( uint i = 8; i < 10; i++ )
      if( mem[i] != XCallPtr[i] )
        return false;

    return true;
  }


  inline void ProcessImm32Collection::AnalizeTextXCalls( SegmentInfo* segment, OUT Array<void*>& xcalls ) {
    byte* it = (byte*)segment->BaseAddress;
    byte* end = (byte*)segment->BaseAddress + segment->Size - 10;
    StringANSI::Format( "Analizing %t[%x]: %x-%x", segment->Name, segment->Size, it, end ).StdPrintLine();
    while( it < end ) {
      byte instruction = *it;
      if( IsXCallPtr( it ) )
        xcalls.Insert( it );

      it = (byte*)DetourCopyInstruction( nullptr, nullptr, it, nullptr, nullptr );
    }
  }


  inline void ProcessImm32Collection::CollectSegments( ModuleImm32Collection* moduleImm32 ) {
    uint moduleBase = (uint)moduleImm32->Module;
    IMAGE_NT_HEADERS* ntHeader = ImageNtHeader( moduleImm32->Module );
    PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION( ntHeader );
    uint segments = ntHeader->FileHeader.NumberOfSections;
    for( uint i = 0; i < segments; i++ ) {
      SegmentInfo* segmentInfo = new SegmentInfo();
      memcpy( segmentInfo->Name, sectionHeader->Name, 8 );
      segmentInfo->BaseAddress = (void*)(moduleBase + sectionHeader->VirtualAddress);
      segmentInfo->Size = sectionHeader->Misc.VirtualSize;
      moduleImm32->Segments.Insert( segmentInfo );
      sectionHeader++;
    }
  }


  // TODO: collect inline assembler memory when it will be done
  inline void ProcessImm32Collection::FillModuleInfo( ModuleImm32Collection* moduleImm32 ) {
    HMODULE module = moduleImm32->Module;
    MODULEINFO sysModuleInfo;
    memset( &sysModuleInfo, 0, sizeof( sysModuleInfo ) );
    if( GetModuleInformation( GetCurrentProcess(), module, &sysModuleInfo, sizeof( sysModuleInfo ) ) == 0 )
      return;
    
    moduleImm32->BaseAddress = sysModuleInfo.lpBaseOfDll;
    moduleImm32->Size = sysModuleInfo.SizeOfImage;

    CollectSegments( moduleImm32 );

    bool isUnionModule = GetProcAddress( module, "UnionSharedMemoryInstance" );
    for( auto&& segment : moduleImm32->Segments ) {
      if( !isUnionModule ) {
        // Analize all instructions which
        // theoretically can has imm32 in
        // the address or offset context.
        if( segment->Is( ".text" ) ) {
          AnalizeText( segment, moduleImm32->Addresses, moduleImm32->Offsets );
        }
        // Analize all integer values
        // which may look like addresses
        else if( segment->Is( ".data" ) || segment->Is( ".idata" ) || segment->Is( ".rdata" ) ) {
          AnalizeData( segment, moduleImm32->Addresses );
        }
      }
      else {
        // Analize all instructions
        // which are xcall commands
        if( segment->Is( ".text" ) ) {
          AnalizeTextXCalls( segment, moduleImm32->XCalls );
        }
      }
    }
  }


  inline void ProcessImm32Collection::AnalizeModule( HMODULE module ) {
    for( auto moduleImm32 : ModuleImm32Collections )
      if( moduleImm32->Module == module )
        return;
    
    char moduleName[1024 + 1];
    GetModuleFileNameA( module, moduleName, 1024 );
    StringANSI::Format( "Analyzing module: %t", moduleName ).StdPrintLine();

    ModuleImm32Collection* moduleImm32 = new ModuleImm32Collection();
    moduleImm32->Module = module;
    FillModuleInfo( moduleImm32 );
    ModuleImm32Collections.Insert( moduleImm32 );

    StringANSI::Format( "  Addresses: %i\n  Offsets: %i\n  XCalls: %i", 
      moduleImm32->Addresses.GetCount(), moduleImm32->Offsets.GetCount(), moduleImm32->XCalls.GetCount() ).StdPrintLine();
  }


  inline ProcessImm32Collection& ProcessImm32Collection::GetInstance() {
    static ProcessImm32Collection* instance =
      (ProcessImm32Collection*)CreateSharedSingleton( "ProcessImm32Collection", []() -> void* { return new ProcessImm32Collection(); } );
    return *instance;
  }


  inline ProcessImm32Collection* ProcessImm32Collection::CreateSpecificCollection() {
    return new ProcessImm32Collection();
  }


  inline void ProcessImm32Collection::GetImm32For( void* target, ModuleImm32Collection* moduleImm32, OUT Array<void*>& addresses, OUT Array<void*>& offsets ) {
    StringANSI::Format( "Getting Imm32 for %x", target ).StdPrintLine();
    for( auto imm32 : moduleImm32->Addresses )
      if( *(void**)imm32 == target )
        addresses.Insert( imm32 );

    for( auto imm32 : moduleImm32->XCalls )
      if( *(void**)imm32 == target )
        addresses.Insert( imm32 );

    for( auto imm32 : moduleImm32->Offsets ) {
      uint from   =  (uint) imm32;
      uint offset = *(uint*)imm32;
      uint to     =  (uint) target;

      if( to - from - 4 == offset )
        offsets.Insert( imm32 );
    }

    StringANSI::Format( "Found\n  addresses: %i\n  offsets: %i",
      addresses.GetCount(), offsets.GetCount() ).StdPrintLine();
  }


  inline void ProcessImm32Collection::GetImm32For( void* address, HMODULE module, OUT Array<void*>& addresses, OUT Array<void*>& offsets ) {
    AnalizeModule( module );
    for( auto moduleImm32 : ModuleImm32Collections )
      if( moduleImm32->Module == module )
        return GetImm32For( address, moduleImm32, addresses, offsets );
  }


  inline void ProcessImm32Collection::GetImm32For( void* address, OUT Array<void*>& addresses, OUT Array<void*>& offsets ) {
    for( auto moduleImm32 : ModuleImm32Collections )
      return GetImm32For( address, moduleImm32, addresses, offsets );
  }
#endif
}
#pragma warning(pop)
#endif // __UNION_PROCESS_IMM32_COLLECTION_H__