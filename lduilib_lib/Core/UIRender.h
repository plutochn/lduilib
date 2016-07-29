#ifndef UIRENDER_H_INCLUDED
#define UIRENDER_H_INCLUDED
namespace DuiLib
{
	class DUI_API CRenderEngine
	{
	public:
		static void DrawColor(HDC hDC, const RECT& rc, const COLOR& color);
	};
}

#endif
