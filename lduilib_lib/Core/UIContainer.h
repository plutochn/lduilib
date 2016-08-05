#ifndef __UICONTAINER_H__
#define __UICONTAINER_H__

namespace DuiLib
{
	class IContainerUI
	{
	public:
		virtual bool Add(CControlUI* pControl) = 0;
	};

	class DUI_API CContainerUI : public CControlUI,public IContainerUI
	{
		virtual void SetPos(DuiRECT& pos,bool bNeedInvalidate = false);
		virtual void SetFloatPos(int iIndex);

		virtual bool Add(CControlUI* pControl);

		bool DoPaint(DuiHDC hDC, const DuiRECT& rcPaint, CControlUI* pStopControl);

		void* GetInterface(const char* pstrName);
		string GetClass();

	protected:
		vector<CControlUI*> m_items;
	};
}

#endif