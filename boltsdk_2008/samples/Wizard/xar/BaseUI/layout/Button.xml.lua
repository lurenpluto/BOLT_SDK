local function UpdateRes(self, state, ani)
	local style = self:GetStyle()
	local attr = self:GetAttribute()
	local bkgTexture, textFont, textColor
	--if state ~= attr.Status then
		if state == "hover" then
			bkgTexture = style.bkg_hover_texture
			textFont = style.text_hover_font
			textColor = style.text_hover_color
		elseif state == "down" then
			bkgTexture = style.bkg_down_texture
			textFont = style.text_down_font
			textColor = style.text_down_color
		elseif state == "disable" then
			bkgTexture = style.bkg_disable_texture
			textFont = style.text_disable_font
			textColor = style.text_disable_color
		else
			bkgTexture = style.bkg_normal_texture
			textFont = style.text_normal_font
			textColor = style.text_normal_color
		end
	
		local bkgObj = self:GetControlObject("ctrl")
		local textObj = self:GetControlObject("text")
		local oldObj = self:GetControlObject("button.oldbkg")
		old_texture_id = bkgObj:GetTextureID()
		if bkgTexture and bkgTexture ~= "" then
			bkgObj:SetTextureID(bkgTexture)
			if textFont and textFont ~= "" then
				textObj:SetTextFontResID(textFont)
			end
			if textColor and textColor ~= "" then
				textObj:SetTextColorResID(textColor)
			end
			if old_texture_id ~= "" and old_texture_id ~= bkgTexture and ani ~= true then
				oldObj:SetTextureID(old_texture_id)
				local animationHelper = XLGetGlobal("xunlei.LuaThunderAnimationHelper")
				if animationHelper then
					animationHelper:AppearDestObjDisappearSrcObj(bkgObj, false, oldObj, false, nil, nil, "")
				end
			end
		end
		attr.status = state
	--end
	
	local left,top,right,bottom = self:GetObjPos()

	local left,top,right,bottom = textObj:GetObjPos()
	textObj:SetObjPos(style.text_pos_left,style.text_pos_top,"father.width - "..style.text_pos_left, "father.height -".. style.text_pos_top)
	
end

function UpdateUI(self)
    local attr = self:GetAttribute()
    UpdateRes(self, attr.status, true)
end

function OnUpdateStyle(self)
	UpdateUI(self)
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
	if attr.Text ~= nil and attr.Text ~= "" then
		textObj:SetText(attr.Text)
	end
	

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

function SetText(self, text)
	local attr = self:GetAttribute()
	attr.Text = text
	
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
	local style = self:GetStyle()
	style.text_pos_left,style.text_pos_top = left,top
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