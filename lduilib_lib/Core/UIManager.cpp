#include "UIlib.h"

namespace DuiLib
{

tagTDrawInfo::tagTDrawInfo()
{
	Clear();
}

tagTDrawInfo::tagTDrawInfo(const char* lpsz)
{
	Clear();
	sDrawString = lpsz;
}

void tagTDrawInfo::Clear()
{
	sDrawString.clear();
	sImageName.clear();
	memset(&bLoaded, 0,sizeof(tagTDrawInfo) - offsetof(tagTDrawInfo, bLoaded));
	uFade = 255;
}

string CPaintManagerUI::GetResourcePath()
{
	return m_sResPath;
}

void CPaintManagerUI::SetResourcePath(const char* pszResPath)
{
	m_sResPath = pszResPath;
}

string CPaintManagerUI::m_sResPath;

CPaintManagerUI::CPaintManagerUI() :
	m_hWndPaint(NULL),
	m_pRoot(NULL),
	m_bFirstLayout(true),
	m_bForceUseSharedRes(false)
{
	m_ResInfo.m_ImageHash.clear();
}

DuiHWND CPaintManagerUI::GetPaintWindow() const
{
	return m_hWndPaint;
}

void CPaintManagerUI::SetInitSize(int cx, int cy)
{
	m_szInitWindowSize.cx = cx;
	m_szInitWindowSize.cy = cy;
	if (m_pRoot == NULL && m_hWndPaint != NULL)
	{
		glfwSetWindowSize(m_hWndPaint,cx,cy);
	}
}

bool CPaintManagerUI::InitControls(CControlUI* pControl, CControlUI* pParent /*= NULL*/)
{
	ASSERT(pControl);
	if (pControl == NULL) return false;
	pControl->SetManager(this, pParent != NULL ? pParent : pControl->GetParent(), true);

	return true;
}

void CPaintManagerUI::SetWindowAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (strcmp(pstrName, _T("size")) == 0) 
	{
		LPTSTR pstr = NULL;
		int cx = strtoul(pstrValue, &pstr, 10);  ASSERT(pstr);
		int cy = strtoul(pstr + 1, &pstr, 10);    ASSERT(pstr);
		SetInitSize(cx, cy);
	}
}

void CPaintManagerUI::Init(DuiHWND hWnd, const char* pstrName )
{
	m_sName.clear();
	if (pstrName != NULL) m_sName = pstrName;

	if (m_hWndPaint != hWnd) 
	{
		m_hWndPaint = hWnd;
	}
	CPaintManagerUI::s_PaintWnds.push_back(hWnd);
}

const TImageInfo* CPaintManagerUI::GetImage(const char* bitmap)
{
	TImageInfo* data = nullptr;

	map<string, void*>::iterator it = m_ResInfo.m_ImageHash.find(bitmap);
	if (it != m_ResInfo.m_ImageHash.end())
	{
		data = static_cast<TImageInfo*>(it->second);
	}
	if (!data)
	{
		it = m_SharedResInfo.m_ImageHash.find(bitmap);
		if (it != m_SharedResInfo.m_ImageHash.end())
		{
			data = static_cast<TImageInfo*>(it->second);
		}
	}
	
	return data;
}

const TImageInfo* CPaintManagerUI::GetImageEx(const char* bitmap, const char* type, DWORD mask, bool bUseHSL)
{
	const TImageInfo* data = GetImage(bitmap);
	if (!data) {
		if (AddImage(bitmap, type, mask, bUseHSL, false)) {
			if (m_bForceUseSharedRes) data = static_cast<TImageInfo*>(m_SharedResInfo.m_ImageHash[bitmap]);
			else data = static_cast<TImageInfo*>(m_ResInfo.m_ImageHash[bitmap]);
		}
	}

	return data;
}

const TImageInfo* CPaintManagerUI::AddImage(const char* bitmap, const char* type, DWORD mask, bool bUseHSL, bool bShared)
{
	if (bitmap == NULL || bitmap[0] == _T('\0')) return NULL;

	TImageInfo* data = NULL;
	
	data = CRenderEngine::LoadImage(bitmap, NULL, mask);

	data->bUseHSL = bUseHSL;
	if (type != NULL) data->sResType = type;
	data->dwMask = mask;

	if (data == NULL) return NULL;
	if (data->bUseHSL) {
	}
	else data->pSrcBits = NULL;

	if (data)
	{
		if (m_ResInfo.m_ImageHash.find(bitmap) != m_ResInfo.m_ImageHash.end())
		{
			TImageInfo* pOldImageInfo = static_cast<TImageInfo*>(m_ResInfo.m_ImageHash[bitmap]);
			if (pOldImageInfo)
			{
				CRenderEngine::FreeImage(pOldImageInfo);
				m_ResInfo.m_ImageHash.erase(bitmap);
			}
		}
		m_ResInfo.m_ImageHash[bitmap] = data;
	}
	return data;
}

void CPaintManagerUI::UpdateRootControlSize(int w, int h)
{
	DuiRECT r = { 0 };
	r.right = w;
	r.bottom = h;
	if (m_pRoot)
		m_pRoot->SetPos(r);
}

bool CPaintManagerUI::AttachDialog(CControlUI* pControl)
{
	if (!pControl)
		return false;

	m_pRoot = pControl;
	m_pRoot->SetPaintManager(this);

	UpdateRootControlSize(m_WndPaintWidth, m_WndPaintHeight);

	return true;
}

bool CPaintManagerUI::AddNotifier(INotifyUI* pNotifier)
{
	if (pNotifier == NULL) return false;

	m_aNotifiers.insert(pNotifier);
	
	return true;
}

void CPaintManagerUI::SetPaintWindowSize(unsigned long width, unsigned long height)
{
	UpdateRootControlSize(width, height);

	m_WndPaintWidth = width;
	m_WndPaintHeight = height;
	CRenderEngine::SetWndSize(m_WndPaintWidth, m_WndPaintHeight);
}

bool CPaintManagerUI::MessageHandler(DuiULONG uMsg, DuiULONG wParam, DuiULONG lParam, LRESULT& lRes)
{
	if (m_hWndPaint == NULL) return false;

	switch (uMsg) 
	{
	case UI_WM_SIZE:
	{
		SetPaintWindowSize(wParam, lParam);
	}
		case UI_WM_PAINT:
		{
			CRenderEngine::BeginRender(m_hWndPaint);
			do
			{
				if (m_pRoot == NULL) {
					DuiRECTf r = { -1.0f,-1.0f,1.0f,1.0f };
					DuiCOLOR color = { 0.1f,0.1f,0,0 };
					CRenderEngine::DrawColor(0, r, color);
					break;
				}
				if (m_bFirstLayout) {
					m_bFirstLayout = false;
					SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWINIT, 0, 0, false);
				}
				DuiRECT rcPaint = {0,m_WndPaintHeight,m_WndPaintWidth,0};
				DuiHDC hdc = 0;
				m_pRoot->Paint(hdc, rcPaint, NULL);
			} while (0);
			CRenderEngine::EndRender(m_hWndPaint);
			
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
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_DECORATED, 1);
}

vector<DuiHWND> CPaintManagerUI::s_PaintWnds;

void CPaintManagerUI::MessageLoop()
{
	while (1)
	{
		size_t i = 0;
		for (i = 0; i < s_PaintWnds.size(); i++)
		{
			if (!glfwWindowShouldClose(s_PaintWnds[i]))
				break;
		}

		if (i == s_PaintWnds.size())
			break;
		glfwPollEvents();
	}
	glfwTerminate();
}

}
