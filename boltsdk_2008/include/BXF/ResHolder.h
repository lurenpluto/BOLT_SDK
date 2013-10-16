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

template<typename _ResHandle>
struct ResTraits
{
	typedef _ResHandle ResHandle;
};

//template<typename ResHandle, ResType type, typename AddRefFunc, RefFunc releasefunc2, LoadFunc funcs>
#define DECLARE_XLGRAPHIC_RESTRAITS(resName, resHandle, resType) \
struct resName##ResTraits \
	: public ResTraits<resHandle> \
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
		static const char* GetResType() \
		{\
			return resType;\
		}\
	};

DECLARE_XLGRAPHIC_RESTRAITS(Bitmap, XL_BITMAP_HANDLE, XLUE_RESTYPE_BITMAP);
DECLARE_XLGRAPHIC_RESTRAITS(Texture, XL_TEXTURE_HANDLE, XLUE_RESTYPE_TEXTURE);
DECLARE_XLGRAPHIC_RESTRAITS(Font, XL_FONT_HANDLE, XLUE_RESTYPE_FONT);
DECLARE_XLGRAPHIC_RESTRAITS(ImageList, XL_IMAGELIST_HANDLE, XLUE_RESTYPE_IMAGELIST);
DECLARE_XLGRAPHIC_RESTRAITS(ImageSeq, XL_IMAGESEQ_HANDLE, XLUE_RESTYPE_IMAGESEQ);
DECLARE_XLGRAPHIC_RESTRAITS(Curve, XL_CURVE_HANDLE, XLUE_RESTYPE_CURVE);
DECLARE_XLGRAPHIC_RESTRAITS(Pen, XL_PEN_HANDLE, XLUE_RESTYPE_PEN);
DECLARE_XLGRAPHIC_RESTRAITS(Brush, XL_BRUSH_HANDLE, XLUE_RESTYPE_BRUSH);

struct ColorResTraits
	: public ResTraits<XL_Color>
{
	static unsigned long AddRef(XL_Color /*cr*/)
	{
		return 1;
	}

	static unsigned long Release(XL_Color /*cr*/)
	{
		return 1;
	}

	static long Load(XLUE_RESPROVIDER_HANDLE hResProvider, const char* id, XL_Color* lpColor, XLUE_RESPROVIDER_HANDLE* lpFromResProvider)
	{
		assert(hResProvider);

		return XLUE_GetColorFromProvider(hResProvider, id, lpColor, lpFromResProvider);
	}

	static const char* GetResType()
	{
		return XLUE_RESTYPE_COLOR;
	}
};

// 扩展资源类型的traits基类
// 子类T需要实现const char* GetResType()方法
template<typename T>
struct ExtResTraits
	: public ResTraits<XLUE_RESOURCE_HANDLE>
{
	static unsigned long AddRef(XLUE_RESOURCE_HANDLE hResHandle)
	{
		return XLUE_AddRefResource(hResHandle);
	}

	static unsigned long Release(XLUE_RESOURCE_HANDLE hResHandle)
	{
		return XLUE_ReleaseResource(hResHandle);
	}

	static long Load(XLUE_RESPROVIDER_HANDLE hResProvider, const char* id, XLUE_RESOURCE_HANDLE* lpResHandle, XLUE_RESPROVIDER_HANDLE* lpFromResProvider)
	{
		assert(hResProvider);

		return XLUE_GetResFromProvider(hResProvider, T::GetResType(), id, lpResHandle, lpFromResProvider);
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

		if (lpID != NULL)
		{
			if(::strcmp(lpID, m_id.c_str()) == 0)
			{
				return;
			}
		}
		else
		{
			if (m_id.empty())
			{
				return;
			}
		}

		if(!m_id.empty())
		{
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

	const char* GetType() const
	{
		return ResTraits::GetResType();
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
			assert(::strcmp(GetType(), XLUE_RESTYPE_UNKNOWN) != 0);

			m_eventCookie = XLUE_ResProviderAttachResEvent(m_hFromResProvider, m_id.c_str(), GetType(), OnResEventCallBack, this);

			assert(m_eventCookie);
		}
	}

	void DetachEvent()
	{
		if(m_hFromResProvider != NULL && m_eventCookie != 0)
		{
			assert(::strcmp(GetType(), XLUE_RESTYPE_UNKNOWN) != 0);
			assert(!m_id.empty());

			BOOL ret = XLUE_ResProviderDetachResEvent(m_hFromResProvider, m_id.c_str(), GetType(), m_eventCookie);
			assert(ret);

			m_eventCookie =  0;
		}
	}

	bool OnResEvent(const char* /*id*/, ResEventFlag flag)
	{
		if (flag == ResEventFlag_load || flag == ResEventFlag_update)
		{
			if (m_loadStatus != LoadStatus_Loading)
			{
				Release();
				m_loadStatus = LoadStatus_NoLoad;

				OnResChange();
			}
		}

		return true;
	}

	static BOOL XLUE_STDCALL OnResEventCallBack(void* userData, const char* id, ResEventFlag flag)
	{
		this_class* lpThis = (this_class*)userData;
		assert(lpThis);

		return lpThis->OnResEvent(id, flag)? TRUE : FALSE;
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

template<typename OwnerClass, typename ResTraits>
class BaseResHolderEx
	: public BaseResHolder<ResTraits>
{
public:
	typedef OwnerClass owner_class;
	typedef BaseResHolder<ResTraits> base_class;

	typedef void (owner_class::*LPFNOnResChange)();
	typedef void (owner_class::*LPFNOnResLoadFailed)();

public:

	BaseResHolderEx()
		:m_lpOwner(NULL),
		m_lpfnOnResChange(NULL),
		m_lpfnOnResLoadFailed(NULL)
	{

	}

	void SetOwner(owner_class* lpOwner)
	{
		m_lpOwner = lpOwner;
	}

	void SetResFunc(LPFNOnResChange lpfnOnResChange, LPFNOnResLoadFailed lpfnOnResLoadFailed)
	{
		m_lpfnOnResChange = lpfnOnResChange;
		m_lpfnOnResLoadFailed = lpfnOnResLoadFailed;
	}

protected:

	// 资源发生改变
	virtual void OnResChange()
	{
		if (m_lpOwner != NULL && m_lpfnOnResChange != NULL)
		{
			(m_lpOwner->*m_lpfnOnResChange)();
		}

		base_class::OnResChange();
	}

	// 资源加载发生错误
	virtual void OnResLoadFailed()
	{
		if (m_lpOwner != NULL && m_lpfnOnResLoadFailed != NULL)
		{
			(m_lpOwner->*m_lpfnOnResLoadFailed)();
		}

		base_class::OnResLoadFailed();
	}

private:

	owner_class* m_lpOwner;

	LPFNOnResChange m_lpfnOnResChange;
	LPFNOnResLoadFailed m_lpfnOnResLoadFailed;
};

} // Bolt
} // Xunlei

#endif // __RESHOLDER_H__