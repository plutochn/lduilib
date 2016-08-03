#include "UIlib.h"

namespace DuiLib
{
	void* CContainerUI::GetInterface(const char* pstrName)
	{
		if (strcmp(pstrName, DUI_CTR_CONTAINER) == 0) 
			return static_cast<CContainerUI*>(this);
		else if (strcmp(pstrName, DUI_CTR_ICONTAINER) == 0)
			return static_cast<IContainerUI*>(this);

		return CControlUI::GetInterface(pstrName);
	}

	string CContainerUI::GetClass()
	{
		return DUI_CTR_CONTAINER;
	}

	bool CContainerUI::Add(CControlUI* pControl)
	{
		if (pControl == NULL) return false;

		if (m_pManager != NULL) m_pManager->InitControls(pControl, this);
		if (IsVisible()) NeedUpdate();
		else pControl->SetInternVisible(false);
		
		m_items.push_back(pControl);

		return true;
	}

	bool CContainerUI::DoPaint(DuiHDC hDC, const DuiRECT& rcPaint, CControlUI* pStopControl)
	{
		DuiRECT rcTemp = { 0 };
		if (!IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) return true;

		CControlUI::DoPaint(hDC, rcPaint, pStopControl);

		for (int it = 0; it < m_items.size(); it++) {

			DuiRECT r = m_rcItem;
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
			if (pControl == pStopControl) return false;
			if (!pControl->IsVisible()) continue;
			
			if (!IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos())) continue;

			if (pControl->IsFloat()) {
				if (!IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos())) continue;
			 
				if (!pControl->Paint(hDC, rcPaint, pStopControl)) return false;
			 
			}
			else {
				//if (!IntersectRect(&rcTemp, &rc, &pControl->GetPos())) continue;
				if (!pControl->Paint(hDC, rcPaint, pStopControl)) return false;
			}
		}

		return true;
	}
}
