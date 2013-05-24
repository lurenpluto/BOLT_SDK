#include "StdAfx.h"
#include ".\dataobjecthelper.h"

CDataObjectHelper::CDataObjectHelper(void)
{
}

CDataObjectHelper::~CDataObjectHelper(void)
{
}

CDataObjectHelper* CDataObjectHelper::GetInstance()
{
    static CDataObjectHelper s_instance;

    return &s_instance;
}

HANDLE StringToHandle (const wchar_t *szText, int nTextLen)
{
    void  *ptr;
    // if text length is -1 then treat as a nul-terminated string
    if(nTextLen == -1)
    {
        nTextLen = wcslen(szText)*2 + 2;
    }
    // allocate and lock a global memory buffer. Make it fixed
    // data so we don't have to use GlobalLock
    ptr = (void *)GlobalAlloc(GMEM_FIXED, nTextLen);
    // copy the string into the buffer
    memcpy (ptr, szText, nTextLen);
    return ptr;
}


IDataObject* CDataObjectHelper::CreateDataObjectFromText(std::wstring& text)
{
    FORMATETC fmtetc = { CF_UNICODETEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stgmed = {TYMED_HGLOBAL, {0}, 0};
    stgmed.hGlobal = StringToHandle(text.c_str(), -1);
    CDataObject* lpDataObject = new CDataObject();
    lpDataObject->AddRef();
    lpDataObject->SetData(&fmtetc, &stgmed, TRUE);
    return lpDataObject;
}

void CDataObjectHelper::PraseDataObject(IDataObject* lpDataObject, std::wstring& text)
{
    FORMATETC fmtetc = { CF_UNICODETEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stgmed;
    if (lpDataObject->GetData(&fmtetc, &stgmed) == S_OK)
    {
        wchar_t* data = (wchar_t*)::GlobalLock(stgmed.hGlobal);
        text = data;
        ::GlobalUnlock(stgmed.hGlobal);
        ::ReleaseStgMedium(&stgmed);
    }
}

BOOL CDataObjectHelper::IsCFTEXTData(IDataObject* lpDataObject)
{
    CComPtr<IEnumFORMATETC> spEnumFORMATETC;
    lpDataObject->EnumFormatEtc(DATADIR_GET, &spEnumFORMATETC);
    FORMATETC formatetc;
    ULONG celtFetched;
    if (spEnumFORMATETC)
    {
        while (S_OK == spEnumFORMATETC->Next(1, &formatetc, &celtFetched))
        {
            if (formatetc.cfFormat == CF_UNICODETEXT)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}