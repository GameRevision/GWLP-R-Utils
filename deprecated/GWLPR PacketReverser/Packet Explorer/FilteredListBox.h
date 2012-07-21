#pragma once
#include "stdafx.h"
#include "WindowPP.h"

class FilteredListBox : public WindowPP::ListBox
{
	
public:
	FilteredListBox(DWORD dwStyle,int x,int y,int nWidth,int nHeigth,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance);
	~FilteredListBox(void);

	void SetFilter(std::tstring Filter);

	LRESULT OnMeasureItem(PMEASUREITEMSTRUCT pmis);
	LRESULT OnDrawItem(PDRAWITEMSTRUCT pdis);
private:
	TEXTMETRIC m_TextMetrics;
};

