#ifndef __UILABEL_H__
#define __UILABEL_H__

namespace DuiLib
{
	class DUI_API CLabelUI : public CControlUI
	{
	public:
		CLabelUI();
		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor();

		void PaintText(DuiHDC hDC);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	protected:
		DWORD m_dwTextColor;
	};
}

#endif // __UILABEL_H__