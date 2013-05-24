// WebBrowserDisp.cpp : Implementation of CWebBrowserDisp

#include "stdafx.h"
#include "WebBrowserDisp.h"
#include ".\webbrowserdisp.h"


// CWebBrowserDisp


STDMETHODIMP CWebBrowserDisp::ShowMessage(BSTR msg)
{
	if (msg != NULL)
	{
		::MessageBox(NULL, msg, L"letter from c++!", MB_OK);
	}

	return S_OK;
}
