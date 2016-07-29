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
CWindowWnd* CWindowWnd::ms_UniqueWindow = NULL;

CWindowWnd* CWindowWnd::sharedWindow()
{
	return ms_UniqueWindow;
}

CWindowWnd::CWindowWnd() : m_hWnd(NULL)
{
	ms_UniqueWindow = this;
}

HWND CWindowWnd::GetHWND() const
{
    return m_hWnd;
}

void CWindowWnd::OnGlutWindowSize(int w, int h)
{
	CWindowWnd* pWnd = CWindowWnd::sharedWindow();
	pWnd->HandleMessage(UI_WM_SIZE, w, h);
}

void CWindowWnd::OnGlutPaint()
{
	CWindowWnd* pWnd = CWindowWnd::sharedWindow();
	pWnd->HandleMessage(UI_WM_PAINT,0,0);
}

LRESULT CWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

HWND CWindowWnd::Create(HWND hwndParent, const char* pstrName,int x, int y, int cx , int cy)
{
	glutInitWindowSize(cx, cy);
	m_hWnd = glutCreateWindow(pstrName);

	glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE);

	HandleMessage(UI_WM_CREATE, cx, cy);

	glutDisplayFunc(&CWindowWnd::OnGlutPaint);
	glutReshapeFunc(&CWindowWnd::OnGlutWindowSize);

    return m_hWnd;
}

void CWindowWnd::OnFinalMessage()
{
}

void CWindowWnd::ShowWindow(bool bShow  )
{

}

}
