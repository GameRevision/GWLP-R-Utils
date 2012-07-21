#include "StdAfx.h"
#include "GUI.h"
#include "WindowPP.h"
#include "Resource.h"

GUI::GUI( int x,int y,int nWidth,int nHeight,HMENU hMenu,HINSTANCE hInstance )
:WindowPP::BaseWindow(WindowPP::WindowClass::GetClass(PACKET_EXPLORER_CLASSNAME)->lpszClassName,PACKET_EXPLORER_TITLE,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,x,y,nWidth,nHeight,0,hMenu,hInstance),
m_PacketListBox(0),m_PacketDisplayBox(0),m_FilterEdit(0),m_Rescale(0),m_PacketStream(0)
{
	OnCreate();
}
	
GUI::~GUI(void)
{
}

ATOM GUI::Init()
{
	WindowPP::WindowClass wnd(PACKET_EXPLORER_CLASSNAME);
	wnd->lpszMenuName = MAKEINTRESOURCE(IDC_PACKETEXPLORER);
	wnd->hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PACKETEXPLORER));
	return wnd.Register();
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK GUI::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	PMEASUREITEMSTRUCT pmis;
	PDRAWITEMSTRUCT pdis;
	int wmId, wmEvent;

	switch(uMsg)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPEN:
			return OnOpenFile();
			break;
		case IDC_FILTEREDIT:
			if(wmEvent == EN_CHANGE)
			{
				std::tstring ntext;
				ntext.resize(m_FilterEdit->GetWindowTextLength() + 1);
				m_FilterEdit->GetWindowText(&ntext);
				m_PacketListBox->SetFilter(std::tstring(_T("Header: ")) + ntext);
			}
			break;
		case IDC_PACETLISTBOX:
			if(wmEvent == LBN_SELCHANGE)
			{
				m_PacketDisplayBox->DeleteAllItems();
				return OnDisplayPacket();
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		switch(wmEvent)
		
		break;
	case WM_MEASUREITEM:
		pmis = reinterpret_cast<PMEASUREITEMSTRUCT>(lParam);
		if(wParam == IDC_PACETLISTBOX && m_PacketListBox)
				return m_PacketListBox->OnMeasureItem(pmis);
	case WM_DRAWITEM:
		pdis = reinterpret_cast<PDRAWITEMSTRUCT>(lParam);
		if(wParam == IDC_PACETLISTBOX && m_PacketListBox)
			return m_PacketListBox->OnDrawItem(pdis);
		break;
	case WM_SIZE:
		return OnResize();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return TRUE;
}

LRESULT GUI::OnCreate()
{
	m_PacketDisplayBox = new WindowPP::ListView(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,WS_EX_CLIENTEDGE,0,0,800,GetHeight() - 10,*this,(HMENU)IDC_PACKETDISPLAYBOX,GetModuleHandle(0));
	m_FilterEdit = new WindowPP::EditControl(WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT ,WS_EX_CLIENTEDGE,810,0,180,20,*this,(HMENU)IDC_FILTEREDIT,GetModuleHandle(0));
	m_PacketListBox = new FilteredListBox(WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL  | LBS_DISABLENOSCROLL,810,21,180,GetHeight() - 30,*this,(HMENU)IDC_PACETLISTBOX,GetModuleHandle(0));
	m_Rescale = new WindowPP::VerticalRescaleControl(GetModuleHandle(0),*this,800,0,10,500,(HMENU)IDC_RESCALE);

 	m_Rescale->AttachWindow(m_PacketListBox);
 	m_Rescale->AttachWindow(m_FilterEdit);
 	m_Rescale->AttachWindow(m_PacketDisplayBox);

	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.iSubItem = 0;
	col.cx = 60;
	col.pszText = _T("Type");
	m_PacketDisplayBox->InsertColumn(0,&col);
	col.cx = 100;
	col.pszText = _T("Name");
	m_PacketDisplayBox->InsertColumn(1,&col);
	col.pszText = _T("Data");
	col.cx = 640;
	m_PacketDisplayBox->InsertColumn(2,&col);

	SetWindowText(PACKET_EXPLORER_TITLE);//Somehow doesn't get applied -.-
	return 0;
}

LRESULT GUI::OnOpenFile()
{
	OPENFILENAME ofn;

	TCHAR File[MAX_PATH] = {'\0'};

	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = File;
	ofn.nMaxFile = sizeof(File);
	ofn.lpstrFilter = _T("Stream dumps\0*.dump");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn) == TRUE)
	{
		if(m_PacketStream)
			m_PacketStream->~PacketStreamParser();
		if(m_PacketListBox->GetCount() > 0)
			m_PacketListBox->ResetContent();
		m_PacketStream = new PacketStreamParser(File);
		m_PacketStream->Parse();
		for(int i=0;i<m_PacketStream->GetPacketCount();i++)
		{
			Packet* p = m_PacketStream->GetPacket(i);
			std::tstringstream name;
			name << _T("Header: ") << std::dec << p->Header;
			int pos = m_PacketListBox->AddString(name.str());
			m_PacketListBox->SetItemData(p,pos);
		}
	}
	return 0;
}

LRESULT GUI::OnResize()
{
	m_PacketDisplayBox->SetHeight(GetClientRectHeight());
	m_PacketListBox->SetHeight(GetClientRectHeight() - 20);
	m_Rescale->SetHeight(GetClientRectHeight());

	m_PacketListBox->SetX(GetWidth() - m_PacketListBox->GetWidth() - 10);
	m_FilterEdit->SetX(m_PacketListBox->GetX());
	m_Rescale->SetX(m_PacketListBox->GetX() - 10);
	m_PacketDisplayBox->SetWidth(m_Rescale->GetX());
	return 0;
}

LRESULT GUI::OnDisplayPacket()
{
	int sel = m_PacketListBox->GetCurSel();
	Packet* pack = m_PacketListBox->GetItemData<Packet*>(sel);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE | LVIF_DI_SETITEM;;
	lvi.state = 0;
	lvi.stateMask = 0;

	for(int i=0;i<pack->PacketData.size();i++)
	{
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = const_cast<TCHAR*>(std::tr1::get<0>(pack->PacketData[i]).c_str());
		lvi.lParam = 0;
		m_PacketDisplayBox->InsertItem(&lvi);

		LVITEM slvi;
		slvi.iItem = i;
		slvi.iSubItem = 1;
		slvi.mask = LVIF_TEXT;
		slvi.pszText = const_cast<TCHAR*>(std::tr1::get<1>(pack->PacketData[i]).c_str());
		m_PacketDisplayBox->SetItem(&slvi);

		slvi.iItem = i;
		slvi.iSubItem = 2;
		slvi.mask = LVIF_TEXT;
		slvi.pszText = const_cast<TCHAR*>(std::tr1::get<2>(pack->PacketData[i]).c_str());
		m_PacketDisplayBox->SetItem(&slvi);
	}

	return 0;
}