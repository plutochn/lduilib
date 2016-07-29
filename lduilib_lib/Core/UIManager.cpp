#include "UIlib.h"

namespace DuiLib
{
CPaintManagerUI::CPaintManagerUI() :
	m_hWndPaint(NULL),
	m_pRoot(NULL),
	m_bFirstLayout(true)
{

}

void CPaintManagerUI::Init(int hWnd, const char* pstrName )
{
	m_sName.clear();
	if (pstrName != NULL) m_sName = pstrName;

	if (m_hWndPaint != hWnd) 
	{
		m_hWndPaint = hWnd;
	}
}

bool CPaintManagerUI::AddNotifier(INotifyUI* pNotifier)
{
	if (pNotifier == NULL) return false;

	m_aNotifiers.insert(pNotifier);

	return true;
}

void CPaintManagerUI::SetPaintWindowSize(unsigned long width, unsigned long height)
{
	m_WndPaintWidth = width;
	m_WndPaintHeight = height;
}

bool CPaintManagerUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
{
	if (m_hWndPaint == NULL) return false;

	switch (uMsg) 
	{
		case UI_WM_PAINT:
		{
			if (m_pRoot == NULL) {
				RECT r = { -1.0f,-1.0f,1.0f,1.0f };
				COLOR color = { 0.1f,0.1f,0,0 };
				CRenderEngine::DrawColor(0, r, color);
				return true;
			}
			if (m_bFirstLayout) {
				m_bFirstLayout = false;
				SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWINIT, 0, 0, false);
			}
			RECT rcPaint = { 0,0,m_WndPaintWidth,m_WndPaintHeight };
			HDC hdc = 0;
			m_pRoot->Paint(hdc, rcPaint, NULL);
		}
		return true;
	}
	return true;
}

void CPaintManagerUI::SendNotify(TNotifyUI& Msg, bool bAsync /*= false*/, bool bEnableRepeat /*= true*/)
{
	Msg.ptMouse = m_ptLastMousePos;
	if (!bAsync) {
		set<INotifyUI*>::iterator it = m_aNotifiers.begin();

		for (; it != m_aNotifiers.end(); it++) {
			static_cast<INotifyUI*>(*it)->Notify(Msg);
		}
	}
	else
	{

	}
}

void CPaintManagerUI::SendNotify(CControlUI* pControl, const char* pstrMessage, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/, bool bAsync /*= false*/, bool bEnableRepeat /*= true*/)
{
	TNotifyUI Msg;
	Msg.pSender = pControl;
	Msg.sType = pstrMessage;
	Msg.wParam = wParam;
	Msg.lParam = lParam;
	Msg.reserved = NULL;
	SendNotify(Msg, bAsync);
}

void CPaintManagerUI::init(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
}

void CPaintManagerUI::MessageLoop()
{
	glutMainLoop();
}

}
