#include "StdAfx.h"
#include ".\wizardapp.h"
#include "./Transcode.h"
#include "./WebBrowserDisp.h"

WizardApp::WizardApp(void)
:m_lpEvent(NULL)
{
}

WizardApp::~WizardApp(void)
{
	if (m_lpEvent != NULL)
	{
		delete m_lpEvent;
		m_lpEvent = NULL;
	}
}

bool WizardApp::GetString( const wchar_t* lpSection, const wchar_t* lpKey, std::wstring& strRet, const wchar_t* lpDefault /*= L""*/ )
{
	static wchar_t szBuffer[2048];
	::GetPrivateProfileString(lpSection, lpKey, lpDefault, szBuffer, 2048, m_strConfig.c_str());

	strRet = szBuffer;

	return true;
}

bool WizardApp::SetString( const wchar_t* lpSection, const wchar_t* lpKey, const wchar_t* lpValue )
{
	std::wstring strCurValue;
	GetString(lpSection, lpKey, strCurValue, L"{EA93CC4C-E460-4465-AEC9-57F2A8A348E1}");

	bool changed = false;
	if (lpValue != NULL)
	{
		if (strCurValue != lpValue)
		{
			changed = true;
		}
	}
	else
	{
		if (!strCurValue.empty())
		{
			changed = true;
		}
	}

	if (changed)
	{
		::WritePrivateProfileString(lpSection, lpKey, lpValue, m_strConfig.c_str());

		GetEvent()->FireConfigChange(lpSection, lpKey, lpValue);
	}

	return true;
}

bool WizardApp::Init()
{
	wchar_t szPath[MAX_PATH];
	::GetModuleFileName(_Module.GetModuleInstance(), szPath, MAX_PATH);

	::PathCombine(szPath, szPath, L"..\\config.ini");

	m_strConfig = szPath;

	CComObject<CWebBrowserDisp>* lpDisp = NULL;
	CComObject<CWebBrowserDisp>::CreateInstance(&lpDisp);
	assert(lpDisp);

	HRESULT hr = lpDisp->QueryInterface(IID_IDispatch, (void**)&m_spWebDisp);
	assert(hr == S_OK);
	assert(m_spWebDisp);

	return true;
}

bool WizardApp::Uinit()
{
	if (m_lpEvent != NULL)
	{
		m_lpEvent->Clear();
		delete m_lpEvent;
		m_lpEvent = NULL;
	}

	m_spWebDisp.Release();

	return true;
}

WizardApp* WizardApp::GetInstance()
{
	static WizardApp s_instance;

	return &s_instance;
}

WizardAppEvent* WizardApp::GetEvent()
{
	if (m_lpEvent == NULL)
	{
		m_lpEvent = new WizardAppEvent();
	}

	return m_lpEvent;
}

bool WizardApp::Quit(int exitCode)
{
	::PostQuitMessage(exitCode);

	return true;
}

int WizardApp::GetInt( const wchar_t* lpSection, const wchar_t* lpKey, int defaultValue /*= 0*/ )
{
	return ::GetPrivateProfileInt(lpSection, lpKey, defaultValue, m_strConfig.c_str());

	return true;
}

bool WizardApp::SetInt( const wchar_t* lpSection, const wchar_t* lpKey, int value )
{
	bool changed = true;

	if (IsKeyExist(lpSection, lpKey))
	{
		if (GetInt(lpSection, lpKey) == value)
		{
			changed = false;
		}
	}

	if (changed)
	{
		wchar_t szValue[32];
		_itow(value, szValue, 10);

		::WritePrivateProfileString(lpSection, lpKey, szValue, m_strConfig.c_str());
		GetEvent()->FireConfigChange(lpSection, lpKey, value);
	}

	return true;
}

bool WizardApp::IsKeyExist( const wchar_t* lpSection, const wchar_t* lpKey )
{
	const wchar_t defaultValue[] = L"{EA93CC4C-E460-4465-AEC9-57F2A8A348E1}";
	
	std::wstring strRet;
	GetString(lpSection, lpKey, strRet, defaultValue);

	if (strRet == defaultValue)
	{
		return false;
	}

	return true;
}

IDispatch* WizardApp::GetWebBrowserDisp()
{
	CComQIPtr<IDispatch> spDisp = m_spWebDisp;

	return spDisp.Detach();
}

void WizardApp::Crash()
{
	throw("wizard crash");
}

bool WizardApp::GetFlashFile( std::wstring& strFile )
{
	wchar_t szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	::PathCombine(szPath, szPath, L"..\\..\\samples\\Wizard\\test.swf");

	assert(::PathFileExists(szPath));

	strFile = szPath;

	return true;
}

void WizardAppEvent::FireConfigChange( const wchar_t* lpSection, const wchar_t* lpKey, const wchar_t* lpValue )
{
	if (m_configChangeEventContainer.GetEventCount() > 0)
	{
		std::string strSection, strKey, strValue;

		if (lpSection != NULL)
		{
			Transcode::Unicode_to_UTF8(lpSection, wcslen(lpSection), strSection);
		}

		if (lpKey != NULL)
		{
			Transcode::Unicode_to_UTF8(lpKey, wcslen(lpKey), strKey);
		}

		if (lpValue != NULL)
		{
			Transcode::Unicode_to_UTF8(lpValue, wcslen(lpValue), strValue);
		}

		size_t nCount = m_configChangeEventContainer.GetEventCount();
		LuaConfigChangeEvent* lpEvent;
		long lRet = 0;

		for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
		{
			lpEvent = m_configChangeEventContainer.GetEventByIndex(nIndex);
			assert(lpEvent);
			if (lpEvent != NULL)
			{
				lRet = (*lpEvent)(strSection.c_str(), strKey.c_str(), strValue.c_str());
			}
		}
	}
}

void WizardAppEvent::FireConfigChange( const wchar_t* lpSection, const wchar_t* lpKey, int value )
{
	if (m_configChangeEventContainer.GetEventCount() > 0)
	{
		std::string strSection, strKey;

		if (lpSection != NULL)
		{
			Transcode::Unicode_to_UTF8(lpSection, wcslen(lpSection), strSection);
		}

		if (lpKey != NULL)
		{
			Transcode::Unicode_to_UTF8(lpKey, wcslen(lpKey), strKey);
		}

		size_t nCount = m_configChangeEventContainer.GetEventCount();
		LuaConfigChangeEvent* lpEvent;
		long lRet = 0;

		for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
		{
			lpEvent = m_configChangeEventContainer.GetEventByIndex(nIndex);
			assert(lpEvent);
			if (lpEvent != NULL)
			{
				lRet = (*lpEvent)(strSection.c_str(), strKey.c_str(), value);
			}
		}
	}
}

bool WizardAppEvent::Clear()
{
	m_configChangeEventContainer.Clear();

	return true;
}