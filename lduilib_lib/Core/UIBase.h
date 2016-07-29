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
	static CWindowWnd* sharedWindow();

    CWindowWnd();
    HWND GetHWND() const;
    HWND Create(HWND hwndParent, const char* pstrName,int x, int y, int cx , int cy);
    void ShowWindow(bool bShow = true);

    virtual void OnFinalMessage();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	static void OnGlutPaint();
	static void OnGlutWindowSize(int w, int h);

protected:
    int m_hWnd;
	static CWindowWnd* ms_UniqueWindow;
};

}
#endif // __UIBASE_H__
