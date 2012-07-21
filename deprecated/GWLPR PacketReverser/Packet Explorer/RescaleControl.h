#ifndef RescaleControl_h__
#define RescaleControl_h__
/*
 © 2011 ACB
*/
#include "stdafx.h"
#include "BaseWindow.h"

#define HORIZONTAL_RESCALECONTROL_CLASS TEXT ("HorizontalRescaleControl")
#define VERTICAL_RESCALECONTROL_CLASS TEXT ("VerticalRescaleControl")
#define RCN_RESCALE (WM_USER + 1)

namespace WindowPP
{
	typedef enum WNDPos : int
	{
		WND_LEFT,
		WND_RIGHT,
		WND_TOP,
		WND_BOTTOM,
	};

	class HorizontalRescaleControl : public BaseWindow
	{
	public:
		HorizontalRescaleControl::HorizontalRescaleControl( HINSTANCE hInstance,HWND hwndParent,int x,int y,int width,int heigth,HMENU hMenu )
			:BaseWindow(0,WindowClass::GetClass(HORIZONTAL_RESCALECONTROL_CLASS)->lpszClassName,TEXT(""),WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,x,y,width,heigth,hwndParent,hMenu,hInstance)
		{

		}

		~HorizontalRescaleControl(){}


		static ATOM Init()
		{
			try
			{
				WindowClass::GetClass(HORIZONTAL_RESCALECONTROL_CLASS);
			}
			catch(WindowPPException)
			{
				WindowClass hrc;
				hrc->hCursor = LoadCursor(NULL,IDC_SIZENS);
				hrc->lpszClassName = HORIZONTAL_RESCALECONTROL_CLASS;

				return hrc.Register();	
			}
			return TRUE;	
		}


		void AttachWindow( BaseWindow* wnd)
		{
			if(wnd->GetY() < GetY())
				m_AttachedWindows.insert(std::make_pair(WND_TOP,wnd));
			else
				m_AttachedWindows.insert(std::make_pair(WND_BOTTOM,wnd));
		}

	private:
		virtual LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			RECT rcClient;
			RECT rcParent;
			POINT ptClientUL;
			POINT ptClientLR;
			POINT ptCursor;

			switch(uMsg)
			{
			case WM_LBUTTONDOWN:
				SetCapture(hWnd);
				break;
			case WM_MOUSEMOVE:
				if (wParam & MK_LBUTTON) 
				{ 
					GetCursorPos(&ptCursor);
					GetWindowRect(&rcClient);
					GetParent()->GetClientRect(&rcParent);
					ptClientUL.x = rcClient.left;
					ptClientUL.y = rcClient.top;
					ptClientLR.x = rcClient.right;
					ptClientLR.y = rcClient.bottom;

					ScreenToClient(*GetParent(),&ptClientUL);
					ScreenToClient(*GetParent(),&ptClientLR);
					ScreenToClient(*GetParent(),&ptCursor);

					int ny = ptCursor.y;
					if(ny < 0)
						ny = 0;
					else if(ny > rcParent.bottom - GetWidth())
						ny = rcParent.bottom - GetWidth();

					std::multimap<WNDPos,BaseWindow*>::iterator it;
					for(it = m_AttachedWindows.begin(); it != m_AttachedWindows.end();++it)
					{
						if(it->first == WND_TOP)
						{
							it->second->MoveWindow(it->second->GetX(),it->second->GetY(),it->second->GetWidth(),ny - it->second->GetY(),true);
						}
						else if(it->first == WND_BOTTOM)
						{
							it->second->MoveWindow(it->second->GetX(),ny + GetHeight(),it->second->GetWidth(),(it->second->GetY() + it->second->GetHeight()) - (ny + GetHeight()),true);
						}
					}
					NMHDR nmh;
					nmh.code = RCN_RESCALE;
					nmh.idFrom = GetDlgCtrlID(hWnd);
					nmh.hwndFrom = hWnd;
					GetParent()->SendMessage(WM_NOTIFY,(WPARAM)hWnd,(LPARAM)&nmh);
					MoveWindow(GetX(),ny,GetWidth(),GetHeight(),true);
				}
				break;
			case WM_LBUTTONUP:
				ReleaseCapture();
				break;
			default:
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			return 0;
		}
	private:
		std::multimap<WNDPos,BaseWindow*> m_AttachedWindows;
	};

	class VerticalRescaleControl : public BaseWindow
	{
	public:
		VerticalRescaleControl::VerticalRescaleControl( HINSTANCE hInstance,HWND hwndParent,int x,int y,int width,int heigth,HMENU hMenu )
			:BaseWindow(0,WindowClass::GetClass(VERTICAL_RESCALECONTROL_CLASS)->lpszClassName,TEXT(""),WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,x,y,width,heigth,hwndParent,hMenu,hInstance)
		{

		}

		~VerticalRescaleControl(){}


		static ATOM Init()
		{
			try
			{
				WindowClass::GetClass(VERTICAL_RESCALECONTROL_CLASS);
			}
			catch(WindowPPException)
			{
				WindowClass hrc;
				hrc->hCursor = LoadCursor(NULL,IDC_SIZEWE);
				hrc->lpszClassName = VERTICAL_RESCALECONTROL_CLASS;

				return hrc.Register();	
			}
			return TRUE;
		}


		void AttachWindow( BaseWindow* wnd )
		{
			if(wnd->GetX() < GetX())
				m_AttachedWindows.insert(std::make_pair(WND_LEFT,wnd));
			else
				m_AttachedWindows.insert(std::make_pair(WND_RIGHT,wnd));
		}

	private:
		virtual LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			RECT rcClient;
			RECT rcParent;
			POINT ptClientUL;
			POINT ptClientLR;
			POINT ptCursor;

			switch(uMsg)
			{
			case WM_LBUTTONDOWN:
				SetCapture(hWnd);
				break;
			case WM_MOUSEMOVE:
				if (wParam & MK_LBUTTON) 
				{ 
					GetCursorPos(&ptCursor);
					GetWindowRect(&rcClient);
					GetParent()->GetClientRect(&rcParent);
					ptClientUL.x = rcClient.left;
					ptClientUL.y = rcClient.top;
					ptClientLR.x = rcClient.right;
					ptClientLR.y = rcClient.bottom;

					ScreenToClient(*GetParent(),&ptClientUL);
					ScreenToClient(*GetParent(),&ptClientLR);
					ScreenToClient(*GetParent(),&ptCursor);

					int nx = ptCursor.x;
					if(nx < 0)
						nx = 0;
					else if(nx > rcParent.right - GetWidth())
						nx = rcParent.right - GetWidth();

					std::multimap<WNDPos,BaseWindow*>::iterator it;
					for(it = m_AttachedWindows.begin(); it != m_AttachedWindows.end();++it)
					{
						if(it->first == WND_LEFT)
						{
							it->second->MoveWindow(it->second->GetX(),it->second->GetY(),nx - it->second->GetX(),it->second->GetHeight(),true);
						}
						else if(it->first == WND_RIGHT)
						{
							it->second->MoveWindow(nx + GetWidth(),it->second->GetY(),(it->second->GetX() + it->second->GetWidth()) - (nx + GetWidth()),it->second->GetHeight(),true);
						}
					}
					NMHDR nmh;
					nmh.code = RCN_RESCALE;
					nmh.idFrom = GetDlgCtrlID(hWnd);
					nmh.hwndFrom = hWnd;
					GetParent()->SendMessage(WM_NOTIFY,(WPARAM)hWnd,(LPARAM)&nmh);
					MoveWindow(nx,GetY(),GetWidth(),GetHeight(),true);
				}
				break;
			case WM_LBUTTONUP:
				ReleaseCapture();
				break;
			default:
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			return 0;
		}
	private:
		std::multimap<WNDPos,BaseWindow*> m_AttachedWindows;
	};
}
#endif // RescaleControl_h__