function OnMenuItemInitControl(self)
	local attr = self:GetAttribute()
	
	local textObj = self:GetControlObject("menuitem.text")
	textObj:SetText(attr.Text)
end

function OnMenuItemControlMouseEnter(self)
	local bkg = self:GetControlObject("menuitem.bkg")
	bkg:SetVisible(true)
end

function OnMenuItemControlMouseLeave(self)
	local bkg = self:GetControlObject("menuitem.bkg")
	bkg:SetVisible(false)
end

function OnMenuItemLButtonUp(self)
	self:FireExtEvent("OnClick")
	self:GetOwner():GetBindHostWnd():EndMenu()
end

function OnTransprentClick(self)
	local app = XLGetObject("Wizard.App")
	
	local opaque = app:GetInt("ConfigGraphics", "ConfigGraphics_MainWndOpaque", 0)
	if opaque == 0 then
		opaque = 1
	else
		opaque = 0
	end
	
	app:SetInt("ConfigGraphics", "ConfigGraphics_MainWndOpaque", opaque)
end

function OnBlurClick(self)
	local app = XLGetObject("Wizard.App")
	
	local blur = app:GetInt("ConfigGraphics", "ConfigGraphics_EnableBlur", 0)
	if blur == 0 then
		blur = 1
	else
		blur = 0
	end
	
	app:SetInt("ConfigGraphics", "ConfigGraphics_EnableBlur", blur)
	
end

function OnAlphaLButtonDown(self, x, y)
	local left, top, right, bottom = self:GetObjPos()
	local alpha = x/(right - left) * 255
	if alpha < 5 then
		alpha = 5
	elseif alpha > 255 then
		alpha = 255
	end
	
	local app = XLGetObject("Wizard.App")
	app:SetInt("ConfigGraphics", "ConfigGraphics_MainWndAlpha", alpha)
end

function OnPopupMenu(self)
end