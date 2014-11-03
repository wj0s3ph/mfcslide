
/************************************************************************/
/* HGUI Lib @Copyright 2014                                             */
/* -------------------------------------------------------              */
/* Author        : Wang.c                                               */
/* FileName      : PngManager.h                                         */
/* Platform      : Win32                                                */
/* Module        : Res Pool                                             */
/* Description   : PNG池,保存管理所有png图片.				        */
/*                                                                      */
/************************************************************************/

#pragma once

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
#include <map>

typedef std::map<UINT, Gdiplus::Image*> TypePngMap;
class HGUIPngPool
{
public:
	HGUIPngPool()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~HGUIPngPool()  
	{
		Gdiplus::Image* pGdipImg = NULL;
		TypePngMap::iterator iter = _Instance()->m_mapPng.begin();
		while (iter != _Instance()->m_mapPng.end())
		{
			pGdipImg = iter->second;
			if (pGdipImg)
			{
				delete pGdipImg;
				pGdipImg = NULL;
				_Instance()->m_mapPng.erase(iter);
				iter = _Instance()->m_mapPng.begin();
			}
			else
				iter++;
		}	
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

	static void Release()
	{
		//不知为什么程序退出时没有析构静态成员实例，导致内存泄漏
		//添加手动释放资源接口
		delete _Instance();
	}
	static Gdiplus::Image* GetPng(const UINT uResID)
	{
		Gdiplus::Image* pGdipImg = NULL;
		TypePngMap::iterator iter = _Instance()->m_mapPng.find(uResID);
		if (iter != _Instance()->m_mapPng.end())	
			pGdipImg = iter->second;
		else
		{
			_LoadPngByResID(uResID, pGdipImg);
			if (pGdipImg)
				_Instance()->m_mapPng[uResID] = pGdipImg;
		}
		return pGdipImg;
	}
protected:
	TypePngMap m_mapPng;

	static BOOL LoadRes(const UINT uResID, CStringA& strOut)
	{
		BOOL bRet = FALSE;
		bRet = LoadInnerRes(uResID, strOut);
		return bRet;
	}

	static BOOL LoadInnerRes(const UINT uResID, CStringA& strOut)
	{
		HRSRC hsrc = ::FindResource((HINSTANCE)&__ImageBase, MAKEINTRESOURCE(uResID), L"png");
		if (!hsrc)
			return FALSE;

		DWORD dwResSize = ::SizeofResource((HINSTANCE)&__ImageBase, hsrc);
		if (dwResSize == 0)
			return FALSE;

		HGLOBAL hglo = ::LoadResource((HINSTANCE)&__ImageBase, hsrc);
		if (!hglo)
			return FALSE;

		LPVOID pResBuf = ::LockResource(hglo);
		if (!pResBuf)
			return FALSE;

		memcpy(strOut.GetBuffer(dwResSize + 1), pResBuf, dwResSize);
		strOut.ReleaseBuffer(dwResSize); /*  如果使用默认参数，会导致字符串读取出错 */

		::FreeResource(hglo);
		return TRUE;
	}
	static BOOL _LoadPngByResID(const UINT uResID, Gdiplus::Image* &pGdipImg)
	{
		CStringA strPng;
		BOOL bRet = FALSE;

		bRet = LoadRes(uResID, strPng);
		if (bRet == FALSE)
			return FALSE;
		
		int nLen = strPng.GetLength();

		HGLOBAL hglo = ::GlobalAlloc(GMEM_FIXED, nLen);
		BYTE* pByte = (BYTE*)::GlobalLock(hglo);
		
		memcpy(pByte, (LPCSTR)strPng, nLen);

		IStream* pStrm = NULL;
		::CreateStreamOnHGlobal(hglo, FALSE, &pStrm);

		pGdipImg = Gdiplus::Image::FromStream(pStrm);

		::GlobalUnlock(hglo);
		pStrm->Release();

		return TRUE;
	}

	ULONG_PTR m_gdiplusToken;

	static HGUIPngPool* m_pInstance;

	static HGUIPngPool* _Instance()
	{
		if (!m_pInstance)
			m_pInstance = new HGUIPngPool;
		return m_pInstance;
	}

};
__declspec(selectany) HGUIPngPool* HGUIPngPool::m_pInstance = NULL;


class HGUIPngDEngine
{
public:
	HGUIPngDEngine(): m_pImage(NULL), m_uItemWidth(0), m_uResID(0){}
	~HGUIPngDEngine()
	{
	}

	HGUIPngDEngine(UINT uWidth, UINT uResID):m_pImage(NULL), m_uItemWidth(uWidth)
	{
		m_uResID = uResID;
		if (m_uResID != 0)
		{
			LoadPNG();
		}
	}
	static HGUIPngDEngine* Check(UINT uResID)
	{
		return new HGUIPngDEngine(0, uResID);
	}
	void SetItemWidth(UINT uWid)	
	{
		m_uItemWidth = uWid;
	}
	
	void SetResID(UINT uResID)
	{
		m_uResID = uResID;
	}

	void LoadPNG()
	{
		m_pImage = HGUIPngPool::GetPng(m_uResID);
	}
	BOOL DrawItem(HDC dc, CRect rc, DWORD dwState)
	{
		if (!m_pImage)
			return FALSE;

		Gdiplus::Graphics gdipgra(dc);
		SIZE size = {0, 0};

		if (m_uItemWidth == 0)
			size.cx = m_pImage->GetWidth();
		else
			size.cx = m_uItemWidth;
		size.cy = m_pImage->GetHeight();

		if (m_uItemWidth == 0)
			gdipgra.DrawImage(m_pImage, Gdiplus::Rect(rc.left, rc.top, size.cx, size.cy));
		else
			gdipgra.DrawImage(m_pImage, Gdiplus::Rect(rc.left, rc.top, size.cx, size.cy), 
							m_uItemWidth * dwState, 0, size.cx, size.cy, Gdiplus::UnitPixel);
		
		return TRUE;
	}

	/* 不能用带透明处理的图片做滑动效果，效率很低 */
	BOOL DrawItem(HDC dc, CRect rc, DWORD dwState, float fAlpha)
	{
		if (!m_pImage)
			return FALSE;

		Gdiplus::Graphics gdipgra(dc);
		SIZE size = {0, 0};

		if (m_uItemWidth == 0)
			size.cx = m_pImage->GetWidth();
		else
			size.cx = m_uItemWidth;
		size.cy = m_pImage->GetHeight();

		
		Gdiplus::ColorMatrix GrayMat = {1.00f, 0.00f, 0.00f, 0.00f, 0.00f,
										0.00f, 1.00f, 0.00f, 0.00f, 0.00f,
										0.00f, 0.00f, 1.00f, 0.00f, 0.00f,
										0.00f, 0.00f, 0.00f, fAlpha, 0.00f,
										0.00f, 0.00f, 0.00f, 0.00f, 1.00f};
		Gdiplus::ImageAttributes ia;
		ia.SetColorMatrix(&GrayMat);
		if (m_uItemWidth == 0)
			gdipgra.DrawImage(m_pImage, Gdiplus::Rect(rc.left, rc.top, size.cx, size.cy));
		else
			gdipgra.DrawImage(m_pImage, Gdiplus::Rect(rc.left, rc.top, size.cx, size.cy), 
							m_uItemWidth * dwState, 0, size.cx, size.cy, Gdiplus::UnitPixel, &ia);

		return TRUE;
	}

protected:
	Gdiplus::Image* m_pImage;
	UINT m_uItemWidth;
	UINT m_uResID;
};

class DrawEngines
{
public:
	DrawEngines(){}
	~DrawEngines()
	{
		HGUIPngDEngine* pEngine = NULL;
		TypeEnginesMap::iterator iter = m_mapEng.begin();
		while (iter != m_mapEng.end())
		{
			pEngine = iter->second;
			if (pEngine)
			{
				delete pEngine;
				m_mapEng.erase(iter);
				iter = m_mapEng.begin();
			}
			else
				iter++;
		}
	}
	static void Release()
	{
		delete _Instance();
	}
	static HGUIPngDEngine* GetEngine(UINT uResID)
	{
		HGUIPngDEngine* pEngine = NULL;
		TypeEnginesMap::iterator iter = _Instance()->m_mapEng.find(uResID);
		if (iter != _Instance()->m_mapEng.end())
		{
			pEngine = iter->second;
		}
		else
		{
			pEngine = HGUIPngDEngine::Check(uResID);
			_Instance()->m_mapEng[uResID] = pEngine;
		}
		return pEngine;
	}
protected:
	typedef std::map<UINT, HGUIPngDEngine*> TypeEnginesMap;
	TypeEnginesMap m_mapEng;
	static DrawEngines* m_pInstance;
	static DrawEngines* _Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new DrawEngines;
		}
		return m_pInstance;
	}
};

__declspec(selectany) DrawEngines* DrawEngines::m_pInstance = NULL;