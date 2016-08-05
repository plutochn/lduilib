#ifndef __UICONTROL_H__
#define __UICONTROL_H__

class CPaintManagerUI;

namespace DuiLib {
class  DUI_API  CControlUI
{
public:
	CControlUI();

string  GetName() const;
virtual void* GetInterface(const char* pstrName);
virtual string GetClass();

bool DrawImage(DuiHDC hDC, TDrawInfo& drawInfo);

CControlUI* GetParent();
virtual void DoInit();
virtual void Init();
virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

virtual bool Paint(DuiHDC hDC, const DuiRECT& rcPaint, CControlUI* pStopControl = NULL); // 返回要不要继续绘制
virtual bool DoPaint(DuiHDC hDC, const DuiRECT& rcPaint, CControlUI* pStopControl);
virtual void PaintBkColor(DuiHDC hDC);
virtual void PaintBkImage(DuiHDC hDC);
virtual void PaintStatusImage(DuiHDC hDC);
virtual void PaintText(DuiHDC hDC);
virtual void PaintBorder(DuiHDC hDC);

void SetBkImage(const char* pStrImage);

void NeedUpdate();
bool IsVisible() const;
bool IsFloat() const;
void SetFloat(bool bFloat = false);

void SetInternVisible(bool InternVisible);

void SetPaintManager(CPaintManagerUI* pManager);

virtual void SetPos(DuiRECT& pos,bool bNeedInvalidate = false);
const DuiRECT& GetPos();

DWORD GetBkColor() const;
void SetBkColor(DWORD bkColor);

void SetFixedXY(DuiSIZE& fixedXY);
DuiSIZE GetFixedXY();

void  SetFixedWidth(int cx);
void  SetFixedHeight(int cy);

int  GetFixedHeight() const;
int  GetFixedWidth() const;

void SetText(const char* text);
const string& GetText() const;

protected:
	TDrawInfo m_diBk;

	bool m_bUpdateNeeded;
	CPaintManagerUI* m_pManager;
	CControlUI*		m_pParent;
	string m_sName;
	DuiRECT m_rcPaint;
	DuiRECT m_rcItem;

	bool m_bVisible;
	bool m_bFloat;
	bool m_bInternVisible;	
	DWORD m_dwBackColor;

	string m_sText;

	DuiSIZE m_xyFixed;
	DuiSIZE m_cxyFixed;
};

}

#endif
