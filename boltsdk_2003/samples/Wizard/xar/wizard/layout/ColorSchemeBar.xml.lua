function SetEnable(self, enable)
	local attr = self:GetAttribute()
	
	if attr.Enable == enable then
		return
	end
	
	attr.Enable = enable
end

function GetEnable(self)
	local attr = self:GetAttribute()
	
	return attr.Enable
end

function SetSelect(self,select)
	local attr = self:GetAttribute()
	self:FireExtEvent("OnSelect")
end

function OnInitControl(self)
	local attr = self:GetAttribute()
	local colorObj = self:GetControlObject("color")
	colorObj:SetMonochromeColor(attr.Color)
end

function OnColorSchemeLButtonUp(self)
	self:SetSelect(true)
end
