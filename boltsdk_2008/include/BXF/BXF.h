/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   BXF
*   Author      :   李亚星
*   Create      :   2013-5-23 0:55
*   LastChange  :   2013-5-23 0:55
*   History     :	
*
*   Description :   Bolt扩展框架的涵括头文件，一般工程只需要include该头文件即可
*
********************************************************************/ 
#ifndef __BOLTEXTFRAMEWORK_H__
#define __BOLTEXTFRAMEWORK_H__

#ifndef __cplusplus
	#error BXF requires C++ compilation
#endif

#include <BXF/RectHelper.h>

#include <BXF/LayoutObjectWrapper.h>
#include <BXF/HostWndWrapper.h>
#include <BXF/ObjectTreeWrapper.h>
#include <BXF/ResHolder.h>
#include <BXF/TimerManagerWrapper.h>

#include <BXF/ExtLayoutObjMethodsImpl.h>
#include <BXF/ExtImageObjMethodsImpl.h>
#include <BXF/ExtRealObjMethodsImpl.h>

#include <BXF/ExtObjParserImpl.h>
#include <BXF/ExtObjCreatorImpl.h>
#include <BXF/ExtObjLuaHostImpl.h>
#include <BXF/ExtObjEventImpl.h>

#include <BXF/ExtObjRegisterHelper.h>

#include <BXF/ExtResourceImpl.h>
#include <BXF/ExpWrapper.h>
#include <BXF/ExtObjSafeLock.h>

#endif // __BOLTEXTFRAMEWORK_H__