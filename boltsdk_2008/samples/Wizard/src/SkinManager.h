#pragma once
#include <xl_lib/xml/expat_parser.h>
#include <vector>

class CSkinManager : public xl::xml::expat_parser<CSkinManager>
{
public:
    CSkinManager(void);
    ~CSkinManager(void);

    static CSkinManager* GetInstance();

    BOOL LoadSkinXml(const wchar_t* lpFilePath);
    XL_Color GetBitmapHSLColor(XL_BITMAP_HANDLE bitmap);

    XL_Color GetColorByPixel(XL_BITMAP_HANDLE bitmap, unsigned long nx, unsigned long ny);

    const char* GetBitmapIdByIndex(size_t index) const;
    const char* GetTextureIdByIndex(size_t index) const;
    const char* GetColorIdByIndex(size_t index) const;

    size_t GetColorableBitmapCount() const;
    size_t GetColorableColorCount() const;
    size_t GetColorableTextureCount() const;

public:
    void OnPostCreate();
    void OnStartElement (const XML_Char *pszName, const XML_Char **papszAttrs);
    void OnEndElement (const XML_Char *);

private:
    std::vector<std::string> m_ColorableBitmaps;
    std::vector<std::string> m_ColorableColors;
    std::vector<std::string> m_ColorableTextures;
};
