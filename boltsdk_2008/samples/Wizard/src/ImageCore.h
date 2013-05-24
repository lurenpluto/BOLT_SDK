#pragma once

#include <GdiPlus.h>

class ImageCore
{
public:
	ImageCore(void);
	~ImageCore(void);

	static ImageCore* GetInstance();

	bool ParseDataObject(IDataObject* lpObj, std::wstring& strFile);
	XL_BITMAP_HANDLE LoadImage(const wchar_t* lpFile);
	XL_Color GetBitmapMainColor(XL_BITMAP_HANDLE hBitmap);

private:

	bool LoadFileToStream( const wchar_t* lpFile, IStream* lpStream );
	XL_BITMAP_HANDLE LoadByGDIPlus( IStream* lpStream );
	XL_BITMAP_HANDLE ConvertImageToBitmap( Gdiplus::Image* lpImage );
	XL_BITMAP_HANDLE ConvertHBITMAPToXLBitmap( HBITMAP hBitmap );

	XL_BITMAP_HANDLE LoadPng(const wchar_t* lpFile);

	bool IsFileAccept(const wchar_t* lpFile);

private:

	ULONG_PTR   m_token;
};
