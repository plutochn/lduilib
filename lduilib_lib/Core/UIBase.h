#ifndef __UIBASE_H__
#define __UIBASE_H__

namespace DuiLib {

#define ASSERT(x) assert(x)

class DUI_API CNotifyPump
{
public:
    bool AddVirtualWnd(string& strName,CNotifyPump* pObject);
    bool RemoveVirtualWnd(string& strName);
	void NotifyPump(TNotifyUI& msg);
	bool LoopDispatch(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP()

private:
    map<string,void*> m_VirtualWndMap;
};

class DUI_API CWindowWnd
{
public:

    CWindowWnd();
    DuiHWND GetHWND() const;
	DuiHWND Create(DuiHWND hwndParent, const char* pstrName,int x=0, int y=0, int cx=100 , int cy=200);
    void ShowWindow(bool bShow = true);

    virtual void OnFinalMessage();
	virtual LRESULT HandleMessage(DuiULONG uMsg, DuiULONG wParam, DuiULONG lParam);

public:

protected:
	DuiHWND m_hWnd;
};

}
#endif // __UIBASE_H__
