#ifndef UIMANAGER_H_INCLUDED
#define UIMANAGER_H_INCLUDED
namespace DuiLib
{

// Structure for notifications from the system
// to the control implementation.
typedef struct DUI_API tagTEventUI
{
	int Type;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	char chKey;
	WORD wKeyState;
	WPARAM wParam;
	LPARAM lParam;
} TEventUI;

// Listener interface
class DUI_API INotifyUI
{
public:
	virtual void Notify(TNotifyUI& msg) = 0;
};

class  DUI_API CPaintManagerUI
{
public:
	CPaintManagerUI();
	void Init(int hWnd, const char* pstrName = NULL);

	bool AddNotifier(INotifyUI* pNotifier);

	bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

	void SetPaintWindowSize(unsigned long width, unsigned long height);
	void SendNotify(CControlUI* pControl, const char* pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false, bool bEnableRepeat = true);
	void SendNotify(TNotifyUI& Msg, bool bAsync = false, bool bEnableRepeat = true);

	 

    static void init(int argc, char** argv);

    static void MessageLoop();
protected:
	set<INotifyUI*> m_aNotifiers;

public:
	unsigned long m_WndPaintWidth;
	unsigned long m_WndPaintHeight;
	POINT m_ptLastMousePos;
	CControlUI* m_pRoot;
	int		m_hWndPaint;
	string	m_sName;
	bool m_bFirstLayout;
};

}

#endif // UIMANAGER_H_INCLUDED
