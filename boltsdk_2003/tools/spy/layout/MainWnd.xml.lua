function OnClose(self)
	local app = XLGetObject("SpyApp")
	app:Exit()
end

function minBtnOnClick(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Spy.MainFrame")
	hostwnd:Min()
end

function maxBtnOnClick(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Spy.MainFrame")
	local state = hostwnd:GetWindowState()
	if state == "max" then
		hostwnd:Restore()
		self:SetIcon( "bitmap.sysbtn.icon.max" )
	else
		hostwnd:Max()
		self:SetIcon( "bitmap.sysbtn.icon.restore" )
	end
end

function maxBtnOnInitControl(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Spy.MainFrame")
	local state = hostwnd:GetWindowState()
	if state == "max" then
		self:SetIcon( "bitmap.sysbtn.icon.restore" )
	else
		self:SetIcon( "bitmap.sysbtn.icon.max" )
	end
end	

function OnSize(self, type, x, y)
	if type == "min" then return end
	
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	rootObject:SetObjPos(0, 0, x, y)
end

function OnCheck(self, check1, check2)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Spy.MainFrame")
	hostwnd:SetTopMost(check2)
end

mapitem = {}
mapProcess = {}

function RefreshBtnOnClick(self)
	local owner = self:GetOwnerControl()
	Refresh(owner)
end

function Refresh(owner)
	mapProcess = {}
	local tree = owner:GetControlObject("tree")
	local item1 = tree:GetRootItem()
	for i=tree:GetChildrenItemCount(item1),1,-1 do
		tree:RemoveItem(tree:GetChildItemByIndex(item1,i))
	end
	local obj1 = tree:InsertItemText("本地包含XLUE进程",item1)
	tree:SetExpand(obj1,true)
	local promanage = XLGetObject("ProManage.Factory")
	local pro = promanage:CreateInstance()
	pro:Init()
		
		local cnt = pro:GetProcessCount()
		for i = 0,cnt-1 do
			local p = pro:GetProcess(i)
			local item = tree:InsertItemText(string.format("%s(%d)",p.name,p.pid),obj1)
			tree:SetExpand(item,true)
			
			local msgfactory = XLGetObject("Message.Factory")
			local msg = msgfactory:CreateInstance(p.mainwnd, p.tid)
			msg:Init()
			
			local index = #mapProcess+1
			mapProcess[index] = {}
			mapProcess[index].msg = msg
			mapProcess[index].name = p.name
			mapProcess[index].pid = p.pid
			mapProcess[index].hostwnd = {}
			_G["hostwnd"] = {}
			
			item.process = index
			
			msg:DoString("MakeTable()")
			local app = XLGetObject("SpyApp")
			local path = app:GetExeFolder()
			--XLMessageBox(path.."ggg.lua")
			dofile(path.."\\ggg.lua")
			--XLMessageBox(path.."ggg.lua")
			mapProcess[index].hostwnd = hostwnd
			--XLMessageBox(mapProcess[index].hostwnd[1].id)
			for i=1,#hostwnd do
				local itemhost = tree:InsertItemText(string.format("%s(%s)%d",hostwnd[i].id,hostwnd[i].class,hostwnd[i].index), item)
				itemhost.index = hostwnd[i].index
				mapitem[itemhost.index] = itemhost
				local objtree = hostwnd[i].objtree
				if objtree then
					local itemobj = tree:InsertItemText(string.format("%s(%s)%d",objtree.id,objtree.class,objtree.index), itemhost)
					itemobj.index = objtree.index
					mapitem[itemobj.index] = itemobj
					local root = objtree.root
					local itemroot = tree:InsertItemText(string.format("%s(%s)%d",root.id,root.class,root.index), itemobj)
					itemroot.index = root.index
					mapitem[itemroot.index] = itemroot
					itemroot.color = GetColorString(root.IsControl,root.class)
					local childcnt = AddTableChild(tree,itemroot, root.children,index)
					tree:SetItemText(itemroot, string.format("%s(%s)%d %d", root.id,root.class,root.index, childcnt))
					tree:SetItemText(itemobj, string.format("%s(%s)%d %d",objtree.id,objtree.class,objtree.index, childcnt))
					tree:SetExpand(itemhost,true)	
					tree:SetExpand(itemobj,true)	
					tree:SetExpand(itemroot,true)	
					itemhost.process = index
					itemobj.process = index
					itemroot.process = index
				end
				
			end
		end
	--end
	--tree:SetExpand(obj1,true)	
	tree:SetAdjust()
end

function GetColorString(iscontrol,class)
	local colorstring = "system.black"
	if class == "TextObject" then
		colorstring = "system.red"
	elseif class == "ImageObject" then
		colorstring = "system.green"
	elseif class == "EditObject" then
		colorstring = "system.navy"
	elseif class == "MaskObject" then
		colorstring = "system.indigo"
	elseif class == "FillObject" then
		colorstring = "system.violet"
	elseif class == "TextureObject" then
		colorstring = "system.blue"
	elseif class == "LayoutObject" then
		colorstring = "system.purple"
	end
	if iscontrol then
		colorstring = "system.orange"
	end
	return colorstring
end

function AddTableChild(tree,item, parent,proindex)
	local totalcnt = 0
	for i=1,#parent do
		local child = parent[i]
		local itemparent = tree:InsertItemText(string.format("%s(%s)%d", child.id,child.class,child.index),item)
		mapitem[child.index] = itemparent
		itemparent.process = proindex
		itemparent.index = child.index
		itemparent.color = GetColorString(child.IsControl,child.class)
		local childcnt = AddTableChild(tree,itemparent,child.children,proindex)
		totalcnt = totalcnt + childcnt
		--tree:SetExpand(itemparent,true)	
		tree:SetItemText(itemparent, string.format("%s(%s)%d %d", child.id,child.class,child.index, childcnt))
	end
	
	return #parent + totalcnt
end

function AddTree(tree, curid, parid, txt)
	local item
	if parid == 0 then
		item = tree:InsertItemText(txt)
	else
		item = tree:InsertItemText(txt,mapitem[parid])
		--mapitem[parid].expand = true
	end
	mapitem[curid] = item
end

function FillAttribute(list,tab)
	list:DeleteAllItems()
	
	--排序
	local tabkey = {}
	for k,v in pairs(tab) do
		table.insert(tabkey, k)
	end
	
	table.sort(tabkey)
	XLPrint("spy:-----------------------")
	for k,key in pairs(tabkey) do
		XLPrint("spy:"..key.." "..tostring(tab[key]))
		list:InsertItem(key,tostring(tab[key]))
	end
	
	list:UpdateScroll()
end

function UpdatePath(edit,item)
	local tmp = ""
	while item.level >= 3 do
		local txt = item.txt
		local id = string.match(txt,"^([^\(]*)")
		tmp = string.format("%s\\%s",tostring(id),tmp)
		item = item.parent
	end
	edit:SetText(tmp)
end

function GetAttrAndBmp(itemattr,preview,list,edit)
	local app = XLGetObject("SpyApp")
	local path = app:GetExeFolder()
	if itemattr.level == 1 then--本地XLUE进程
		
	elseif itemattr.level == 2 then--进程
		
	elseif itemattr.level == 3 then--hostwnd
		if itemattr.process and mapProcess[itemattr.process] and mapProcess[itemattr.process].msg then
			local msg = mapProcess[itemattr.process].msg
			msg:DoString(string.format("GetSingleHostWndByIndex(%d)",itemattr.index))
			--dofile(path.."ggg.lua")
			dofile(path.."\\ggg.lua")
			if list then FillAttribute(list,tmp) end
			if edit then UpdatePath(edit,itemattr) end
		end
	elseif itemattr.level == 4 then--objtree
		local msg = mapProcess[itemattr.process].msg
		msg:DoString(string.format("GetSingleObjTreeByIndex(%d)",itemattr.index))
		--dofile(path.."ggg.lua")
		dofile(path.."\\ggg.lua")
		if list then FillAttribute(list,tmp) end
		if edit then UpdatePath(edit,itemattr) end
	else
		local msg = mapProcess[itemattr.process].msg
		msg:DoString(string.format("GetSingleLayoutByIndex(%d)",itemattr.index))
		--dofile(path.."ggg.lua")
		dofile(path.."\\ggg.lua")
		if list then FillAttribute(list,tmp) end
		GetBmp(msg, tmp, itemattr.index,preview)
		if edit then UpdatePath(edit,itemattr) end
	end
end

function OnSelectChanged(self, fun, item)
	_G["tmp"] = {}
	local app = XLGetObject("SpyApp")
	local path = app:GetExeFolder()
	local attr = item:GetAttribute()
	local owner = self:GetOwnerControl()
	local list = owner:GetControlObject("list")
	local edit = owner:GetControlObject("Main.Edit.path")
	local preview = owner:GetControlObject("preview")	
	GetAttrAndBmp(attr.itemattr,preview,list,edit)
	
	if attr.itemattr.level > 2 then
		local edit1 = owner:GetControlObject("Main.Edit.String")
		edit1:SetText(string.format("allobjmap[%d]:", attr.itemattr.index))
	end
end

function GetBmp(msg,attr,index)
	if not enablepreview then return end
	local app = XLGetObject("SpyApp")
	local path = app:GetExeFolder()
	
	--应该加判断,某些不可见的object不取bmp
	local notseeobj = {"ResizeObject", "FlashObject","UIObjectTree"}
	for k,v in pairs(notseeobj) do
		if v == attr.Class then  return end
	end
	if attr.Pos then
		local left,top,right,bottom = string.match(attr.Pos, "(-?%d+),(-?%d+),(-?%d+),(-?%d+)")
		left = tonumber(left)
		top = tonumber(top)
		right = tonumber(right)
		bottom = tonumber(bottom)
		if right - left == 0 or bottom - top == 0 then return end
		msg:DoString(string.format("GetObjBitmap(%d)",index))
		--dofile(path.."ggg.lua")
		dofile(path.."\\ggg.lua")
		if tmp.width and tmp.height then
			if tmp.width > 0 and tmp.height > 0 then
				local bmpmemory = XLGetObject("BmpMemory")
				local bmp = bmpmemory:GetBitmap(tmp.width,tmp.height)
				local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
				local hostwnd = hostwndManager:GetHostWnd("Spy.ShareBmpFrame")
				local objtree = hostwnd:GetBindUIObjectTree()
				local preview = objtree:GetUIObject("Preview")
				preview:SetObjPos(0,0,tmp.width,tmp.height)
				preview:SetBitmap(bmp)
			end
		end
	end
end

enablehittest = true

function OnInitControl(self)
	local owner = self:GetOwner()
	local bkg = owner:GetUIObject("mainwnd.bkg")
	local app = XLGetObject("SpyApp")
	local path = app:GetExeFolder()
	function OnHotKey(wparam)
		if wparam == 2 then
			enablehittest = not enablehittest
		elseif wparam == 3 then
			Refresh(bkg)
		elseif wparam == 4 then
			DoString(bkg)
		end
	end
	app:AddHotkeyCallBack(OnHotKey)
	
	local owner = self:GetOwner()
	local bkg = owner:GetUIObject("mainwnd.bkg")
	local tree = bkg:GetControlObject("tree")
	local preview = bkg:GetControlObject("preview")
	local list = bkg:GetControlObject("list")
	local edit = bkg:GetControlObject("Main.Edit.path")
	local edit1 = bkg:GetControlObject("Main.Edit.String")
	edit1:SetText("XLMessageBox('haha')")
	
	function OnTimer()
		if enablehittest then
			local isue,pid,hwnd,x,y = app:CheckUE()
			if isue then
				for i=1,#mapProcess do
					if mapProcess[i].pid == pid then
						mapProcess[i].msg:SendLightData("GHWND",hwnd)
						mapProcess[i].msg:DoString(string.format("NewHitTest(%d,%d)",x,y))
						--dofile(path.."ggg.lua")
						dofile(path.."\\ggg.lua")
						local item = mapitem[tmp.index]
						-- XLPrint(string.format("Find item = %s, by index=%d", tostring(item), tmp.index))
						local index = tmp.index
						--if not item then Refresh(bkg) end
						if item and item ~= tree:GetSelected() then
							tree:SetSelected(item)
							tree:EnsureVisible(item,true) 	
							mapProcess[i].msg:DoString(string.format("GetSingleLayoutByIndex(%d)",index))
							--dofile(path.."ggg.lua")
							dofile(path.."\\ggg.lua")
							FillAttribute(list,tmp)
							GetBmp(mapProcess[i].msg, tmp, index,preview)	
							UpdatePath(edit,item)
						end
					end
				end
			end
		end
	end
	
	local timermanage = XLGetObject("Xunlei.UIEngine.TimerManager")
	timermanage:SetTimer(OnTimer, 500)
end

function Split(szFullString, szSeparator)
	local nFindStartIndex = 1
	local nSplitIndex = 1
	local nSplitArray = {}
	while true do
	   local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)
	   if not nFindLastIndex then
			nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))
			break
	   end
	   nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)
	   nFindStartIndex = nFindLastIndex + string.len(szSeparator)
	   nSplitIndex = nSplitIndex + 1
	end
	for i=#nSplitArray,1,-1 do
		if string.len(nSplitArray[i]) == 0 then
			table.remove(nSplitArray,i)
		end
	end
	return nSplitArray
end

function FindHostByTxt(txt)
	for i=1,#mapProcess do
		for j=1,#mapProcess[i].hostwnd do
			if string.match(mapProcess[i].hostwnd[j].id, txt) then
				return mapProcess[i].hostwnd[j]
			end
		end
	end
end

function FindItemByTxt(txt,parent)
	if parent.objtree then return parent.objtree end
	if parent.root then return parent.root end
	for i=1,#parent.children do
		if string.match(parent.children[i].id, txt) then
			return parent.children[i]
		end
	end
end

function OnEditKeyDown(self, func, uChar, uRepeatCount, uFlags)
	if uChar == 13 then
		local control = self:GetOwnerControl()
		local tree = control:GetControlObject("tree")
		local list = control:GetControlObject("list")
		local preview = control:GetControlObject("preview")
		local txt = self:GetText()
		local array = Split(txt,"\\")
		if #array > 0 then
			local item = FindHostByTxt(array[1])
			for i=2,#array do
				if item then
					item = FindItemByTxt(array[i],item)
				else
					XLMessageBox("找不到节点"..array[i-1])
					break
				end
			end
			if not item then
				XLMessageBox("找不到节点"..array[#array])
				return
			end
			if mapitem[item.index] then
				tree:SetSelected(mapitem[item.index])
				tree:SetExpand(mapitem[item.index],true)
				tree:EnsureVisible(mapitem[item.index],true) 
			else
				XLMessageBox("找不到对应树节点")
			end
			
			GetAttrAndBmp(mapitem[item.index],preview,list,self)
		end
	elseif uChar == 17 then --Ctrl 后退一个节点
		local txt = self:GetText()
		local array = Split(txt,"\\")
		if #array > 0 then
			table.remove(array)
			txt = table.concat(array,"\\")
			self:SetText(txt)
			OnEditKeyDown(self,func,13,uRepeatCount, uFlags)
		end
	end
end

function DoString(control)
	local tree = control:GetControlObject("tree")
	local list = control:GetControlObject("list")
	local edit = control:GetControlObject("Main.Edit.String")
	local item = tree:GetSelected()
	if item then
		if item.process then
			mapProcess[item.process].msg:DoString(edit:GetText())
			GetAttrAndBmp(tree:GetSelected(),preview,list)
		else
			XLMessageBox("请选中进程,或进程下面的任意节点")
		end
	else
		XLMessageBox("请选中进程,或进程下面的任意节点")
	end
end

function DoStringBtnOnClick(self)
	local control = self:GetOwnerControl()
	DoString(control)
end

function TransKeyToSetKey(key, value)
	if key == "Alpha" then
		return string.format("SetAlpha(%d)",value)
	elseif key == "Visible" then
		return string.format("SetVisible(%s)",value)
	elseif key == "LimitChild" then
		return string.format("SetLimitChild(%s)",value)
	elseif key == "TabOrder" then
		return string.format("SetTabOrder(%s)",value)
	elseif key == "ZOrder" then
		return string.format("SetZOrder(%s)",value)
	elseif key == "ChildVisible" then
		return string.format("SetChildrenVisible(%s)",value)
	elseif key == "Pos" then
		return string.format("SetObjPos(%s)",value)
	elseif key == "ResId" then
		return string.format("SetResID(%s)",value)		
	elseif key == "WordEllipsis" then
		return string.format("SetWordEllipsis(%s)",value)		
	elseif key == "HAlign" then
		return string.format("SetHAlign(%s)",value)		
	elseif key == "Multiline" then
		return string.format("SetMultiline(%s)",value)	
	elseif key == "Text" then
		return string.format("SetText(%q)",value)	
	elseif key == "VAlign" then
		return string.format("SetVAlign(%s)",value)	
	elseif key == "EndEllipsis" then
		return string.format("SetEndEllipsis(%s)",value)	
	elseif key == "Caption" then
		return string.format("SetCaption(%s)",value)	
	end
	return ""
end

function List_OnSelectChanged(self)
	local item = self:GetSel()
	local attr = item:GetAttribute()
	local control = self:GetOwnerControl()
	local edit = control:GetControlObject("Main.Edit.String")
	local txt = edit:GetText()
	local txt1 = string.match(txt,"^allobjmap%[%d+%]:")
	if txt1 then
		edit:SetText(txt1..TransKeyToSetKey(attr.key,attr.value))
	end
end

function OnCreate(self)
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local frameHostWndTemplate = templateMananger:GetTemplate("Spy.ShareBmpFrame","HostWndTemplate")
	if frameHostWndTemplate then  
		local frameHostWnd = frameHostWndTemplate:CreateInstance("Spy.ShareBmpFrame")
		if frameHostWnd then
			local objectTreeTemplate = templateMananger:GetTemplate("SpyShareBmpTree","ObjectTreeTemplate")
			if objectTreeTemplate then
				local uiObjectTree = objectTreeTemplate:CreateInstance("Spy.ShareBmpObjectTree")
				if uiObjectTree then
					frameHostWnd:BindUIObjectTree(uiObjectTree)
					frameHostWnd:Create()
					frameHostWnd:SetVisible(false)
				end
			end
		end
	end
end

enablepreview = false

function PreviewOnCheck(self,check1, check2)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Spy.ShareBmpFrame")
	hostwnd:SetVisible(check2)
	hostwnd:BringWindowToTop(check2)
	enablepreview = check2
end