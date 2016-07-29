#include <iostream>
#include "UIlib.h"

using namespace std;
using namespace DuiLib;

class CDemoWnd : public CWindowWnd , public INotifyUI
{
public:
	void Notify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
		{
			int debug = 0;
		}
	}

	void Init()
	{

	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		m_pm.Init(m_hWnd);
		m_pm.SetPaintWindowSize(wParam, lParam);

		/*CDialogBuilder builder;
		CDialogBuilderCallbackEx cb;
		CControlUI* pRoot = builder.Create(_T("UILogin.xml"), (UINT)0, &cb, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);*/
		m_pm.AddNotifier(this);
		Init();
		return 0;
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		bool bHandled = true;
		switch (uMsg) 
		{
		case UI_WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = false;
		}
		if (bHandled) return lRes;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}
public:
	DuiLib::CPaintManagerUI		m_pm;
};

int main(int argc, char** argv)
{
	DuiLib::CPaintManagerUI::init(argc,argv);

	CDemoWnd* pWindow = new CDemoWnd();
    pWindow->Create(NULL,"CallFromExe",10,10,400,400);

    pWindow->ShowWindow();

	DuiLib::CPaintManagerUI::MessageLoop();

    return 0;
}
