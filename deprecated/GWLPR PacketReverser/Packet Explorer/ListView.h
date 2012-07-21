#ifndef ListView_h__
#define ListView_h__
#include "stdafx.h"
#include "BaseWindow.h"

namespace WindowPP
{
	class ListView : public BaseWindow
	{
	public:
		ListView( DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow( WindowClass::GetClass(WC_LISTVIEW)->lpszClassName ,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		ListView( DWORD dwStyle,DWORD dwExStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow(dwExStyle,WindowClass::GetClass(WC_LISTVIEW)->lpszClassName,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		~ListView(){}

		int InsertColumn(int iCol,LPLVCOLUMN pcol)
		{
			return ListView_InsertColumn(*this,iCol,pcol);
		}
		int InsertItem(LPLVITEM pitem)
		{
			return ListView_InsertItem(*this,pitem);
		}
		int SetItem(LPLVITEM pitem)
		{
			return ListView_SetItem(*this,pitem);
		}
		BOOL DeleteAllItems()
		{
			return ListView_DeleteAllItems(*this);
		}
	};
}

#endif // ListView_h__
