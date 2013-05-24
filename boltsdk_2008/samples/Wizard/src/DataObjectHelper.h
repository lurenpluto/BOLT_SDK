#pragma once
#include "./CDataObjectImpl.h"

class CDataObjectHelper
{
public:
    CDataObjectHelper(void);
    ~CDataObjectHelper(void);

    static CDataObjectHelper* GetInstance();
    static IDataObject* CreateDataObjectFromText(std::wstring& text);
    static void PraseDataObject(IDataObject* lpDataObject, std::wstring& text);
    static BOOL IsCFTEXTData(IDataObject* lpDataObject);
};
