#ifndef __DUI_WND_H__
#define __DUI_WND_H__

#define DUILIB_API

#include "UIDefine.h"
#include "UIManager.h"
#include "UIBase.h"

using namespace DuiLib;

class CDUIWnd : public CWindowWnd, public INotifyUI
{
public:
    LPCTSTR GetWindowClassName() const
    {
        return _T("DUIAspen");
    }
};

#endif /* __DUI_WND_H__ */