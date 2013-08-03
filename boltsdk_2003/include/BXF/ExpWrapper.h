/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ExpWrapper
*   Author      :   李亚星
*   Create      :   2013-7-31 
*   LastChange  :   2013-7-31
*   History     :	
*
*   Description :   表达式支持的封装
*
********************************************************************/ 
#ifndef __EXPWRAPPER_H__
#define __EXPWRAPPER_H__

namespace Xunlei
{
namespace Bolt
{

// 表达式的绑定算子，可以定义各种绑定算子
class ExpBinder
{
public:
	virtual ~ExpBinder()
	{}

	virtual bool Bind(XL_EXP_HANDLE hExp) = 0;
};

class ExpItem
{
public:

	enum ExpType
	{
		ExpType_exp = 1,
		ExpType_number = 2
	};

public:
	ExpItem()
		:m_hExp(NULL),
		m_type(ExpType_number),
		m_cacheValue(0)
	{

	}

	~ExpItem()
	{
		Clean();
	}

	bool SetNumberValue(long value)
	{
		if (m_type == ExpType_exp)
		{
			assert(m_hExp);
			if (m_hExp != NULL)
			{
				XL_DeleteExp(m_hExp);
				m_hExp = NULL;
			}
		}

		m_type = ExpType_number;

		m_value = value;

		return true;
	}

	bool SetStringValue(const char* lpExp)
	{
		if (lpExp == NULL)
		{
			return SetNumberValue(0);
		}

		if (m_type != ExpType_exp)
		{
			m_type = ExpType_exp;
			m_hExp = XL_CreateExp();
			assert(m_hExp);
		}

		BOOL bPureNum;
		BOOL bRet = XL_SetExp(m_hExp, lpExp, &bPureNum);
		assert(bRet);
		if (!bRet)
		{
			return false;
		}

		if (bPureNum)
		{
			long value = XL_GetExpValue(m_hExp);
			XL_DeleteExp(m_hExp);

			m_value = value;
			m_type = ExpType_number;
		}

		return true;
	}

	ExpType GetType() const
	{
		return m_type;
	}

	long GetValue() const
	{
		return m_cacheValue;
	}

	bool GetExpString(std::string& expString) const
	{
		bool ret = true;
		if (m_type == ExpType_exp)
		{
			assert(m_hExp);

			ret = GetExpString(m_hExp, expString);
		}
		else
		{
			char szBuffer[20];
			::sprintf_s(szBuffer, "%d", m_value);

			expString = szBuffer;
		}

		return ret;
	}

	bool Bind(ExpBinder* lpBinder)
	{
		assert(lpBinder);
		if (m_type != ExpType_exp)
		{
			return true;
		}

		assert(m_hExp);

		return lpBinder->Bind(m_hExp);
	}

	bool Cacl()
	{
		long newValue;
		if (m_type == ExpType_exp)
		{
			assert(m_hExp);
			XL_CaclExp(m_hExp);
			newValue = XL_GetExpValue(m_hExp);
		}
		else
		{
			newValue = m_value;
		}

		if (m_cacheValue == newValue)
		{
			return false;
		}

		m_cacheValue = newValue;

		return true;
	}

	void Clean()
	{
		if (m_type == ExpType_exp)
		{
			assert(m_hExp);
			XL_DeleteExp(m_hExp);
			m_hExp = NULL;
		}

		m_type = ExpType_number;
		m_cacheValue = 0;
	}

private:

	static bool GetExpString(XL_EXP_HANDLE hExp, std::string& expString)
	{
		assert(hExp);

		char szBuffer[256];
		int len = XL_GetExpString(hExp, szBuffer, 256);
		if (len == 0)
		{
			assert(false);
			return false;
		}

		if (len >= 256)
		{
			char* lpBuffer = (char*)::malloc(len + 1);
			if (lpBuffer == NULL)
			{
				return false;
			}

			XL_GetExpString(hExp, lpBuffer, len + 1);

			expString = lpBuffer;

			::free(lpBuffer);
		}
		else
		{
			expString = szBuffer;
		}

		return true;
	}

private:

	ExpType m_type;

	union
	{
		XL_EXP_HANDLE   m_hExp;
		long            m_value;
	};

	long m_cacheValue;
};

// 绑定到元对象的算子
class ObjectBinder
	: public ExpBinder
{
public:
	ObjectBinder(XLUE_LAYOUTOBJ_HANDLE hBindObj)
		:m_hBindObj(hBindObj)
	{

	}

	virtual ~ObjectBinder()
	{
		assert(m_hBindObj);
		m_hBindObj = NULL;
	}

	// ExpBinder
	virtual bool Bind(XL_EXP_HANDLE hExp)
	{
		assert(hExp);
		assert(m_hBindObj);

		return XL_BindExpObject(hExp, (void*)this, BindProc)? true : false;
	}

private:

	static LPCRECT BindProc(void* lpData, const char* lpcstrObject)
	{
		assert(lpData);
		assert(lpcstrObject);

		ObjectBinder* lpThis = (ObjectBinder*)lpData;
		assert(lpThis);

		return lpThis->GetBindRect(lpcstrObject);
	}

	LPCRECT GetBindRect(const char* lpcstrObject)
	{
		assert(lpcstrObject);
		assert(m_hBindObj);

		return GetObjectRect(m_hBindObj, lpcstrObject);
	}

	LPCRECT GetObjectRect(XLUE_LAYOUTOBJ_HANDLE hBindObj, const char* lpcstrObject) const
	{
		assert(hBindObj);
		assert(lpcstrObject);

		if (*lpcstrObject == '\0')
		{
			return XLUE_GetObjPos(hBindObj);
		}

		const char* lpEnd = lpcstrObject;
		while (*lpEnd != '\0' && *lpEnd != '.')
		{
			++lpEnd;
		}

		const char* lpNextToken = ((*lpEnd == '.')? (lpEnd + 1) : lpEnd);

		if (::strncmp(lpcstrObject, "father", lpEnd - lpcstrObject) == 0)
		{
			XLUE_LAYOUTOBJ_HANDLE hFather = XLUE_GetObjFather(hBindObj);
			if (hFather != NULL)
			{
				return GetObjectRect(hFather, lpNextToken);
			}
		}
		else
		{
			assert(false);
		}

		return NULL;   
	}

private:

	XLUE_LAYOUTOBJ_HANDLE m_hBindObj;
};

// 绑定到rect的算子
class RectBinder
	: public ExpBinder
{
public:
	RectBinder(LPCRECT lpBindRect)
		:m_lpBindRect(lpBindRect)
	{
		assert(m_lpBindRect);
	}

	virtual ~RectBinder()
	{
		assert(m_lpBindRect);
		m_lpBindRect = NULL;
	}

	// ExpBinder
	virtual bool Bind(XL_EXP_HANDLE hExp)
	{
		assert(hExp);
		assert(m_lpBindRect);

		return XL_BindExpRect(hExp, m_lpBindRect)? true : false;
	}

private:

	LPCRECT m_lpBindRect;
};

class BaseExp
{
public:
	BaseExp()
		:m_lpBinder(NULL)
	{

	}

	virtual ~BaseExp()
	{
		if (m_lpBinder != NULL)
		{
			delete m_lpBinder;
			m_lpBinder = NULL;
		}
	}

	bool IsBind() const
	{
		return (m_lpBinder != NULL);
	}

	bool BindObject(XLUE_LAYOUTOBJ_HANDLE hObj)
	{
		assert(m_lpBinder == NULL);
		m_lpBinder = new ObjectBinder(hObj);

		return BindImpl();
	}

	bool BindRect(LPCRECT lpRect)
	{
		assert(m_lpBinder == NULL);
		m_lpBinder = new RectBinder(lpRect);

		return BindImpl();
	}
	
protected:

	bool SetNumberValue(ExpItem& item, long value)
	{
		return item.SetNumberValue(value);
	}

	bool SetStringValue(ExpItem& item, const char* value)
	{
		bool ret = item.SetStringValue(value);
		if (ret 
			&& IsBind()
			&& item.GetType() == ExpItem::ExpType_exp)
		{
			item.Bind(m_lpBinder);
		}

		return ret;
	}

	// 子类需要重写该方法
	virtual bool BindImpl() = 0;

protected:

	ExpBinder* m_lpBinder;
};

// POINT坐标点表达式
class PointExp
	: public BaseExp
{
public:
	PointExp()
	{

	}

	~PointExp()
	{
		
	}

	// 设置X表达式
	bool SetNumberX(long value)
	{
		return SetNumberValue(m_xExp, value);
	}

	bool SetStringX(const char* value)
	{
		return SetStringValue(m_xExp, value);
	}

	// 设置Y表达式
	bool SetNumberY(long value)
	{
		return SetNumberValue(m_yExp, value);
	}

	bool SetStringY(const char* value)
	{
		return SetStringValue(m_yExp, value);
	}

	// 计算表达式
	bool Cacl()
	{
		bool ret = m_xExp.Cacl();
		ret |= m_yExp.Cacl();

		return ret;
	}

	// 获取表达式计算结果
	void GetValue(POINT& pt) const
	{
		pt.x = m_xExp.GetValue();
		pt.y = m_yExp.GetValue();
	}

private:

	// BaseExp
	virtual bool BindImpl()
	{
		assert(m_lpBinder);

		m_xExp.Bind(m_lpBinder);
		m_yExp.Bind(m_lpBinder);

		return true;
	}

private:

	ExpItem m_xExp;
	ExpItem m_yExp;
};

class RectExp
	: public BaseExp
{
public:
	RectExp()
	{
	}

	~RectExp()
	{
	}

	// 设置left表达式
	bool SetNumberLeft(long value)
	{
		return SetNumberValue(m_leftExp, value);
	}

	bool SetStringLeft(const char* value)
	{
		return SetStringValue(m_leftExp, value);
	}

	// 设置top表达式
	bool SetNumberTop(long value)
	{
		return SetNumberValue(m_topExp, value);
	}

	bool SetStringTop(const char* value)
	{
		return SetStringValue(m_topExp, value);
	}

	// 设置right表达式
	bool SetNumberRight(long value)
	{
		return SetNumberValue(m_rightExp, value);
	}

	bool SetStringRight(const char* value)
	{
		return SetStringValue(m_rightExp, value);
	}

	// 设置bottom表达式
	bool SetNumberBottom(long value)
	{
		return SetNumberValue(m_bottomExp, value);
	}

	bool SetStringBottom(const char* value)
	{
		return SetStringValue(m_bottomExp, value);
	}

	bool Cacl()
	{
		bool ret = m_leftExp.Cacl();
		ret |= m_topExp.Cacl();
		ret |= m_rightExp.Cacl();
		ret |= m_bottomExp.Cacl();

		return ret;
	}

	void GetValue(RECT& rc) const
	{
		rc.left = m_leftExp.GetValue();
		rc.top = m_topExp.GetValue();
		rc.right = m_rightExp.GetValue();
		rc.bottom = m_bottomExp.GetValue();
	}

private:

	// BaseExp
	virtual bool BindImpl()
	{
		assert(m_lpBinder);

		m_leftExp.Bind(m_lpBinder);
		m_topExp.Bind(m_lpBinder);
		m_rightExp.Bind(m_lpBinder);
		m_bottomExp.Bind(m_lpBinder);

		return true;
	}

private:

	ExpItem m_leftExp;
	ExpItem m_topExp;
	ExpItem m_rightExp;
	ExpItem m_bottomExp;
};

} // Bolt
} // Xunlei

#endif // __EXPWRAPPER_H__