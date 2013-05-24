/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ResHolder
*   Author      :   李亚星
*   Create      :   2013-5-19 22:25
*   LastChange  :   2013-5-19 22:25
*   History     :	
*
*   Description :   BOLT资源的holder，方便使用bolt定义的各种资源并监听相关事件
*
********************************************************************/ 
#ifndef __RESHOLDER_H__
#define __RESHOLDER_H__

#include <XLUE.h>
#include <assert.h>
#include <string>

namespace Xunlei
{
namespace Bolt
{

template<typename _ResHandle, ResType _type>
struct ResTraits
{
	typedef _ResHandle ResHandle;
	static const ResType type = _type;
};

//template<typename ResHandle, ResType type, typename AddRefFunc, RefFunc releasefunc2, LoadFunc funcs>
#define DECLARE_XLGRAPHIC_RESTRAITS(resName, resHandle, resType) \
struct resName##ResTraits \
	: public ResTraits<resHandle, resType> \
	{ \
		static unsigned long AddRef(resHandle handle) \
		{ \
			return XL_AddRef##resName(handle); \
		} \
		static unsigned long Release(resHandle handle) \
		{ \
			return XL_Release##resName(handle); \
		} \
		static long Load(XLUE_RESPROVIDER_HANDLE hResProvider, const char* id, resHandle* lpResHandle, XLUE_RESPROVIDER_HANDLE* lpFromResProvider) \
		{ \
			assert(hResProvider); \
			return XLUE_Get##resName##FromProvider(hResProvider, id, lpResHandle, lpFromResProvider); \
		} \
	};

DECLARE_XLGRAPHIC_RESTRAITS(Bitmap, XL_BITMAP_HANDLE, ResType_bitmap);
DECLARE_XLGRAPHIC_RESTRAITS(Texture, XL_TEXTURE_HANDLE, ResType_texture);
DECLARE_XLGRAPHIC_RESTRAITS(Font, XL_FONT_HANDLE, ResType_font);
DECLARE_XLGRAPHIC_RESTRAITS(ImageList, XL_IMAGELIST_HANDLE, ResType_imagelist);
DECLARE_XLGRAPHIC_RESTRAITS(ImageSeq, XL_IMAGESEQ_HANDLE, ResType_imageseq);
DECLARE_XLGRAPHIC_RESTRAITS(Curve, XL_CURVE_HANDLE, ResType_curve);
DECLARE_XLGRAPHIC_RESTRAITS(Pen, XL_PEN_HANDLE, ResType_pen);
DECLARE_XLGRAPHIC_RESTRAITS(Brush, XL_BRUSH_HANDLE, ResType_brush);

struct ColorResTraits
	: public ResTraits<XL_Color, ResType_color>
{
	static unsigned long AddRef(XL_Color cr)
	{
		return 1;
	}

	static unsigned long Release(XL_Color cr)
	{
		return 1;
	}

	static long Load(XLUE_RESPROVIDER_HANDLE hResProvider, const char* id, XL_Color* lpColor, XLUE_RESPROVIDER_HANDLE* lpFromResProvider)
	{
		assert(hResProvider);

		return XLUE_GetColorFromProvider(hResProvider, id, lpColor, lpFromResProvider);
	}
};

template<typename ResTraits>
class BaseResHolder
{
public:

	typedef BaseResHolder<ResTraits> this_class;
	typedef typename ResTraits::ResHandle res_handle;

	enum LoadStatus
	{
		LoadStatus_NoLoad	= 1,
		LoadStatus_Loading	= 2,
		LoadStatus_Loaded	= 3
	};

	enum Source
	{
		Source_none		= 0,
		Source_Handle	= 1,
		Source_ID		= 2
	};

public:

	BaseResHolder()
		:m_handle(NULL),
		m_source(Source_none),
		m_hResProvider(NULL),
		m_hFromResProvider(NULL),
		m_loadStatus(LoadStatus_NoLoad),
		m_eventCookie(0)
	{

	}

	virtual ~BaseResHolder()
	{
		DetachEvent();

		Release();
	}

	void SetResID(const char* lpID)
	{
		m_source = Source_ID;

		if(!m_id.empty() && lpID != NULL)
		{
			if(strcmp(lpID, m_id.c_str()) == 0)
			{
				return;
			}

			DetachEvent();
		}

		if (lpID != NULL)
		{
			m_id = lpID;
		}
		else
		{
			m_id.clear();
		}

		Release();

		m_loadStatus = LoadStatus_NoLoad;

		OnResChange();
	}

	void SetHandle(res_handle hResHandle)
	{
		m_source = Source_Handle;

		res_handle hOldHandle = m_handle;
		m_handle = hResHandle;

		if (m_handle != NULL)
		{
			ResTraits::AddRef(m_handle);
		}

		if(hOldHandle != NULL)
		{
			ResTraits::Release(hOldHandle);
		}

		if (!m_id.empty())
		{
			DetachEvent();    
			m_id.clear();
		}

		OnResChange();
	}

	const char* GetID() const
	{
		return m_id.c_str();
	}

	res_handle GetHandle()
	{
		if (m_source == Source_ID && m_loadStatus == LoadStatus_NoLoad)
		{
			Load();
		}

		return m_handle;
	}

	// ResProvider一般只需要设置一次
	void SetResProvider(XLUE_RESPROVIDER_HANDLE hResProvider)
	{
		if (m_hResProvider != NULL)
		{
			DetachEvent();
		}

		m_hResProvider = hResProvider;
		m_hFromResProvider = NULL;

		if (m_source == Source_ID)
		{
			Release();
		}
	}

protected:

	// 资源发生改变，使用者可能需要更新显示
	virtual void OnResChange()
	{

	}

	// 资源加载发生错误
	virtual void OnResLoadFailed()
	{

	}

private:

	void Load()
	{
		assert(m_source == Source_ID);

		if(m_hResProvider == NULL)
		{
			return;
		}

		assert(m_loadStatus != LoadStatus_Loaded);
		if (m_loadStatus == LoadStatus_Loading)
		{
			return;
		}

		m_loadStatus = LoadStatus_Loading;

		if(!m_id.empty())
		{
			res_handle handle = NULL;
			XLUE_RESPROVIDER_HANDLE hFromResProvider = NULL;
			ResTraits::Load(m_hResProvider, m_id.c_str(), &handle, &hFromResProvider);
			if (handle != NULL)
			{
				Release();

				m_handle = handle;

				assert(m_hFromResProvider == NULL);
				m_hFromResProvider = hFromResProvider;
				assert(m_hFromResProvider);

				AttchEvent();

				OnResChange();
			}
			else
			{
				OnResLoadFailed();
			}
		}

		m_loadStatus = LoadStatus_Loaded;
	}

	void Release()
	{
		if (m_handle != NULL)
		{
			ResTraits::Release(m_handle);
			m_handle = NULL;
		}

		m_hFromResProvider = NULL;
	}

	void AttchEvent()
	{
		assert(m_hFromResProvider);
		assert(!m_id.empty());
		if(m_hFromResProvider != NULL && !m_id.empty())
		{
			assert(m_eventCookie == 0);
			assert(ResTraits::type != ResType_unknown);
			m_eventCookie = XLUE_ResProviderAttachResEvent(m_hFromResProvider, m_id.c_str(), ResTraits::type, OnResEventCallBack, this);

			assert(m_eventCookie);
		}
	}

	void DetachEvent()
	{
		if(m_hFromResProvider != NULL && m_eventCookie != 0)
		{
			assert(ResTraits::type != ResType_unknown);
			assert(!m_id.empty());

			BOOL ret = XLUE_ResProviderDetachResEvent(m_hFromResProvider, m_id.c_str(), ResTraits::type, m_eventCookie);
			assert(ret);

			m_eventCookie =  0;
		}
	}

	BOOL OnResEvent(const char* id, ResEventFlag flag)
	{
		if(flag == ResEventFlag_load || flag == ResEventFlag_update)
		{
			if (m_loadStatus != LoadStatus_Loading)
			{
				Release();
				m_loadStatus = LoadStatus_NoLoad;

				OnResChange();
			}
		}
	}

	BOOL XLUE_STDCALL OnResEventCallBack(void* userData, const char* id, ResEventFlag flag)
	{
		this_class* lpThis = (this_class*)userData;
		assert(lpThis);

		lpThis->OnResEvent(id, flag);
	}

private:

	std::string m_id;
	res_handle m_handle;
	Source m_source;

	XLUE_RESPROVIDER_HANDLE m_hResProvider;
	XLUE_RESPROVIDER_HANDLE m_hFromResProvider;

	LoadStatus m_loadStatus;

	unsigned long m_eventCookie;
};

typedef BaseResHolder<BitmapResTraits>		BitmapResHolder;
typedef BaseResHolder<TextureResTraits>		TextureResHolder;
typedef BaseResHolder<FontResTraits>		FontResHolder;
typedef BaseResHolder<ImageListResTraits>	ImageListResHolder;
typedef BaseResHolder<ImageSeqResTraits>	ImageSeqResHolder;
typedef BaseResHolder<ColorResTraits>		ColorResHolder;
typedef BaseResHolder<CurveResTraits>		CurveResHolder;
typedef BaseResHolder<PenResTraits>			PenResHolder;
typedef BaseResHolder<BrushResTraits>		BrushResHolder;

} // Bolt
} // Xunlei

#endif // __RESHOLDER_H__