

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Nov 29 23:07:58 2016
 */
/* Compiler settings for linkom_atl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __linkom_atl_i_h__
#define __linkom_atl_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILinkom_FWD_DEFINED__
#define __ILinkom_FWD_DEFINED__
typedef interface ILinkom ILinkom;

#endif 	/* __ILinkom_FWD_DEFINED__ */


#ifndef __Linkom_FWD_DEFINED__
#define __Linkom_FWD_DEFINED__

#ifdef __cplusplus
typedef class Linkom Linkom;
#else
typedef struct Linkom Linkom;
#endif /* __cplusplus */

#endif 	/* __Linkom_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ILinkom_INTERFACE_DEFINED__
#define __ILinkom_INTERFACE_DEFINED__

/* interface ILinkom */
/* [unique][nonextensible][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_ILinkom;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("601BF357-E6D9-4E3E-8DEC-908E559A9D69")
    ILinkom : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetSyntax( 
            /* [in] */ BSTR syntaxSpec,
            /* [out] */ BSTR *invalidToken,
            /* [retval][out] */ unsigned long *reasonCode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Parse( 
            /* [in] */ BSTR commandLine,
            /* [out] */ BSTR *invalidArg,
            /* [retval][out] */ unsigned long *reasonCode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsParamPresent( 
            /* [in] */ BSTR paramName,
            /* [retval][out] */ BOOL *bPresent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetParamValue( 
            /* [in] */ BSTR paramName,
            /* [out] */ BSTR *paramValue,
            /* [retval][out] */ unsigned long *reasonCode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetExtErrorString( 
            /* [retval][out] */ BSTR *errString) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILinkomVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILinkom * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILinkom * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILinkom * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetSyntax )( 
            ILinkom * This,
            /* [in] */ BSTR syntaxSpec,
            /* [out] */ BSTR *invalidToken,
            /* [retval][out] */ unsigned long *reasonCode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Parse )( 
            ILinkom * This,
            /* [in] */ BSTR commandLine,
            /* [out] */ BSTR *invalidArg,
            /* [retval][out] */ unsigned long *reasonCode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsParamPresent )( 
            ILinkom * This,
            /* [in] */ BSTR paramName,
            /* [retval][out] */ BOOL *bPresent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetParamValue )( 
            ILinkom * This,
            /* [in] */ BSTR paramName,
            /* [out] */ BSTR *paramValue,
            /* [retval][out] */ unsigned long *reasonCode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetExtErrorString )( 
            ILinkom * This,
            /* [retval][out] */ BSTR *errString);
        
        END_INTERFACE
    } ILinkomVtbl;

    interface ILinkom
    {
        CONST_VTBL struct ILinkomVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILinkom_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILinkom_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILinkom_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILinkom_SetSyntax(This,syntaxSpec,invalidToken,reasonCode)	\
    ( (This)->lpVtbl -> SetSyntax(This,syntaxSpec,invalidToken,reasonCode) ) 

#define ILinkom_Parse(This,commandLine,invalidArg,reasonCode)	\
    ( (This)->lpVtbl -> Parse(This,commandLine,invalidArg,reasonCode) ) 

#define ILinkom_IsParamPresent(This,paramName,bPresent)	\
    ( (This)->lpVtbl -> IsParamPresent(This,paramName,bPresent) ) 

#define ILinkom_GetParamValue(This,paramName,paramValue,reasonCode)	\
    ( (This)->lpVtbl -> GetParamValue(This,paramName,paramValue,reasonCode) ) 

#define ILinkom_GetExtErrorString(This,errString)	\
    ( (This)->lpVtbl -> GetExtErrorString(This,errString) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILinkom_INTERFACE_DEFINED__ */



#ifndef __Linkom_LIBRARY_DEFINED__
#define __Linkom_LIBRARY_DEFINED__

/* library Linkom */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Linkom;

EXTERN_C const CLSID CLSID_Linkom;

#ifdef __cplusplus

class DECLSPEC_UUID("AB8BC6BA-2478-4849-B4B3-058D4EDE3660")
Linkom;
#endif
#endif /* __Linkom_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


