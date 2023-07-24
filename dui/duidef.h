#ifndef __DUIDEF_H__
#define __DUIDEF_H__


namespace DUI
{
/////////////////////////////////////////////////////////////////////////////
// Main version numbers

#define _DUI     1      // Active Template Library
#define _DUI_VER 0x0E00 // Active Template Library version 14.00

#ifndef _DUI_FILENAME_VER
#define _DUI_FILENAME_VER "140"
#endif

#ifndef _DUI_FILENAME_VER_NUM
#define _DUI_FILENAME_VER_NUM 140
#endif

#ifndef _DUI_VER_RBLD
#define _DUI_VER_RBLD "14.00"
#endif

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE         // UNICODE is used by Windows headers
#endif
#endif

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE        // _UNICODE is used by C-runtime/MFC headers
#endif
#endif

#ifndef DUIASSUME
#define DUIASSUME(expr)		do {} while(0)  //do { ATLASSERT(expr); _Analysis_assume_(!!(expr)); } __pragma(warning(suppress:4127)) while (0)
#endif // DUIASSUME

#ifndef DUIASSERT
#define DUIASSERT(expr) 
//#define DUIASSERT(expr) _ASSERTE(expr)
#endif // DUIASSERT

#ifdef _DUI_DISABLE_NO_VTABLE
#define DUI_NO_VTABLE
#else
#define DUI_NO_VTABLE __declspec(novtable)
#endif

#define DUIPREFAST_SUPPRESS(x) __pragma(warning(push)) __pragma(warning(disable: x))
#define DUIPREFAST_UNSUPPRESS() __pragma(warning(pop))

#ifndef DuiThrow
#ifndef _DUI_CUSTOM_THROW
#define DuiThrow
//#define DuiThrow DUI::DuiThrowImpl
#endif
#endif // DuiThrow

#ifndef DUIENSURE_THROW
#define DUIENSURE_THROW(expr, hr)          \
do {                                       \
	int __atl_condVal=!!(expr);            \
	DUIASSUME(__atl_condVal);              \
	if(!(__atl_condVal)) DuiThrow(hr);     \
} __pragma(warning(suppress:4127)) while (0)
#endif // DUIENSURE_THROW

#ifndef DUIENSURE
#define DUIENSURE(expr) DUIENSURE_THROW(expr, E_FAIL)
#endif // DUIENSURE


}
#endif /* __DUIDEF_H__ */



#if 0
/*====================================================================*/
// This is a part of the Active Template Library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#ifndef __DUIDEF_H__
#define __DUIDEF_H__

#pragma once

#pragma warning(disable : 4619)	// there is no warning number

// Check if building using WINAPI_FAMILY_APP
#ifndef _DUI_USE_WINAPI_FAMILY_DESKTOP_APP
#ifdef WINAPI_FAMILY
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#define _DUI_USE_WINAPI_FAMILY_DESKTOP_APP
#else // WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#ifdef WINAPI_FAMILY_PHONE_APP
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#define _DUI_USE_WINAPI_FAMILY_PHONE_APP
#endif // WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#endif // WINAPI_FAMILY_PHONE_APP
#ifdef WINAPI_FAMILY_APP
#if WINAPI_FAMILY == WINAPI_FAMILY_APP
#define _DUI_USE_WINAPI_FAMILY_APP
#endif // WINAPI_FAMILY == WINAPI_FAMILY_APP
#endif // WINAPI_FAMILY_APP
#endif // WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#else // WINAPI_FAMILY
// Default to Desktop family app
#define _DUI_USE_WINAPI_FAMILY_DESKTOP_APP
#endif // WINAPI_FAMILY
#endif // _DUI_USE_WINAPI_FAMILY_DESKTOP_APP

#ifndef _DUI_USE_WINAPI_FAMILY_DESKTOP_APP

// These are available for WINAPI_FAMILY_DESKTOP_APP only

#ifndef _DUI_NO_SERVICE
// No service supported
#define _DUI_NO_SERVICE
#endif

#ifndef _DUI_NO_COM_SUPPORT
// No COM support
#define _DUI_NO_COM_SUPPORT
#endif

#ifndef  _DUI_NO_COMMODULE
// No CComModule
#define _DUI_NO_COMMODULE
#endif

#ifndef _DUI_NO_WIN_SUPPORT
// No AtlWinModule support
#define _DUI_NO_WIN_SUPPORT
#endif

#endif // _DUI_USE_WINAPI_FAMILY_DESKTOP_APP

#include <atlrc.h>
#include <errno.h>
#include <stdlib.h>
#include <sal.h>

#ifndef _DUI_DISABLE_NOTHROW_NEW
#include <new.h>
#define _DUI_NEW		new(std::nothrow)
#else
#define _DUI_NEW		new
#endif

// preprocessor string helpers
#ifndef _DUI_STRINGIZE
#define __DUI_STRINGIZE(_Value) #_Value
#define _DUI_STRINGIZE(_Value) __DUI_STRINGIZE(_Value)
#endif

#ifndef _DUI_APPEND
#define __DUI_APPEND(_Value1, _Value2) _Value1 ## _Value2
#define _DUI_APPEND(_Value1, _Value2) __DUI_APPEND(_Value1, _Value2)
#endif

#ifndef RC_INVOKED

#if defined(_CHAR_UNSIGNED) && !defined(_DUI_ALLOW_CHAR_UNSIGNED)
#error ATL does not support compilation with /J or _CHAR_UNSIGNED flag enabled
#endif

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifdef UNDER_CE
	#error This version of ATL is not currently supported for CE. Look for the CE specific version.
#endif

// If you are mixing compilation units that are built as
// native code with those that are built /clr, you must define
// the symbol '_DUI_MIXED'. _DUI_MIXED must be defined for all
// compilation units in an executable or it must be defined for none of them.
#if defined(_M_CEE)
#ifdef _DUI_MIXED
#pragma detect_mismatch("_DUI_MIXED", "Defined")
#else
#pragma detect_mismatch("_DUI_MIXED", "Undefined")
#endif  // _DUI_MIXED
#endif  // defined(_M_CEE)

#if defined(_DUI_MIXED) || !defined(_M_CEE)

// Include the delete() operator
#if defined(_M_HYBRID)
#pragma comment(linker, "/include:??3@$$hYAXPAX@Z")
#elif defined _M_ARM64EC
#pragma comment(linker, "/include:??3@$$hYAXPEAX@Z")
#elif defined _M_IX86 || defined _M_ARM
#pragma comment(linker, "/include:??3@YAXPAX@Z")
#elif defined _M_X64 || defined _M_ARM64
#pragma comment(linker, "/include:??3@YAXPEAX@Z")
#else
#error Unsupported target architecture.
#endif
#ifndef _DUI_NATIVE_INITIALIZATION
#define _DUI_NATIVE_INITIALIZATION
#endif

#endif  // defined(_DUI_MIXED) || !defined(_M_CEE)


#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE         // UNICODE is used by Windows headers
#endif
#endif

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE        // _UNICODE is used by C-runtime/MFC headers
#endif
#endif

#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#if !defined(_DUI_USE_WINAPI_FAMILY_DESKTOP_APP) && !defined(_UNICODE)
#error _UNICODE has to be defined to use ATL under the current WINAPI_FAMILY
#endif

//PREFAST support static_assert from version 16.00
#if defined(_PREFAST_) && (_MSC_VER < 1600)
#define ATLSTATIC_ASSERT(expr, comment)
#else
#define ATLSTATIC_ASSERT(expr, comment)		static_assert(expr, comment)
#endif

#ifdef _WIN64
#define _DUI_SUPPORT_VT_I8  // Always support VT_I8 on Win64.
#endif

#ifndef DuiThrow
#ifndef _DUI_CUSTOM_THROW
#define DuiThrow ATL::AtlThrowImpl
#endif
#endif // DuiThrow

#ifndef ATLASSERT
#define ATLASSERT(expr) _ASSERTE(expr)
#endif // ATLASSERT

/*
Why does ATLASSUME exist?

ATL 8 has two existing validation models

ATLASSERT/ATLVERIFY - These are used to make sure a debug build reports a problem with the expression/invariant
DUIENSURE - Debug is the same as ATLVERIFY, retail throws a C++ exception

We added DUIENSURE because there were too many unreported error paths in ATL and we wanted to bail out of more
error conditions rather than just trying to continue in retail.

There might be a case for changing 'lots' of ATLASSERT to DUIENSURE, but we chose an incremental approach and only
changed over where we saw a problem with code reported from a customer or test case. This reduces code churn in our
code for this version.

In general, our approach is to try to make sure that when something goes wrong
- the client does not continue to run, because we report an error condition
- debug builds see an assertion about the problem

Sometimes we have code like

HRESULT ComMethod(void)
{
	ATLASSUME(m_pFoo);
	return m_pFoo->Method();
}

We could add
	if(!m_pFoo) return E_POINTER;

But this is very unlikely to help, since it removes the ability of the developer to debug this problem if it's seen in a retail
build of the application.

We could try something more severe

	if(!m_pFoo) terminate(); // or your favourite shutdown function

This would ensure good reporting (because VC8 terminate generates a Windows Error Report and crash dump), but hardly seems a big win
over the previous crash.

DUIENSURE might seem slightly better. It is debuggable and consistent with ATL in general. In fact, many parts of ATL do just this.
But in this specific context, it doesn't look like a great choice. COM methods should not in general be emitting native C++ exceptions
as an error reporting strategy.

So we find ourselves in a quandry. For these kinds of methods, the traditional code (ATLASSERT followed by a crash), seems be the most
debuggable thing to do in this situation. At least for VS8, we have decided to stick with this shape.

---

Now consider the impact of cl /analyze. We want cl /analyze to not warn about our potential dereferences when they refer to member variables
whose state was previously validated by another method. But we do want to see the impact of function contracts on the parameters of the
function.

So we've done a broad replace of all the member-related ATLASSERT to ATLASSUME.

*/

#ifndef ATLASSUME
#define ATLASSUME(expr) do { ATLASSERT(expr); _Analysis_assume_(!!(expr)); } __pragma(warning(suppress:4127)) while (0)
#endif // ATLASSUME

#ifndef ATLVERIFY
#ifdef _DEBUG
#define ATLVERIFY(expr) ATLASSERT(expr)
#else
#define ATLVERIFY(expr) (expr)
#endif // DEBUG
#endif // ATLVERIFY

#ifndef DUIENSURE_THROW
#define DUIENSURE_THROW(expr, hr)          \
do {                                       \
	int __atl_condVal=!!(expr);            \
	ATLASSUME(__atl_condVal);              \
	if(!(__atl_condVal)) DuiThrow(hr);     \
} __pragma(warning(suppress:4127)) while (0)
#endif // DUIENSURE_THROW

#ifndef DUIENSURE
#define DUIENSURE(expr) DUIENSURE_THROW(expr, E_FAIL)
#endif // DUIENSURE

#ifndef DUIENSURE_SUCCEEDED
#define DUIENSURE_SUCCEEDED(hrExpr)								\
do {															\
	HRESULT __atl_hresult = (hrExpr);							\
	DUIENSURE_THROW(SUCCEEDED(__atl_hresult), __atl_hresult);   \
} __pragma(warning(suppress:4127)) while (0)
#endif // DUIENSURE_SUCCEEDED

/* Used inside COM methods that do not want to throw */
#ifndef DUIENSURE_RETURN_VAL
#define DUIENSURE_RETURN_VAL(expr, val)        \
do {                                           \
	int __atl_condVal=!!(expr);                \
	ATLASSERT(__atl_condVal);                  \
	if(!(__atl_condVal)) return val;           \
} __pragma(warning(suppress:4127)) while (0)
#endif // DUIENSURE_RETURN_VAL

/* Used inside COM methods that do not want to throw */
#ifndef DUIENSURE_RETURN
#define DUIENSURE_RETURN(expr) DUIENSURE_RETURN_HR(expr, E_FAIL)
#endif // DUIENSURE_RETURN

/* Naming is slightly off in these macros
DUIENSURE_RETURN is an HRESULT return of E_FAIL
DUIENSURE_RETURN_VAL is any return value (function can pick)
DUIENSURE_RETURN_HR is HRESULT-specific, though currently the same as _VAL
*/
#ifndef DUIENSURE_RETURN_HR
#define DUIENSURE_RETURN_HR(expr, hr) DUIENSURE_RETURN_VAL(expr, hr)
#endif // DUIENSURE_RETURN_HR

#ifndef ATL_CRT_ERRORCHECK
#define ATL_CRT_ERRORCHECK(expr) AtlCrtErrorCheck(expr)
#endif // ATL_CRT_ERRORCHECK

#ifndef ATL_CRT_ERRORCHECK_SPRINTF
#define ATL_CRT_ERRORCHECK_SPRINTF(expr) \
do { \
	errno_t _saveErrno = errno; \
	errno = 0; \
	(expr); \
	if(0 != errno) \
	{ \
		AtlCrtErrorCheck(errno); \
	} \
	else \
	{ \
		errno = _saveErrno; \
	} \
} __pragma(warning(suppress:4127)) while (0)
#endif // ATL_CRT_ERRORCHECK_SPRINTF

///////////////////////////////////////////////////////////////////////////////
// __declspec(novtable) is used on a class declaration to prevent the vtable
// pointer from being initialized in the constructor and destructor for the
// class.  This has many benefits because the linker can now eliminate the
// vtable and all the functions pointed to by the vtable.  Also, the actual
// constructor and destructor code are now smaller.
///////////////////////////////////////////////////////////////////////////////
// This should only be used on a class that is not directly createable but is
// rather only used as a base class.  Additionally, the constructor and
// destructor (if provided by the user) should not call anything that may cause
// a virtual function call to occur back on the object.
///////////////////////////////////////////////////////////////////////////////
// By default, the wizards will generate new ATL object classes with this
// attribute (through the ATL_NO_VTABLE macro).  This is normally safe as long
// the restriction mentioned above is followed.  It is always safe to remove
// this macro from your class, so if in doubt, remove it.
///////////////////////////////////////////////////////////////////////////////

#ifdef _DUI_DISABLE_NO_VTABLE
#define ATL_NO_VTABLE
#else
#define ATL_NO_VTABLE __declspec(novtable)
#endif

#ifdef _DUI_DISABLE_NOTHROW
#define ATL_NOTHROW
#else
#define ATL_NOTHROW __declspec(nothrow)
#endif

#ifdef _DUI_DISABLE_FORCEINLINE
#define ATL_FORCEINLINE
#else
#define ATL_FORCEINLINE __forceinline
#endif

#ifdef _DUI_DISABLE_NOINLINE
#define ATL_NOINLINE
#else
#define ATL_NOINLINE __declspec( noinline )
#endif

#ifdef _DUI_DISABLE_DEPRECATED
#define ATL_DEPRECATED(_Message)
#else
#define ATL_DEPRECATED(_Message) __declspec( deprecated(_Message) )
#endif

#ifdef _DUI_DEBUG_REFCOUNT
#ifndef _DUI_DEBUG_INTERFACES
#define _DUI_DEBUG_INTERFACES
#endif
#endif

#ifdef _DEBUG
#ifndef _DUI_DEBUG
#define _DUI_DEBUG
#endif // _DUI_DEBUG
#endif // _DEBUG

#ifdef _DUI_DEBUG_INTERFACES
#ifndef _DUI_DEBUG
#define _DUI_DEBUG
#endif // _DUI_DEBUG
#endif // _DUI_DEBUG_INTERFACES

#ifndef _DUI_HEAPFLAGS
#ifdef _MALLOC_ZEROINIT
#define _DUI_HEAPFLAGS HEAP_ZERO_MEMORY
#else
#define _DUI_HEAPFLAGS 0
#endif
#endif

#define _DUI_PACKING 8

#define ATLAPI __declspec(nothrow) HRESULT __stdcall
#define ATLAPI_(x) __declspec(nothrow) x __stdcall
#define ATLAPIINL ATLAPI
#define ATLAPIINL_(x) ATLAPI_(x)
#define ATLINLINE inline

#ifdef _DUI_NO_EXCEPTIONS
	#ifdef _AFX
	#error MFC projects cannot define _DUI_NO_EXCEPTIONS
	#endif
#else
	#ifndef _CPPUNWIND
	#define _DUI_NO_EXCEPTIONS
	#endif
#endif

#ifdef _CPPUNWIND

#ifndef ATLTRYALLOC

#ifdef _AFX
#define ATLTRYALLOC(x) try{x;} catch(CException* e){e->Delete();}
#else
/* prefast noise VSW 489981 */
#define ATLTRYALLOC(x) __pragma(warning(push)) __pragma(warning(disable: 4571)) try{x;} catch(...) {} __pragma(warning(pop))
#endif	//__AFX

#endif	//ATLTRYALLOC

// If you define _DUITRY before including this file, then
// you should define _DUICATCH and _DUIRETHROW as well.
#ifndef _DUITRY
#define _DUITRY try
#ifdef _AFX
#define _DUICATCH( e ) catch( CException* e )
#else
#define _DUICATCH( e ) catch( CAtlException e )
#endif

#define _DUICATCHALL() __pragma(warning(push)) __pragma(warning(disable: 4571)) catch( ... ) __pragma(warning(pop))

#ifdef _AFX
#define _DUIDELETEEXCEPTION(e) e->Delete();
#else
#define _DUIDELETEEXCEPTION(e) e;
#endif

#define _DUIRETHROW throw
#endif	// _DUITRY

/*
COM functions should not throw. Which means we should protect their callers from C++ exceptions leaking out. These macros
can help with that, though they have not yet been applied to the whole of ATL, which uses a variety of patterns to achieve
this end
*/

#ifndef _DUI_COM_BEGIN
#define _DUI_COM_BEGIN \
	HRESULT __hrAtlComMethod = S_OK; \
	try \
	{
#endif

#ifdef _AFX
/* Nice to do something more complex here in future to translate an MFC exception to a better HR */
#define _AFX_COM_END_PART \
	catch(CException *e) \
	{ \
		if(e) \
		{ \
			e->Delete(); \
		} \
		__hrAtlComMethod = E_FAIL; \
	}
#else
#define _AFX_COM_END_PART \
	catch(CAtlException e) \
	{ \
		__hrAtlComMethod = e.m_hr; \
	}
#endif

#ifndef _DUI_COM_END
#define _DUI_COM_END \
	} \
	_AFX_COM_END_PART \
	catch(...) \
	{ \
		__hrAtlComMethod = E_FAIL; \
	} \
	return __hrAtlComMethod;
#endif



#else //_CPPUNWIND

#ifndef ATLTRYALLOC
#define ATLTRYALLOC(x) x;
#endif	//ATLTRYALLOC

// if _DUITRY is defined before including this file then
// _DUICATCH and _DUIRETHROW should be defined as well.
#ifndef _DUITRY
#define _DUITRY
#define _DUICATCH( e ) __pragma(warning(push)) __pragma(warning(disable: 4127)) if( false ) __pragma(warning(pop))
#define _DUICATCHALL() __pragma(warning(push)) __pragma(warning(disable: 4127)) if( false ) __pragma(warning(pop))
#define _DUIDELETEEXCEPTION(e)
#define _DUIRETHROW
#endif	// _DUITRY

#endif	//_CPPUNWIND

#ifndef ATLTRY
#define ATLTRY(x) ATLTRYALLOC(x)
#endif	//ATLTRY

#define offsetofclass(base, derived) ((DWORD_PTR)(static_cast<base*>((derived*)_DUI_PACKING))-_DUI_PACKING)

/////////////////////////////////////////////////////////////////////////////
// Main version numbers

#define _DUI     1      // Active Template Library
#define _DUI_VER 0x0E00 // Active Template Library version 14.00

#ifndef _DUI_FILENAME_VER
#define _DUI_FILENAME_VER "140"
#endif

#ifndef _DUI_FILENAME_VER_NUM
#define _DUI_FILENAME_VER_NUM 140
#endif

#ifndef _DUI_VER_RBLD
#define _DUI_VER_RBLD "14.00"
#endif

/////////////////////////////////////////////////////////////////////////////
// Threading

#ifndef _DUI_SINGLE_THREADED
#ifndef _DUI_APARTMENT_THREADED
#ifndef _DUI_FREE_THREADED
#define _DUI_FREE_THREADED
#endif
#endif
#endif

// UUIDOF
#ifndef _DUI_NO_UUIDOF
#define _DUI_IIDOF(x) __uuidof(x)
#else
#define _DUI_IIDOF(x) IID_##x
#endif

// Lean and mean
#ifndef ATL_NO_LEAN_AND_MEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMCX
#define NOMCX
#endif
#endif	// ATL_NO_LEAN_AND_MEAN

#ifdef NOSERVICE
#ifndef _DUI_NO_SERVICE
#define _DUI_NO_SERVICE
#endif	// _DUI_NO_SERVICE
#else
#ifdef _DUI_NO_SERVICE
#ifndef NOSERVICE
#define NOSERVICE
#endif	// NOSERVICE
#endif	// _DUI_NO_SERVICE
#endif	// NOSERVICE

#include <malloc.h>
#ifdef _DEBUG
#include <stdlib.h>
#endif
#ifndef _DUI_NO_DEBUG_CRT
// Warning: if you define the above symbol, you will have
// to provide your own definition of the ATLASSERT(x) macro
// in order to compile ATL
	#include <crtdbg.h>
#endif

#endif // RC_INVOKED

// Note : we can not use macros to generate the window class names because it
//        will require nested macros. rc.exe does not handle nested macros.
// #define ATLAXWIN_CLASS	_DUI_STRINGIZE(_DUI_APPEND(AtlAxWin, _DUI_FILENAME_VER_NUM))
// #define ATLAXWINLIC_CLASS	_DUI_STRINGIZE(_DUI_APPEND(AtlAxWinLic, _DUI_FILENAME_VER_NUM))

#define ATLAXWIN_CLASS "AtlAxWin140"
#define ATLAXWINLIC_CLASS "AtlAxWinLic140"

#if defined(_DUI_SECURE_NO_DEPRECATE) && !defined(_DUI_SECURE_NO_WARNINGS)
#define _DUI_SECURE_NO_WARNINGS
#endif

// _DUI_INSECURE_DEPRECATE define
#ifndef _DUI_INSECURE_DEPRECATE
#ifdef _DUI_SECURE_NO_WARNINGS
#define _DUI_INSECURE_DEPRECATE(_Message)
#else
#define _DUI_INSECURE_DEPRECATE(_Message) __declspec(deprecated(_Message))
#endif // _DUI_SECURE_NO_WARNINGS
#endif // _DUI_INSECURE_DEPRECATE

/*
This is called when something really bad happens -- so bad
that we consider it dangerous to even throw an exception
*/
#ifndef RC_INVOKED
 #if !defined(_DUI_FATAL_SHUTDOWN) && defined( _DUI_USE_WINAPI_FAMILY_DESKTOP_APP)
  #define _DUI_FATAL_SHUTDOWN do { ::TerminateProcess(::GetCurrentProcess(), 0); } __pragma(warning(suppress:4127)) while (0)
 #endif // _DUI_FATAL_SHUTDOWN
#endif // RC_INVOKED

//ATL/MFC code should use standard pointer to member standard syntax &MyClass::MyMethod, instead
//of the legacy non-standard syntax - MyMethod.
#ifdef _DUI_ENABLE_PTM_WARNING
#define PTM_WARNING_DISABLE
#define PTM_WARNING_RESTORE
#else
#define PTM_WARNING_DISABLE \
	__pragma(warning( push )) \
	__pragma(warning( disable : 4867 ))
#define PTM_WARNING_RESTORE \
	__pragma(warning( pop ))
#endif //_DUI_ENABLE_PTM_WARNING

/* we have to define our own versions of MAKEINTRESOURCE and IS_INTRESOURCE to
 * fix warning 6268. At least until those macros are not cleanend in PSDK.
   Same comes true for those definitions of constants which use the above macros
*/
#define ATL_MAKEINTRESOURCEA(i) ((LPSTR)((ULONG_PTR)((WORD)(i))))
#define ATL_MAKEINTRESOURCEW(i) ((LPWSTR)((ULONG_PTR)((WORD)(i))))
#ifdef UNICODE
#define ATL_MAKEINTRESOURCE  ATL_MAKEINTRESOURCEW
#else
#define ATL_MAKEINTRESOURCE  ATL_MAKEINTRESOURCEA
#endif // !UNICODE
#define ATL_IS_INTRESOURCE(_r) ((((ULONG_PTR)(_r)) >> 16) == 0)

#if _MSC_VER >= 1900 && !defined(__EDG__)
#define _DUI_DECLSPEC_ALLOCATOR __declspec(allocator)
#else
#define _DUI_DECLSPEC_ALLOCATOR
#endif

#ifndef ATL_IUNKNOWN_NOEXCEPT
#define ATL_IUNKNOWN_NOEXCEPT
#endif

/*
 * Predefined Resource Types
 */
#define ATL_RT_CURSOR           ATL_MAKEINTRESOURCE(1)
#define ATL_RT_BITMAP           ATL_MAKEINTRESOURCE(2)
#define ATL_RT_ICON             ATL_MAKEINTRESOURCE(3)
#define ATL_RT_MENU             ATL_MAKEINTRESOURCE(4)
#define ATL_RT_DIALOG           ATL_MAKEINTRESOURCE(5)
#define ATL_RT_STRING           ATL_MAKEINTRESOURCE(6)
#define ATL_RT_FONTDIR          ATL_MAKEINTRESOURCE(7)
#define ATL_RT_FONT             ATL_MAKEINTRESOURCE(8)
#define ATL_RT_ACCELERATOR      ATL_MAKEINTRESOURCE(9)
#define ATL_RT_RCDATA           ATL_MAKEINTRESOURCE(10)
#define ATL_RT_MESSAGETABLE     ATL_MAKEINTRESOURCE(11)

#define ATL_DIFFERENCE     11
#define ATL_RT_GROUP_CURSOR ATL_MAKEINTRESOURCE((ULONG_PTR)ATL_RT_CURSOR + ATL_DIFFERENCE)
#define ATL_RT_GROUP_ICON   ATL_MAKEINTRESOURCE((ULONG_PTR)ATL_RT_ICON + ATL_DIFFERENCE)
#define ATL_RT_VERSION      ATL_MAKEINTRESOURCE(16)
#define ATL_RT_DLGINCLUDE   ATL_MAKEINTRESOURCE(17)
#define ATL_RT_PLUGPLAY     ATL_MAKEINTRESOURCE(19)
#define ATL_RT_VXD          ATL_MAKEINTRESOURCE(20)
#define ATL_RT_ANICURSOR    ATL_MAKEINTRESOURCE(21)
#define ATL_RT_ANIICON      ATL_MAKEINTRESOURCE(22)
#define ATL_RT_HTML         ATL_MAKEINTRESOURCE(23)

#define DUIPREFAST_SUPPRESS(x) __pragma(warning(push)) __pragma(warning(disable: x))
#define ATLPREFAST_UNSUPPRESS() __pragma(warning(pop))

#ifndef _FormatMessage_format_string_
#define _FormatMessage_format_string_
#endif

/*
	Helper functions for SAL annotation
*/
namespace ATL {

DUIPREFAST_SUPPRESS(6001 6101)
template < typename T >
_Ret_maybenull_ _Post_writable_byte_size_(dwLen) inline __declspec(noalias) T* SAL_Assume_bytecap_for_opt_(
	_Out_writes_opt_(0) T* buf,
	_In_ size_t dwLen)
{
	(void)(dwLen);
	return buf;
}
ATLPREFAST_UNSUPPRESS()

template < typename T >
_Ret_z_ inline __declspec(noalias) T* SAL_Assume_notnull_for_opt_z_(_In_opt_z_ T* buf)
{
	ATLASSUME(buf!=0);
	return buf;
}

} // namespace ATL

#endif // __DUIDEF_H__

// Macro for calling GetProcAddress, with type safety for C++ clients.
// Parameters are the HINSTANCE and the function name.  The return value
// is automatically cast to match the function prototype.
//
// Sample usage:
//
// auto pfnSendMail = AtlGetProcAddressFn(hinstMAPI, MAPISendMailW);
// if (pfnSendMail)
// {
//    pfnSendMail(0, 0, pmm, MAPI_USE_DEFAULT, 0);
// }

#define AtlGetProcAddressFn(hinst, fn) reinterpret_cast<decltype(::fn)*>(GetProcAddress(hinst, #fn))

/////////////////////////////////////////////////////////////////////////////

#ifndef _WINSOCKAPI_
#include <WinSock2.h>
#endif
#include <Windows.h>

namespace ATL
{
#ifndef _DUI_CUSTOM_THROW
ATL_NOINLINE __declspec(noreturn) inline void WINAPI AtlThrowImpl(_In_ HRESULT hr);
#endif // _DUI_CUSTOM_THROW

ATL_NOINLINE __declspec(noreturn) inline void WINAPI AtlThrowLastWin32();
}
/*====================================================================*/
#endif

