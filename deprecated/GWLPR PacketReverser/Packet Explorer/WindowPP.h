#ifndef WindowPP_h__
#define WindowPP_h__
/*
 © 2011 ACB
*/
/*
	stripped from my WindowPP lib
	*currently in development :P
*/
/*
	most functions should be auto inlined
*/
#include "stdafx.h"
#include "WindowPPException.h"
#include "WindowClass.h"
#include "BaseWindow.h"
#include "SubclassedControl.h"
#include "RescaleControl.h"
#include "ListBox.h"
#include "EditControl.h"
#include "StaticControl.h"
#include "ListView.h"

namespace WindowPP
{
	inline BOOL InitWindowPP()
	{
		//InitCommonControls();
		INITCOMMONCONTROLSEX init;
		init.dwSize = sizeof(INITCOMMONCONTROLSEX);
		init.dwICC = ICC_ANIMATE_CLASS | ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_DATE_CLASSES | ICC_HOTKEY_CLASS | ICC_INTERNET_CLASSES | ICC_LINK_CLASS | ICC_LISTVIEW_CLASSES |
			ICC_NATIVEFNTCTL_CLASS | ICC_PAGESCROLLER_CLASS | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES | ICC_UPDOWN_CLASS |ICC_USEREX_CLASSES;

		InitCommonControlsEx(&init);

		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		return VerticalRescaleControl::Init() && 
			HorizontalRescaleControl::Init();
	}
}
#endif // WindowPP_h__