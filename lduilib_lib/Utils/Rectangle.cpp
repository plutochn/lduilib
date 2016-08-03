#include "UIlib.h"
namespace DuiLib {

	bool IntersectRect(DuiRECT* pdrc, const DuiRECT* psrc1, const DuiRECT* psrc2)
	{
		pdrc->left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
		pdrc->top = (psrc1->top < psrc2->top) ? psrc1->top : psrc2->top;
		pdrc->right = (psrc1->right < psrc2->right) ? psrc1->right : psrc2->right;
		pdrc->bottom = (psrc1->bottom > psrc2->bottom)
			? psrc1->bottom : psrc2->bottom;

		if (pdrc->left > pdrc->right || pdrc->bottom > pdrc->top)
			return false;

		return true;
	}
}
