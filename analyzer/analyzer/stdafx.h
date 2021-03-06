
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <afx.h>





//第一种  

//1. 在APP开头处包含以下代码 

 #define CRTDBG_MAP_ALLOC 

 #include <stdlib.h> 

 #include <crtdbg.h> 




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



//#define MESSAGE_BUSY			WM_USER+1
//#define MESSAGE_WAIT_RESPONSE	WM_USER+2
//#define MESSAGE_OVER			WM_USER+3
//#define MESSAGE_SWITCH_FIGURE	WM_USER+4
//#define MESSAGE_READY			WM_USER+5
//#define MESSAGE_OVER_H			WM_USER+6
#define MESSAGE_CHANGE_APPLOOK	WM_USER+7
//#define MESSAGE_GET_PLOTSPEC	WM_USER+8
#define MESSAGE_UPDATE_DOL		WM_USER+9
#define MESSAGE_UPDATE_VIEW		WM_USER+10
#define MESSAGE_UPDATE_RAW		WM_USER+11
#define MESSAGE_UPDATE_TEST		WM_USER+12
#define MESSAGE_SHOW_DOL		WM_USER+13
//#define MESSAGE_COMPUTE_RESULT	WM_USER+14
//#define MESSAGE_REFRESH_DOL		WM_USER+15
//#define MESSAGE_CHANGE_ANP		WM_USER+16
//#define MESSAGE_RAW_CHANGE		WM_USER+17
#define MESSAGE_CLOSE_SAP_SHEET	WM_USER+18
#define MESSAGE_CHANGE_LANG		WM_USER+19

#define SPIN_ID 3456
#define PWIN_ID 3457
#define EDRS_ID 3458


