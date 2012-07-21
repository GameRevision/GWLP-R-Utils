#ifndef StaticControl_h__
#define StaticControl_h__
#include "stdafx.h"
#include "BaseWindow.h"

namespace WindowPP
{
	class StaticControl : public BaseWindow
	{
	public:
		StaticControl( DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow( WindowClass::GetClass(WC_STATIC)->lpszClassName ,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		StaticControl( DWORD dwStyle,DWORD dwExStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow(dwExStyle,WindowClass::GetClass(WC_STATIC)->lpszClassName,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		~StaticControl(){}
	};
}

#endif // StaticControl_h__
