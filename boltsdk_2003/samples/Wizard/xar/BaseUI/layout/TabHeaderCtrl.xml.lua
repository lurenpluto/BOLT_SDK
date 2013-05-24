

function SetButtonActive(self, button, active)
	local attr = self:GetAttribute()
	-- if active then
		-- button:SetNormalBkgTexture(attr.ActiveBtnBkgNormal)
		-- button:SetHoverBkgTexture(attr.ActiveBtnBkgHover)
		-- button:SetDownBkgTexture(attr.ActiveBtnBkgDown)
		-- button:SetDisableBkgTexture(attr.ActiveBtnBkgDisable)
		-- button:SetTextColorID("color.text.normal")
	-- else
		-- button:SetNormalBkgTexture(attr.BtnBkgNormal)
		-- button:SetHoverBkgTexture(attr.BtnBkgHover)
		-- button:SetDownBkgTexture(attr.BtnBkgDown)
		-- button:SetDisableBkgTexture(attr.BtnBkgDisable)
		-- button:SetTextColorID("color.text.description")
	-- end
end


--	active:bool 是否激活
function AddTabItem(self, id, text, icon, active)
	local objExist = self:GetControlObject(id)
	if objExist ~= nil then
		return
	end
	local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	local objFactory = xarManager:GetXARFactory()

	local attr = self:GetAttribute()

	local bkgObj = self:GetControlObject("bkg")
	local btn = objFactory:CreateUIObject(id, "BaseUI.Button", "BaseUI")
	
	local btnAttr = btn:GetAttribute()
	-- btnAttr.IconLeftPos = attr.IconLeftPos
	-- btnAttr.IconTopPos = attr.IconTopPos
	-- btnAttr.TextLeftPos = attr.TextLeftPos
	-- btnAttr.TextTopPos = attr.TextTopPos
	-- btnAttr.IconSize = attr.IconSize
	-- btnAttr.IconBitmapID = icon


	-- btnAttr.TextFontID = attr.TextFontID
	-- btnAttr.TextColorID = attr.TextColorID
	
	btn:SetText(text)	
	btn:UpdateUI()
	local pos = 0
	local btnCount = bkgObj:GetChildCount()
	if btnCount > 0 then
		local lastBtn = bkgObj:GetChildByIndex(btnCount - 1)
		local left, top, right, bottom = lastBtn:GetObjPos()
		pos = right + attr.ButtonInternalSpace
	end

	local function OnButtonClick(btn)
		local id = btn:GetID()
		if id == attr.CurrentActiveTab then
			return
		end
		self:SetActiveTab(id, true)
	end
	btn:AttachListener("OnClick", true, OnButtonClick)
	bkgObj:AddChild(btn)

	btn:SetObjPos(pos, 0, pos + attr.ButtonWidth, 0 + attr.ButtonHeight)

	if active == true or
		(active == nil and attr.CurrentActiveTab == nil) then
		self:SetActiveTab(id, true)
	else
		SetButtonActive(self, btn, false)
	end
end



function RemoveTabItem(self, remove_id, active_id)
	local btn = self:GetControlObject(remove_id)
	if btn == nil then
		return
	end
	local bkgObj = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	local pos = 0
	local count = bkgObj:GetChildCount()
	for i = 0, count - 1 do
		local child = bkgObj:GetChildByIndex(i)
		local childID = child:GetID()
		if childID ~= remove_id then
			local childLeft, childTop, childRight, childBottom = child:GetObjPos()
			child:SetObjPos(pos, childTop, pos + attr.ButtonWidth, childBottom)
			pos = pos + attr.ButtonWidth + attr.ButtonInternalSpace
		end
	end
	bkgObj:RemoveChild(btn)
end





function SetActiveTab(self, tabID, fireEvent)
	local attr = self:GetAttribute()
	if attr.CurrentActiveTab == tabID then
		return
	end

	local btn = self:GetControlObject(tabID)
	if btn == nil then
		return
	end

	SetButtonActive(self, btn, true)
	
	local pre_active = attr.CurrentActiveTab
	if pre_active ~= nil then
		local btn = self:GetControlObject(pre_active)
		if btn == nil then
			return
		end
		SetButtonActive(self, btn, false)
	end
	
	attr.CurrentActiveTab = tabID
	if fireEvent then
		self:FireExtEvent("OnActiveTabChanged", tabID, pre_active)
	end
end


function OnBind(self)
end