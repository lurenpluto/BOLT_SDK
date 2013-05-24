/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   ObjectTreeWrapper
*   Author      :   李亚星
*   Create      :   2013-5-19 0:07
*   LastChange  :   2013-5-19 0:07
*   History     :	
*
*   Description :   BOLT的对象树C++辅助类
*
********************************************************************/ 
#ifndef __OBJECTTREEWRAPPER_H__
#define __OBJECTTREEWRAPPER_H__

#include <XLUE.h>
#include <assert.h>

namespace Xunlei
{
namespace Bolt
{

class ObjectTreeWrapper
{
public:
	XLUE_OBJTREE_HANDLE m_hObjectTree;

public:

	ObjectTreeWrapper(XLUE_OBJTREE_HANDLE hObjTree = NULL):
	  m_hObjectTree(hObjTree)
	{
	}

	ObjectTreeWrapper& operator=(XLUE_OBJTREE_HANDLE hObjTree)
	{
		m_hObjectTree = hObjTree;

		return *this;
	}

	void Attach(XLUE_OBJTREE_HANDLE hNewObjTree)
	{
		assert(m_hObjectTree == NULL);
		assert((hNewObjTree == NULL) || XLUE_IsObjTreeValid(hNewObjTree));

		m_hObjectTree = hNewObjTree;
	}

	XLUE_OBJTREE_HANDLE Detach()
	{
		XLUE_OBJTREE_HANDLE hObjTree = m_hObjectTree;
		m_hObjectTree = NULL;

		return hObjTree;
	}

	operator XLUE_OBJTREE_HANDLE() const
	{ 
		return m_hObjectTree; 
	}

public:

	bool IsValid() const
	{
		return (m_hObjectTree != NULL && !!XLUE_IsObjTreeValid(m_hObjectTree));
	}

	const char* GetID() const
	{ 
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_GetObjTreeID(m_hObjectTree);
	}

	void SetMaxClosure(LPCRECT lpClosure) const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_SetTreeMaxClosure(m_hObjectTree, lpClosure);
	}

	XLUE_LAYOUTOBJ_HANDLE GetUIObject(const char* id) const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_GetUIObject(m_hObjectTree, id);
	}

	size_t GetObjectCount() const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_GetObjTreeObjectCount(m_hObjectTree);
	}

	XLUE_LAYOUTOBJ_HANDLE GetRootObj() const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_GetObjTreeRootObj(m_hObjectTree);
	}

	XLUE_LAYOUTOBJ_HANDLE GetFocusObj() const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_GetObjTreeFocusObj(m_hObjectTree);
	}
	
	XLUE_LAYOUTOBJ_HANDLE GetCaptureMouseObj() const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_GetObjTreeCaptureMouseObj(m_hObjectTree);
	}

	long SetRootObj(XLUE_LAYOUTOBJ_HANDLE hObj) const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_SetObjTreeRootObj(m_hObjectTree, hObj);
	}

	XLUE_HOSTWND_HANDLE GetBindHostWnd() const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_GetObjTreeBindHostWnd(m_hObjectTree);
	}

	XLUE_LAYOUTOBJ_HANDLE HitTest(long x, long y) const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_HitTest(m_hObjectTree, x, y);
	}

	void PushDirtyRect(const RECT* lpDirtyRect) const
	{
		assert(m_hObjectTree);
		assert(XLUE_IsObjTreeValid(m_hObjectTree));

		return XLUE_PushObjTreeDirtyRect(m_hObjectTree, lpDirtyRect);
	}

};

} // Bolt
} // Xunlei

#endif // __OBJECTTREEWRAPPER_H__