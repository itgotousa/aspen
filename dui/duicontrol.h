#ifndef __DUI_CONTROL_H__
#define __DUI_CONTROL_H__

#include "duiutils.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace DUI
{
    typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

    class DUI_API CControlUI
    {
    public:
        CControlUI() :
            m_pManager(nullptr),
            m_pParent(nullptr),
            m_pCover(nullptr),
            m_bUpdateNeeded(true),
            m_bMenuUsed(false),
            m_bAsyncNotify(false),
            m_bVisible(true),
            m_bInternVisible(true),
            m_bFocused(false),
            m_bEnabled(true),
            m_bMouseEnabled(true),
            m_bKeyboardEnabled(true),
            m_bFloat(false),
            m_bSetPos(false),
            m_chShortcut('\0'),
            m_pTag(NULL),
            m_dwBackColor(0),
            m_dwBackColor2(0),
            m_dwBackColor3(0),
            m_dwBorderColor(0),
            m_dwFocusBorderColor(0),
            m_bColorHSL(false),
            m_nBorderStyle(PS_SOLID),
            m_nTooltipWidth(300)
        {
            m_cXY.cx = m_cXY.cy = 0;
            m_cxyFixed.cx = m_cxyFixed.cy = 0;
            m_cxyMin.cx = m_cxyMin.cy = 0;
            m_cxyMax.cx = m_cxyMax.cy = 9999;
            m_cxyBorderRound.cx = m_cxyBorderRound.cy = 0;

            ::ZeroMemory(&m_rcPadding, sizeof(RECT));
            ::ZeroMemory(&m_rcItem, sizeof(RECT));
            ::ZeroMemory(&m_rcPaint, sizeof(RECT));
            ::ZeroMemory(&m_rcBorderSize, sizeof(RECT));
            m_piFloatPercent.left = m_piFloatPercent.top = m_piFloatPercent.right = m_piFloatPercent.bottom = 0.0f;
        }

        virtual void Delete()
        {
            if (m_pManager) 
                m_pManager->RemoveMouseLeaveNeeded(this);
            delete this;
        }

    protected:
        virtual ~CControlUI()
        {
            if (nullptr != m_pCover) 
            {
                m_pCover->Delete();
                m_pCover = nullptr;
            }

            RemoveAllCustomAttribute();
            if (OnDestroy) 
                OnDestroy(this);
            if (nullptr != m_pManager) 
                m_pManager->ReapObjects(this);

        }

    public:
        virtual CDUIString GetName() const
        {
            return m_sName;
        }

        virtual void SetName(LPCTSTR pstrName)
        {
            if (m_sName != pstrName) 
            {
                m_sName = pstrName;
                if (nullptr != m_pManager) 
                    m_pManager->RenameControl(this, pstrName);
            }

        }
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual UINT GetControlFlags() const;
        virtual HWND GetNativeWindow() const;

        virtual bool Activate();
        virtual CPaintManagerUI* GetManager() const;
        virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
        virtual CControlUI* GetParent() const;
        virtual CControlUI* GetCover() const;
        virtual void SetCover(CControlUI* pControl);

        virtual CDUIString GetText() const;
        virtual void SetText(LPCTSTR pstrText);

        DWORD GetBkColor() const;
        void SetBkColor(DWORD dwBackColor);
        DWORD GetBkColor2() const;
        void SetBkColor2(DWORD dwBackColor);
        DWORD GetBkColor3() const;
        void SetBkColor3(DWORD dwBackColor);
        LPCTSTR GetBkImage();
        void SetBkImage(LPCTSTR pStrImage);
        DWORD GetFocusBorderColor() const;
        void SetFocusBorderColor(DWORD dwBorderColor);
        bool IsColorHSL() const;
        void SetColorHSL(bool bColorHSL);
        SIZE GetBorderRound() const;
        void SetBorderRound(SIZE cxyRound);
        bool DrawImage(HDC hDC, TDrawInfo& drawInfo);

        DWORD GetBorderColor() const;
        void SetBorderColor(DWORD dwBorderColor);
        RECT GetBorderSize() const;
        void SetBorderSize(RECT rc);
        void SetBorderSize(int iSize);
        int GetBorderStyle() const;
        void SetBorderStyle(int nStyle);

        virtual const RECT& GetPos() const;
        virtual RECT GetRelativePos() const;
        virtual RECT GetClientPos() const;
        virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
        virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);
        virtual int GetWidth() const;
        virtual int GetHeight() const;
        virtual int GetX() const;
        virtual int GetY() const;
        virtual RECT GetPadding() const;
        virtual void SetPadding(RECT rcPadding);
        virtual SIZE GetFixedXY() const;    
        virtual void SetFixedXY(SIZE szXY); 
        virtual TPercentInfo GetFloatPercent() const;
        virtual void SetFloatPercent(TPercentInfo piFloatPercent);
        virtual int GetFixedWidth() const;  
        virtual void SetFixedWidth(int cx); 
        virtual int GetFixedHeight() const; 
        virtual void SetFixedHeight(int cy);
        virtual int GetMinWidth() const;
        virtual void SetMinWidth(int cx);
        virtual int GetMaxWidth() const;
        virtual void SetMaxWidth(int cx);
        virtual int GetMinHeight() const;
        virtual void SetMinHeight(int cy);
        virtual int GetMaxHeight() const;
        virtual void SetMaxHeight(int cy);

        virtual CDUIString GetToolTip() const;
        virtual void SetToolTip(LPCTSTR pstrText);
        virtual void SetToolTipWidth(int nWidth);
        virtual int	  GetToolTipWidth(void);

        virtual TCHAR GetShortcut() const;
        virtual void SetShortcut(TCHAR ch);

        virtual bool IsContextMenuUsed() const;
        virtual void SetContextMenuUsed(bool bMenuUsed);

        virtual const CDUIString& GetUserData();
        virtual void SetUserData(LPCTSTR pstrText);
        virtual UINT_PTR GetTag() const;
        virtual void SetTag(UINT_PTR pTag);

        virtual bool IsVisible() const;
        virtual void SetVisible(bool bVisible = true);
        virtual void SetInternVisible(bool bVisible = true); 
        virtual bool IsEnabled() const;
        virtual void SetEnabled(bool bEnable = true);
        virtual bool IsMouseEnabled() const;
        virtual void SetMouseEnabled(bool bEnable = true);
        virtual bool IsKeyboardEnabled() const;
        virtual void SetKeyboardEnabled(bool bEnable = true);
        virtual bool IsFocused() const;
        virtual void SetFocus();
        virtual bool IsFloat() const;
        virtual void SetFloat(bool bFloat = true);

        void AddCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
        LPCTSTR GetCustomAttribute(LPCTSTR pstrName) const;
        bool RemoveCustomAttribute(LPCTSTR pstrName);
        void RemoveAllCustomAttribute();

        virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

        void Invalidate();
        bool IsUpdateNeeded() const;
        void NeedUpdate();
        void NeedParentUpdate();
        DWORD GetAdjustColor(DWORD dwColor);

        virtual void Init();
        virtual void DoInit();

        virtual void Event(TEventUI& event);
        virtual void DoEvent(TEventUI& event);

        virtual CDUIString GetAttribute(LPCTSTR pstrName);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        virtual CDUIString GetAttributeList(bool bIgnoreDefault = true);
        virtual void SetAttributeList(LPCTSTR pstrList);

        virtual SIZE EstimateSize(SIZE szAvailable);

        virtual bool Paint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl = NULL);
        virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
        virtual void PaintBkColor(HDC hDC);
        virtual void PaintBkImage(HDC hDC);
        virtual void PaintStatusImage(HDC hDC);
        virtual void PaintText(HDC hDC);
        virtual void PaintBorder(HDC hDC);

        virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

        void SetVirtualWnd(LPCTSTR pstrValue);
        CDUIString GetVirtualWnd() const;

    public:
        CEventSource OnInit;
        CEventSource OnDestroy;
        CEventSource OnSize;
        CEventSource OnEvent;
        CEventSource OnNotify;
        CEventSource OnPaint;
        CEventSource OnPostPaint;

    protected:
        CPaintManagerUI* m_pManager = nullptr;
        CControlUI* m_pParent = nullptr;
        CControlUI* m_pCover = nullptr;
        CDUIString m_sVirtualWnd;
        CDUIString m_sName;
        bool m_bUpdateNeeded;
        bool m_bMenuUsed;
        bool m_bAsyncNotify;
        RECT m_rcItem;
        RECT m_rcPadding;
        SIZE m_cXY;
        SIZE m_cxyFixed;
        SIZE m_cxyMin;
        SIZE m_cxyMax;
        bool m_bVisible;
        bool m_bInternVisible;
        bool m_bEnabled;
        bool m_bMouseEnabled;
        bool m_bKeyboardEnabled;
        bool m_bFocused;
        bool m_bFloat;
        TPercentInfo m_piFloatPercent;
        bool m_bSetPos; 

        CDUIString m_sText;
        CDUIString m_sToolTip;
        TCHAR m_chShortcut;
        CDUIString m_sUserData;
        UINT_PTR m_pTag;

        DWORD m_dwBackColor;
        DWORD m_dwBackColor2;
        DWORD m_dwBackColor3;
        TDrawInfo m_diBk;
        TDrawInfo m_diFore;
        DWORD m_dwBorderColor;
        DWORD m_dwFocusBorderColor;
        bool m_bColorHSL;
        int m_nBorderStyle;
        int m_nTooltipWidth;
        SIZE m_cxyBorderRound;
        RECT m_rcPaint;
        RECT m_rcBorderSize;
        CDUIStringPtrMap m_mCustomAttrHash;
    };
}
/////////////////////////////////////////////////////////////////////////////////////

#endif /* __DUI_CONTROL_H__ */