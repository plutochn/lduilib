#include "UIlib.h"

namespace DuiLib
{
	void CRenderEngine::DrawColor(HDC hDC, const RECT& rc, const COLOR&   color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(color.r, color.g,color.b,color.a);
		glRectf(rc.left,rc.top,rc.bottom,rc.right);

		glFlush();
	}
}