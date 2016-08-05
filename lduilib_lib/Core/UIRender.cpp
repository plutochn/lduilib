#include "UIlib.h"

#define FONTSTASH_IMPLEMENTATION
#include "Utils/fontstash.h"

#define GLFONTSTASH_IMPLEMENTATION
#include "Utils/glfontstash.h"

namespace DuiLib
{
	unsigned long CRenderEngine::s_WndWidth = 0;
	unsigned long CRenderEngine::s_WndHeight = 0;

	FONScontext* CRenderEngine::s_fs = NULL;
	int CRenderEngine::s_fontNormal = 0;

	map<string, GLuint> CRenderEngine::s_TextureCache;
	
	void CRenderEngine::BeginRender(DuiHWND WndPaint)
	{
		glfwMakeContextCurrent(WndPaint);

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, s_WndWidth, s_WndHeight,0, 0.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		
	}
	void CRenderEngine::EndRender(DuiHWND WndPaint)
	{
		glfwSwapBuffers(WndPaint);
	}
	void CRenderEngine::SetWndSize(unsigned long w, unsigned long h)
	{
		s_WndWidth = w;
		s_WndHeight = h;
	}

#define WIDTH_TRANSPORT(x) ( (float)(x) )
#define HEIGHT_TRANSPORT(y) ( (float)(y) )
#define RECT_TRANSPORT(r,rf) {\
	rf.left = WIDTH_TRANSPORT(r.left); \
	rf.top = HEIGHT_TRANSPORT(r.top); \
	rf.right = WIDTH_TRANSPORT(r.right); \
	rf.bottom = HEIGHT_TRANSPORT(r.bottom); \
}

void CRenderEngine::DrawColor(DuiHDC hDC, const DuiRECT& rc, unsigned long   color)
{
	DuiRECTf r;
	RECT_TRANSPORT(rc, r)
	DuiCOLOR c;
	unsigned char one = color >> 24;
	c.a =  one / 255.f;

	one = (color & 0xff0000) >> 16;
	c.r =  one / 255.f;
 
	one = (color & 0xff00) >> 8;
	c.g =  one / 255.f;

	one = color & 0xff;
	c.b = one/ 255.f;

	CRenderEngine::DrawColor(hDC, r, c);
}

void CRenderEngine::DrawLine(float sx, float sy, float ex, float ey)
{
	glBegin(GL_LINES);
	glVertex2f(WIDTH_TRANSPORT(sx), HEIGHT_TRANSPORT(sy));
	glVertex2f(WIDTH_TRANSPORT(ex), HEIGHT_TRANSPORT(ey));
	glEnd();
}

void CRenderEngine::DrawColor(DuiHDC hDC, const DuiRECTf& rc, const DuiCOLOR&   color)
{
	
	glColor4f(color.r, color.g,color.b,color.a);

	glRectf(rc.left,rc.bottom,rc.right,rc.top);
}

bool CRenderEngine::DrawImage(DuiHDC hDC, CPaintManagerUI* pManager, const DuiRECT& rcItem, const DuiRECT& rcPaint,
	TDrawInfo& drawInfo)
{
	if (pManager == NULL) return true;
	if (drawInfo.pImageInfo == NULL) 
	{
		if (drawInfo.bLoaded) return false;
		drawInfo.bLoaded = true;
		if (drawInfo.sDrawString.empty()) return false;
			
		DWORD dwMask = 0;
		bool bUseHSL = false;

		string sImageName = drawInfo.sDrawString;
		string sItem;
		string sValue;
		char* pstr = NULL;
		const char* pstrImage = drawInfo.sDrawString.c_str();
		while (*pstrImage != _T('\0')) {
			sItem.clear();
			sValue.clear();
			while (*pstrImage > _T('\0') && *pstrImage <= _T(' ')) pstrImage++;
			while (*pstrImage != _T('\0') && *pstrImage != _T('=') && *pstrImage > _T(' ')) {
				const char* pstrTemp = ++pstrImage;
				while (pstrImage < pstrTemp) {
					sItem += *pstrImage++;
				}
			}
			while (*pstrImage > _T('\0') && *pstrImage <= _T(' ')) pstrImage++;
			if (*pstrImage++ != _T('=')) break;
			while (*pstrImage > _T('\0') && *pstrImage <= _T(' ')) pstrImage++;
			if (*pstrImage++ != _T('\'')) break;
			while (*pstrImage != _T('\0') && *pstrImage != _T('\'')) {
				const char* pstrTemp = ++pstrImage;
				while (pstrImage < pstrTemp) {
					sValue += *pstrImage++;
				}
			}
			if (*pstrImage++ != _T('\'')) break;
			if (!sValue.empty()) {
				if (sItem == _T("file")) {
					sImageName = sValue;
				}
				else if (sItem == _T("dest")) {
					drawInfo.rcDestOffset.left = strtol(sValue.c_str(), &pstr, 10);  ASSERT(pstr);
					drawInfo.rcDestOffset.top = strtol(pstr + 1, &pstr, 10);    ASSERT(pstr);
					drawInfo.rcDestOffset.right = strtol(pstr + 1, &pstr, 10);  ASSERT(pstr);
					drawInfo.rcDestOffset.bottom = strtol(pstr + 1, &pstr, 10); ASSERT(pstr);
				}
				else if (sItem == _T("source")) {
					drawInfo.rcBmpPart.left = strtol(sValue.c_str(), &pstr, 10);  ASSERT(pstr);
					drawInfo.rcBmpPart.top = strtol(pstr + 1, &pstr, 10);    ASSERT(pstr);
					drawInfo.rcBmpPart.right = strtol(pstr + 1, &pstr, 10);  ASSERT(pstr);
					drawInfo.rcBmpPart.bottom = strtol(pstr + 1, &pstr, 10); ASSERT(pstr);
				}
			}
			if (*pstrImage++ != _T(' ')) break;
		}
		drawInfo.sImageName = sImageName;

		const TImageInfo* data = NULL;
		data = pManager->GetImageEx(sImageName.c_str(), NULL, dwMask, bUseHSL);
		if (!data) return false;
		drawInfo.pImageInfo = data;
		if (drawInfo.rcBmpPart.left == 0 && drawInfo.rcBmpPart.right == 0 &&
			drawInfo.rcBmpPart.top == 0 && drawInfo.rcBmpPart.bottom == 0) {
			drawInfo.rcBmpPart.right = data->nX;
			drawInfo.rcBmpPart.bottom = data->nY;
		}
	}
	if (drawInfo.rcBmpPart.right > drawInfo.pImageInfo->nX) drawInfo.rcBmpPart.right = drawInfo.pImageInfo->nX;
	if (drawInfo.rcBmpPart.bottom > drawInfo.pImageInfo->nY) drawInfo.rcBmpPart.bottom = drawInfo.pImageInfo->nY;
		
	DuiRECT rcDest = rcItem;
	if (drawInfo.rcDestOffset.left != 0 || drawInfo.rcDestOffset.top != 0 ||
		drawInfo.rcDestOffset.right != 0 || drawInfo.rcDestOffset.bottom != 0) {
		rcDest.left = rcItem.left + drawInfo.rcDestOffset.left;
		rcDest.top = rcItem.top + drawInfo.rcDestOffset.top;
		rcDest.right = rcItem.left + drawInfo.rcDestOffset.right;
		if (rcDest.right > rcItem.right) rcDest.right = rcItem.right;
		rcDest.bottom = rcItem.top + drawInfo.rcDestOffset.bottom;
		if (rcDest.bottom > rcItem.bottom) rcDest.bottom = rcItem.bottom;
	}

	DuiRECT rcTemp;
	if (!IntersectRect(&rcTemp, &rcDest, &rcItem)) return true;
	if (!IntersectRect(&rcTemp, &rcDest, &rcPaint)) return true;

	return CRenderEngine::DrawImage(hDC, rcTemp, drawInfo);
}

GLuint CRenderEngine::CreateTexture(const TImageInfo& imageInfo)
{
	int x, y;
	x = imageInfo.nX;
	y = imageInfo.nY;

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageInfo.pBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

bool CRenderEngine::DrawTextSt(const char* text, DuiRECT& pos,int style,int iFont,DWORD dwTextColor)
{
	if (s_fs == NULL)
	{
		s_fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
		if (s_fs == NULL)
		{
			return false;
		}
		s_fontNormal = fonsAddFont(s_fs, "sans", "msyh.ttf");
		if (s_fontNormal == FONS_INVALID)
		{
			return false;
		}
	}
	float line_height = 15.0f;

	//根据style左右中间对齐计算
	float s_x = pos.left;
	float s_y = (pos.bottom - pos.top) / 2.f + pos.top;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 
	unsigned char a = (dwTextColor) >> 24;
	unsigned char r = (dwTextColor & 0xff0000) >> 16;
	unsigned char g = (dwTextColor & 0xff00) >> 8;
	unsigned char b = (dwTextColor & 0xff);

	unsigned int  text_clr = glfonsRGBA(r,g,b,a);

	fonsClearState(s_fs);
	fonsSetSize(s_fs, line_height);
	fonsSetFont(s_fs, s_fontNormal);
	fonsSetColor(s_fs, text_clr);
	fonsDrawText(s_fs, s_x, s_y, text, NULL);

	return true;
}
void CRenderEngine::DrawQuad(GLuint texture,const DuiRECT& rc)
{
	float gl_left =  0 ;
	float gl_right = s_WndWidth  ;

	float gl_top = s_WndHeight;
	float gl_bottom = 0 ;
	
	glBegin(GL_QUADS);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(gl_left, gl_bottom);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(gl_right, gl_bottom);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(gl_right, gl_top);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(gl_left, gl_top);

	glEnd();
}

bool CRenderEngine::DrawImage(DuiHDC hDC, const DuiRECT& rc, const TDrawInfo& drawInfo)
{
	GLuint texture = 0;
	if (s_TextureCache.find(drawInfo.sImageName) != s_TextureCache.end())
	{
		texture = s_TextureCache[drawInfo.sImageName];
	}
	else
	{
		texture = CRenderEngine::CreateTexture(*drawInfo.pImageInfo);
	}

	DrawQuad(texture, rc);

	return true;
}

bool CRenderEngine::LoadFileContent(const char* file, LPBYTE& data, DWORD& len)
{
	bool ret = false;

	FILE* fp = NULL;
	do
	{
		string sFile = CPaintManagerUI::GetResourcePath();
		sFile += file;

		fp = fopen(sFile.c_str(), "rb");
		if (fp == NULL)
			break;

		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		if (size == 0) break;

		DWORD dwRead = 0;
		data = new unsigned char[size];
		fseek(fp, 0, SEEK_SET);

		dwRead = fread(data, 1, size, fp);
		if (dwRead != size)
		{
			delete data;
			data = NULL;
		}
		len = dwRead;
		ret = true;
	} while (0);

	if (fp)
		fclose(fp);
	if (!ret)
	{
		data = NULL;
		len = 0;
	}
	return ret;
}

TImageInfo* CRenderEngine::LoadImage(const char* bitmap, const char* type, DWORD mask)
{
	LPBYTE pData = NULL;
	DWORD dwSize = 0;

	if (type == NULL) 
	{
		CRenderEngine::LoadFileContent(bitmap,pData, dwSize);
	}
	if (!pData)
	{
		//::MessageBox(0, _T("读取图片数据失败！"), _T("抓BUG"), MB_OK);
		return NULL;
	}

	LPBYTE pImage = NULL;
	int x = 1, y = 1, n;
	if (!type) 
	{
		pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
		delete[] pData;
		if (!pImage) {
			//::MessageBox(0, _T("解析图片失败"), _T("抓BUG"), MB_OK);
			return NULL;
		}
	}
	LPBYTE pDest = new unsigned char[4*x*y];
	bool bAlphaChannel = false;
	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (unsigned char)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (unsigned char)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (unsigned char)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			/*pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];*/
			pDest[i * 4] = pImage[i * 4 ];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4 + 2];
		}

		if (*(DWORD*)(&pDest[i * 4]) == mask) {
			pDest[i * 4] = (unsigned char)0;
			pDest[i * 4 + 1] = (unsigned char)0;
			pDest[i * 4 + 2] = (unsigned char)0;
			pDest[i * 4 + 3] = (unsigned char)0;
			bAlphaChannel = true;
		}
	}

	if (!type ) 
	{
		stbi_image_free(pImage);
	}

	TImageInfo* data = new TImageInfo;

	data->pBits = pDest;
	data->nX = x;
	data->nY = y;
	data->bAlpha = false;
	data->bUseHSL = false;
	data->pSrcBits = NULL;
	return data;
}

void CRenderEngine::FreeImage(TImageInfo* bitmap, bool bDelete)
{
	if (bitmap == NULL) return;
	
	if (bitmap->pSrcBits) {
		delete[] bitmap->pSrcBits;
		bitmap->pSrcBits = NULL;
	}
	if (bDelete) delete bitmap;
}

}