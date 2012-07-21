#include "StdAfx.h"
#include "FilteredListBox.h"


FilteredListBox::FilteredListBox( DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance )
	:ListBox(dwStyle | LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS,WS_EX_CLIENTEDGE,x,y,nWidth,nHeigth,hWndParent,hMenu,hInstance)
{
	HDC dc = GetWindowDC();
	GetTextMetrics(dc,&m_TextMetrics);
	ReleaseDC(dc);
}

FilteredListBox::~FilteredListBox(void)
{
}

LRESULT FilteredListBox::OnMeasureItem( PMEASUREITEMSTRUCT pmis )
{
	pmis->itemHeight = m_TextMetrics.tmHeight;
	return TRUE;
}

LRESULT FilteredListBox::OnDrawItem( PDRAWITEMSTRUCT pdis )
{
	if(pdis->itemID == -1)
		return 0;

	if(GetItemHeight(pdis->itemID) == 1)
		return TRUE;

	switch(pdis->itemAction)
	{
	case ODA_SELECT:
	case ODA_DRAWENTIRE:
		{
			DWORD ColorRef = GetDCBrushColor(pdis->hDC);
			FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)ColorRef);
			std::tstring text;
			text.resize(GetTextLen(pdis->itemID) + 1);
			GetText(&text,pdis->itemID);
			TextOut(pdis->hDC,pdis->rcItem.left,pdis->rcItem.top,text.c_str(),text.length());
			if(pdis->itemState & ODS_SELECTED)
			{
				RECT focusRect;
				focusRect.left = pdis->rcItem.left;
				focusRect.top = pdis->rcItem.top;
				focusRect.bottom = pdis->rcItem.bottom;
				focusRect.right = pdis->rcItem.right;
				DrawFocusRect(pdis->hDC,&focusRect);
				InvertRect(pdis->hDC,&focusRect);
			}
		}
	case ODA_FOCUS:
		break;
	}
	return TRUE;
}

void FilteredListBox::SetFilter( std::tstring Filter )
{
	for(int i=0;i<GetCount();i++)
	{
		std::tstring text;
		text.resize(GetTextLen(i) + 1);
		GetText(&text,i);
		if(text == Filter)
			SetItemHeight(i,m_TextMetrics.tmHeight);
		else
			SetItemHeight(i,1);
			
	}
	RECT r;
	GetClientRect(&r);
	InvalidateRect(&r);
}