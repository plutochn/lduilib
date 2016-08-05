#include "UIlib.h"

namespace DuiLib
{
	CLabelUI::CLabelUI():
		m_dwTextColor(0xff000000)
	{

	}
	void CLabelUI::SetTextColor(DWORD dwTextColor)
	{
		m_dwTextColor = dwTextColor;
	}
	DWORD CLabelUI::GetTextColor()
	{
		return m_dwTextColor;
	}
	void CLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (strcmp(pstrName, _T("textcolor")) == 0)
		{
			while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) ++pstrValue;
			if (*pstrValue == _T('#')) ++pstrValue;
			LPTSTR pstr = NULL;
			DWORD clrColor = strtoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CLabelUI::PaintText(DuiHDC hDC)
	{
		if (m_sText.empty()) return;

		CRenderEngine::DrawTextSt(m_sText.c_str(),m_rcItem,0,0,m_dwTextColor);
	}
}
