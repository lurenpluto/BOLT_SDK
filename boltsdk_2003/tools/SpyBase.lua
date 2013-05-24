function GetImageAttr(obj, file)
	file:write("ResId="..obj:GetResID().."\n")
	file:write("Alpha="..obj:GetAlpha().."\n")
end

function GetEditObject(obj, file)
	file:write("Text="..obj:GetText().."\n")
	file:write("SelText="..obj:GetSelText().."\n")
	file:write("TextColorID="..obj:GetTextColorID().."\n")
	file:write("BknColorID"..obj:GetBknColorID().."\n")
	file:write("FontID"..obj:GetFontID().."\n")
	file:write("ReadOnly"..tostring(obj:GetReadOnly()).."\n")
	file:write("Multiline"..tostring(obj:GetMultiline()).."\n")
	file:write("NoCaret"..tostring(obj:GetNoCaret()).."\n")
	file:write("AllowHSB"..tostring(obj:GetAllowHSB()).."\n")
	file:write("AllowVSB"..tostring(obj:GetAllowVSB()).."\n")
	file:write("DisableNoScroll"..tostring(obj:GetDisableNoScroll()).."\n")
	file:write("IsPassword"..tostring(obj:GetIsPassword()).."\n")
	file:write("MaxLength"..obj:GetMaxLength().."\n")
	file:write("IsNumber"..tostring(obj:GetIsNumber()).."\n")
end

function GetTextObjectAttr(obj, file)
	file:write("Text="..obj:GetText().."\n")
	file:write("TextFontResID="..obj:GetTextFontResID().."\n")
	file:write("BkColorResID="..obj:GetBkColorResID().."\n")
	local x,y = obj:GetTextExtent()
	file:write("TextExtent="..x..","..y.."\n")
	file:write("HAlign="..tostring(obj:GetHAlign()).."\n")
	file:write("VAlign="..tostring(obj:GetVAlign()).."\n")
	file:write("EndEllipsis="..tostring(obj:GetEndEllipsis()).."\n")
	file:write("WordEllipsis="..tostring(obj:GetWordEllipsis()).."\n")
	file:write("Multiline="..tostring(obj:GetMultiline()).."\n")
	file:write("MultilineTextLimitWidth="..obj:GetMultilineTextLimitWidth().."\n")
	file:write("ShadowColorResID="..obj:GetShadowColorResID().."\n")
	file:write("EnableShadow="..tostring(obj:GetEnableShadow()).."\n")
	file:write("Alpha="..obj:GetAlpha().."\n")
	file:write("TextType="..obj:GetTextType().."\n")
end

function GetTextureObjectAttr(obj, file)
	file:write("TextureID="..obj:GetTextureID().."\n")
	file:write("Alpha="..obj:GetAlpha().."\n")
end

function GetCaptionObjectAttr(obj, file)
	file:write("Caption="..tostring(obj:GetCaption()).."\n")
end

function EnumControlAttribute(obj ,file)
	if obj:GetClass() == "Thunder.TreeCtrl" then
		return	--貌似Thunder.TreeCtrl有问题.暂时先绕开
	end
	local attr = obj:GetAttribute()
	for k,v in pairs(attr) do
		file:write(k.."="..tostring(v).."\n")
	end
end

function DispatchObj(obj, file)
	local class = obj:GetClass()
	if class == "ImageObject"  then
		GetImageAttr(obj, file)
	elseif class == "TextObject" then
		GetTextObjectAttr(obj, file)
	elseif class == "TextureObject"  then
		GetTextureObjectAttr(obj, file)
	elseif class == "CaptionObject"  then
		GetCaptionObjectAttr(obj, file)
	elseif class == "EditObject" then
		GetEditObject(obj,file)
	end
	if obj:IsControl() then
		EnumControlAttribute(obj, file)
	end
end

function GetBaseAttr(obj, file)
	file:write("ID="..obj:GetID().."\n")
	file:write("Class="..obj:GetClass().."\n")
	file:write("ChildCount="..obj:GetChildCount().."\n")
	local left,top,right,bottom = obj:GetObjPos();
	file:write("Pos="..left..","..top..","..right..","..bottom.."\n")
	left,top,right,bottom=obj:GetAbsPos()
	file:write("AbsPos="..left..","..top..","..right..","..bottom.."\n")
	file:write("ZOrder="..obj:GetZorder().."\n")
	file:write("TabOrder="..obj:GetTabOrder().."\n")
	file:write("Visible="..tostring(obj:GetVisible()).."\n")
	file:write("ChildVisible="..tostring(obj:GetChildrenVisible()).."\n")
	file:write("LimitChild="..tostring(obj:GetLimitChild()).."\n")
	DispatchObj(obj, file)
end

map = {}
revmap = {}

function GetNormalHostAttr(obj,file)
	file:write("AppWindow="..tostring(obj:GetAppWindow()).."\n")
	file:write("TopMost="..tostring(obj:GetTopMost()).."\n")
	file:write("MinTrackSize="..obj:GetMinTrackSize().."\n")
	file:write("MaxTrackSize="..obj:GetMaxTrackSize().."\n")
	file:write("Title="..obj:GetTitle().."\n")
end

function GetMenuHostWndAttr(obj, file)
	local left,top,right,bottom = obj:GetMonitorRect();
	file:write("MonitorRect="..left..","..top..","..right..","..bottom.."\n")
	file:write("TrapMouse="..tostring(obj:GetTrapMouse()).."\n")
end

function GetSingleHostWndAttr(obj, file)
	file:write("ID="..obj:GetID().."\n")
	file:write("Class="..obj:GetClassName().."\n")
	file:write("UpdateFPS="..obj:GetUpdateFPS().."\n")
	file:write("WindowState="..obj:GetWindowState().."\n")
	local left,top,right,bottom = obj:GetWindowRect();
	file:write("WindowRect="..left..","..top..","..right..","..bottom.."\n")
	left,top,right,bottom = obj:GetCacheRect();
	file:write("CacheRect="..left..","..top..","..right..","..bottom.."\n")
	file:write("Visible="..tostring(obj:GetVisible()).."\n")
	file:write("Layered="..tostring(obj:GetLayered()).."\n")	
	
	local class=obj:GetClassName();
	if class == "MenuHostWnd" then
		GetMenuHostWndAttr(obj,file)
	elseif class== "ModalHostWnd" then
		GetNormalHostAttr(obj,file)
	elseif class== "FrameHostWnd" then
		GetNormalHostAttr(obj,file)
	end
end

function GetHostWndAttr(obj, file,parent)
	file:write("===========================\n")
	local n = #map+1
	file:write(""..n.."\n")
	file:write(""..parent.."\n")
	map[n] = obj	
	
	GetSingleHostWndAttr(obj,file)
	return n 
end

function GetObjTreeAttr(obj,file,parent)
	file:write("===========================\n")
	local n = #map+1
	file:write(""..n.."\n")
	file:write(""..parent.."\n")
	map[n] = obj		
	
	file:write("ID="..obj:GetID().."\n")
	file:write("Class=UIObjectTree\n")
	
	return n 
end

function TraveChild(obj,file,parent)
	file:write("===========================\n")
	local n = #map+1
	file:write(""..n.."\n")
	file:write(""..parent.."\n")
	map[n] = obj	
	revmap[obj:GetHandle()] = n;
	GetBaseAttr(obj,file)
	for i=0,obj:GetChildCount()-1,1 do
		TraveChild(obj:GetChildByIndex(i), file, n)
	end
end

function GetSingleObj(obj,file)
	GetBaseAttr(obj,file)
end

mapHWNDObjtree = {}
--遍历HostWnd 将HWND和ObjTree放在mapHWNDObjtree
function GetHostWnd()
	mapHWNDObjtree = {}
	XLPrint("XLSpy:GetHostWnd() enter\n")
	local hostwndManage = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local n = hostwndManage:GetHostWndCount()
	XLPrint("XLSpy:There are "..n.." hostwnd!\n")
	hostwndManage:BeginEnumHostWnd()
	local hostwnd = hostwndManage:GetNextHostWnd()
	while hostwnd ~= nil do
		local objtree=hostwnd:GetBindUIObjectTree()
		mapHWNDObjtree[hostwnd:GetWndHandle()]=objtree
		hostwnd = hostwndManage:GetNextHostWnd()
	end
	XLPrint("XLSpy:GetHostWnd() end\n")
end

function TraveObjTree(objtree)
	XLPrint("XLSpy:TraveObjTree() enter\n")
	local file = io.open(gpath.."data.txt","w")
	local layout = objtree:GetRootObject()
	local hostwnd = objtree:GetBindHostWnd()
	GetHostWndAttr(hostwnd,file,0)
	GetObjTreeAttr(objtree,file,1)
	TraveChild(layout,file,2)
	file:write("===========================\n")
	file:close()
	XLPrint("XLSpy:TraveObjTree() end\n")
end

--给对象画边线

function CreateLine(obj)
	local owner = obj:GetOwner();
	local root = owner:GetRootObject()
	local  graphic = XLGetObject("Xunlei.XLGraphic.Factory.Object")
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local spylineleft ,spylinetop,spylineright,spylinebottom
	XLPrint("XLSpy:root class="..root:GetClass().." iscontrol="..tostring(root:IsControl()))
	if root:IsControl() then
		spylineleft = root:GetControlObject("spylineleft")
	else
		spylineleft = owner:GetUIObject("spylineleft")
	end
	if spylineleft == nil then
		XLPrint("XLSpy:Begin Create line")
		local linecolor = graphic:CreateColor(0,0,0 ,255)
		spylineleft = objFactory:CreateUIObject("spylineleft", "FillObject")
		spylinetop = objFactory:CreateUIObject("spylinetop", "FillObject")
		spylineright = objFactory:CreateUIObject("spylineright", "FillObject")
		spylinebottom = objFactory:CreateUIObject("spylinebottom", "FillObject")
		spylineleft:SetSrcColor(linecolor)
		spylinetop:SetSrcColor(linecolor)
		spylineright:SetSrcColor(linecolor)
		spylinebottom:SetSrcColor(linecolor)
		spylineleft:SetObjPos(0,0,0,0)
		spylinetop:SetObjPos(0,0,0,0)
		spylineright:SetObjPos(0,0,0,0)
		spylinebottom:SetObjPos(0,0,0,0)
		spylineleft:SetZorder(10000000)
		spylinetop:SetZorder(10000000)
		spylineright:SetZorder(10000000)
		spylinebottom:SetZorder(10000000)
		root:AddChild(spylineleft)
		root:AddChild(spylinetop)
		root:AddChild(spylineright)
		root:AddChild(spylinebottom)
		XLPrint("XLSpy:end Create line")
	end
end

function ClearAllLine()
	local hostwndManage = XLGetObject("Xunlei.UIEngine.HostWndManager")
	hostwndManage:BeginEnumHostWnd()
	local hostwnd = hostwndManage:GetNextHostWnd()
	while hostwnd ~= nil do
		ClearObjLine(hostwnd)
		hostwnd = hostwndManage:GetNextHostWnd()
	end
end

function OnTimer()
	local app = XLGetObject("XLUESpyApp")
	if app then
		local hwnd = app:GetMouseWnd()
		local hwnd1 = app:GetSpyWnd()
		if hwnd == hwnd1 then return end
		local hostwndManage = XLGetObject("Xunlei.UIEngine.HostWndManager")
		hostwndManage:BeginEnumHostWnd()
		local hostwnd = hostwndManage:GetNextHostWnd()
		while hostwnd ~= nil do
			if hostwnd:GetWndHandle() ~= hwnd then
				ClearObjLine(hostwnd)
			end
			hostwnd = hostwndManage:GetNextHostWnd()
		end
	end
end

function DrawObjLine(obj)
	CreateLine(obj)
	local timermanage = XLGetObject("Xunlei.UIEngine.TimerManager")
	if not cleartime then
		cleartime = timermanage:SetTimer(OnTimer, 500)
	end
	local owner = obj:GetOwner();
	local root = owner:GetRootObject()
	local spylineleft,spylinetop,spylineright,spylinebottom
	if root:IsControl() then
		spylineleft = root:GetControlObject("spylineleft");
		spylinetop = root:GetControlObject("spylinetop");
		spylineright = root:GetControlObject("spylineright");
		spylinebottom = root:GetControlObject("spylinebottom");
	else
		spylineleft = owner:GetUIObject("spylineleft");
		spylinetop = owner:GetUIObject("spylinetop");
		spylineright = owner:GetUIObject("spylineright");
		spylinebottom = owner:GetUIObject("spylinebottom");
	end
	local left,top,right,bottom = obj:GetAbsPos();
	local rootleft,roottop,rootright,rootbottom = root:GetObjPos()
	left = left - rootleft
	top = top - roottop
	right = right - rootleft
	bottom = bottom - roottop
	
	spylineleft:SetObjPos(left,top,left+1,bottom)
	spylinetop:SetObjPos(left,top,right,top+1)
	spylineright:SetObjPos(right-1,top,right,bottom)
	spylinebottom:SetObjPos(left,bottom-1,right,bottom)
	
	XLPrint("XLSpy:line moved")
end

function ClearObjLine(hostwnd)
	-- local hostwndManage = XLGetObject("Xunlei.UIEngine.HostWndManager")
	-- hostwndManage:BeginEnumHostWnd()
	-- local hostwnd = hostwndManage:GetNextHostWnd()
	-- while hostwnd ~= nil do
	if hostwnd then
		local objtree=hostwnd:GetBindUIObjectTree()
		local root = objtree:GetRootObject()
		local spylineleft,spylinetop,spylineright,spylinebottom
		local obj = objtree:GetRootObject()
		if root:IsControl() then
			spylineleft = root:GetControlObject("spylineleft")
			if spylineleft ~= nil then
				spylineleft = root:GetControlObject("spylineleft");
				spylinetop = root:GetControlObject("spylinetop");
				spylineright = root:GetControlObject("spylineright");
				spylinebottom = root:GetControlObject("spylinebottom");
			end
		else
			spylineleft = objtree:GetUIObject("spylineleft")
			if spylineleft ~= nil then
				spylineleft = objtree:GetUIObject("spylineleft");
				spylinetop = objtree:GetUIObject("spylinetop");
				spylineright = objtree:GetUIObject("spylineright");
				spylinebottom = objtree:GetUIObject("spylinebottom");
			end
		end
		if spylineleft ~= nil then
			spylineleft:SetObjPos(0,0,0,0)
			spylinetop:SetObjPos(0,0,0,0)
			spylineright:SetObjPos(0,0,0,0)
			spylinebottom:SetObjPos(0,0,0,0)
		end		
		-- hostwnd = hostwndManage:GetNextHostWnd()
	end	
end

--生成树,HitHWND是全局变量
function MakeTree()
	XLPrint("XLSpy:MakeTree() enter\n")
	map = {}
	revmap = {}

	local objtree = mapHWNDObjtree[HitHWND]
	TraveObjTree(objtree)
	XLPrint("XLSpy:MakeTree() end\n")
end

--HitHWND,HitX,HitY是全局变量,将对象编号返回放在layoutindex中
function HitTest()	
	XLPrint("XLSpy:HitTest() enter\n")
	local objtree = mapHWNDObjtree[HitHWND]
	if objtree ~= nil then
		local layout = objtree:HitTest(HitX, HitY, 0)
		if layout ~= nil then
			if layoutindex  ~= revmap[layout:GetHandle()] then
				DrawObjLine(layout)
				layoutindex = revmap[layout:GetHandle()]
			end			
			XLPrint("XLSpy:layoutindex="..layoutindex.."\n")
		else
			XLPrint("XLSpy:objtree:HitTest Failed objtree="..tostring(objtree))
		end		
		XLPrint("XLSpy:HitTest() end\n")
	end
end

function SignObj(index)
	DrawObjLine(map[index])
end

function GetSingleAttr(deep, index)
	local file = io.open(gpath.."data1.txt","w")	
	if deep == 1 then
		GetSingleHostWndAttr(map[index],file)
	elseif deep == 2 then
		file:write("ID="..map[index]:GetID().."\n")
		file:write("Class=UIObjectTree\n")
	else
		GetSingleObj(map[index],file)
	end	
	file:close()
end

map1 = {}

function GetAllHostWnd()	
	allobjmap = {}
	local file = io.open(gpath.."spy.lua", "w")
	local hostwndManage = XLGetObject("Xunlei.UIEngine.HostWndManager")
	hostwndManage:BeginEnumHostWnd()
	local hostwnd = hostwndManage:GetNextHostWnd()
	while hostwnd ~= nil do
		allobjmap[#allobjmap+1] = hostwnd
		file:write("local n = #mapProcess[#mapProcess].hostwnd\n")
		file:write("mapProcess[#mapProcess].hostwnd[n+1] = {}\n")
		file:write("mapProcess[#mapProcess].hostwnd[n+1].index = " .. #allobjmap .. "\n")
		file:write("mapProcess[#mapProcess].hostwnd[n+1].id = '".. hostwnd:GetID() .."'\n")
		file:write("mapProcess[#mapProcess].hostwnd[n+1].class = '".. hostwnd:GetClassName() .."'\n")
		hostwnd = hostwndManage:GetNextHostWnd()
	end
	file:close()
end

function GetObjTreeByIndex(index)
	local file = io.open(gpath.."spy.lua", "w")
	local objtree = allobjmap[index]:GetBindUIObjectTree()
	local n = #allobjmap
	allobjmap[n+1] = objtree
	file:write("hostwnd.objtree = {} ")
	file:write("local objtree = hostwnd.objtree\n")
	file:write("objtree.index = ".. n+1 .."\n")
	file:write("objtree.id = '".. objtree:GetID() .."'\n")
	file:write("objtree.class = 'UIObjectTree'\n")
	file:close()
end

function GetRootByIndex(index)
	local file = io.open(gpath.."spy.lua", "w")
	local root = allobjmap[index]:GetRootObject()
	local n = #allobjmap
	allobjmap[n+1] = root
	file:write("objtree.child = {}\n")
	file:write("local child = objtree.child\n")
	file:write("child.index = ".. n+1 .."\n")
	file:write("child.id = '".. root:GetID() .."'\n")
	file:write("child.class = '".. root:GetClass() .."'\n")
	file:write("child.children = {}\n")	
	file:close()
end

function GetLayoutByIndex(index)
	local file = io.open(gpath.."spy.lua", "w")
	local layout = allobjmap[index]
	for i=0, layout:GetChildCount()-1 do
		local child = layout:GetChildByIndex(i)
		local n = #allobjmap
		allobjmap[n+1] = child
		file:write("local n = #child.children\n")
		file:write("child.children[n+1] = {}\n")
		file:write("local child = child.children[n+1]\n")
		file:write("child.index = ".. n+1 .."\n")
		file:write("child.id = '"..child:GetID().."'\n")
		file:write("child.class = '"..child:GetClass().."'\n")
		file:write("child.children = {}\n")
	end	
	file:close()
end

function save(t,file,expand,isattr)		--isattr是否是枚举控件的属性,如果是,只枚举一层attr
	if type(t) == "number" then
		file:write(t)
	elseif type(t) == "userdata" then
		file:write(string.format("%q",tostring(t)))
	elseif type(t) == "boolean" then
		file:write(tostring(t))
	elseif type(t) == "function" then
		file:write(string.format("%q",tostring(t)))
	elseif type(t) == "string" then
		file:write(string.format("%q",t))
	elseif type(t) == "table" then
		if not expand then
			file:write("{\n")
			for k,v in pairs(t) do
				if type(k)=="number" then
					file:write("["..k.."]=")
				else
					file:write(k.."=")
				end
				if isattr then
					save(v,file, true)
				else
					save(v,file)
				end
				file:write(",\n")
			end
			file:write("}\n")
		else
			file:write(string.format("%q",tostring(t)))
		end
	end
end

function MakeTable()
	local file = io.open(gpath.."\\ggg.lua","w")
	assert(file)
	MakeHostTable()
	file:write("hostwnd=")
	save(gmap, file)
	file:close()
	revobjmap = {}
	for k,v in pairs(allobjmap) do
		if v.GetHandle then
			revobjmap[v:GetHandle()] = k
		end
	end
end

function MakeHostTable()
	gmap = {}
	allobjmap = {}
	local hostwndManage = XLGetObject("Xunlei.UIEngine.HostWndManager")
	hostwndManage:BeginEnumHostWnd()
	local hostwnd = hostwndManage:GetNextHostWnd()
	while hostwnd ~= nil do
		allobjmap[#allobjmap+1] = hostwnd
		local n = #gmap+1
		gmap[n] = {}
		gmap[n].index = #allobjmap
		gmap[n].id = hostwnd:GetID()
		gmap[n].class = hostwnd:GetClassName()
		MakeObjTreeTable(hostwnd, gmap[n])
		hostwnd = hostwndManage:GetNextHostWnd()
	end
end

function MakeObjTreeTable(hostwnd,host)
	local objtree = hostwnd:GetBindUIObjectTree()
	if objtree then
		allobjmap[#allobjmap+1] = objtree
		host.objtree = {}
		host.objtree.index = #allobjmap
		host.objtree.id = objtree:GetID()
		host.objtree.class = "UIObjectTree"
		MakeRootTable(objtree, host.objtree)
		end
end

function MakeRootTable(objtree,obj)
	local root = objtree:GetRootObject()
	allobjmap[#allobjmap+1] = root
	obj.root = {}
	obj.root.index = #allobjmap
	obj.root.id = root:GetID()
	obj.root.class = root:GetClass()
	obj.root.children = {}
	obj.root.IsControl = root:IsControl()
	MakeChildTable(root, obj.root.children)
end

function MakeChildTable(parent, par)
	for i=0,parent:GetChildCount()-1 do
		local child = parent:GetChildByIndex(i)
		if child then
			allobjmap[#allobjmap+1] = child
			par[i+1] = {}
			par[i+1].index = #allobjmap
			par[i+1].id = child:GetID()
			par[i+1].class = child:GetClass()
			par[i+1].IsControl = child:IsControl()
			par[i+1].children = {}
			MakeChildTable(child, par[i+1].children)
		end
	end
end

function GetNormalHostAttrTab(obj,tab)
	tab.AppWindow=obj:GetAppWindow()
	tab.TopMost=obj:GetTopMost()
	tab.MinTrackSize=obj:GetMinTrackSize()
	tab.MaxTrackSize=obj:GetMaxTrackSize()
	tab.Title=obj:GetTitle()
end

function GetMenuHostWndAttrTab(obj, tab)
	tab.MonitorRect=string.format("%d,%d,%d,%d",obj:GetMonitorRect());
	tab.TrapMouse=obj:GetTrapMouse()
end

function GetSingleHostWndTab(obj, tab)
	tab.ID = obj:GetID()
	tab.Class = obj:GetClassName()
	tab.UpdateFPS=obj:GetUpdateFPS()
	tab.WindowState=obj:GetWindowState()
	tab.WindowRect=string.format("%d,%d,%d,%d",obj:GetWindowRect());
	tab.CacheRect=string.format("%d,%d,%d,%d",obj:GetCacheRect());
	tab.Visible=obj:GetVisible()
	tab.Layered=obj:GetLayered()
	
	local class=obj:GetClassName();
	if class == "MenuHostWnd" then
		GetMenuHostWndAttrTab(obj,tab)
	elseif class== "ModalHostWnd" then
		GetNormalHostAttrTab(obj,tab)
	elseif class== "FrameHostWnd" then
		GetNormalHostAttrTab(obj,tab)
	end
end


function GetSingleHostWndByIndex(index)
	local hostwnd = allobjmap[index]
	local tab = {}
	GetSingleHostWndTab(hostwnd,tab)
	local file = io.open(gpath.."\\ggg.lua","w")
	file:write("tmp=")
	save(tab,file)
	file:close()
end

function GetSingleObjTreeByIndex(index)
	local objtree = allobjmap[index]
	local tab = {}
	tab.ID=objtree:GetID()
	tab.Class="UIObjectTree"
	local file = io.open(gpath.."\\ggg.lua","w")
	file:write("tmp=")
	save(tab,file)
	file:close()
end

function GetImageAttrTab(obj, tab)
	tab.ResId=obj:GetResID()
	tab.Alpha=obj:GetAlpha()
end

function GetEditObjectTab(obj, tab)
	tab.Text=obj:GetText()
	tab.SelText=obj:GetSelText()
	tab.TextColorID=obj:GetTextColorID()
	tab.BknColorID=obj:GetBknColorID()
	tab.FontID=obj:GetFontID()
	tab.ReadOnly=obj:GetReadOnly()
	tab.Multiline=obj:GetMultiline()
	tab.NoCaret=obj:GetNoCaret()
	tab.AllowHSB=obj:GetAllowHSB()
	tab.AllowVSB=obj:GetAllowVSB()
	tab.DisableNoScroll=obj:GetDisableNoScroll()
	tab.IsPassword=obj:GetIsPassword()
	tab.MaxLength=obj:GetMaxLength()
	tab.IsNumber=obj:GetIsNumber()
end

function GetTextObjectAttrTab(obj, tab)
	tab.Text=obj:GetText()
	tab.TextFontResID=obj:GetTextFontResID()
	tab.BkColorResID=obj:GetBkColorResID()
	tab.TextExtent=string.format("%d,%d",obj:GetTextExtent())
	tab.HAlign=obj:GetHAlign()
	tab.VAlign=obj:GetVAlign()
	tab.EndEllipsis=obj:GetEndEllipsis()
	tab.WordEllipsis=obj:GetWordEllipsis()
	tab.Multiline=obj:GetMultiline()
	tab.MultilineTextLimitWidth=obj:GetMultilineTextLimitWidth()
	tab.ShadowColorResID=obj:GetShadowColorResID()
	tab.EnableShadow=obj:GetEnableShadow()
	tab.Alpha=obj:GetAlpha()
	if obj.GetTextType then tab.TextType = obj:GetTextType() end
	tab.TextColorResID = obj:GetTextColorResID()
end

function GetTextureObjectAttrTab(obj, tab)
	tab.TextureID=obj:GetTextureID()
	tab.Alpha=obj:GetAlpha()
end

function GetFillObjectTab(obj, tab)
	if obj.GetSrcColorId then
		tab.SrcColorId = obj:GetSrcColorId()
		tab.DstColorId = obj:GetDstColorId()
	end
end

function GetCaptionObjectAttrTab(obj, tab)
	tab.Caption=obj:GetCaption()
end

function EnumControlAttributeTab(obj ,tab)
	local attr = obj:GetAttribute()
	for k,v in pairs(attr) do
		tab[k] = v
	end
end

function DispatchObjTab(obj, tab)
	local class = obj:GetClass()
	if class == "ImageObject"  then
		GetImageAttrTab(obj, tab)
	elseif class == "TextObject" then
		GetTextObjectAttrTab(obj, tab)
	elseif class == "TextureObject"  then
		GetTextureObjectAttrTab(obj, tab)
	elseif class == "CaptionObject"  then
		GetCaptionObjectAttrTab(obj, tab)
	elseif class == "EditObject" then
		GetEditObjectTab(obj,tab)
	elseif class == "FillObject" then
		GetFillObjectTab(obj, tab)
	end
	if obj:IsControl() then
		EnumControlAttributeTab(obj, tab)
	end
end

function GetBaseAttrTab(obj, tab)
	tab.ID=obj:GetID()
	tab.Class=obj:GetClass()
	tab.ChildCount=obj:GetChildCount()
	tab.Pos=string.format("%d,%d,%d,%d",obj:GetObjPos())
	tab.AbsPos=string.format("%d,%d,%d,%d",obj:GetAbsPos())
	tab.ZOrder=obj:GetZorder()
	tab.AbsZOrder=obj:GetAbsZorder()
	tab.TabOrder=obj:GetTabOrder()
	tab.Visible=obj:GetVisible()
	tab.ChildVisible=obj:GetChildrenVisible()
	tab.LimitChild=obj:GetLimitChild()
	tab.IsControl = obj:IsControl()
	DispatchObjTab(obj, tab)
end

function GetSingleLayoutByIndex(index)
	local layout = allobjmap[index]
	local tab = {}
	GetBaseAttrTab(layout,tab)
	local file = io.open(gpath.."\\ggg.lua","w")
	file:write("tmp=")
	save(tab,file,false,true)
	file:close()
	DrawObjLine(layout)
end

function GetObjBitmap(index)
	local layout = allobjmap[index]
	
	local left,top,right,bottom = layout:GetObjPos()
	local width = right - left
	local height = bottom - top
	if width > 0 and height > 0 then
		local bmpmemory = XLGetObject("BmpMemory")
		local bmp = bmpmemory:CreateBitmap(width,height)
		local renderfactory = XLGetObject("Xunlei.UIEngine.RenderFactory")
		renderfactory:RenderObject(layout,bmp)
		--bmpmemory:Close()		
	end
	local file = io.open(gpath.."\\ggg.lua","w")
	file:write("tmp=")
	local tmp = {}
	tmp.width = width
	tmp.height = height
	save(tmp,file)
	file:close()
end

function NewHitTest(x,y)	
	XLPrint(string.format("XLSpy:NewHitTest(%d,%d)",x,y))
	local hostwndManage = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local n = hostwndManage:GetHostWndCount()
	hostwndManage:BeginEnumHostWnd()
	local hostwnd = hostwndManage:GetNextHostWnd()
	XLPrint(string.format("XLSpy:GHWND=%s", tostring(GHWND)))
	while hostwnd ~= nil do
		if hostwnd:GetWndHandle() == GHWND then
			XLPrint("XLSpy:Find a hostwnd")
			local objtree=hostwnd:GetBindUIObjectTree()
			local layout = objtree:HitTest(x, y, 0)
			if layout then
				XLPrint("XLSpy:Find a layout,and index="..tostring(revobjmap[layout:GetHandle()]))
				local file = io.open(gpath.."\\ggg.lua","w")
				file:write("tmp=")
				local tmp = {}
				tmp.index = revobjmap[layout:GetHandle()]
				save(tmp,file)
				file:close()
				DrawObjLine(layout)
			end
			break
		end
		hostwnd = hostwndManage:GetNextHostWnd()
	end	
end