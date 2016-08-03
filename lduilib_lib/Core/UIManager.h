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

typedef struct DUI_API tagTImageInfo
{
	LPBYTE pBits;
	LPBYTE pSrcBits;
	int nX;
	int nY;
	bool bAlpha;
	bool bUseHSL;
	string sResType;
	DWORD dwMask;
} TImageInfo;

typedef struct DUI_API tagTDrawInfo
{
	tagTDrawInfo();
	tagTDrawInfo(const char* lpsz);
	void Clear();
	string sDrawString;
	string sImageName;
	bool bLoaded;
	const TImageInfo* pImageInfo;
	RECT rcDestOffset;
	RECT rcBmpPart;
	RECT rcScale9;
	unsigned char uFade;
	bool bHole;
	bool bTiledX;
	bool bTiledY;
} TDrawInfo;

typedef struct DUI_API tagTResInfo
{
	DWORD m_dwDefaultDisabledColor;
	DWORD m_dwDefaultFontColor;
	DWORD m_dwDefaultLinkFontColor;
	DWORD m_dwDefaultLinkHoverFontColor;
	DWORD m_dwDefaultSelectedBkColor;
	//TFontInfo m_DefaultFontInfo;
	//CDuiStringPtrMap m_CustomFonts;
	//CDuiStringPtrMap m_ImageHash;
	map<string, void*> m_ImageHash;
	//CDuiStringPtrMap m_AttrHash;
	//CDuiStringPtrMap m_MultiLanguageHash;
} TResInfo;

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
	void Init(DuiHWND hWnd, const char* pstrName = NULL);

	const TImageInfo* GetImage(const char* bitmap);
	const TImageInfo* GetImageEx(const char* bitmap, const char* type = NULL, DWORD mask = 0, bool bUseHSL = false);
	const TImageInfo* AddImage(const char* bitmap, const char* type = NULL, DWORD mask = 0, bool bUseHSL = false, bool bShared = false);

	void UpdateRootControlSize(int w, int h);
	DuiHWND GetPaintWindow() const;
	void SetWindowAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetInitSize(int cx, int cy);

	bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);

	bool AttachDialog(CControlUI* pControl);
	bool AddNotifier(INotifyUI* pNotifier);

	bool MessageHandler(DuiULONG uMsg, DuiULONG wParam, DuiULONG lParam, LRESULT& lRes);

	void SetPaintWindowSize(unsigned long width, unsigned long height);
	void SendNotify(CControlUI* pControl, const char* pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false, bool bEnableRepeat = true);
	void SendNotify(TNotifyUI& Msg, bool bAsync = false, bool bEnableRepeat = true);

    static void init(int argc, char** argv);
    static void MessageLoop();
	static vector<DuiHWND> s_PaintWnds;

	static string GetResourcePath();
	static void SetResourcePath(const char* pszResPath);
	static string m_sResPath;
protected:
	set<INotifyUI*>		m_aNotifiers;
	TResInfo			m_ResInfo;
	TResInfo			m_SharedResInfo;
	bool				m_bForceUseSharedRes;
	SIZE				m_szInitWindowSize;

public:
	unsigned long m_WndPaintWidth;
	unsigned long m_WndPaintHeight;
	POINT m_ptLastMousePos;
	CControlUI* m_pRoot;
	DuiHWND		m_hWndPaint;
	string	m_sName;
	bool m_bFirstLayout;
};

}

#endif // UIMANAGER_H_INCLUDED
