#include "UIlib.h"

namespace  DuiLib {

CControlUI* CDialogBuilder::Create(const char* xml, CPaintManagerUI* pManager, CControlUI* pParent)
{
	if (!m_xml.LoadFromFile(xml)) return NULL;
	
	CMarkupNode root = m_xml.GetRoot();
	if (!root.IsValid()) return NULL;
	if (pManager)
	{
		LPCTSTR pstrClass = NULL;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		pstrClass = root.GetName();
		if (strcmp(pstrClass, _T("Window")) == 0) {
			if (pManager->GetPaintWindow()) {
				int nAttributes = root.GetAttributeCount();
				for (int i = 0; i < nAttributes; i++) {
					pstrName = root.GetAttributeName(i);
					pstrValue = root.GetAttributeValue(i);
					pManager->SetWindowAttribute(pstrName, pstrValue);
				}
			}
		}
	}
	return _Parse(&root, pParent, pManager);
}

CControlUI* CDialogBuilder::_Parse(CMarkupNode* pRoot, CControlUI* pParent, CPaintManagerUI* pManager)
{
	CControlUI* pReturn = NULL;
	for (CMarkupNode node = pRoot->GetChild(); node.IsValid(); node = node.GetSibling())
	{
		LPCTSTR pstrClass = node.GetName();
		CControlUI* pControl = NULL;
		if (strcmp(pstrClass, _T("Include")) == 0) 
		{
			if (!node.HasAttributes()) continue;
		}
		else if (strcmp(pstrClass, _T("TreeNode")) == 0)
		{

		}
		else
		{
			SIZE_T cchLen = strlen(pstrClass);
			switch (cchLen)
			{
			case 9:
				if (strcmp(pstrClass, DUI_CTR_CONTAINER) == 0)             pControl = new CContainerUI;
				break;
			case 7:
				if (strcmp(pstrClass, DUI_CTR_CONTROL) == 0)               pControl = new CControlUI;
			}
		}
		if (pControl == NULL)
			continue;

		if (node.HasChildren()) 
		{
			_Parse(&node, pControl, pManager);
		}
		if (pParent != NULL)
		{
			if (pParent->GetClass() == DUI_CTR_CONTAINER)
			{
				IContainerUI* pContainer = static_cast<IContainerUI*>(pParent->GetInterface(DUI_CTR_ICONTAINER));
				pContainer->Add(pControl);
			}
		}
		if (node.HasAttributes()) {
			// Set ordinary attributes
			int nAttributes = node.GetAttributeCount();
			for (int i = 0; i < nAttributes; i++) {
				pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
			}
		}

		if (pManager) 
		{
			pControl->SetManager(pManager, NULL, false);
		}

		if (pReturn == NULL) pReturn = pControl;
	}
	return pReturn;
}

}