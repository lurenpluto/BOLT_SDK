#include "StdAfx.h"
#include "./ImageCore.h"
#include "./Transcode.h"
#include <ShlObj.h>

ImageCore::ImageCore(void)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::Status ret = GdiplusStartup(&m_token, &gdiplusStartupInput, NULL);
	assert(ret == Gdiplus::Ok);
}

ImageCore::~ImageCore(void)
{
	assert(m_token);
	Gdiplus::GdiplusShutdown(m_token);
	m_token = 0;
}

bool ImageCore::ParseDataObject( IDataObject* lpObj, std::wstring& strFile )
{
	assert(lpObj);
	CComPtr<IEnumFORMATETC> spEnumFORMATETC;
	lpObj->EnumFormatEtc(DATADIR_GET, &spEnumFORMATETC);
	if (spEnumFORMATETC == NULL)
	{
		return false;
	}

	bool accept = false;

	FORMATETC formatetc;
	ULONG celtFetched;
	while (S_OK == spEnumFORMATETC->Next(1, &formatetc, &celtFetched))
	{
		if (formatetc.cfFormat != CF_HDROP)
		{
			continue;
		}

		STGMEDIUM medium;
		lpObj->GetData(&formatetc, &medium);
		if (medium.tymed == TYMED_HGLOBAL)
		{
			BYTE* lpBuffer = (BYTE*)::GlobalLock(medium.hGlobal);
			assert(lpBuffer);
			if (lpBuffer != NULL)
			{
				DROPFILES *lpDropFiles = (DROPFILES *)lpBuffer;
				if (lpDropFiles->fWide)
				{
					strFile = (wchar_t *)(lpBuffer + lpDropFiles->pFiles);
				}
				else
				{
					std::string strTmp = (char *)(lpBuffer + lpDropFiles->pFiles);
					Transcode::ANSI_to_Unicode(strTmp.c_str(), strTmp.length(), strFile);
				}

				accept = IsFileAccept(strFile.c_str());
			}

			::GlobalUnlock(medium.hGlobal);
			
			break;
		}
	}

	return accept;
}

XL_BITMAP_HANDLE ImageCore::LoadImage( const wchar_t* lpFile )
{
	assert(lpFile);
	if (lpFile == NULL)
	{
		return NULL;
	}

	const wchar_t* lpExt = ::PathFindExtension(lpFile);
	if (lpExt != NULL && ::lstrcmpi(lpExt, L".png") == 0)
	{
		return LoadPng(lpFile); 
	}

	CComPtr<IStream> spStream;
	::CreateStreamOnHGlobal(NULL, TRUE, &spStream);
	assert(spStream);
	if (spStream == NULL)
	{
		return NULL;
	}

	if (!LoadFileToStream(lpFile, spStream))
	{
		return NULL;
	}

	return LoadByGDIPlus(spStream);
}

XL_BITMAP_HANDLE ImageCore::LoadPng( const wchar_t* lpFile )
{
	assert(lpFile);

	XL_BITMAP_HANDLE hBitmap = XL_LoadBitmapFromFile(lpFile, XLGRAPHIC_CT_ARGB32);
	assert(hBitmap);

	return hBitmap;
}

XL_BITMAP_HANDLE ImageCore::LoadByGDIPlus( IStream* lpStream )
{
	Gdiplus::Image img(lpStream);

	Gdiplus::Status ret = img.GetLastStatus();
	if (ret != Gdiplus::Ok)
	{
		return NULL;
	}

	UINT uRealHeight = img.GetHeight();
	UINT uRealWidth = img.GetWidth();
	if ( uRealHeight == 0 || uRealWidth == 0 )
	{
		assert(false);
		return NULL;
	}

	return ConvertImageToBitmap(&img);
}

XL_BITMAP_HANDLE ImageCore::ConvertImageToBitmap( Gdiplus::Image* lpImage )
{
	assert(lpImage);

	CClientDC dc(NULL);
	CDC MemoryDC;
	MemoryDC.CreateCompatibleDC(dc);

	assert(MemoryDC);

	UINT height = lpImage->GetHeight();
	UINT width = lpImage->GetWidth();
	assert(height > 0 && width > 0);
	if (height <= 0 || width <= 0)
	{
		return NULL;
	}

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biHeight =  -(LONG)height;
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;
	BYTE* pbase = NULL;
	CBitmap memoryBmp = ::CreateDIBSection(MemoryDC, &bmpInfo, DIB_RGB_COLORS, (void**)&pbase, 0, 0);
	assert(memoryBmp);
	if (memoryBmp.IsNull())
	{
		return NULL;
	}

	HBITMAP hOldBmp = MemoryDC.SelectBitmap(memoryBmp);

	Gdiplus::Graphics graphics(MemoryDC);

	Gdiplus::Rect rect(0, 0, width, height);
	graphics.DrawImage(lpImage, rect);

	assert(graphics.GetLastStatus() == Gdiplus::Ok);

	MemoryDC.SelectBitmap(hOldBmp);

	return ConvertHBITMAPToXLBitmap(memoryBmp);
}

XL_BITMAP_HANDLE ImageCore::ConvertHBITMAPToXLBitmap( HBITMAP hBitmap )
{
	assert(hBitmap);

	CBitmapHandle srcBitmap(hBitmap);
	BITMAP bmp;
	srcBitmap.GetBitmap(bmp);
	if (bmp.bmHeight <= 0 || bmp.bmWidth <= 0)
	{
		assert(false);
		return NULL;
	}

	XL_BITMAP_HANDLE hXLBitmap = XL_CreateBitmap(XLGRAPHIC_CT_ARGB32, bmp.bmWidth, bmp.bmHeight);
	assert(hXLBitmap);
	if (hXLBitmap == NULL)
	{
		return NULL;
	}

	BYTE* lpbyDest = XL_GetBitmapBuffer(hXLBitmap, 0, 0);
	assert(lpbyDest);

	if (bmp.bmBits != NULL && bmp.bmBitsPixel == 32)
	{
		::memcpy(lpbyDest, bmp.bmBits, bmp.bmWidthBytes * bmp.bmHeight);
	}
	else
	{
		CClientDC dc(NULL);
		CDC MemoryDC, srcDC;
		MemoryDC.CreateCompatibleDC(dc);
		srcDC.CreateCompatibleDC(dc);
		assert(MemoryDC);
		assert(srcDC);

		BITMAPINFO bmpInfo;
		bmpInfo.bmiHeader.biHeight =  -bmp.bmHeight;
		bmpInfo.bmiHeader.biWidth = bmp.bmWidth;
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 32;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		bmpInfo.bmiHeader.biSizeImage = 0;
		bmpInfo.bmiHeader.biXPelsPerMeter = 0;
		bmpInfo.bmiHeader.biYPelsPerMeter = 0;
		bmpInfo.bmiHeader.biClrUsed = 0;
		bmpInfo.bmiHeader.biClrImportant = 0;

		BYTE* pbase = NULL;
		CBitmap memoryBmp = ::CreateDIBSection(MemoryDC, &bmpInfo, DIB_RGB_COLORS, (void**)&pbase, 0, 0);
		assert(memoryBmp);

		HBITMAP hOldSrcBmp = srcDC.SelectBitmap(srcBitmap);
		HBITMAP hOldBmp = MemoryDC.SelectBitmap(memoryBmp);

		MemoryDC.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

		::memcpy(lpbyDest, pbase, bmp.bmWidthBytes * bmp.bmHeight);

		srcDC.SelectBitmap(hOldSrcBmp);
		MemoryDC.SelectBitmap(hOldBmp);
	}

	return hXLBitmap;
}

bool ImageCore::LoadFileToStream( const wchar_t* lpFile, IStream* lpStream )
{
	assert(lpFile);
	assert(lpStream);

	FILE* file = _wfopen(lpFile, L"rb");
	assert(file);
	if (file == NULL)
	{
		return false;
	}

	byte buffer[1024];
	size_t len;
	while((len = fread(buffer, sizeof(byte), 1024, file)) != 0)
	{
		lpStream->Write(buffer, (ULONG)len, NULL);
	}

	fclose(file);

	return true;
}

ImageCore* ImageCore::GetInstance()
{
	static ImageCore s_instance;

	return &s_instance;
}

bool ImageCore::IsFileAccept( const wchar_t* lpFile )
{
	assert(lpFile);
	const wchar_t* lpExt = ::PathFindExtension(lpFile);
	if (lpExt == NULL)
	{
		return false; 
	}

	if (::lstrcmpi(lpExt, L".png") == 0
		|| ::lstrcmpi(lpExt, L".jpg") == 0
		|| ::lstrcmpi(lpExt, L".jpeg") == 0
		|| ::lstrcmpi(lpExt, L".bmp") == 0)
	{
		return true;
	}

	return false;
}

XL_Color ImageCore::GetBitmapMainColor( XL_BITMAP_HANDLE hBitmap )
{
	assert(hBitmap);
	if (hBitmap == NULL)
	{
		return XLCOLOR_BGR(255,255,255);
	}

	return XL_GetBitmapMainColor(hBitmap, NULL, 0);
}