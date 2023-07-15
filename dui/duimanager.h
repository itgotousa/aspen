#ifndef __DUI_MANAGER_H__
#define __DUI_MANAGER_H__

#include "duiutils.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace DUI
{
    class CControlUI;

    typedef enum EVENTTYPE_UI
    {
        UIEVENT__FIRST = 1,
        UIEVENT__KEYBEGIN,
        UIEVENT_KEYDOWN,
        UIEVENT_KEYUP,
        UIEVENT_CHAR,
        UIEVENT_SYSKEY,
        UIEVENT__KEYEND,
        UIEVENT__MOUSEBEGIN,
        UIEVENT_MOUSEMOVE,
        UIEVENT_MOUSELEAVE,
        UIEVENT_MOUSEENTER,
        UIEVENT_MOUSEHOVER,
        UIEVENT_BUTTONDOWN,
        UIEVENT_BUTTONUP,
        UIEVENT_RBUTTONDOWN,
        UIEVENT_DBLCLICK,
        UIEVENT_CONTEXTMENU,
        UIEVENT_SCROLLWHEEL,
        UIEVENT__MOUSEEND,
        UIEVENT_KILLFOCUS,
        UIEVENT_SETFOCUS,
        UIEVENT_WINDOWSIZE,
        UIEVENT_SETCURSOR,
        UIEVENT_TIMER,
        UIEVENT_NOTIFY,
        UIEVENT_COMMAND,
        UIEVENT__LAST,
    };

    // Flags for CControlUI::GetControlFlags()
    #define UIFLAG_TABSTOP       0x00000001
    #define UIFLAG_SETCURSOR     0x00000002
    #define UIFLAG_WANTRETURN    0x00000004

    // Flags for FindControl()
    #define UIFIND_ALL           0x00000000
    #define UIFIND_VISIBLE       0x00000001
    #define UIFIND_ENABLED       0x00000002
    #define UIFIND_HITTEST       0x00000004
    #define UIFIND_UPDATETEST    0x00000008
    #define UIFIND_TOP_FIRST     0x00000010
    #define UIFIND_ME_FIRST      0x80000000

    // Flags for the CDialogLayout stretching
    #define UISTRETCH_NEWGROUP   0x00000001
    #define UISTRETCH_NEWLINE    0x00000002
    #define UISTRETCH_MOVE_X     0x00000004
    #define UISTRETCH_MOVE_Y     0x00000008
    #define UISTRETCH_SIZE_X     0x00000010
    #define UISTRETCH_SIZE_Y     0x00000020

    // Flags used for controlling the paint
    #define UISTATE_FOCUSED      0x00000001
    #define UISTATE_SELECTED     0x00000002
    #define UISTATE_DISABLED     0x00000004
    #define UISTATE_HOT          0x00000008
    #define UISTATE_PUSHED       0x00000010
    #define UISTATE_READONLY     0x00000020
    #define UISTATE_CAPTURED     0x00000040

    typedef struct DUI_API tagTFontInfo
    {
        HFONT hFont;
        CDUIString sFontName;
        int iSize;
        bool bBold;
        bool bUnderline;
        bool bItalic;
        TEXTMETRIC tm;
    } TFontInfo;

    typedef struct DUI_API tagTImageInfo
    {
        HBITMAP hBitmap;
        LPBYTE pBits;
        LPBYTE pSrcBits;
        int nX;
        int nY;
        bool bAlpha;
        bool bUseHSL;
        CDUIString sResType;
        DWORD dwMask;
    } TImageInfo;

    typedef struct DUI_API tagTDrawInfo
    {
        tagTDrawInfo()
        {
            Clear();
        }

        tagTDrawInfo(LPCTSTR lpsz)
        {
            Clear();
            sDrawString = lpsz;
        }

        void Clear()
        {
            sDrawString.Empty();
            sImageName.Empty();
            ::ZeroMemory(&bLoaded, sizeof(tagTDrawInfo) - offsetof(tagTDrawInfo, bLoaded));
            uFade = 255;
        }

        CDUIString sDrawString;
        CDUIString sImageName;
        bool bLoaded;
        const TImageInfo* pImageInfo;
        RECT rcDestOffset;
        RECT rcBmpPart;
        RECT rcScale9;
        BYTE uFade;
        bool bHole;
        bool bTiledX;
        bool bTiledY;
    } TDrawInfo;

    typedef struct DUI_API tagTPercentInfo
    {
        double left;
        double top;
        double right;
        double bottom;
    } TPercentInfo;

    typedef struct DUI_API tagTResInfo
    {
        DWORD m_dwDefaultDisabledColor;
        DWORD m_dwDefaultFontColor;
        DWORD m_dwDefaultLinkFontColor;
        DWORD m_dwDefaultLinkHoverFontColor;
        DWORD m_dwDefaultSelectedBkColor;
        TFontInfo m_DefaultFontInfo;
        CDUIStringPtrMap m_CustomFonts;
        CDUIStringPtrMap m_ImageHash;
        CDUIStringPtrMap m_AttrHash;
        CDUIStringPtrMap m_MultiLanguageHash;
    } TResInfo;

    // Structure for notifications from the system
    // to the control implementation.
    typedef struct DUI_API tagTEventUI
    {
        int Type;
        CControlUI* pSender;
        DWORD dwTimestamp;
        POINT ptMouse;
        TCHAR chKey;
        WORD wKeyState;
        WPARAM wParam;
        LPARAM lParam;
    } TEventUI;

    typedef struct tagTNotifyUI
    {
        CDUIString sType;
        CDUIString sVirtualWnd;
        CControlUI* pSender;
        DWORD dwTimestamp;
        POINT ptMouse;
        WPARAM wParam;
        LPARAM lParam;
    } TNotifyUI;

    // Listener interface
    class DUI_API INotifyUI
    {
    public:
        virtual void Notify(TNotifyUI& msg) = 0;
    };

    // MessageFilter interface
    class DUI_API IMessageFilterUI
    {
    public:
        virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
    };

    class DUI_API ITranslateAccelerator
    {
    public:
        virtual LRESULT TranslateAccelerator(MSG* pMsg) = 0;
    };


    /////////////////////////////////////////////////////////////////////////////////////
    //
    typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);

    HPEN m_hUpdateRectPen = NULL;
    class DUI_API CPaintManagerUI
    {
    public:
        CPaintManagerUI() :
            m_hWndPaint(NULL),
            m_hDcPaint(NULL),
            m_hDcOffscreen(NULL),
            m_hDcBackground(NULL),
            m_hbmpOffscreen(NULL),
            m_pOffscreenBits(NULL),
            m_hbmpBackground(NULL),
            m_pBackgroundBits(NULL),
            m_iTooltipWidth(-1),
            m_iLastTooltipWidth(-1),
            m_hwndTooltip(NULL),
            m_iHoverTime(1000),
            m_bNoActivate(false),
            m_bShowUpdateRect(false),
            m_uTimerID(0x1000),
            m_pRoot(NULL),
            m_pFocus(NULL),
            m_pEventHover(NULL),
            m_pEventClick(NULL),
            m_pEventKey(NULL),
            m_pLastToolTip(NULL),
            m_bFirstLayout(true),
            m_bFocusNeeded(false),
            m_bUpdateNeeded(false),
            m_bMouseTracking(false),
            m_bMouseCapture(false),
            m_bIsPainting(false),
            m_bOffscreenPaint(true),
            m_bUsedVirtualWnd(false),
            m_bAsyncNotifyPosted(false),
            m_bForceUseSharedRes(false),
            m_nOpacity(0xFF),
            m_bLayered(false),
            m_bLayeredChanged(false)
        {
            if (m_SharedResInfo.m_DefaultFontInfo.sFontName.IsEmpty())
            {
                m_SharedResInfo.m_dwDefaultDisabledColor = 0xFFA7A6AA;
                m_SharedResInfo.m_dwDefaultFontColor = 0xFF000000;
                m_SharedResInfo.m_dwDefaultLinkFontColor = 0xFF0000FF;
                m_SharedResInfo.m_dwDefaultLinkHoverFontColor = 0xFFD3215F;
                m_SharedResInfo.m_dwDefaultSelectedBkColor = 0xFFBAE4FF;

                LOGFONT lf = { 0 };
                ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
                lf.lfCharSet = DEFAULT_CHARSET;
                HFONT hDefaultFont = ::CreateFontIndirect(&lf);
                m_SharedResInfo.m_DefaultFontInfo.hFont = hDefaultFont;
                m_SharedResInfo.m_DefaultFontInfo.sFontName = lf.lfFaceName;
                m_SharedResInfo.m_DefaultFontInfo.iSize = -lf.lfHeight;
                m_SharedResInfo.m_DefaultFontInfo.bBold = (lf.lfWeight >= FW_BOLD);
                m_SharedResInfo.m_DefaultFontInfo.bUnderline = (lf.lfUnderline == TRUE);
                m_SharedResInfo.m_DefaultFontInfo.bItalic = (lf.lfItalic == TRUE);
                ::ZeroMemory(&m_SharedResInfo.m_DefaultFontInfo.tm, sizeof(m_SharedResInfo.m_DefaultFontInfo.tm));
            }

            m_ResInfo.m_dwDefaultDisabledColor = m_SharedResInfo.m_dwDefaultDisabledColor;
            m_ResInfo.m_dwDefaultFontColor = m_SharedResInfo.m_dwDefaultFontColor;
            m_ResInfo.m_dwDefaultLinkFontColor = m_SharedResInfo.m_dwDefaultLinkFontColor;
            m_ResInfo.m_dwDefaultLinkHoverFontColor = m_SharedResInfo.m_dwDefaultLinkHoverFontColor;
            m_ResInfo.m_dwDefaultSelectedBkColor = m_SharedResInfo.m_dwDefaultSelectedBkColor;

            if (m_hUpdateRectPen == NULL) 
            {
                m_hUpdateRectPen = ::CreatePen(PS_SOLID, 1, RGB(220, 0, 0));
                // Boot Windows Common Controls (for the ToolTip control)
                ::InitCommonControls();
                ::LoadLibrary(_T("msimg32.dll"));
            }

            m_szMinWindow.cx = 0;
            m_szMinWindow.cy = 0;
            m_szMaxWindow.cx = 0;
            m_szMaxWindow.cy = 0;
            m_szInitWindowSize.cx = 0;
            m_szInitWindowSize.cy = 0;
            m_szRoundCorner.cx = m_szRoundCorner.cy = 0;
            ::ZeroMemory(&m_rcSizeBox, sizeof(m_rcSizeBox));
            ::ZeroMemory(&m_rcCaption, sizeof(m_rcCaption));
            ::ZeroMemory(&m_rcLayeredInset, sizeof(m_rcLayeredInset));
            ::ZeroMemory(&m_rcLayeredUpdate, sizeof(m_rcLayeredUpdate));
            m_ptLastMousePos.x = m_ptLastMousePos.y = -1;

        }

        ~CPaintManagerUI()
        {
#if 0
            // Delete the control-tree structures
            for (int i = 0; i < m_aDelayedCleanup.GetSize(); i++) 
                static_cast<CControlUI*>(m_aDelayedCleanup[i])->Delete();
            for (int i = 0; i < m_aAsyncNotify.GetSize(); i++) 
                delete static_cast<TNotifyUI*>(m_aAsyncNotify[i]);
            
            m_mNameHash.Resize(0);

            if (nullptr != m_pRoot) 
                m_pRoot->Delete();
            ::DeleteObject(m_ResInfo.m_DefaultFontInfo.hFont);
            RemoveAllFonts();
            RemoveAllImages();
            RemoveAllDefaultAttributeList();
            RemoveAllWindowCustomAttribute();
            RemoveAllOptionGroups();
            RemoveAllTimers();

            // Reset other parts...
            if (nullptr != m_hwndTooltip)
            {
                ::DestroyWindow(m_hwndTooltip);
                m_hwndTooltip = nullptr;
            }
            m_pLastToolTip = nullptr;
            if (m_hDcOffscreen != NULL) 
                ::DeleteDC(m_hDcOffscreen);
            if (m_hDcBackground != NULL) 
                ::DeleteDC(m_hDcBackground);
            if (m_hbmpOffscreen != NULL) 
                ::DeleteObject(m_hbmpOffscreen);
            if (m_hbmpBackground != NULL) 
                ::DeleteObject(m_hbmpBackground);
            if (m_hDcPaint != NULL) 
                ::ReleaseDC(m_hWndPaint, m_hDcPaint);
            m_aPreMessages.Remove(m_aPreMessages.Find(this));
#endif
        }

    public:
#if 0
        void Init(HWND hWnd, LPCTSTR pstrName = NULL);
        bool IsUpdateNeeded() const;
        void NeedUpdate();
        void Invalidate();
        void Invalidate(RECT& rcItem);

        LPCTSTR GetName() const;
        HDC GetPaintDC() const;
        HBITMAP GetPaintOffscreenBitmap();
        HWND GetPaintWindow() const;
        HWND GetTooltipWindow() const;
        int GetTooltipWindowWidth() const;
        void SetTooltipWindowWidth(int iWidth);
        int GetHoverTime() const;
        void SetHoverTime(int iTime);

        POINT GetMousePos() const;
        SIZE GetClientSize() const;
        SIZE GetInitSize();
        void SetInitSize(int cx, int cy);
        RECT& GetSizeBox();
        void SetSizeBox(RECT& rcSizeBox);
        RECT& GetCaptionRect();
        void SetCaptionRect(RECT& rcCaption);
        SIZE GetRoundCorner() const;
        void SetRoundCorner(int cx, int cy);
        SIZE GetMinInfo() const;
        void SetMinInfo(int cx, int cy);
        SIZE GetMaxInfo() const;
        void SetMaxInfo(int cx, int cy);
        bool IsShowUpdateRect() const;
        void SetShowUpdateRect(bool show);
        bool IsNoActivate();
        void SetNoActivate(bool bNoActivate);

        BYTE GetOpacity() const;
        void SetOpacity(BYTE nOpacity);

        bool IsLayered();
        void SetLayered(bool bLayered);
        RECT& GetLayeredInset();
        void SetLayeredInset(RECT& rcLayeredInset);
        BYTE GetLayeredOpacity();
        void SetLayeredOpacity(BYTE nOpacity);
        LPCTSTR GetLayeredImage();
        void SetLayeredImage(LPCTSTR pstrImage);

        static HINSTANCE GetInstance();
        static CDUIString GetInstancePath();
        static CDUIString GetCurrentPath();
        static HINSTANCE GetResourceDll();
        static const CDUIString& GetResourcePath();
        static const CDUIString& GetResourceZip();
        static bool IsCachedResourceZip();
        static HANDLE GetResourceZipHandle();
        static void SetInstance(HINSTANCE hInst);
        static void SetCurrentPath(LPCTSTR pStrPath);
        static void SetResourceDll(HINSTANCE hInst);
        static void SetResourcePath(LPCTSTR pStrPath);
        static void SetResourceZip(LPVOID pVoid, unsigned int len);
        static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false);
        static bool GetHSL(short* H, short* S, short* L);
        static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
        static void ReloadSkin();
        static CPaintManagerUI* GetPaintManager(LPCTSTR pstrName);
        static CDUIPtrArray* GetPaintManagers();
        static bool LoadPlugin(LPCTSTR pstrModuleName);
        static CDUIPtrArray* GetPlugins();

        bool IsForceUseSharedRes() const;
        void SetForceUseSharedRes(bool bForce);

        DWORD GetDefaultDisabledColor() const;
        void SetDefaultDisabledColor(DWORD dwColor, bool bShared = false);
        DWORD GetDefaultFontColor() const;
        void SetDefaultFontColor(DWORD dwColor, bool bShared = false);
        DWORD GetDefaultLinkFontColor() const;
        void SetDefaultLinkFontColor(DWORD dwColor, bool bShared = false);
        DWORD GetDefaultLinkHoverFontColor() const;
        void SetDefaultLinkHoverFontColor(DWORD dwColor, bool bShared = false);
        DWORD GetDefaultSelectedBkColor() const;
        void SetDefaultSelectedBkColor(DWORD dwColor, bool bShared = false);

        TFontInfo* GetDefaultFontInfo();
        void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
        DWORD GetCustomFontCount(bool bShared = false) const;
        HFONT AddFont(int id, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
        HFONT GetFont(int id);
        HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
        int GetFontIndex(HFONT hFont, bool bShared = false);
        int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
        void RemoveFont(HFONT hFont, bool bShared = false);
        void RemoveFont(int id, bool bShared = false);
        void RemoveAllFonts(bool bShared = false);
        TFontInfo* GetFontInfo(int id);
        TFontInfo* GetFontInfo(HFONT hFont);

        const TImageInfo* GetImage(LPCTSTR bitmap);
        const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, bool bUseHSL = false);
        const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0, bool bUseHSL = false, bool bShared = false);
        const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha, bool bShared = false);
        void RemoveImage(LPCTSTR bitmap, bool bShared = false);
        void RemoveAllImages(bool bShared = false);
        static void ReloadSharedImages();
        void ReloadImages();

        void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, bool bShared = false);
        LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
        bool RemoveDefaultAttributeList(LPCTSTR pStrControlName, bool bShared = false);
        void RemoveAllDefaultAttributeList(bool bShared = false);

        void AddWindowCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
        LPCTSTR GetWindowCustomAttribute(LPCTSTR pstrName) const;
        bool RemoveWindowCustomAttribute(LPCTSTR pstrName);
        void RemoveAllWindowCustomAttribute();

        CDUIString GetWindowAttribute(LPCTSTR pstrName);
        void SetWindowAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        CDUIString GetWindowAttributeList(bool bIgnoreDefault = true);
        void SetWindowAttributeList(LPCTSTR pstrList);
        bool RemoveWindowAttribute(LPCTSTR pstrName);

        CDUIString GetWindowXML();

        static void AddMultiLanguageString(int id, LPCTSTR pStrMultiLanguage);
        static LPCTSTR GetMultiLanguageString(int id);
        static bool RemoveMultiLanguageString(int id);
        static void RemoveAllMultiLanguageString();
        static void ProcessMultiLanguageTokens(CDUIString& pStrMultiLanguage);

        bool AttachDialog(CControlUI* pControl);
        bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
        bool RenameControl(CControlUI* pControl, LPCTSTR pstrName);
        void ReapObjects(CControlUI* pControl);

        bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
        CDUIPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
        void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
        void RemoveAllOptionGroups();

        CControlUI* GetFocus() const;
        void SetFocus(CControlUI* pControl, bool bFocusWnd = true);
        void SetFocusNeeded(CControlUI* pControl);

        bool SetNextTabControl(bool bForward = true);

        bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
        bool KillTimer(CControlUI* pControl, UINT nTimerID);
        void KillTimer(CControlUI* pControl);
        void RemoveAllTimers();

        void SetCapture();
        void ReleaseCapture();
        bool IsCaptured();

        bool IsPainting();
        void SetPainting(bool bIsPainting);

        bool AddNotifier(INotifyUI* pControl);
        bool RemoveNotifier(INotifyUI* pControl);
        void SendNotify(TNotifyUI& Msg, bool bAsync = false, bool bEnableRepeat = true);
        void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false, bool bEnableRepeat = true);

        bool AddPreMessageFilter(IMessageFilterUI* pFilter);
        bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

        bool AddMessageFilter(IMessageFilterUI* pFilter);
        bool RemoveMessageFilter(IMessageFilterUI* pFilter);

        int GetPostPaintCount() const;
        bool AddPostPaint(CControlUI* pControl);
        bool RemovePostPaint(CControlUI* pControl);
        bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

        int GetNativeWindowCount() const;
        RECT GetNativeWindowRect(HWND hChildWnd);
        bool AddNativeWindow(CControlUI* pControl, HWND hChildWnd);
        bool RemoveNativeWindow(HWND hChildWnd);

        void AddDelayedCleanup(CControlUI* pControl);
        void AddMouseLeaveNeeded(CControlUI* pControl);
        bool RemoveMouseLeaveNeeded(CControlUI* pControl);

        bool AddTranslateAccelerator(ITranslateAccelerator* pTranslateAccelerator);
        bool RemoveTranslateAccelerator(ITranslateAccelerator* pTranslateAccelerator);
        bool TranslateAccelerator(LPMSG pMsg);

        CControlUI* GetRoot() const;
        CControlUI* FindControl(POINT pt) const;
        CControlUI* FindControl(LPCTSTR pstrName) const;
        CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
        CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
        CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
        CDUIPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);

        static int MessageLoop();
        static bool TranslateMessage(const LPMSG pMsg);
        static void Term();

        bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
        bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
        void UsedVirtualWnd(bool bUsed);

    private:
        CDUIPtrArray* GetFoundControls();
        static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);
        static CControlUI* CALLBACK __FindControlsFromUpdate(CControlUI* pThis, LPVOID pData);

        static void AdjustSharedImagesHSL();
        void AdjustImagesHSL();
        void PostAsyncNotify();
#endif
    private:
        CDUIString m_sName;
        HWND m_hWndPaint;
        HDC m_hDcPaint;
        HDC m_hDcOffscreen;
        HDC m_hDcBackground;
        HBITMAP m_hbmpOffscreen;
        COLORREF* m_pOffscreenBits;
        HBITMAP m_hbmpBackground;
        COLORREF* m_pBackgroundBits;
        int m_iTooltipWidth;
        int m_iLastTooltipWidth;
        HWND m_hwndTooltip;
        TOOLINFO m_ToolTip;
        int m_iHoverTime;
        bool m_bNoActivate;
        bool m_bShowUpdateRect;
        //
        CControlUI* m_pRoot;
        CControlUI* m_pFocus;
        CControlUI* m_pEventHover;
        CControlUI* m_pEventClick;
        CControlUI* m_pEventKey;
        CControlUI* m_pLastToolTip;
        //
        POINT m_ptLastMousePos;
        SIZE m_szMinWindow;
        SIZE m_szMaxWindow;
        SIZE m_szInitWindowSize;
        RECT m_rcSizeBox;
        SIZE m_szRoundCorner;
        RECT m_rcCaption;
        UINT m_uTimerID;
        bool m_bFirstLayout;
        bool m_bUpdateNeeded;
        bool m_bFocusNeeded;
        bool m_bOffscreenPaint;

        BYTE m_nOpacity;
        bool m_bLayered;
        RECT m_rcLayeredInset;
        bool m_bLayeredChanged;
        RECT m_rcLayeredUpdate;
        TDrawInfo m_diLayered;

        bool m_bMouseTracking;
        bool m_bMouseCapture;
        bool m_bIsPainting;
        bool m_bUsedVirtualWnd;
        bool m_bAsyncNotifyPosted;

        //
        CDUIPtrArray m_aNotifiers;
        CDUIPtrArray m_aTimers;
        CDUIPtrArray m_aPreMessageFilters;
        CDUIPtrArray m_aMessageFilters;
        CDUIPtrArray m_aPostPaintControls;
        CDUIPtrArray m_aNativeWindow;
        CDUIPtrArray m_aNativeWindowControl;
        CDUIPtrArray m_aDelayedCleanup;
        CDUIPtrArray m_aAsyncNotify;
        CDUIPtrArray m_aFoundControls;
        CDUIPtrArray m_aNeedMouseLeaveNeeded;
        CDUIStringPtrMap m_mNameHash;
        CDUIStringPtrMap m_mWindowAttrHash;
        CDUIStringPtrMap m_mOptionGroup;

        //
        bool m_bForceUseSharedRes;
        TResInfo m_ResInfo;

        //
        static HINSTANCE m_hResourceInstance;
        static CDUIString m_pStrResourcePath;
        static CDUIString m_pStrResourceZip;
        static HANDLE m_hResourceZip;

        static bool m_bCachedResourceZip;
        static TResInfo m_SharedResInfo;
        static HINSTANCE m_hInstance;
        static bool m_bUseHSL;
        static short m_H;
        static short m_S;
        static short m_L;
        static CDUIPtrArray m_aPreMessages;
        static CDUIPtrArray m_aPlugins;

    public:
        CDUIPtrArray m_aTranslateAccelerator;
    };

}
/////////////////////////////////////////////////////////////////////////////////////

#endif /* __DUI_MANAGER_H__ */