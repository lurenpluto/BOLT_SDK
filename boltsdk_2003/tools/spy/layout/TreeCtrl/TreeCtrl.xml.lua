function InsertItem(self,item, parent)
	table.insert(parent.children, item)
	return item
end

function InsertItemText(self,txt,parent)
	if parent == nil then
		local attr = self:GetAttribute()
		parent = attr.root
	end
	item = {}
	item.txt = txt
	item.children = {}
	item.parent = parent
	item.level = parent.level + 1
	item.visible = false	
	item.left = 0
	item.top = 0
	item.expand = false
	item.select = false
	item.tree = self
	item.color = "system.black"
	return self:InsertItem(item, parent)
end

function OnBind(self)
	local attr = self:GetAttribute()
	local left,top,right,bottom = self:GetObjPos()
	m_itemheight = 22
	attr.root = {}
	attr.root.children = {}
	attr.root.level = 0
	attr.m_top = 1
	attr.m_left = 0
	attr.VisibleItems = {}
	attr.maxwidth = 0
end

function RecAdjust(item, left)
	local tree = item.tree
	local attr = tree:GetAttribute()
	mapTopItem[attr.m_top] = item
	item.left = left
	item.top = attr.m_top
	attr.m_top = attr.m_top + 1
	testTextLength:SetText(item.txt)
	attr.maxwidth = math.max(attr.maxwidth, left * 12 + testTextLength:GetTextExtent())
	if item.expand then
		for i=1,#item.children do
			RecAdjust(item.children[i],left+1)
		end
	end
end

function SetAdjust(self,bVscroll)
	local attr = self:GetAttribute()
	local root = attr.root
	mapTopItem = {}
	attr.mapTopItem = mapTopItem
	--attr.maxwidth = 0
	
	if attr.m_top > 1 then attr.m_top = 1 end
	local tmp = attr.m_top
	
	for i=1,#root.children do
		RecAdjust(root.children[i], -attr.m_left)
	end
	
	for i=1,ItemVisibleCount do
		attr.VisibleItems[i]:BindAttr(mapTopItem[i])
	end
	
	totalVisible = attr.m_top - tmp
	attr.m_top = tmp
	--self:SetVScrollAdjust(bVscroll)
	--self:SetHScrollAdjust()
	--XLMessageBox(""..attr.maxwidth)
	
	if not bVscroll then
		self:UpdateVScroll()
		local vscroll = self:GetControlObject("treectrl.vscroll")
		vscroll:SetScrollPos(-(attr.m_top-1)*22, true)
	end
end

function SetHScrollAdjust(self)
	local attr = self:GetAttribute()
	local content = self:GetControlObject("Item.Cotent")
	local left,top,right,bottom = content:GetObjPos()
	local width = right - left
	local hscroll = self:GetControlObject("treectrl.hscroll")
	if width >= attr.maxwidth then
		hscroll:Show(false)
	else
		hscroll:Show(true)
	end
end

function GetRootItem(self)
	local attr = self:GetAttribute()
	return attr.root
end

function GetChildrenItemCount(self, item)
	return #item.children
end

function GetChildItemByIndex(self,item,index)
	return item.children[index]
end

function OnInitControl(self)
	local attr = self:GetAttribute()
	local content = self:GetControlObject("Item.Cotent")
	local left,top,right,bottom = self:GetObjPos()
	local height = bottom - top
	ItemVisibleCount = math.ceil(height / m_itemheight)
	attr.ItemVisibleCount = ItemVisibleCount
	for i=1,ItemVisibleCount do
		local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
		local obj = objFactory:CreateUIObject("item."..i, "TreeItem")
		content:AddChild(obj)
		table.insert(attr.VisibleItems, obj)
	end	
	local vscroll = self:GetControlObject("treectrl.vscroll")
	--local hscroll = self:GetControlObject("treectrl.hscroll")
	vscroll:Show(false)
	--hscroll:Show(false)
	
	testTextLength = self:GetControlObject("ItemTestText")
end

function GetSelected(self)
	return self:GetAttribute().select
end

function SetSelected(self,item,update)
	local attr = self:GetAttribute()
	if attr.select == item then return end
	if attr.select then
		attr.select.select = false
	end
	item.select = true
	attr.select = item
	if update then self:SetAdjust() end
end

function RemoveItem(self, item)
	--XLMessageBox(string.format("RemoveItem item.txt=%s",item.txt))
	if item.select then self:GetAttribute().select = nil end
	for i=1,#item.parent.children do
		if item.parent.children[i] == item then
			table.remove(item.parent.children,i)
			break
		end
	end
end

function TreeCtrl_OnVScroll(self, fun, type, newpos)
	local owner = self:GetOwnerControl()
	local attr = owner:GetAttribute()
	if type == 4 then
		attr.m_top = math.ceil(-newpos / 22)
		owner:SetAdjust(true)
	end
end

function SetText(self,value)
	local control = self:GetOwnerControl()
	local txt = control:GetControlObject("ItemText")
	txt:SetText(value)
end

function BindAttr(self, itemattr)
	if itemattr == nil then 
		self:SetObjPos(0,0,0,0)
		return
	end
	itemattr.obj = self
	local tree = self:GetOwnerControl()
	local content = tree:GetControlObject("Item.Cotent")
	local left,top,right,bottom = content:GetObjPos()
	local width = right - left
	local attr = self:GetAttribute()
	self:SetObjPos(itemattr.left*12,itemattr.top*m_itemheight-m_itemheight,width-12,itemattr.top*m_itemheight)
	
	local txt = self:GetControlObject("ItemText")
	txt:SetText(itemattr.txt)
	txt:SetTextColorResID(itemattr.color)
	local len = txt:GetTextExtent()
	
	local expand = self:GetControlObject("Expand")
	if #itemattr.children > 0 then
		expand:SetVisible(true)
		if itemattr.expand then
			expand:SetResID("bitmap.categorytree.expand")
		else
			expand:SetResID("bitmap.categorytree.collapse")
		end
	else
		expand:SetVisible(false)
	end
	
	local select = self:GetControlObject("ItemSelect")
	select:SetVisible(itemattr.select)
	local left,top,right,bottom = select:GetObjPos()
	select:SetObjPos(left,top,len+left,bottom)
	
	attr.itemattr = itemattr
end

function Item_OnLButtonDown(self,x,y)
	local tree = self:GetOwnerControl()
	local attr = self:GetAttribute()
	local itemattr = attr.itemattr
	if x>=0 and x<=15 and y>=0 and y<=22 then
		itemattr.expand = not itemattr.expand
	else
		tree:SetSelected(itemattr)
		tree:FireExtEvent("OnSelectChanged", itemattr.obj)
	end
	tree:SetAdjust()
end

function UpdateVScroll(self)
	local content = self:GetControlObject("Item.Cotent")
	local left,top,right,bottom = content:GetObjPos()
	local height = bottom - top
	local vscroll = self:GetControlObject("treectrl.vscroll")
	vscroll:SetPageSize(height)

	local range = totalVisible*22 - height
	if range < 0 then
		vscroll:SetVisible(false)
		vscroll:SetChildrenVisible(false)
		range = 0
	else
		vscroll:SetVisible(true)
		vscroll:SetChildrenVisible(true)
	end
	vscroll:SetScrollRange(0, range)
end

function Item_OnLButtonDbClick(self)
	local tree = self:GetOwnerControl()
	local attr = self:GetAttribute()
	local itemattr = attr.itemattr
	itemattr.expand = not itemattr.expand
	tree:SetAdjust()
end

function MouseWheel(self, distance)
	local attr = self:GetAttribute()
	local step = 1
	if distance < 0 then
		if #mapTopItem >= ItemVisibleCount then  
			attr.m_top = attr.m_top - step
		end
	else
		attr.m_top = attr.m_top + step
	end
	self:SetAdjust()
end

function OnMouseWheel(self, x, y, distance)	
	local tree = self:GetOwnerControl()
	tree:MouseWheel(distance)
end

function SetExpand(self, item, expand,update)
	item.expand = expand
	if update then self:SetAdjust() end
end

function TreeCtrl_OnHScroll(self, fun, type, pos)
	if m_hpos then
		if m_hpos == pos then return end
	end
	m_hpos = pos
		
	local tree = self:GetOwnerControl()
	local attr = tree:GetAttribute()
	local content = tree:GetControlObject("Item.Cotent")
	local left,top,right,bottom = content:GetObjPos()
	local width = right - left
	m_left = pos / 100 * (attr.maxwidth + 30 - width) / 12
	if attr.m_left == m_left then return end
	attr.m_left = m_left
	--XLPrint(string.format("XLSpy:type=%d,pos=%d,m_left=%d,attr.maxwidth-width=%d", type,pos,attr.m_left,attr.maxwidth-width))
	tree:SetAdjust()
end

function OnPosChange(self)
	local attr = self:GetAttribute()
	local content = self:GetControlObject("Item.Cotent")
	for i=1,#attr.VisibleItems do
		content:RemoveChild(attr.VisibleItems[i])
	end
	attr.VisibleItems = {}
	local left,top,right,bottom = content:GetObjPos()
	local height = bottom - top
	ItemVisibleCount = math.ceil(height / m_itemheight)
	attr.ItemVisibleCount = ItemVisibleCount

	for i=1,ItemVisibleCount do
		local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
		local obj = objFactory:CreateUIObject("item."..i, "TreeItem")
		content:AddChild(obj)
		table.insert(attr.VisibleItems, obj)
	end	
	self:SetAdjust()
end

function EnsureVisible(self,item,update)
	local attr = self:GetAttribute()
	--if #attr.VisibleItems < math.abs(attr.m_top) then
		attr.m_top = attr.m_top - item.top + 1
		while item.parent do
			item = item.parent
			item.expand = true
		end
	--end
	if update then 
		self:SetAdjust() 
	end
end

function Tree_OnMouseWheel(self,x,y,distance)
	self:MouseWheel(distance)
end

function OnScrollBarMouseWheel(self,func,x,y,distance)
	self:GetOwnerControl():MouseWheel(distance)
end

function OnLButtonDbClick(self)
	local tree = self:GetOwnerControl()
	local attr = self:GetAttribute()
	local itemattr = attr.itemattr
	itemattr.expand = not itemattr.expand
	tree:SetAdjust()
end

function SetItemText(self, item, txt)
	item.txt = txt
end