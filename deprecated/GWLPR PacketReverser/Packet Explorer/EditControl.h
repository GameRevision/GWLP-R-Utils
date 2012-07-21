#ifndef EditControl_h__
#define EditControl_h__
/*
 © 2011 ACB
*/
#include "stdafx.h"
#include "BaseWindow.h"

namespace WindowPP
{
	class EditControl : public BaseWindow
	{
	public:
		EditControl( DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow( WindowClass::GetClass(WC_EDIT)->lpszClassName ,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		EditControl( DWORD dwStyle,DWORD dwExStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow(dwExStyle,WindowClass::GetClass(WC_EDIT)->lpszClassName,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		~EditControl(){}
	};
}

#endif // EditControl_h__
