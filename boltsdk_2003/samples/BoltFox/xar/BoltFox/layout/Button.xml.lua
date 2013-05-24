local function UpdateRes(self, state, ani)
	local attr = self:GetAttribute()
	local bkgTexture, textFont, textColor
	--if state ~= attr.Status then
		if state == "hover" then
			bkgTexture = attr.BkgHoverTextureID
			textFont = attr.TextHoverFontID
			textColor = attr.TextHoverColorID
		elseif state == "down" then
			bkgTexture = attr.BkgDownTextureID
			textFont = attr.TextDownFontID
			textColor = attr.TextDownColorID
		elseif state == "disable" then
			bkgTexture = attr.BkgDisableTextureID
			textFont = attr.TextDisableFontID
			textColor = attr.TextDisableColorID
		else
			bkgTexture = attr.BkgNormalTextureID
			textFont = attr.TextNormalFontID
			textColor = attr.TextNormalColorID
		end
	
		local bkgObj = self:GetControlObject("ctrl")
		local textObj = self:GetControlObject("text")
		local oldObj = self:GetControlObject("button.oldbkg")
		old_texture_id = bkgObj:GetTextureID()
		if bkgTexture then
			bkgObj:SetTextureID(bkgTexture)
			if textFont then
				textObj:SetTextFontResID(textFont)
			end
			if textColor then
				textObj:SetTextColorResID(textColor)
			end
			if old_texture_id ~= bkgTexture and ani ~= true then
				oldObj:SetTextureID(old_texture_id)
				local animationHelper = XLGetGlobal("xunlei.LuaThunderAnimationHelper")
				if animationHelper then
					animationHelper:AppearDestObjDisappearSrcObj(bkgObj, false, oldObj, false, nil, nil, "")
				end
			end
		end
		attr.status = state
	--end
end

function UpdateUI(self)
    local attr = self:GetAttribute()
    UpdateRes(self, attr.status, true)
end

function OnInitControl(self)
	local attr = self:GetAttribute()
	local bkgObj = self:GetControlObject("ctrl")
	if not self:GetVisible() then
		bkgObj:SetVisible(false)
		bkgObj:SetChildrenVisible(false)
	end
	
	local newState = nil
	
	if not self:GetEnable() then
		bkgObj:SetEnable(false)
		bkgObj:SetChildrenEnable(false)
		
		newState = "disable"
	else
	    newState = "normal"
	end
	
	local textObj = self:GetControlObject("text")
	textObj:SetHAlign(attr.HAlign)
	if attr.Text ~= nil and attr.Text ~= "" then
		textObj:SetText(attr.Text)
	end
	
	local left,top,right,bottom = self:GetObjPos()
	if attr.TextPosRight == -1 then
		attr.TextPosRight = right - left
	end
	if attr.TextPosBottom == -1 then
		attr.TextPosBottom = bottom - top
	end
	local left,top,right,bottom = textObj:GetObjPos()
	textObj:SetObjPos(attr.TextPosLeft,attr.TextPosTop,"father.width-"..attr.TextPosLeft,"father.height-"..attr.TextPosTop)
	
	UpdateRes(self, newState)	
	if attr.IsDefaultButton then
	    local animationHelper = XLGetGlobal("xunlei.LuaThunderAnimationHelper")
		if animationHelper then
			animationHelper:FocusLight(bkgObj, "")
		end
	end
end

function ChangeStatus(self, newStatus)
	local attr = self:GetAttribute()	
	if attr.Status == newStatus then
		return
	end	
	attr.Status = newStatus	
	UpdateRes(self, newStatus)	
end

function GetEnable(self)
	local attr = self:GetAttribute()
	return (attr.Status ~= "disable")
end

function RouteToFather__OnChar(self, char)
    if char == 9 then
        self:RouteToFather()
    end
end

function SetText(self, text)
	local textObj = self:GetControlObject("text")
	textObj:SetText(text)	
end

function GetText(self)
	local textObj = self:GetControlObject("text")
	return textObj:GetText()	
end

function SetTextPos(self,leftparam,topparam)
	local textObj = self:GetControlObject("text")
	local left,top,right,bottom = textObj:GetObjPos()
	textObj:SetObjPos(leftparam,topparam,right-left,bottom-top)
	local attr = self:GetAttribute()
	attr.TextPosLeft,attr.TextPosTop = left,top
end

function OnVisibleChange(self, visible)
	local bkgObj = self:GetControlObject("ctrl")
	if bkgObj then
		bkgObj:SetVisible(visible)
		bkgObj:SetChildrenVisible(visible)
	end
end

function OnEnableChange(self, enable)
	local bkgObj = self:GetControlObject("ctrl")
	bkgObj:SetEnable(enable)
	bkgObj:SetChildrenEnable(enable)
	
	if enable then
		self:ChangeStatus("normal")
	else
		self:ChangeStatus("disable")
	end
end

function OnLButtonDown(self)
	local attr = self:GetAttribute()	
	self:SetCaptureMouse(true)
	attr.Capture = true	
	self:ChangeStatus("down")	
	return 0, false
end

function OnLButtonUp(self, x, y)
	local attr = self:GetAttribute()	
	if attr.Capture then
		self:SetCaptureMouse(false)
		attr.Capture = false
	end
	
	local left, top, right, bottom = self:GetObjPos()
	if x >= 0 and x < (right - left) and y >= 0 and y < (bottom - top) then
		self:ChangeStatus("hover")
	else
		self:ChangeStatus("normal")
	end
	
	self:FireExtEvent("OnClick")
	
	return 0, false
end

function OnMouseMove(self, x, y)
	local attr = self:GetAttribute()
	
	if attr.Capture then		
		local left, top, right, bottom = self:GetObjPos()
		if x >= 0 and x < (right- left) and y >= 0 and y < (bottom - top) then
			self:ChangeStatus("down")
		else
			self:ChangeStatus("hover")
		end		
	else	
		self:ChangeStatus("hover")		
	end
	return 0, false
end

function OnMouseLeave(self)
	self:ChangeStatus("normal")	
	return 0, false
end

function OnKeyDown(self, uChar, uRepeatCount, uFlags )
	if uChar == 13 or uChar == 32 then
		self:FireExtEvent("OnClick")
	end
end

function OnFocusChange(self, focus)
	local attr = self:GetAttribute()
	if attr.HasFocusLine and focus then
		self:GetControlObject("focusrectangle"):SetVisible(true)
	else
		self:GetControlObject("focusrectangle"):SetVisible(false)
	end
end