#ifndef __UICONTROL_H__
#define __UICONTROL_H__

class CPaintManagerUI;

namespace DuiLib {
class  DUI_API  CControlUI
{
public:
string  GetName() const;
void* GetInterface(const char* pstrName);

virtual bool Paint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl = NULL); // 返回要不要继续绘制
virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
virtual void PaintBkColor(HDC hDC);
virtual void PaintBkImage(HDC hDC);
virtual void PaintStatusImage(HDC hDC);
virtual void PaintText(HDC hDC);
virtual void PaintBorder(HDC hDC);

void NeedUpdate();
bool IsVisible() const;

protected:
	bool m_bUpdateNeeded;
	CPaintManagerUI* m_pManager;
	string m_sName;
	RECT m_rcPaint;
	RECT m_rcItem;

	bool m_bVisible;
	bool m_bInternVisible;	
};

}

#endif
