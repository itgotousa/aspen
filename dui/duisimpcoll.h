#ifndef __DUISIMPCOLL_H__
#define __DUISIMPCOLL_H__

#include "duidef.h"

namespace DUI
{
	/////////////////////////////////////////////////////////////////////////////
	// Collection helpers - DSimpleArray & DSimpleMap

	DUIPREFAST_SUPPRESS(6319)
	// template class helpers with functions for comparing elements
	// override if using complex types without operator==
	template <class T>
	class DSimpleArrayEqualHelper
	{
	public:
		static bool IsEqual(
			_In_ const T& t1,
			_In_ const T& t2)
		{
			return (t1 == t2);
		}
	};
	DUIPREFAST_UNSUPPRESS()

	template <class T>
	class DSimpleArrayEqualHelperFalse
	{
	public:
		static bool IsEqual(
			_In_ const T&,
			_In_ const T&)
		{
			DUIASSERT(false);
			return false;
		}
	};

	template <class TKey, class TVal>
	class DSimpleMapEqualHelper
	{
	public:
		static bool IsEqualKey(
			_In_ const TKey& k1,
			_In_ const TKey& k2)
		{
			return DSimpleArrayEqualHelper<TKey>::IsEqual(k1, k2);
		}

		static bool IsEqualValue(
			_In_ const TVal& v1,
			_In_ const TVal& v2)
		{
			return DSimpleArrayEqualHelper<TVal>::IsEqual(v1, v2);
		}
	};

	template <class TKey, class TVal>
	class DSimpleMapEqualHelperFalse
	{
	public:
		static bool IsEqualKey(
			_In_ const TKey& k1,
			_In_ const TKey& k2)
		{
			return DSimpleArrayEqualHelper<TKey>::IsEqual(k1, k2);
		}

		static bool IsEqualValue(
			_In_ const TVal&,
			_In_ const TVal&)
		{
			DUIASSERT(FALSE);
			return false;
		}
	};

	template <class T, class TEqual = DSimpleArrayEqualHelper< T > >
	class DSimpleArray
	{
	public:
		// Construction/destruction
		DSimpleArray() :
			m_aT(NULL), m_nSize(0), m_nAllocSize(0)
		{
		}

		~DSimpleArray();

		DSimpleArray(_In_ const DSimpleArray< T, TEqual >& src) :
			m_aT(NULL), m_nSize(0), m_nAllocSize(0)
		{
			if (src.GetSize())
			{
				m_aT = (T*)calloc(src.GetSize(), sizeof(T));
				if (m_aT != NULL)
				{
					m_nAllocSize = src.GetSize();
					for (int i = 0; i < src.GetSize(); i++)
						Add(src[i]);
				}
			}
		}
		DSimpleArray< T, TEqual >& operator=(_In_ const DSimpleArray< T, TEqual >& src)
		{
			if (GetSize() != src.GetSize())
			{
				RemoveAll();
				m_aT = (T*)calloc(src.GetSize(), sizeof(T));
				if (m_aT != NULL)
					m_nAllocSize = src.GetSize();
			}
			else
			{
				for (int i = GetSize(); i > 0; i--)
					RemoveAt(i - 1);
			}
			for (int i = 0; i < src.GetSize(); i++)
				Add(src[i]);
			return *this;
		}

		// Operations
		int GetSize() const
		{
			return m_nSize;
		}

		_Success_(return != FALSE)
			BOOL Add(_In_ const T& t)
		{
			if (m_nSize == m_nAllocSize)
			{
				// Make sure newElement is not a reference to an element in the array.
				// Or else, it will be invalidated by the reallocation.
				DUIENSURE((&t < m_aT) ||
					(&t >= (m_aT + m_nAllocSize)));

				T* aT;
				int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);

				if (nNewAllocSize<0 || nNewAllocSize>INT_MAX / sizeof(T))
				{
					return FALSE;
				}

				aT = (T*)_recalloc(m_aT, nNewAllocSize, sizeof(T));
				if (aT == NULL)
					return FALSE;
				m_nAllocSize = nNewAllocSize;
				m_aT = aT;
			}
			InternalSetAtIndex(m_nSize, t);
			m_nSize++;
			return TRUE;
		}

		_Success_(return != FALSE)
			BOOL Remove(_In_ const T& t)
		{
			int nIndex = Find(t);
			if (nIndex == -1)
				return FALSE;
			return RemoveAt(nIndex);
		}

		_Success_(return != FALSE)
			BOOL RemoveAt(_In_ int nIndex)
		{
			DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
			if (nIndex < 0 || nIndex >= m_nSize)
				return FALSE;
			m_aT[nIndex].~T();
			if (nIndex != (m_nSize - 1))
				Checked::memmove_s((void*)(m_aT + nIndex), (m_nSize - nIndex) * sizeof(T), (void*)(m_aT + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(T));
			m_nSize--;
			return TRUE;
		}
		void RemoveAll()
		{
			if (m_aT != NULL)
			{
				for (int i = 0; i < m_nSize; i++)
					m_aT[i].~T();
				free(m_aT);
				m_aT = NULL;
			}
			m_nSize = 0;
			m_nAllocSize = 0;
		}
		const T& operator[] (_In_ int nIndex) const
		{
			DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
			if (nIndex < 0 || nIndex >= m_nSize)
			{
				AtlThrow(E_BOUNDS);
			}
			return m_aT[nIndex];
		}
		T& operator[] (_In_ int nIndex)
		{
			DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
			if (nIndex < 0 || nIndex >= m_nSize)
			{
				AtlThrow(E_BOUNDS);
			}
			return m_aT[nIndex];
		}
		T* GetData() const
		{
			return m_aT;
		}

		int Find(_In_ const T& t) const
		{
			for (int i = 0; i < m_nSize; i++)
			{
				if (TEqual::IsEqual(m_aT[i], t))
					return i;
			}
			return -1;  // not found
		}

		_Success_(return != FALSE)
			BOOL SetAtIndex(
				_In_ int nIndex,
				_In_ const T& t)
		{
			if (nIndex < 0 || nIndex >= m_nSize)
				return FALSE;
			InternalSetAtIndex(nIndex, t);
			return TRUE;
		}

		// Implementation
		class Wrapper
		{
		public:
			Wrapper(_In_ const T& _t) : t(_t)
			{
			}
			template <class _Ty>
			void* __cdecl operator new(
				_In_ size_t,
				_In_ _Ty* p)
			{
				return p;
			}
			template <class _Ty>
			void __cdecl operator delete(
				_In_ void* /* pv */,
				_In_ _Ty* /* p */)
			{
			}
			T t;
		};

		// Implementation
		void InternalSetAtIndex(
			_In_ int nIndex,
			_In_ const T& t)
		{
			new(m_aT + nIndex) Wrapper(t);
		}

		typedef T _ArrayElementType;
		T* m_aT;
		int m_nSize;
		int m_nAllocSize;
	};

#define DSimpleValArray DSimpleArray
	template <class T, class TEqual> inline DSimpleArray<T, TEqual>::~DSimpleArray()
	{
		RemoveAll();
	}

}
#endif /* __DUISIMPCOLL_H__ */



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

#ifndef __ATLSIMPCOLL_H__
#define __ATLSIMPCOLL_H__

#pragma once

#include <atldef.h>
#include <atlchecked.h>
#include <wchar.h>

#pragma push_macro("malloc")
#undef malloc
#pragma push_macro("calloc")
#undef calloc
#pragma push_macro("realloc")
#undef realloc
#pragma push_macro("_recalloc")
#undef _recalloc
#pragma push_macro("free")
#undef free

#pragma warning(push)
#pragma warning(disable: 4800) // forcing 'int' value to bool


#pragma pack(push,_ATL_PACKING)
namespace ATL
{

#pragma push_macro("new")
#undef new

/////////////////////////////////////////////////////////////////////////////
// Collection helpers - DSimpleArray & DSimpleMap

ATLPREFAST_SUPPRESS(6319)
// template class helpers with functions for comparing elements
// override if using complex types without operator==
template <class T>
class DSimpleArrayEqualHelper
{
public:
	static bool IsEqual(
		_In_ const T& t1,
		_In_ const T& t2)
	{
		return (t1 == t2);
	}
};
ATLPREFAST_UNSUPPRESS()

template <class T>
class DSimpleArrayEqualHelperFalse
{
public:
	static bool IsEqual(
		_In_ const T&,
		_In_ const T&)
	{
		DUIASSERT(false);
		return false;
	}
};

template <class TKey, class TVal>
class DSimpleMapEqualHelper
{
public:
	static bool IsEqualKey(
		_In_ const TKey& k1,
		_In_ const TKey& k2)
	{
		return DSimpleArrayEqualHelper<TKey>::IsEqual(k1, k2);
	}

	static bool IsEqualValue(
		_In_ const TVal& v1,
		_In_ const TVal& v2)
	{
		return DSimpleArrayEqualHelper<TVal>::IsEqual(v1, v2);
	}
};

template <class TKey, class TVal>
class DSimpleMapEqualHelperFalse
{
public:
	static bool IsEqualKey(
		_In_ const TKey& k1,
		_In_ const TKey& k2)
	{
		return DSimpleArrayEqualHelper<TKey>::IsEqual(k1, k2);
	}

	static bool IsEqualValue(
		_In_ const TVal&,
		_In_ const TVal&)
	{
		DUIASSERT(FALSE);
		return false;
	}
};

template <class T, class TEqual = DSimpleArrayEqualHelper< T > >
class DSimpleArray
{
public:
// Construction/destruction
	DSimpleArray() :
		m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{
	}

	~DSimpleArray();

	DSimpleArray(_In_ const DSimpleArray< T, TEqual >& src) :
		m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{
        if (src.GetSize())
        {
			m_aT = (T*)calloc(src.GetSize(), sizeof(T));
			if (m_aT != NULL)
			{
				m_nAllocSize = src.GetSize();
				for (int i=0; i<src.GetSize(); i++)
					Add(src[i]);
			}
		}
	}
	DSimpleArray< T, TEqual >& operator=(_In_ const DSimpleArray< T, TEqual >& src)
	{
		if (GetSize() != src.GetSize())
		{
			RemoveAll();
			m_aT = (T*)calloc(src.GetSize(), sizeof(T));
			if (m_aT != NULL)
				m_nAllocSize = src.GetSize();
		}
		else
		{
			for (int i = GetSize(); i > 0; i--)
				RemoveAt(i - 1);
		}
		for (int i=0; i<src.GetSize(); i++)
			Add(src[i]);
		return *this;
	}

// Operations
	int GetSize() const
	{
		return m_nSize;
	}

	_Success_(return != FALSE)
	BOOL Add(_In_ const T& t)
	{
		if(m_nSize == m_nAllocSize)
		{
			// Make sure newElement is not a reference to an element in the array.
			// Or else, it will be invalidated by the reallocation.
			DUIENSURE(	(&t < m_aT) ||
						(&t >= (m_aT + m_nAllocSize) ) );

			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);

			if (nNewAllocSize<0||nNewAllocSize>INT_MAX/sizeof(T))
			{
				return FALSE;
			}

			aT = (T*)_recalloc(m_aT, nNewAllocSize, sizeof(T));
			if(aT == NULL)
				return FALSE;
			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}
		InternalSetAtIndex(m_nSize, t);
		m_nSize++;
		return TRUE;
	}

	_Success_(return != FALSE)
	BOOL Remove(_In_ const T& t)
	{
		int nIndex = Find(t);
		if(nIndex == -1)
			return FALSE;
		return RemoveAt(nIndex);
	}

	_Success_(return != FALSE)
	BOOL RemoveAt(_In_ int nIndex)
	{
		DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
		if (nIndex < 0 || nIndex >= m_nSize)
			return FALSE;
		m_aT[nIndex].~T();
		if(nIndex != (m_nSize - 1))
			Checked::memmove_s((void*)(m_aT + nIndex), (m_nSize - nIndex) * sizeof(T), (void*)(m_aT + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(T));
		m_nSize--;
		return TRUE;
	}
	void RemoveAll()
	{
		if(m_aT != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
				m_aT[i].~T();
			free(m_aT);
			m_aT = NULL;
		}
		m_nSize = 0;
		m_nAllocSize = 0;
    }
	const T& operator[] (_In_ int nIndex) const
	{
		DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex < 0 || nIndex >= m_nSize)
		{
			AtlThrow(E_BOUNDS);
		}
		return m_aT[nIndex];
	}
	T& operator[] (_In_ int nIndex)
	{
		DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex < 0 || nIndex >= m_nSize)
		{
			AtlThrow(E_BOUNDS);
		}
		return m_aT[nIndex];
	}
	T* GetData() const
	{
		return m_aT;
	}

	int Find(_In_ const T& t) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(TEqual::IsEqual(m_aT[i], t))
				return i;
		}
		return -1;  // not found
	}

	_Success_(return != FALSE)
	BOOL SetAtIndex(
		_In_ int nIndex,
		_In_ const T& t)
	{
		if (nIndex < 0 || nIndex >= m_nSize)
			return FALSE;
		InternalSetAtIndex(nIndex, t);
		return TRUE;
	}

// Implementation
	class Wrapper
	{
	public:
		Wrapper(_In_ const T& _t) : t(_t)
		{
		}
		template <class _Ty>
		void * __cdecl operator new(
			_In_ size_t,
			_In_ _Ty* p)
		{
			return p;
		}
		template <class _Ty>
		void __cdecl operator delete(
			_In_ void* /* pv */,
			_In_ _Ty* /* p */)
		{
		}
		T t;
	};

// Implementation
	void InternalSetAtIndex(
		_In_ int nIndex,
		_In_ const T& t)
	{
		new(m_aT + nIndex) Wrapper(t);
	}

	typedef T _ArrayElementType;
	T* m_aT;
	int m_nSize;
	int m_nAllocSize;
};

#define DSimpleValArray DSimpleArray

template <class T, class TEqual> inline DSimpleArray<T, TEqual>::~DSimpleArray()
{
	RemoveAll();
}

// intended for small number of simple types or pointers
template <class TKey, class TVal, class TEqual = DSimpleMapEqualHelper< TKey, TVal > >
class DSimpleMap
{
public:
	TKey* m_aKey;
	TVal* m_aVal;
	int m_nSize;

	typedef TKey _ArrayKeyType;
	typedef TVal _ArrayElementType;

// Construction/destruction
	DSimpleMap() :
		m_aKey(NULL), m_aVal(NULL), m_nSize(0)
	{
	}

	~DSimpleMap()
	{
		RemoveAll();
	}

// Operations
	int GetSize() const
	{
		return m_nSize;
	}
	BOOL Add(
		_In_ const TKey& key,
		_In_ const TVal& val)
	{
		TKey* pKey;
		pKey = (TKey*)_recalloc(m_aKey, (m_nSize + 1), sizeof(TKey));
		if(pKey == NULL)
			return FALSE;
		m_aKey = pKey;
		TVal* pVal;
		pVal = (TVal*)_recalloc(m_aVal, (m_nSize + 1), sizeof(TVal));
		if(pVal == NULL)
			return FALSE;
		m_aVal = pVal;
		InternalSetAtIndex(m_nSize, key, val);
		m_nSize++;
		return TRUE;
	}
	BOOL Remove(_In_ const TKey& key)
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return FALSE;
		return RemoveAt(nIndex);
	}
	BOOL RemoveAt(_In_ int nIndex)
	{
		DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
		if (nIndex < 0 || nIndex >= m_nSize)
			return FALSE;
		m_aKey[nIndex].~TKey();
		m_aVal[nIndex].~TVal();
		if(nIndex != (m_nSize - 1))
		{
			Checked::memmove_s((void*)(m_aKey + nIndex), (m_nSize - nIndex) * sizeof(TKey), (void*)(m_aKey + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(TKey));
			Checked::memmove_s((void*)(m_aVal + nIndex), (m_nSize - nIndex) * sizeof(TVal), (void*)(m_aVal + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(TVal));
		}
		TKey* pKey;
		pKey = (TKey*)_recalloc(m_aKey, (m_nSize - 1), sizeof(TKey));
		if(pKey != NULL || m_nSize == 1)
			m_aKey = pKey;
		TVal* pVal;
		pVal = (TVal*)_recalloc(m_aVal, (m_nSize - 1), sizeof(TVal));
		if(pVal != NULL || m_nSize == 1)
			m_aVal = pVal;
		m_nSize--;
		return TRUE;
	}
	void RemoveAll()
	{
		if(m_aKey != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
			{
				m_aKey[i].~TKey();
				m_aVal[i].~TVal();
			}
			free(m_aKey);
			m_aKey = NULL;
		}
		if(m_aVal != NULL)
		{
			free(m_aVal);
			m_aVal = NULL;
		}

		m_nSize = 0;
	}
	BOOL SetAt(
		_In_ const TKey& key,
		_In_ const TVal& val)
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return FALSE;
		DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_aKey[nIndex].~TKey();
		m_aVal[nIndex].~TVal();
		InternalSetAtIndex(nIndex, key, val);
		return TRUE;
	}
	TVal Lookup(_In_ const TKey& key) const
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return NULL;    // must be able to convert
		return GetValueAt(nIndex);
	}
	TKey ReverseLookup(_In_ const TVal& val) const
	{
		int nIndex = FindVal(val);
		if(nIndex == -1)
			return NULL;    // must be able to convert
		return GetKeyAt(nIndex);
	}
	TKey& GetKeyAt(_In_ int nIndex) const
	{
		DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex < 0 || nIndex >= m_nSize)
			AtlThrow(E_BOUNDS);

		return m_aKey[nIndex];
	}
	TVal& GetValueAt(_In_ int nIndex) const
	{
		DUIASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex < 0 || nIndex >= m_nSize)
			AtlThrow(E_BOUNDS);

		return m_aVal[nIndex];
	}

	int FindKey(_In_ const TKey& key) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(TEqual::IsEqualKey(m_aKey[i], key))
				return i;
		}
		return -1;  // not found
	}
	int FindVal(_In_ const TVal& val) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(TEqual::IsEqualValue(m_aVal[i], val))
				return i;
		}
		return -1;  // not found
	}

	BOOL SetAtIndex(
		_In_ int nIndex,
		_In_ const TKey& key,
		_In_ const TVal& val)
	{
		if (nIndex < 0 || nIndex >= m_nSize)
			return FALSE;
		InternalSetAtIndex(nIndex, key, val);
		return TRUE;
	}


// Implementation

	template <typename T>
	class Wrapper
	{
	public:
		Wrapper(_In_ const T& _t) : t(_t)
		{
		}
		template <class _Ty>
		void *operator new(
			_In_ size_t,
			_In_ _Ty* p)
		{
			return p;
		}
		template <class _Ty>
		void operator delete(
			_In_ void* /* pv */,
			_In_ _Ty* /* p */)
		{
		}
		T t;
	};
	void InternalSetAtIndex(
		_In_ int nIndex,
		_In_ const TKey& key,
		_In_ const TVal& val)
	{
		new(m_aKey + nIndex) Wrapper<TKey>(key);
		new(m_aVal + nIndex) Wrapper<TVal>(val);
	}
};

#pragma pop_macro("new")

};  // namespace ATL
#pragma pack(pop)

#pragma warning(pop)

#pragma pop_macro("free")
#pragma pop_macro("realloc")
#pragma pop_macro("_recalloc")
#pragma pop_macro("malloc")
#pragma pop_macro("calloc")


#endif  // __ATLSIMPCOLL_H__
/*====================================================================*/
#endif

