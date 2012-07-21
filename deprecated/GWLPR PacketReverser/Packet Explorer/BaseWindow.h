#ifndef BaseWindow_h__
#define BaseWindow_h__
/*
 © 2011 ACB
*/
#include "stdafx.h"
#include "WindowClass.h"

#define WINDOWPP_THIS_PROP TEXT("WINDOWPP::this")



namespace WindowPP
{
	class BaseWindow
	{
		friend class WindowClass;
	public:
		~BaseWindow()
		{
			::DestroyWindow(m_HWND);
		}


		operator HWND() { return m_HWND; }

		BaseWindow* GetParent()
		{ 
			HWND wnd = ::GetParent(m_HWND);
			if(!wnd)
				return NULL;
			return reinterpret_cast<BaseWindow*>(GetProp(wnd,WINDOWPP_THIS_PROP)); 
		}

		BaseWindow* GetOwner()
		{ 
			HWND wnd = ::GetWindow(m_HWND,GW_OWNER);
			return reinterpret_cast<BaseWindow*>(GetProp(wnd,WINDOWPP_THIS_PROP)); 
		}

		BOOL GetWindowRect(LPRECT rect){ return ::GetWindowRect(m_HWND,rect); }

		BOOL GetClientRect(LPRECT rect){ return ::GetClientRect(m_HWND,rect); }

		int GetX()
		{
			RECT r;
			if(GetWindowRect(&r) == FALSE)
				return -1;
			if(MapWindowPoints(NULL,*GetParent(),(LPPOINT)&r,2) == FALSE)
				return -1;
			return r.left;
		}
		BOOL SetX(int x,BOOL bRepaint = TRUE){ return MoveWindow(x,GetY(),GetWidth(),GetHeight(),bRepaint); }

		int GetY()
		{
			RECT r;
			if(GetWindowRect(&r) == FALSE)
				return -1;
			if(MapWindowPoints(NULL,*GetParent(),(LPPOINT)&r,2) == FALSE)
				return -1;
			return r.top;
		}
		BOOL SetY(int y,BOOL bRepaint = TRUE){ return MoveWindow(GetX(),y,GetWidth(),GetHeight(),bRepaint); }

		int GetWidth()
		{
			RECT r;
			if(GetWindowRect(&r) == FALSE)
				return -1;
			return r.right - r.left;
		}
		int GetClientRectWidth()
		{
			RECT r;
			if(GetClientRect(&r) == FALSE)
				return -1;
			return r.right;
		}
		BOOL SetWidth(int nWidth,BOOL bRepaint = TRUE){ return MoveWindow(GetX(),GetY(),nWidth,GetHeight(),bRepaint); }

		int GetHeight()
		{
			RECT r;
			if(GetWindowRect(&r) == FALSE)
				return -1;
			return r.bottom - r.top;
		}
		int GetClientRectHeight()
		{
			RECT r;
			if(GetClientRect(&r) == FALSE)
				return -1;
			return r.bottom;
		}
		BOOL SetHeight(int nHeight,BOOL bRepaint = TRUE){ return MoveWindow(GetX(),GetY(),GetWidth(),nHeight,bRepaint); }

		BOOL MoveWindow(int x,int y,int nWidth,int nHeigth,BOOL bRepaint){ return ::MoveWindow(m_HWND,x,y,nWidth,nHeigth,bRepaint); }

		LRESULT SendMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			return ::SendMessage(m_HWND,uMsg,wParam,lParam);
		}

		HWND GetDlgItem(int nIDDLgItem)
		{
			return ::GetDlgItem(m_HWND,nIDDLgItem);
		}

		BOOL SetWindowText(std::tstring text)
		{
			return ::SetWindowText(m_HWND,text.c_str());
		}

		int GetWindowTextLength()
		{
			return ::GetWindowTextLength(m_HWND);
		}

		BOOL GetWindowText(std::tstring *text)
		{
			return ::GetWindowText(m_HWND,const_cast<LPTSTR>(text->c_str()),text->size());
		}

		BOOL UpdateWindow()
		{
			return ::UpdateWindow(m_HWND);
		}

		BOOL ShowWindow(BOOL show){ return ::ShowWindow(m_HWND,show); }

		HDC GetWindowDC(){ return ::GetWindowDC(m_HWND); }

		int ReleaseDC(HDC dc){ return ::ReleaseDC(m_HWND,dc); }

		BOOL InvalidateRect(const RECT* lpRect,BOOL bErase = true){ return ::InvalidateRect(m_HWND,lpRect,bErase); }
	protected:
		BaseWindow()
			:m_HWND(0)
		{}

		BaseWindow(HWND wnd)
			:m_HWND(wnd)
		{
			SetProp(m_HWND,WINDOWPP_THIS_PROP,reinterpret_cast<HANDLE>(this));
		}

		BaseWindow( std::tstring ClassName ,std::tstring WindowName,DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
		{
			m_HWND = CreateWindow(ClassName.c_str(),WindowName.c_str(),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance,reinterpret_cast<LPVOID>(this));
		}

		BaseWindow( DWORD dwExStyle,std::tstring ClassName,std::tstring WindowName,DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
		{
			m_HWND = CreateWindowEx(dwExStyle,ClassName.c_str(),WindowName.c_str(),dwStyle,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance,reinterpret_cast<LPVOID>(this));
		}

		void SetHWND(HWND wnd){ m_HWND = wnd;}

		virtual LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			return DefWindowProc(hWnd,uMsg,wParam,lParam);
		}
	private:
		static LRESULT CALLBACK WndProcDelegate(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			if(uMsg == WM_NCCREATE)
			{
				SetProp(hWnd,WINDOWPP_THIS_PROP,reinterpret_cast<HANDLE>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams));
				return TRUE;
			}
			BaseWindow* pThis = reinterpret_cast<BaseWindow*>(GetProp(hWnd,WINDOWPP_THIS_PROP));
			if(pThis == NULL)
				return DefWindowProc(hWnd,uMsg,wParam,lParam);
			return pThis->WndProc(hWnd,uMsg,wParam,lParam);
		}

	private:
		HWND m_HWND;
	};
}
#endif // BaseWindow_h__