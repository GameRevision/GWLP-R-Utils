#ifndef WindowClass_h__
#define WindowClass_h__
/*
 © 2011 ACB
*/
#include "stdafx.h"
#include "WindowPPException.h"
#include "BaseWindow.h"

#define WINDOWPP_DEFAULT_WNDCLASS TEXT("WINDOWPP::CLASS%08i")

namespace WindowPP
{	
	class WindowClass
	{
	public:
		WindowClass(LPTSTR lpszClassName = GenerateDefaultName(),
			UINT style = CS_HREDRAW | CS_VREDRAW,
			int cbClsExtra = 0,
			int cbWndExtra = 0,
			HICON hIcon = LoadIcon( 0, IDI_APPLICATION ),
			HCURSOR hCursor = LoadCursor( 0, IDC_ARROW ),
			HBRUSH hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1),
			LPCTSTR lpszMenuName = 0,
			HINSTANCE hInstance = GetModuleHandle(0))
		{
			ZeroMemory(&m_Class,sizeof(WNDCLASSEX));
			m_Class.cbSize = sizeof(WNDCLASSEX);
			m_Class.style = style;
			m_Class.cbClsExtra = cbClsExtra;
			m_Class.cbWndExtra = cbWndExtra;
			m_Class.hIcon = hIcon;
			m_Class.hCursor = hCursor;
			m_Class.hbrBackground = hbrBackground;
			m_Class.lpszMenuName = lpszMenuName;
			m_Class.hInstance = hInstance;
			m_Class.lpfnWndProc = BaseWindow::WndProcDelegate;
			m_Class.lpszClassName = lpszClassName;
		}

		WindowClass(HICON hIcon,
			LPTSTR lpszClassName = GenerateDefaultName(),
			HBRUSH hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ),
			LPCTSTR lpszMenuName = 0,
			UINT style = CS_HREDRAW | CS_VREDRAW,
			HCURSOR hCursor = LoadCursor( 0, IDC_ARROW ),
			HINSTANCE hInstance = GetModuleHandle(0) )
		{
			ZeroMemory(&m_Class,sizeof(WNDCLASSEX));
			m_Class.cbSize = sizeof(WNDCLASSEX);
			m_Class.style = style;
			m_Class.hIcon = hIcon;
			m_Class.hCursor = hCursor;
			m_Class.hbrBackground = hbrBackground;
			m_Class.lpszMenuName = lpszMenuName;
			m_Class.hInstance = hInstance;
			m_Class.lpfnWndProc = BaseWindow::WndProcDelegate;
			m_Class.lpszClassName = GenerateDefaultName();
		}

		~WindowClass(){}

		static WindowClass GetClass( std::tstring ClassName )
		{
			WindowClass cls;
			if(GetClassInfoEx(GetModuleHandle(0),ClassName.c_str(),&cls.m_Class) == FALSE)
				throw WindowPPException(TEXT("Unable to get WindowClass %s"),ClassName.c_str());

			cls->lpszClassName = new TCHAR[ClassName.length()];//ClassName will be destroyed -.-
			_tcscpy((TCHAR*)cls->lpszClassName,ClassName.c_str());

			return cls;
		}

		ATOM Register()
		{
			return RegisterClassEx(&m_Class);
		}

		operator WNDCLASSEX () { return m_Class; }
		WNDCLASSEX *operator ->() { return &m_Class; }
	private:
		static TCHAR* GenerateDefaultName()
		{
			static int ClassIdx = 0;
			static TCHAR buff[200];
			_stprintf_s(buff,WINDOWPP_DEFAULT_WNDCLASS,ClassIdx);
			return buff;
		}
	private:
		WNDCLASSEX m_Class;
	};
}
#endif // WindowClass_h__
