#ifndef ListBox_h__
#define ListBox_h__
/*
 © 2011 ACB
*/
#include "stdafx.h"
#include "BaseWindow.h"

namespace WindowPP
{
	class ListBox : public BaseWindow
	{
	public:
		ListBox( DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow( WindowClass::GetClass(WC_LISTBOX)->lpszClassName ,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		ListBox( DWORD dwStyle,DWORD dwExStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
			:BaseWindow(dwExStyle,WindowClass::GetClass(WC_LISTBOX)->lpszClassName,TEXT(""),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
		{}

		~ListBox(void){}


		LRESULT AddString(std::tstring str,int pos = -1)
		{
			return SendMessage(LB_INSERTSTRING,pos,reinterpret_cast<LPARAM>(str.c_str()));
		}

		LRESULT DeleteString( int pos )
		{
			return SendMessage(LB_INSERTSTRING,pos,0);
		}

		int FindString( int searchStart,std::tstring str,bool exact )
		{
			if(exact)
			{
				return SendMessage(LB_FINDSTRINGEXACT,searchStart,reinterpret_cast<LPARAM>(str.c_str()));
			}
			return SendMessage(LB_FINDSTRING,searchStart,reinterpret_cast<LPARAM>(str.c_str()));
		}

		int GetCount()
		{
			return SendMessage(LB_GETCOUNT,0,0);
		}

		LRESULT GetCurSel()
		{
			return SendMessage(LB_GETCURSEL,0,0);
		}

		template<typename T> LRESULT SetItemData(T data,int pos)
		{
			static_assert(sizeof(T) <= sizeof(LPARAM));
			return SendMessage(LB_SETITEMDATA,pos,reinterpret_cast<LPARAM>(data));
		}

		template<typename T> T GetItemData(int pos)
		{
			static_assert(sizeof(T) <= sizeof(LRESULT));
			return reinterpret_cast<T>(SendMessage(LB_GETITEMDATA,pos,0));
		}

		LRESULT GetTextLen(int pos)
		{
			return SendMessage(LB_GETTEXTLEN,pos,0);
		}

		LRESULT GetText(std::tstring* text,int pos)
		{
			return SendMessage(LB_GETTEXT,pos,reinterpret_cast<LPARAM>(text->c_str()));
		}

		LRESULT GetItemHeight(int pos = WPARAM())
		{
			return SendMessage(LB_GETITEMHEIGHT,pos,LPARAM());
		}

		LRESULT SetItemHeight(int pos,int height)
		{
			return SendMessage(LB_SETITEMHEIGHT,pos,height);
		}

		LRESULT ResetContent(){ return SendMessage(LB_RESETCONTENT,0,0); }
	};
}
#endif // ListBox_h__


