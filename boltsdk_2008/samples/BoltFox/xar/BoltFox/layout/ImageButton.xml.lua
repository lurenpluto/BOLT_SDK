function OnLButtonDown(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetResID(attr.DownBkg)
end

function OnLButtonUp(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetResID(attr.NormalBkg)
	
	self:FireExtEvent("OnClick")
end

function OnMouseEnter(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetResID(attr.HoverBkg)
end

function OnInitControl(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetResID(attr.NormalBkg)
end

function OnMouseLeave(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetResID(attr.NormalBkg)
end