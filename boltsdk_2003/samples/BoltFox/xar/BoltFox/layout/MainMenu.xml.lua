function CreateMenuItemContainer(self,...)
	local itemContainer = {}
	itemContainer.items = {...}
	itemContainer.hoveringItem = nil
	itemContainer.enteredItem = nil
	
	itemContainer.ChangeEnteredItem = function(self2,enteredItem)
		if self2.enteredItem then
			self2.enteredItem:SetEntered(false)
		end
		if enteredItem then
			enteredItem:SetEntered(true)
		end
		self2.enteredItem = enteredItem
	end
	itemContainer.GetHoveringItem = function(self2)
		return self2.hoveringItem
	end
	itemContainer.ChangeHoveringItem = function(self2,hoveringItem)
		if hoveringItem and self2.hoveringItem then
			if hoveringItem:GetID() == self2.hoveringItem:GetID() then
				return 
			end
		end
		if hoveringItem then
			if not hoveringItem:PopupSubMenu() then
				hoveringItem = nil
			end
		end
		if self2.hoveringItem then
			self2.hoveringItem:DestroySubMenu()
		end
		self2.hoveringItem = hoveringItem
	end
	return itemContainer
end


function MainMenu_Item_OnInitControl(self)
	local attr = self:GetAttribute()
	self:SetText(attr.Text)
	if attr.SubMenuTemplate then
		local tosubObj = self:GetControlObject("tosub")
		tosubObj:SetVisible(true)
	end
end

function MainMenu_Item_OnLButtonDown(self)
	local attr = self:GetAttribute() 
	if not attr.SubMenuTemplate then
		self:FireExtEvent("OnSelected")
		self:GetOwner():GetBindHostWnd():EndMenu()
	end
end

function MainMenu_Item_SetEntered(self,entered)
	local hoverBkg = self:GetControlObject("hoverBkg")
	hoverBkg:SetVisible(entered)
end

function MainMenu_Item_OnMouseLeave(self)
	local attr = self:GetAttribute()
	attr.mouseHovering = false
	local hoveringItem = attr.itemContainer:GetHoveringItem()
	if hoveringItem and hoveringItem:GetID() == self:GetID() then
		return 
	end
	self:SetEntered(false)
end


function MainMenu_Item_OnMouseEnter(self)
	local attr = self:GetAttribute() 
	attr.itemContainer:ChangeEnteredItem(self)
	attr.mouseHovering = true
	SetOnceTimer(function()
		if attr.mouseHovering then
			attr.itemContainer:ChangeHoveringItem(self)
		end
	end, 500)
end

function MainMenu_Item_SetContainer(self,itemContainer)
	local attr = self:GetAttribute()
	attr.itemContainer = itemContainer
end

function MainMenu_Item_PopupSubMenu(self)
	local attr = self:GetAttribute()
	if attr.SubMenuTemplate then
		local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")		
		local subMenuTemplate = templateMananger:GetTemplate(attr.SubMenuTemplate,"ObjectTemplate")
		local controlId = self:GetID()
		if not attr.subMenuObj then
			local subMenuObj = subMenuTemplate:CreateInstance(controlId.."."..attr.SubMenuTemplate)
			if subMenuObj then
				self:AddChild(subMenuObj)
				subMenuObj:SetZorder(500)
				local left, top ,right , bottom = subMenuObj:GetObjPos()
				subMenuObj:SetObjPos("father.width-5", "5", right - left, bottom - top)
				attr.subMenuObj = subMenuObj
			end
		end
		return true
	end
	return false
end

function MainMenu_Item_DestroySubMenu(self)
	local attr = self:GetAttribute()
	if attr.SubMenuTemplate then
		local controlId = self:GetID()
		if attr.subMenuObj then
			self:RemoveChild(attr.subMenuObj)
			attr.subMenuObj  = nil
		end
		return true
	end
	return false
end

function MainMenu_Item_SetText(self,text)
	local attr = self:GetAttribute()
	attr.Text = text
	local textObj = self:GetControlObject("text")
	textObj:SetText(attr.Text)
end

function MainMenu_Item_GetText(self)
	local attr = self:GetAttribute()
	return attr.Text
end

function MainMenu_SubMenu_RecentUrls_Item_OnSelected(self)
	local url = self:GetText()
	---- open url here -----
	XLGetGlobal("BoltFox.UIHelper"):NavigateToUrl(url, true)
end

function MainMenu_SubMenu_RecentUrls_OnBind(self)
	local history = XLGetGlobal("BoltFox.History")
	local recentUrls = history:GetRecentUrls()
	--recentUrls = {"xunlei.com", "163.com"}
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")	
	local bkgObj = self:GetChildByIndex(0)
	local itemTop = 0
	local urlCnt = table.maxn(recentUrls)
	local blankItem = bkgObj:GetChildByIndex(0)
	local itemContainer = CreateMenuItemContainer()
	if urlCnt ~= 0 then
		bkgObj:RemoveChild(blankItem)
		local itemTemplate = templateMananger:GetTemplate("BoltFox.MainMenu.SubMenu.RecentUrls.Item","ObjectTemplate")
		XLLog("renct urls has "..urlCnt)
		for i=1, urlCnt do
			local itemObj = itemTemplate:CreateInstance("RecentUrls.item."..i)
			itemObj:SetContainer(itemContainer)
			itemTop = 21 * (i - 1)
			itemObj:SetObjPos("0", tostring(itemTop) , "father.width", tostring(itemTop + 20))
			itemObj:SetText(recentUrls[i])
			bkgObj:AddChild(itemObj)
		end
	else
		blankItem:SetContainer(itemContainer)
	end
	itemTop = itemTop + 21
	bkgObj:SetObjPos("0","0","father.width",tostring(itemTop))
end

function MainMenu_OnInitControl(self)
	local menuItem1 = self:GetOwner():GetUIObject("item.RecentUrls")
	local menuItem2 = self:GetOwner():GetUIObject("item.Quit")
	local itemContainer = CreateMenuItemContainer(menuItem1, menuItem2)
	menuItem1:SetContainer(itemContainer)
	menuItem2:SetContainer(itemContainer)
end

function MainMenu_item_Quit_OnSelected(self)
	local uiHelper = XLGetGlobal("BoltFox.UIHelper")
	uiHelper:QuitApp()
end

function OnPopupMenu()

end