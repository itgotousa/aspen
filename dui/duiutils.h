#ifndef __DUI_UTILS_H__
#define __DUI_UTILS_H__

#include "duiconfig.h"

/////////////////////////////////////////////////////////////////////////////////////
namespace DUI 
{
	class DUI_API STRINGorID
	{
	public:
		STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
		{}
		STRINGorID(unsigned int nID) : m_lpstr(MAKEINTRESOURCE(nID))
		{}
		LPCTSTR m_lpstr;
	};

	class CDUIString;
	class DUI_API CDUIPoint : public tagPOINT
	{
	public:
		CDUIPoint()
		{
			x = y = 0;
		}
		CDUIPoint(const POINT& src)
		{
			x = src.x;
			y = src.y;
		}

		CDUIPoint(long x_, long y_)
		{
			x = x_;
			y = y_;
		}

		CDUIPoint(LPARAM lParam)
		{
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
		}
		CDUIPoint(LPCTSTR pstrValue)
		{
#if 0
			if (pstrValue == NULL || *pstrValue == _T('\0')) x = y = 0;
			LPTSTR pstr = NULL;
			x = y = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);
			y = _tcstol(pstr + 1, &pstr, 10);      ASSERT(pstr);
#endif
		}

#if 0
		CDUIString ToString()
		{
			CDuiString sPoint;
			sPoint.SmallFormat(_T("%ld,%ld"), x, y);
			return sPoint;
		}
#endif
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DUI_API CDUISize : public tagSIZE
	{
	public:
		CDUISize()
		{
			cx = cy = 0;
		}

		CDUISize(const SIZE& src)
		{
			cx = src.cx;
			cy = src.cy;
		}
		CDUISize(const RECT rc)
		{
			cx = rc.right - rc.left;
			cy = rc.bottom - rc.top;
		}

		CDUISize(long cx_, long cy_)
		{
			cx = cx_;
			cy = cy_;
		}

		//CDUISize(LPCTSTR pstrValue);
		//CDUIString ToString();
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DUI_API CDUIRect : public tagRECT
	{
	public:
		CDUIRect()
		{
			left = top = right = bottom = 0;
		}

		CDUIRect(const RECT& src)
		{
			left   = src.left;
			top    = src.top;
			right  = src.right;
			bottom = src.bottom;
		}

		CDUIRect(long iLeft, long iTop, long iRight, long iBottom)
		{
			left   = iLeft;
			top    = iTop;
			right  = iRight;
			bottom = iBottom;
		}

		//CDUIRect(LPCTSTR pstrValue);
		//CDUIString ToString();

		int GetWidth() const
		{
			return right - left;
		}

		int GetHeight() const
		{
			return bottom - top;
		}

		void Empty()
		{
			left = top = right = bottom = 0;
		}

		bool IsNull() const
		{
			return (left == 0 && right == 0 && top == 0 && bottom == 0);
		}

		void Join(const RECT& rc)
		{
			if (rc.left < left)     left   = rc.left;
			if (rc.top < top)       top    = rc.top;
			if (rc.right > right)   right  = rc.right;
			if (rc.bottom > bottom) bottom = rc.bottom;
		}

		void ResetOffset()
		{
			::OffsetRect(this, -left, -top);
		}

		void Normalize()
		{
			int iTemp;

			if (left > right) 
			{ 
				iTemp = left; 
				left = right; 
				right = iTemp; 
			}
			if (top > bottom) 
			{ 
				iTemp = top; 
				top = bottom; 
				bottom = iTemp; 
			}
		}
		void Offset(int cx, int cy)
		{
			::OffsetRect(this, cx, cy);
		}

		void Inflate(int cx, int cy)
		{
			::InflateRect(this, cx, cy);
		}

		void Deflate(int cx, int cy)
		{
			::InflateRect(this, -cx, -cy);
		}

		void Union(CDUIRect& rc)
		{
			::UnionRect(this, this, &rc);
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DUI_API CDUIString
	{
	public:
		enum { MAX_LOCAL_STRING_LEN = 63 };

		CDUIString()
		{
			m_szBuffer[0] = _T('\0');
		}

		CDUIString(const TCHAR ch) : m_pstr(m_szBuffer)
		{
			m_szBuffer[0] = ch;
			m_szBuffer[1] = _T('\0');
		}

		CDUIString(const CDUIString& src) : m_pstr(m_szBuffer)
		{
			m_szBuffer[0] = _T('\0');
			Assign(src.m_pstr);
		}

		CDUIString(LPCTSTR lpsz, int nLen = -1) : m_pstr(m_szBuffer)
		{
			DUI_ASSERT(!::IsBadStringPtr(lpsz, -1) || lpsz == NULL);
			m_szBuffer[0] = _T('\0');
			Assign(lpsz, nLen);
		}

		~CDUIString()
		{
			if (m_pstr != m_szBuffer) 
				free(m_pstr);
		}

		CDUIString ToString()
		{
			return m_pstr;
		}

		void Empty()
		{
			if (m_pstr != m_szBuffer) 
				free(m_pstr);
			m_pstr = m_szBuffer;
			m_szBuffer[0] = _T('\0');
		}

		int GetLength() const
		{
			return (int)_tcslen(m_pstr);
		}

		bool IsEmpty() const
		{
			return (m_pstr[0] == _T('\0'));
		}

		TCHAR GetAt(int nIndex) const
		{
			return m_pstr[nIndex];
		}

		void Append(LPCTSTR pstr)
		{
			int nNewLength = GetLength() + (int)_tcslen(pstr);
			if (nNewLength >= MAX_LOCAL_STRING_LEN) 
			{
				if (m_pstr == m_szBuffer) 
				{
					m_pstr = static_cast<LPTSTR>(malloc((nNewLength + 1) * sizeof(TCHAR)));
					_tcscpy(m_pstr, m_szBuffer);
					_tcscat(m_pstr, pstr);
				}
				else 
				{
					m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (nNewLength + 1) * sizeof(TCHAR)));
					_tcscat(m_pstr, pstr);
				}
			}
			else 
			{
				if (m_pstr != m_szBuffer) 
				{
					free(m_pstr);
					m_pstr = m_szBuffer;
				}
				_tcscat(m_szBuffer, pstr);
			}
		}

		void Assign(LPCTSTR pstr, int cchMax = -1)
		{
			if (nullptr == pstr) pstr = _T("");
			cchMax = (cchMax < 0 ? (int)_tcslen(pstr) : cchMax);
			if (cchMax < MAX_LOCAL_STRING_LEN) 
			{
				if (m_pstr != m_szBuffer) 
				{
					free(m_pstr);
					m_pstr = m_szBuffer;
				}
			}
			else if (cchMax > GetLength() || m_pstr == m_szBuffer) 
			{
				if (m_pstr == m_szBuffer) 
					m_pstr = nullptr;
				m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (cchMax + 1) * sizeof(TCHAR)));
			}
			_tcsncpy(m_pstr, pstr, cchMax);
			m_pstr[cchMax] = _T('\0');
		}

		LPCTSTR GetData() const
		{
			return m_pstr;
		}

		void SetAt(int nIndex, TCHAR ch)
		{
			DUI_ASSERT(nIndex >= 0 && nIndex < GetLength());
			m_pstr[nIndex] = ch;
		}

		operator LPCTSTR() const
		{
			return m_pstr;
		}

		TCHAR operator[] (int nIndex) const
		{
			return m_pstr[nIndex];
		}

		const CDUIString& operator=(const CDUIString& src)
		{
			Assign(src);
			return *this;
		}

		const CDUIString& operator=(const TCHAR ch)
		{
			Empty();
			m_szBuffer[0] = ch;
			m_szBuffer[1] = _T('\0');
			return *this;
		}

		const CDUIString& operator=(LPCTSTR lpStr)
		{
#if 0
			if (lpStr)
			{
				DUI_ASSERT(!::IsBadStringPtrA(lpStr, -1));
				int cchStr = (int)strlen(lpStr) + 1;
				LPWSTR pwstr = (LPWSTR)_alloca(cchStr);
				if (pwstr != NULL) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr);
				Append(pwstr);
			}
#endif
			return *this;
		}
#if 0
#ifdef _UNICODE
		const CDUIString& operator=(LPCSTR lpStr);
		const CDUIString& operator+=(LPCSTR lpStr);
#else
		const CDUIString& operator=(LPCWSTR lpwStr);
		const CDUIString& operator+=(LPCWSTR lpwStr);
#endif

		CDUIString operator+(const CDUIString& src) const;
		CDUIString operator+(LPCTSTR pstr) const;
		const CDUIString& operator+=(const CDUIString& src);
		const CDUIString& operator+=(LPCTSTR pstr);
		const CDUIString& operator+=(const TCHAR ch);

		bool operator == (LPCTSTR str) const;
		bool operator != (LPCTSTR str) const;
		bool operator <= (LPCTSTR str) const;
		bool operator <  (LPCTSTR str) const;
		bool operator >= (LPCTSTR str) const;
		bool operator >  (LPCTSTR str) const;

		int Compare(LPCTSTR pstr) const;
		int CompareNoCase(LPCTSTR pstr) const;

		void MakeUpper();
		void MakeLower();

		CDUIString Left(int nLength) const;
		CDUIString Mid(int iPos, int nLength = -1) const;
		CDUIString Right(int nLength) const;

		int Find(TCHAR ch, int iPos = 0) const;
		int Find(LPCTSTR pstr, int iPos = 0) const;
		int ReverseFind(TCHAR ch) const;
		int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

		int __cdecl Format(LPCTSTR pstrFormat, ...);
		int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);
#endif
	protected:
		LPTSTR m_pstr;
		TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DUI_API CDUIPtrArray
	{
	public:
		CDUIPtrArray(int iPreallocSize = 0) : m_ppVoid(nullptr), m_nCount(0), m_nAllocated(iPreallocSize)
		{
			DUI_ASSERT(iPreallocSize >= 0);
			if (iPreallocSize > 0) 
				m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
		}

		CDUIPtrArray(const CDUIPtrArray& src) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(0)
		{
			for (int i = 0; i < src.GetSize(); i++)
				Add(src.GetAt(i));
		}

		~CDUIPtrArray()
		{
			if (nullptr != m_ppVoid)
			{
				free(m_ppVoid);
				m_ppVoid = nullptr;
			}
		}

		void Empty()
		{
			if (nullptr != m_ppVoid) 
				free(m_ppVoid);
			m_ppVoid = nullptr;
			m_nCount = m_nAllocated = 0;
		}

		void Resize(int iSize)
		{
			Empty();
			m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
			::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
			m_nAllocated = iSize;
			m_nCount = iSize;
		}

		bool IsEmpty() const
		{
			return (0 == m_nCount);
		}

		int Find(LPVOID pData) const
		{
			for (int i = 0; i < m_nCount; i++) 
				if (m_ppVoid[i] == pData) 
					return i;
			return -1;
		}

		bool Add(LPVOID pData)
		{
			if (++m_nCount >= m_nAllocated) 
			{
				int nAllocated = m_nAllocated * 2;
				if (nAllocated == 0) nAllocated = 11;
				LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
				if (nullptr != ppVoid) 
				{
					m_nAllocated = nAllocated;
					m_ppVoid = ppVoid;
				}
				else 
				{
					--m_nCount;
					return false;
				}
			}
			m_ppVoid[m_nCount - 1] = pData;
			return true;
		}

		bool SetAt(int iIndex, LPVOID pData)
		{
			if (iIndex < 0 || iIndex >= m_nCount) 
				return false;
			m_ppVoid[iIndex] = pData;
			return true;
		}

		bool InsertAt(int iIndex, LPVOID pData)
		{
			if (iIndex == m_nCount) 
				return Add(pData);
			if (iIndex < 0 || iIndex > m_nCount) 
				return false;
			if (++m_nCount >= m_nAllocated) 
			{
				int nAllocated = m_nAllocated * 2;
				if (nAllocated == 0) nAllocated = 11;
				LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
				if (nullptr != ppVoid) 
				{
					m_nAllocated = nAllocated;
					m_ppVoid = ppVoid;
				}
				else 
				{
					--m_nCount;
					return false;
				}
			}
			memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
			m_ppVoid[iIndex] = pData;
			return true;
		}

		bool Remove(int iIndex, int iCount = 1)
		{
			if (iIndex < 0 || iCount <= 0 || iIndex + iCount > m_nCount) 
				return false;
			if (iIndex + iCount < m_nCount) 
				::CopyMemory(m_ppVoid + iIndex, m_ppVoid + iIndex + iCount, (m_nCount - iIndex - iCount) * sizeof(LPVOID));
			m_nCount -= iCount;
			return true;
		}

		int GetSize() const
		{
			return m_nCount;
		}
		LPVOID* GetData()
		{
			return m_ppVoid;
		}

		LPVOID GetAt(int iIndex) const
		{
			if (iIndex < 0 || iIndex >= m_nCount) 
				return nullptr;
			return m_ppVoid[iIndex];
		}

		LPVOID operator[] (int iIndex) const
		{
			DUI_ASSERT(iIndex >= 0 && iIndex < m_nCount);
			return m_ppVoid[iIndex];
		}

	protected:
		LPVOID* m_ppVoid;
		int m_nCount = 0;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DUI_API CDUIValArray
	{
	public:
		CDUIValArray(int iElementSize, int iPreallocSize = 0) :
			m_pVoid(NULL),
			m_nCount(0),
			m_iElementSize(iElementSize),
			m_nAllocated(iPreallocSize)
		{
			DUI_ASSERT(iElementSize > 0);
			DUI_ASSERT(iPreallocSize >= 0);
			if (iPreallocSize > 0) 
				m_pVoid = static_cast<LPBYTE>(malloc(iPreallocSize * m_iElementSize));
		}

		~CDUIValArray()
		{
			if (nullptr != m_pVoid) 
				free(m_pVoid);
		}

		void Empty()
		{
			m_nCount = 0;  // NOTE: We keep the memory in place
		}

		bool IsEmpty() const
		{
			return m_nCount == 0;
		}

		bool Add(LPCVOID pData)
		{
			if (++m_nCount >= m_nAllocated) 
			{
				int nAllocated = m_nAllocated * 2;
				if (nAllocated == 0) nAllocated = 11;
				LPBYTE pVoid = static_cast<LPBYTE>(realloc(m_pVoid, nAllocated * m_iElementSize));
				if (pVoid != NULL) 
				{
					m_nAllocated = nAllocated;
					m_pVoid = pVoid;
				}
				else 
				{
					--m_nCount;
					return false;
				}
			}
			::CopyMemory(m_pVoid + ((m_nCount - 1) * m_iElementSize), pData, m_iElementSize);
			return true;

		}

		bool Remove(int iIndex, int iCount = 1)
		{
			if (iIndex < 0 || iCount <= 0 || iIndex + iCount > m_nCount) 
				return false;
			if (iIndex + iCount < m_nCount) 
				::CopyMemory(m_pVoid + (iIndex * m_iElementSize), m_pVoid + (iIndex + iCount) * m_iElementSize, (m_nCount - iIndex - iCount) * m_iElementSize);
			m_nCount -= iCount;
			return true;
		}

		int GetSize() const
		{
			return m_nCount;
		}

		LPVOID GetData()
		{
			return static_cast<LPVOID>(m_pVoid);
		}

		LPVOID GetAt(int iIndex) const
		{
			if (iIndex < 0 || iIndex >= m_nCount) 
				return nullptr;
			return (m_pVoid + (iIndex * m_iElementSize));

		}

		LPVOID operator[] (int iIndex) const
		{
			DUI_ASSERT(iIndex >= 0 && iIndex < m_nCount);
			return m_pVoid + (iIndex * m_iElementSize);
		}

	protected:
		LPBYTE m_pVoid;
		int m_iElementSize;
		int m_nCount;
		int m_nAllocated;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	struct TITEM
	{
		CDUIString Key;
		LPVOID Data;
		struct TITEM* pPrev;
		struct TITEM* pNext;
	};

	class DUI_API CDUIStringPtrMap
	{
	public:
		CDUIStringPtrMap(int nSize = 83)
		{
			if (nSize < 16) 
				nSize = 16;
			m_nBuckets = nSize;
			m_aT = new TITEM * [nSize];
			memset(m_aT, 0, nSize * sizeof(TITEM*));
		}

		~CDUIStringPtrMap()
		{
			if (m_aT) 
			{
				int len = m_nBuckets;
				while (len--) 
				{
					TITEM* pItem = m_aT[len];
					while (pItem) 
					{
						TITEM* pKill = pItem;
						pItem = pItem->pNext;
						delete pKill;
					}
				}
				delete[] m_aT;
				m_aT = NULL;
			}
		}

		void Resize(int nSize = 83)
		{
			if (m_aT) 
			{
				int len = m_nBuckets;
				while (len--) 
				{
					TITEM* pItem = m_aT[len];
					while (pItem) 
					{
						TITEM* pKill = pItem;
						pItem = pItem->pNext;
						delete pKill;
					}
				}
				delete[] m_aT;
				m_aT = NULL;
			}

			if (nSize < 0) nSize = 0;
			if (nSize > 0) 
			{
				m_aT = new TITEM * [nSize];
				memset(m_aT, 0, nSize * sizeof(TITEM*));
			}
			m_nBuckets = nSize;
			m_nCount = 0;
		}

		LPVOID Find(LPCTSTR key, bool optimize = true) const
		{
#if 0
			if (m_nBuckets == 0 || GetSize() == 0) 
				return NULL;

			UINT slot = HashKey(key) % m_nBuckets;
			for (TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext) {
				if (pItem->Key == key) {
					if (optimize && pItem != m_aT[slot]) {
						if (pItem->pNext) {
							pItem->pNext->pPrev = pItem->pPrev;
						}
						pItem->pPrev->pNext = pItem->pNext;
						pItem->pPrev = NULL;
						pItem->pNext = m_aT[slot];
						pItem->pNext->pPrev = pItem;
						m_aT[slot] = pItem;
					}
					return pItem->Data;
				}
			}
#endif
			return NULL;
		}
#if 0
		bool Insert(LPCTSTR key, LPVOID pData);
		LPVOID Set(LPCTSTR key, LPVOID pData);
		bool Remove(LPCTSTR key);
		void RemoveAll();
		int GetSize() const;
		LPCTSTR GetAt(int iIndex) const;
		LPCTSTR operator[] (int nIndex) const;
#endif
	protected:
		TITEM** m_aT;
		int m_nBuckets;
		int m_nCount;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DUI_API CWaitCursor
	{
	public:
		CWaitCursor()
		{
			m_hOrigCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
		}
		~CWaitCursor()
		{
			::SetCursor(m_hOrigCursor);
		}
	protected:
		HCURSOR m_hOrigCursor;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CVariant : public VARIANT
	{
	public:
		CVariant()
		{
			VariantInit(this);
		}
		CVariant(int i)
		{
			VariantInit(this);
			this->vt = VT_I4;
			this->intVal = i;
		}
		CVariant(float f)
		{
			VariantInit(this);
			this->vt = VT_R4;
			this->fltVal = f;
		}
		CVariant(LPOLESTR s)
		{
			VariantInit(this);
			this->vt = VT_BSTR;
			this->bstrVal = s;
		}
		CVariant(IDispatch* disp)
		{
			VariantInit(this);
			this->vt = VT_DISPATCH;
			this->pdispVal = disp;
		}

		~CVariant()
		{
			VariantClear(this);
		}
	};

}
/////////////////////////////////////////////////////////////////////////////////////

#endif /* __DUI_UTILS_H__ */

