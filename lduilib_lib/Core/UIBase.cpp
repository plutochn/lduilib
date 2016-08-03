#include "UIlib.h"

namespace DuiLib {

DUI_BASE_BEGIN_MESSAGE_MAP(CNotifyPump)
DUI_END_MESSAGE_MAP()

static const DUI_MSGMAP_ENTRY* DuiFindMessageEntry(const DUI_MSGMAP_ENTRY* lpEntry,TNotifyUI& msg )
{
	string sMsgType = msg.sType;
	string sCtrlName = msg.pSender->GetName();
	const DUI_MSGMAP_ENTRY* pMsgTypeEntry = NULL;
	while (lpEntry->nSig != DuiSig_end)
	{
		if(lpEntry->sMsgType==sMsgType)
		{
			if(!lpEntry->sCtrlName.empty())
			{
				if(lpEntry->sCtrlName==sCtrlName)
				{
					return lpEntry;
				}
			}
			else
			{
				pMsgTypeEntry = lpEntry;
			}
		}
		lpEntry++;
	}
	return pMsgTypeEntry;
}

bool CNotifyPump::AddVirtualWnd(string& strName,CNotifyPump* pObject)
{
	if( m_VirtualWndMap.find(strName) == m_VirtualWndMap.end() )
	{
		m_VirtualWndMap[strName] = (void*)pObject;
		return true;
	}
	return false;
}

bool CNotifyPump::RemoveVirtualWnd(string& strName)
{
	if( m_VirtualWndMap.find(strName) != m_VirtualWndMap.end() )
	{
		m_VirtualWndMap.erase(strName);
		return true;
	}
	return false;
}

bool CNotifyPump::LoopDispatch(TNotifyUI& msg)
{
	const DUI_MSGMAP_ENTRY* lpEntry = NULL;
	const DUI_MSGMAP* pMessageMap = NULL;

	for(pMessageMap = GetMessageMap(); pMessageMap!=NULL; pMessageMap = (*pMessageMap->pfnGetBaseMap)())
	{

		ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());

		if ((lpEntry = DuiFindMessageEntry(pMessageMap->lpEntries,msg)) != NULL)
		{
			goto LDispatch;
		}
	}
	return false;

LDispatch:
	union DuiMessageMapFunctions mmf;
	mmf.pfn = lpEntry->pfn;

	bool bRet = false;
	int nSig;
	nSig = lpEntry->nSig;
	switch (nSig)
	{
	default:
		ASSERT(false);
		break;
	case DuiSig_lwl:
		(this->*mmf.pfn_Notify_lwl)(msg.wParam,msg.lParam);
		bRet = true;
		break;
	case DuiSig_vn:
		(this->*mmf.pfn_Notify_vn)(msg);
		bRet = true;
		break;
	}
	return bRet;
}

void CNotifyPump::NotifyPump(TNotifyUI& msg)
{
	///遍历虚拟窗口
	if( !msg.sVirtualWnd.empty() )
    {
        map<string,void*>::iterator it = m_VirtualWndMap.begin();

        for(;it != m_VirtualWndMap.end(); it++)
        {
            if (it->first == msg.sVirtualWnd)
            {
                CNotifyPump* pObject = static_cast<CNotifyPump*>(it->second);
                if( pObject && pObject->LoopDispatch(msg) )
                            return;
            }
        }
	}

	///
	//遍历主窗口
	LoopDispatch( msg );
}

///

CWindowWnd::CWindowWnd() : m_hWnd(NULL)
{
	 
}

DuiHWND CWindowWnd::GetHWND() const
{
    return m_hWnd;
}

LRESULT CWindowWnd::HandleMessage(DuiULONG uMsg, DuiULONG wParam, DuiULONG lParam)
{
	return 0;
}

static void window_refresh_callback(GLFWwindow* window)
{
	CWindowWnd* pWnd = (CWindowWnd*)glfwGetWindowUserPointer(window);
	pWnd->HandleMessage(UI_WM_PAINT, 0, 0);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	CWindowWnd* pWnd = (CWindowWnd*)glfwGetWindowUserPointer(window);
	pWnd->HandleMessage(UI_WM_SIZE, width, height);
	glViewport(0, 0, width, height);
}

DuiHWND CWindowWnd::Create(DuiHWND hwndParent, const char* pstrName,int x, int y, int cx , int cy)
{
	m_hWnd = glfwCreateWindow(cx, cy, pstrName, NULL, NULL);
	glfwMakeContextCurrent(m_hWnd);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowUserPointer(m_hWnd, this);
	// bind callback for wnd.
	{
		glfwSetWindowRefreshCallback(m_hWnd, window_refresh_callback);
		glfwSetFramebufferSizeCallback(m_hWnd, framebuffer_size_callback);
	}
	HandleMessage(UI_WM_CREATE, cx, cy);

    return m_hWnd;
}

void CWindowWnd::OnFinalMessage()
{
}

void CWindowWnd::ShowWindow(bool bShow  )
{

}

}
