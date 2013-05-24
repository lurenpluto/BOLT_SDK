function OnLButtonDown(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetTextureID(attr.DownBkg or "")
end

function OnLButtonUp(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetTextureID(attr.NormalBkg or "")
	
	self:FireExtEvent("OnClick")
end

function OnMouseEnter(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetTextureID(attr.HoverBkg or "")
end

function OnInitControl(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetTextureID(attr.NormalBkg or "")
end

function OnMouseLeave(self)
	local bkg = self:GetControlObject("bkg")
	local attr = self:GetAttribute()
	
	bkg:SetTextureID(attr.NormalBkg or "")
end