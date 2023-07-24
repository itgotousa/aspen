// This is a part of the Active Template Library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#pragma warning( disable: 4073 )  // initializers put in library initialization area

#include <Windows.h>

#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

#include "duicore.h"
#include "duibase.h"

namespace DUI
{

// {B62F5910-6528-11d1-9611-0000F81E0D0D}
extern "C" const __declspec(selectany) GUID GUID_ATLVer30 = { 0xb62f5910, 0x6528, 0x11d1, { 0x96, 0x11, 0x0, 0x0, 0xf8, 0x1e, 0xd, 0xd } };

// {394C3DE0-3C6F-11d2-817B-00C04F797AB7}
extern "C" const __declspec(selectany) GUID GUID_ATLVer70 = { 0x394c3de0, 0x3c6f, 0x11d2, { 0x81, 0x7b, 0x0, 0xc0, 0x4f, 0x79, 0x7a, 0xb7 } };

// {5DC0A9CA-92A2-4204-9003-E4CE5F11E1A8}
extern "C" const __declspec(selectany) GUID GUID_ATLVer100 = { 0x5dc0a9ca, 0x92a2, 0x4204, { 0x90, 0x3, 0xe4, 0xce, 0x5f, 0x11, 0xe1, 0xa8 } };

// {D3492828-4138-4fd4-B3EC-DB99135EAE86}
extern "C" const __declspec(selectany) GUID GUID_ATLVer110 = { 0xd3492828, 0x4138, 0x4fd4, { 0xb3, 0xec, 0xdb, 0x99, 0x13, 0x5e, 0xae, 0x86 } };


DAtlBaseModule::DAtlBaseModule() throw()
{
	cbSize = sizeof(_DUI_BASE_MODULE);

	m_hInst = m_hInstResource = reinterpret_cast<HINSTANCE>(&__ImageBase);

	dwAtlBuildVer = _DUI_VER;
	pguidVer = &GUID_ATLVer110;

	if (FAILED(m_csResource.Init()))
	{
		if (IsDebuggerPresent())
		{
			OutputDebugStringW(L"ERROR : Unable to initialize critical section in DAtlBaseModule\n");
		}
		DAtlBaseModule::m_bInitFailed = true;
	}
}

DAtlBaseModule::~DAtlBaseModule() throw ()
{
	m_csResource.Term();
}

bool DAtlBaseModule::AddResourceInstance(_In_ HINSTANCE hInst) throw()
{
#if 10
	DComCritSecLock<DComCriticalSection> lock(m_csResource, false);
	if (FAILED(lock.Lock()))
	{
		if (IsDebuggerPresent())
		{
			OutputDebugStringW(L"ERROR : Unable to lock critical section in DAtlBaseModule\n");
		}

		return false;
	}
	return m_rgResourceInstance.Add(hInst) != FALSE;
#endif
	return true;
}

bool DAtlBaseModule::RemoveResourceInstance(_In_ HINSTANCE hInst) throw()
{
#if 0
	CComCritSecLock<CComCriticalSection> lock(m_csResource, false);
	if (FAILED(lock.Lock()))
	{
		if (IsDebuggerPresent())
		{
			OutputDebugStringW(L"ERROR : Unable to lock critical section in DAtlBaseModule\n");
		}

		return false;
	}
	for (int i = 0; i < m_rgResourceInstance.GetSize(); i++)
	{
		if (m_rgResourceInstance[i] == hInst)
		{
			m_rgResourceInstance.RemoveAt(i);
			return true;
		}
	}
#endif
	return false;
}

HINSTANCE DAtlBaseModule::GetHInstanceAt(_In_ int i) throw()
{
#if 0
	CComCritSecLock<CComCriticalSection> lock(m_csResource, false);
	if (FAILED(lock.Lock()))
	{
		if (IsDebuggerPresent())
		{
			OutputDebugStringW(L"ERROR : Unable to lock critical section in DAtlBaseModule\n");
		}

		return NULL;
	}
	if (i > m_rgResourceInstance.GetSize() || i < 0)
	{
		return NULL;
	}

	if (i == m_rgResourceInstance.GetSize())
	{
		return m_hInstResource;
	}

	return m_rgResourceInstance[i];
#endif
	return nullptr;
}

#pragma init_seg( lib )
	
	//CAtlBaseModule	_AtlBaseModule;
	
	DAtlBaseModule  _DuiBaseModule;

};  // namespace DUI
