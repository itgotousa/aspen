#ifndef __DUI_CONFIG_H__
#define __DUI_CONFIG_H__

/////////////////////////////////////////////////////////////////////////////////////
namespace DUI
{
#define DUI_API

#ifndef DUI_ASSERT
#define DUI_ASSERT(expr)  _ASSERTE(expr)
#endif

#ifdef _DEBUG
#ifndef DUITRACE
#define DUITRACE DUI__Trace
#endif
#define DUITRACEMSG DUI__TraceMsg
#else
#ifndef DUITRACE
#define DUITRACE
#endif
#define DUITRACEMSG _T("")
#endif


}
/////////////////////////////////////////////////////////////////////////////////////

#endif /* __DUI_CONFIG_H__ */