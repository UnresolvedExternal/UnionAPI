#pragma once
#ifndef __UNION_SIGNATURE_H__
#define __UNION_SIGNATURE_H__

#include "Memory.h"
#include "Array.h"
#include "VDFS.h"
#include "String.h"

namespace Union {
#define SIGNATURE_OF(what) Signature(what, #what)

  /**
   * The class stores the information about the function in the signature context
   */
  class UNION_API Signature {
    friend class SignatureFile;
    StringANSI Address;
    StringANSI Type;
    StringANSI CallingConvention;
    StringANSI ClassName;
    StringANSI FunctionName;
    Array<StringANSI> ArgumentsTypes;

    static void ClearTypeName( StringANSI& typeName );
  public:
    Signature();
    /**
     * @brief Creates signature using a pattern
     * @param detailsPtr The base funciton for which the signature will be created
     * @param baseStorageClass Defines the final storage class of the detailed function
     */
    template<typename T>
    Signature( T detailsPtr, const StringANSI& baseStorageClass );
    /**
     * @brief Returns address of the associated with this signature function
     * @return Address of the associated function
     */
    void* GetAddress() const;
    /**
     * @brief Returns readable instance of this signature
     * @return ANSI String
     */
    StringANSI ToString() const;
    /**
     * @brief Returns found compatible signature from the specified file
     * @param signature Base signature for which to search
     * @param fileName In which file to search
     * @return The pointer to compatible signature or nullptr if not found
     */
    static Signature* GetCompatibleSignature( Signature* signature, const String& fileName );
    /**
     * @brief Returns the sortion direction based on the comparison of the two signature pointers
     * @param l The left signature
     * @param r The right signature
     * @return The sortion direction
     */
    static int SortionFunc( Signature* const& l, Signature* const& r );
  };


  /**
   * The class containts list of the functions information in the signature context
   */
  class UNION_API SignatureFile {
    String FileName;
    Array<Signature*> Signatures;

    bool LoadFromFile( const String& fileName );
    static Array<SignatureFile*>& GetSignatureFiles();
  public:
    /**
     * @brief Returns the file of signatures
     * @param fileName Name of the file
     * @return The file of signatures
     */
    static const Array<Signature*>& GetFileSignatures( const String& fileName );
  };


#if !defined(_UNION_API_DLL) || defined(_UNION_API_BUILD)
  inline int Signature::SortionFunc( Signature* const& l, Signature* const& r ) {
    int diff = l->Type.GetDifference( r->Type );
    if( diff != 0 )
      return diff;

    diff = l->ClassName.GetDifference( r->ClassName );
    if( diff != 0 )
      return diff;

    diff = l->FunctionName.GetDifference( r->FunctionName );
    if( diff != 0 )
      return diff;

    diff = l->ArgumentsTypes.GetCount() - r->ArgumentsTypes.GetCount();
    if( diff != 0 )
      return diff;

    for( uint i = 0; i < l->ArgumentsTypes.GetCount(); i++ ) {
      diff = l->ArgumentsTypes[i].GetDifference( r->ArgumentsTypes[i] );
      if( diff != 0 )
        return diff;
    }
   
    return 0;
  }


  inline Signature* Signature::GetCompatibleSignature( Signature* signature, const String& fileName ) {
    Array<Signature*> signatures = SignatureFile::GetFileSignatures( fileName );
    uint index = signatures.IndexOf<Signature::SortionFunc>( signature );
    if( index == -1 )
      return nullptr;

    Signature* found = signatures[index];
    if( found->CallingConvention != signature->CallingConvention ) {
      StringANSI::Format( "Incompatible calling conventions.\n  Engine function: %s\n  Plugin function: %s",
        found->ToString(), signature->ToString() ).StdPrintLine();
      return nullptr;
    }

    return found;
  }


  inline void Signature::ClearTypeName( StringANSI& typeName ) {
    typeName.Replace( " &", "& " );
    typeName.Replace( " *", "* " );
    typeName.Replace( "class ", "" );
    typeName.Replace( "struct ", "" );
    typeName.Replace( "enum \\w+", "enum" );
    typeName.RegexReplace( "\\w+\\:\\:", "" );
    typeName.Trim( " \t()" );
  }


  template<typename T>
  inline Signature::Signature( T detailsPtr, const StringANSI& baseStorageClass ) {
    StringANSI typeId = typeid(T).name();
    Type = typeId.RegexSearch( "^.+?\\(" );
    ClearTypeName( Type );
    StringANSI body = typeId.RegexSearch( "\\(__.+?\\*\\)" );
    CallingConvention = body.RegexSearch( "__\\w+" );
    ClassName = body.RegexSearch( "\\w+\\:\\:\\*" );
    ClassName.Trim( ":*" );
    StringANSI arguments = typeId.RegexSearch( "\\)\\(.+?\\)" );
    ArgumentsTypes = arguments.Split( ',' );
    for( auto& argument : ArgumentsTypes )
      ClearTypeName( argument );

    FunctionName = baseStorageClass.RegexSearch( "\\w+$" );
    FunctionName.Trim( " &:" );

    StringANSI className = baseStorageClass.RegexSearch( "\\w+\\:\\:" );
    className.Trim( " &:" );
    
    if( ClassName.IsEmpty() && !className.IsEmpty() ) {
      ClassName = className;

      // Check ECX argument
      if( ArgumentsTypes.GetCount() > 0 && ArgumentsTypes[0] == StringANSI::Format( "%s*", className ) ) {
        ArgumentsTypes.RemoveAt( 0 );

        // Check __fastcall is __thiscall
        if( ArgumentsTypes.GetCount() > 0 && ArgumentsTypes[0] == "void*" ) {
          if( CallingConvention == "__fastcall" ) {
            CallingConvention = "__thiscall";
            ArgumentsTypes.RemoveAt( 0 );
          }
        }
      }
    }
    
    if( ClassName != className )
      String::Format( "Incompatible classes in the signature: %s, %s", ClassName, className ).StdPrintLine();
  }
  
  
  inline Signature::Signature() {

  }


  void* Signature::GetAddress() const {
    return !Address.IsEmpty() ? Address.ConvertToVoidPtr() : nullptr;
  }


  StringANSI Signature::ToString() const {
    StringANSI out;
    out.Insert( Type ).Insert( " " );
    out.Insert( CallingConvention ).Insert( " " );
    if( !ClassName.IsEmpty() )
      out.Insert( ClassName ).Insert( "::" );
    out.Insert( FunctionName ).Insert( "(" );
    for( uint i = 0; i < ArgumentsTypes.GetCount(); i++ ) {
      const StringANSI& arg = ArgumentsTypes[i];
      if( i > 0 ) out.Insert( ", " );
      out.Insert( arg );
    }
    out.Insert( ")" );
    return out;
  }


  inline Array<SignatureFile*>& SignatureFile::GetSignatureFiles() {
    Array<SignatureFile*>* signatureFiles =
      (Array<SignatureFile*>*)CreateSharedSingleton( "SignatureFiles", []() -> void* { return new Array<SignatureFile*>(); } );
    return *signatureFiles;
  }


  inline bool SignatureFile::LoadFromFile( const String& fileName ) {
    StringANSI fileData;
    if( !fileData.ReadFromVDFS( fileName ) || fileData.IsEmpty() ) // TODO VDFS
      return false;

    Array<StringANSI> fileRows = fileData.Split( "\r\n" );
    if( fileRows.IsEmpty() )
      return false;

    for( auto& row : fileRows ) {
      Array<StringANSI> cells = row.Split( '\t', false );
      if( cells.GetCount() < 5 )
        continue;

      Signature* signature = new Signature();
      signature->Address = cells[0];
      signature->Type = cells[1];
      signature->CallingConvention = cells[2];
      signature->ClassName = cells[3];
      signature->FunctionName = cells[4];
      for( uint i = 5; i < cells.GetCount(); i++ )
        signature->ArgumentsTypes.Insert( cells[i] );

      Signatures.Insert( signature );
    }

    Signatures.QuickSort<Signature::SortionFunc>();
    return !Signatures.IsEmpty();
  }


  inline const Array<Signature*>& SignatureFile::GetFileSignatures( const String& fileName ) {
    static Array<SignatureFile*>& signatureFiles = GetSignatureFiles();
    for( auto file : signatureFiles )
      if( file->FileName == fileName )
        return file->Signatures;

    SignatureFile* file = new SignatureFile();
    file->LoadFromFile( fileName );
    signatureFiles.Insert( file );
    return file->Signatures;
  }
#endif
}

#endif // __UNION_SIGNATURE_H__