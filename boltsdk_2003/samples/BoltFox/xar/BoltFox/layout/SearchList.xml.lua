function SetText(self, txt)
	local text = self:GetControlObject("text")
	text:SetText(txt)
end

function OnMouseEnter(self)
	local bkg = self:GetControlObject("bkg")
	bkg:SetTextureID("pluginitem.all.hover")
end

function OnMouseLeave(self)
	local bkg = self:GetControlObject("bkg")
	-- bkg:SetTextureID("pluginitem.all.down")
	bkg:SetTextureID("")
end

function OnLButtonUp(self)
	self:FireExtEvent("OnClick")
end

function GetText(self)
	local text = self:GetControlObject("text")
	return text:GetText()
end

function AddString(self, text, icon, update)
	local attr = self:GetAttribute()
	table.insert(attr.data, {text = text, icon = icon})
	
	if update then
		self:UpdateUI()
	end
end

function RemoveString(self, text, update)
	local attr = self:GetAttribute()
	for i=1, #attr.data do
		if attr.data[i].text == text then
			table.remove(attr.data, i)
			break
		end
	end
	
	if update then
		self:UpdateUI()
	end
end

function UpdateUI(self)
	local attr = self:GetAttribute()
	local container = self:GetControlObject("container")
	local control = self
	
	local function OnItemClick(self)
		XLPrint("OnItemClick(self) "..self:GetText())
		control:FireExtEvent("OnSelectChange", self:GetText())
	end
	
	-- 仅创建需要的....多余的不管..
	if #attr.data > container:GetChildCount() then
		local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
		local xarFactory = xarManager:GetXARFactory()
		
		for i=1, #attr.data - container:GetChildCount() do
			local obj = xarFactory:CreateUIObject("item."..container:GetChildCount(), "BoltFox.SearchListItem.Ctrl")
			container:AddChild(obj)
			
			obj:AttachListener("OnClick", false, OnItemClick)
		end
	end
	
	local height = 22
	
	for i=1, #attr.data do
		local obj = container:GetChildByIndex(i-1)
		obj:SetText(attr.data[i].text)
		obj:SetIcon(attr.data[i].icon)
		obj:SetObjPos(0, (i-1)*height, "father.width", i*height)
	end
	
	
end

function OnInitControl(self)
	local attr = self:GetAttribute()
	attr.data = {}
end

function SetIcon(self, ico)
	local icon = self:GetControlObject("icon")
	icon:SetResID(ico)
end

function GetIcon(self)
	local icon = self:GetControlObject("icon")
	return icon:GetResID()
end