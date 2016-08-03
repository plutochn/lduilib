#ifndef UIRENDER_H_INCLUDED
#define UIRENDER_H_INCLUDED
namespace DuiLib
{
	class DUI_API CRenderEngine
	{
	public:
		static void BeginRender(DuiHWND WndPaint);
		static void EndRender(DuiHWND WndPaint);

		static unsigned long s_WndWidth;
		static unsigned long s_WndHeight;

		static void SetWndSize(unsigned long w, unsigned long h);
		static bool LoadFileContent(const char* file,LPBYTE& data, DWORD& len);

		static TImageInfo* LoadImage(const char* bitmap, const char* type = NULL, DWORD mask = 0);
		static void  FreeImage(TImageInfo* bitmap, bool bDelete = true);

		static void  DrawColor(DuiHDC hDC, const DuiRECT& rc, unsigned long   color);
		static void DrawColor(DuiHDC hDC, const DuiRECTf& rc, const DuiCOLOR& color);
		static bool DrawImage(DuiHDC hDC, CPaintManagerUI* pManager, const DuiRECT& rcItem, const DuiRECT& rcPaint,
			TDrawInfo& drawInfo);

		static bool DrawImage(DuiHDC hDC,const DuiRECT& rc, const TDrawInfo& drawInfo);
		static void DrawQuad(GLuint texture,const DuiRECT& rc);

		static GLuint CreateTexture(const TImageInfo& imageInfo);

		static map<string, GLuint> s_TextureCache;

	};
}

#endif
