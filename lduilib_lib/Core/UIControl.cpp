#include "UIlib.h"

namespace DuiLib {
string CControlUI::GetName() const
{
    return m_sName;
}
void* CControlUI::GetInterface(const char* pstrName)
{
	if (strcmp(pstrName, DUI_CTR_CONTROL) == 0) return this;
	return NULL;
}
bool CControlUI::Paint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	if (pStopControl == this) return false;
	if (!IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return true;
	
	if (!DoPaint(hDC, rcPaint, pStopControl)) return false;
	
	return true;
}

bool CControlUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	return true;
}

void CControlUI::PaintBkColor(HDC hDC)
{
	 
}

void CControlUI::PaintBkImage(HDC hDC)
{
	 
}
void CControlUI::PaintStatusImage(HDC hDC)
{
	 
}

void CControlUI::PaintText(HDC hDC)
{
	 
}

void CControlUI::PaintBorder(HDC hDC)
{
	 
}
bool CControlUI::IsVisible() const
{
	return m_bVisible && m_bInternVisible;
}

void CControlUI::NeedUpdate()
{
	if (!IsVisible()) return;
	m_bUpdateNeeded = true;
	//Invalidate();

	//if (m_pManager != NULL) m_pManager->NeedUpdate();
}

}

