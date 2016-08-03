#ifndef __UIDLGBUILDER_H__
#define __UIDLGBUILDER_H__

namespace  DuiLib {

class DUI_API CDialogBuilder
{
public:
	CControlUI* Create(const char* xml,CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL);
protected:
	CMarkup m_xml;
private:
	CControlUI* _Parse(CMarkupNode* parent, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);
};

}

#endif
