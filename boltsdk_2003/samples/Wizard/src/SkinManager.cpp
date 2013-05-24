#include "StdAfx.h"
#include ".\skinmanager.h"

CSkinManager::CSkinManager(void)
{
}

CSkinManager::~CSkinManager(void)
{
}

CSkinManager* CSkinManager::GetInstance()
{
    static CSkinManager s_skinManager;

    return &s_skinManager;
}

BOOL CSkinManager::LoadSkinXml(const wchar_t* lpFilePath)
{
    assert(lpFilePath);

    XLFS_FILE_HANDLE hFile = NULL;
    if(XLFS_OpenFile(lpFilePath, L"rb",&hFile,NULL) != XLFS_RESULT_SUCCESS)
    {
        return false;
    }

    char buffer[1024 * 16];

    __int64 nReadLength = 0;
    __int64 nFilePosition = 0;
    bool fSuccess = true;

    if (!create())
    {
		XLFS_CloseFile(hFile);

        return false;
    }

    m_ColorableBitmaps.clear();
    m_ColorableColors.clear();
    m_ColorableTextures.clear();

    while (!XLFS_IsEOF(hFile) && fSuccess)
    {
        nReadLength = XLFS_ReadFile(hFile,(unsigned char*)buffer,(int)sizeof(char) * 1024 * 16);
        fSuccess = this->parse(buffer,(int)nReadLength,0);
    }

    XLFS_CloseFile(hFile);

    destroy();

    return fSuccess;
}

XL_Color CSkinManager::GetBitmapHSLColor(XL_BITMAP_HANDLE bitmap)
{
    XL_Color BitmapMainRGBColor = XL_GetBitmapMainColor(bitmap, NULL, 0);
    return XL_RGB2HSL(BitmapMainRGBColor);
}

XL_Color CSkinManager::GetColorByPixel(XL_BITMAP_HANDLE bitmap, unsigned long nx, unsigned long ny)
{
    BYTE* lpDest = XL_GetBitmapBuffer(bitmap, nx, ny);
    XL_Color clr = XLCOLOR_BGRA(*lpDest++, *lpDest++, *lpDest++, *lpDest);
    return clr;
}

const char* CSkinManager::GetBitmapIdByIndex(size_t index) const
{
    if (index >= 0 && index < m_ColorableBitmaps.size())
    {
        return m_ColorableBitmaps[index].c_str();
    }
    return NULL;
}

const char* CSkinManager::GetColorIdByIndex(size_t index) const
{
    if (index >= 0 && index < m_ColorableColors.size())
    {
        return m_ColorableColors[index].c_str();
    }
    return NULL;
}

const char* CSkinManager::GetTextureIdByIndex(size_t index) const
{
    if (index >= 0 && index < m_ColorableTextures.size())
    {
        return m_ColorableTextures[index].c_str();
    }
    return NULL;
}

size_t CSkinManager::GetColorableBitmapCount() const
{
    return m_ColorableBitmaps.size();
}

size_t CSkinManager::GetColorableColorCount() const
{
    return m_ColorableColors.size();
}

size_t CSkinManager::GetColorableTextureCount() const
{
    return m_ColorableTextures.size();
}

void CSkinManager::OnStartElement(const XML_Char *pszName, const XML_Char **papszAttrs)
{
    if (_stricmp(pszName, "bitmap") == 0)
    {
        if (_stricmp(papszAttrs[0], "id") == 0)
        {
            m_ColorableBitmaps.push_back(papszAttrs[1]);
        }
    }
    else if (_stricmp(pszName, "color") == 0)
    {
        if (_stricmp(papszAttrs[0], "id") == 0)
        {
            m_ColorableColors.push_back(papszAttrs[1]);
        }
    }
    else if (_stricmp(pszName, "texture") == 0)
    {
        if (_stricmp(papszAttrs[0], "id") == 0)
        {
            m_ColorableTextures.push_back(papszAttrs[1]);
        }
    }
}

void CSkinManager::OnEndElement(const XML_Char *)
{
    //to do nothing now
    return;
}

void CSkinManager::OnPostCreate()
{
    enable_element_handler();
}