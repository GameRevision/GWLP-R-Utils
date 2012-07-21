#pragma once
#include "WindowPP.h"
#include "FilteredListBox.h"
#include "PacketStreamParser.h"

#define PACKET_EXPLORER_CLASSNAME TEXT("PacketExplorerWndClass")
#define PACKET_EXPLORER_TITLE TEXT("Guild Wars Packet Explorer")

class GUI : public WindowPP::BaseWindow
{
	enum ControlIds
	{
		IDC_RESCALE = 0x1000,
		IDC_FILTEREDIT,
		IDC_PACKETDISPLAYBOX,
		IDC_PACETLISTBOX,
	};
public:
	GUI(int x,int y,int nWidth,int nHeight,HMENU hMenu,HINSTANCE hInstance);
	~GUI(void);

	static ATOM Init();
private:
	LRESULT OnOpenFile();
	LRESULT OnCreate();
	LRESULT OnResize();
	LRESULT OnDisplayPacket();
	virtual LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
private:
	WindowPP::VerticalRescaleControl* m_Rescale;
	WindowPP::EditControl* m_FilterEdit;
	WindowPP::ListView* m_PacketDisplayBox;
	FilteredListBox* m_PacketListBox;
	PacketStreamParser* m_PacketStream;
};
