#ifndef SubclassedControl_h__
#define SubclassedControl_h__
/*
 © 2011 ACB
*/
#include "stdafx.h"
#include "BaseWindow.h"


#define BASE_CONTROLS_SUBCLASS_ID 0x1337

namespace WindowPP
{
	class SubclassedControl : public BaseWindow
	{
	public:
		~SubclassedControl(void){}

	protected:
		SubclassedControl( std::tstring ClassName,std::tstring WindowName,DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance)
			:BaseWindow(0,ClassName.c_str(),WindowName,dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{
			SetWindowSubclass(*this,SubclassedControl::SubclassProcDelegate,BASE_CONTROLS_SUBCLASS_ID,reinterpret_cast<DWORD_PTR>(this));
		}

		SubclassedControl( DWORD dwExStyle,std::tstring ClassName,std::tstring WindowName,DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow(dwExStyle,ClassName.c_str(),WindowName,dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{
			SetWindowSubclass(*this,SubclassedControl::SubclassProcDelegate,BASE_CONTROLS_SUBCLASS_ID,reinterpret_cast<DWORD_PTR>(this));
		}

	protected:
		virtual LRESULT CALLBACK SubclassProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,UINT_PTR uIdSubclass,DWORD_PTR dwRefData)
		{
			return DefSubclassProc(hWnd,uMsg,wParam,lParam);
		}

	private:
		static LRESULT CALLBACK SubclassProcDelegate( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,UINT_PTR uIdSubclass,DWORD_PTR dwRefData )
		{
			SubclassedControl* pThis = reinterpret_cast<SubclassedControl*>(dwRefData);
			return pThis->SubclassProc(hWnd,uMsg,wParam,lParam,uIdSubclass,dwRefData);
		}
	};


}
#endif // SubclassedControl_h__