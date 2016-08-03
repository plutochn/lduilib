#include "UIlib.h"

namespace DuiLib {

	CControlUI::CControlUI():
		m_dwBackColor(0xffffffff),
		m_bFloat(false),
		m_bVisible(true),
		m_pParent(NULL)
	{
	}

string CControlUI::GetName() const
{
    return m_sName;
}

void* CControlUI::GetInterface(const char* pstrName)
{
	if (strcmp(pstrName, DUI_CTR_CONTROL) == 0) return this;
	return NULL;
}

string CControlUI::GetClass()
{
	return DUI_CTR_CONTROL;
}

bool CControlUI::Paint(DuiHDC hDC, const DuiRECT& rcPaint, CControlUI* pStopControl)
{
	if (pStopControl == this) return false;
	if (!IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return true;
	
	if (!DoPaint(hDC, rcPaint, pStopControl)) return false;
	
	return true;
}

bool CControlUI::DoPaint(DuiHDC hDC, const DuiRECT& rcPaint, CControlUI* pStopControl)
{
	// »æÖÆÑ­Ðò£º±³¾°ÑÕÉ«->±³¾°Í¼->×´Ì¬Í¼->ÎÄ±¾->±ß¿ò
	PaintBkColor(hDC);
	PaintBkImage(hDC);
	PaintStatusImage(hDC);
	PaintText(hDC);
	PaintBorder(hDC);
	return true;
}

void CControlUI::PaintBkColor(DuiHDC hDC)
{
	CRenderEngine::DrawColor(hDC, m_rcItem,m_dwBackColor);
}

bool CControlUI::DrawImage(DuiHDC hDC, TDrawInfo& drawInfo)
{
	return CRenderEngine::DrawImage(hDC, m_pManager, m_rcItem, m_rcPaint, drawInfo);
}

void CControlUI::Init()
{
	DoInit();
}

void CControlUI::DoInit()
{
	m_rcItem.bottom = 0;
	m_rcItem.left = 0;
	m_rcItem.right = 20;
	m_rcItem.top = 20;
}

CControlUI* CControlUI::GetParent()
{
	return m_pParent;
}

void CControlUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
{
	m_pManager = pManager;
	m_pParent = pParent;
	if (bInit && m_pParent) Init();
}

void CControlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (strcmp(pstrName, _T("pos")) == 0)
	{
		RECT rcPos = { 0 };
		LPTSTR pstr = NULL;
		rcPos.left = strtoul(pstrValue, &pstr, 10);  ASSERT(pstr);
		rcPos.top = strtoul(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rcPos.right = strtoul(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rcPos.bottom = strtoul(pstr + 1, &pstr, 10); ASSERT(pstr);
		DuiSIZE szXY = { rcPos.left, rcPos.top };
		SetFixedXY(szXY);
		
		SetFixedWidth(rcPos.right - rcPos.left);
		SetFixedHeight(rcPos.bottom - rcPos.top);
	}
	 else if (strcmp(pstrName, _T("bkcolor")) == 0) 
	 {
		 while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) ++pstrValue;
		 if (*pstrValue == _T('#')) ++pstrValue;
		 LPTSTR pstr = NULL;
		 DWORD clrColor = strtoul(pstrValue, &pstr, 16);
		 SetBkColor(clrColor);
	 }
	 else if (strcmp(pstrName, _T("bkimage")) == 0) SetBkImage(pstrValue);
}

void CControlUI::PaintBkImage(DuiHDC hDC)
{
	DrawImage(hDC, m_diBk);
}

void CControlUI::PaintStatusImage(DuiHDC hDC)
{
	 
}

void CControlUI::PaintText(DuiHDC hDC)
{
	 
}

void CControlUI::PaintBorder(DuiHDC hDC)
{
	 
}

void CControlUI::SetPos(DuiRECT& pos)
{
	m_rcItem = pos;
}
const DuiRECT&  CControlUI::GetPos()
{
	return m_rcItem;
}

DWORD CControlUI::GetBkColor() const
{
	return m_dwBackColor;
}

void CControlUI::SetBkColor(DWORD bkColor)
{
	m_dwBackColor = bkColor;
}

void CControlUI::SetFixedXY(DuiSIZE& fixedXY)
{
	m_xyFixed = fixedXY;
}

void CControlUI::SetFixedWidth(int cx)
{
	if (cx < 0) return;
	m_cxyFixed.cx = cx;
}

void CControlUI::SetFixedHeight(int cy)
{
	if (cy < 0) return;
	m_cxyFixed.cy = cy;
}

int CControlUI::GetFixedHeight() const
{
	return m_cxyFixed.cy;
}

void CControlUI::SetPaintManager(CPaintManagerUI* pManager)
{
	m_pManager = pManager;
}
bool CControlUI::IsVisible() const
{
	return m_bVisible && m_bInternVisible;
}

void CControlUI::SetInternVisible(bool InternVisible)
{
	m_bInternVisible = InternVisible;
}

bool CControlUI::IsFloat() const
{
	return m_bFloat;
}

void CControlUI::SetBkImage(const char* pStrImage)
{
	if (m_diBk.sDrawString == pStrImage && m_diBk.pImageInfo != NULL) return;
	m_diBk.Clear();
	m_diBk.sDrawString = pStrImage;
}

void CControlUI::NeedUpdate()
{
	if (!IsVisible()) return;
	m_bUpdateNeeded = true;
	//Invalidate();

	//if (m_pManager != NULL) m_pManager->NeedUpdate();
}

}

