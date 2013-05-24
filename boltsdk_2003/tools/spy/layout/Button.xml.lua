function SetState(self, newState, forceUpdate)
    local attr = self:GetAttribute()
    if forceUpdate or newState ~= attr.NowState then
        local ownerTree = self:GetOwner()
        local oldBkg = self:GetControlObject("button.oldbkg")
        local bkg = self:GetControlObject("button.bkg")
		local text = self:GetControlObject("button.text")
		
        oldBkg:SetTextureID(bkg:GetTextureID())
        oldBkg:SetAlpha(255)
		text:SetTextColorResID("system.black")
        if newState == 0 then
            bkg:SetTextureID(attr.NormalBkgID)
        elseif newState == 1 then
            bkg:SetTextureID(attr.DownBkgID)
        elseif newState == 2 then
            bkg:SetTextureID(attr.DisableBkgID)
			text:SetTextColorResID("button.disable.color")
        elseif newState == 3 then
            bkg:SetTextureID(attr.HoverBkgID)
        end


        local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")	
        local aniAlpha = aniFactory:CreateAnimation("AlphaChangeAnimation")
        aniAlpha:BindRenderObj(oldBkg)
        aniAlpha:SetTotalTime(200)
        aniAlpha:SetKeyFrameAlpha(255,0)
        ownerTree:AddAnimation(aniAlpha)
        aniAlpha:Resume()
        attr.NowState = newState
    end
end

function SetBitmap( self, nor, down, hover, disable )
	local attr = self:GetAttribute()
	attr.NormalBkgID = ""
	if nor ~= nil then
		attr.NormalBkgID = nor
	end
	attr.DownBkgID = ""
	if down ~= nil then
		attr.DownBkgID = down
	end
	attr.DisableBkgID = ""
	if disable ~= nil then
		attr.DisableBkgID = disable
	end
	attr.HoverBkgID = ""
	if hover ~= nil then
		attr.HoverBkgID = hover
	end
end

function SetText(self, text)
    if text == nil then
        return
    end
    local textObj = self:GetControlObject("button.text")
    textObj:SetText(text)
    local attr = self:GetAttribute()
    attr.Text = text
end

function GetText(self)
    local attr = self:GetAttribute()
	return attr.Text
end

function SetEnable(self, enable)
    local attr = self:GetAttribute()
    attr.Enable = enable
    local bkg = self:GetControlObject("button.bkg")
	local text = self:GetControlObject("button.text")
    if enable then
		bkg:SetTextureID(attr.NormalBkgID)
		attr.NowState = 0
		text:SetTextColorResID(attr.TextColor)
    else
		bkg:SetTextureID(attr.DisableBkgID)
		attr.NowState = 2
		text:SetTextColorResID("button.disable.color")
    end
end

function GetEnable(self)
    local attr = self:GetAttribute()
    return attr.Enable
end

function OnLButtonDown(self, x, y)
    local attr = self:GetAttribute()
    local left, top, right, bottom = self:GetObjPos()
    local width, height = right - left, bottom - top
    if not attr.UseValidPos then
        attr.ValidWidth = width
        attr.ValidHeight = height
    end
    if attr.Enable then
        if ((x >= attr.ValidLeft) and (x <= attr.ValidLeft + attr.ValidWidth) and (y >= attr.ValidTop) and (y <= attr.ValidTop + attr.ValidHeight)) then
            self:SetState(1)
            self:SetCaptureMouse(true)
            attr.BtnDown = true
        end
    end
    return 0, true
end

function OnLButtonUp(self)
    local attr = self:GetAttribute()
    if attr.Enable then
        if attr.NowState==1 then
			self:SetState(0)
            self:FireExtEvent("OnClick")
        end
        self:SetCaptureMouse(false)
        attr.BtnDown = false
    end
    return 0, true
end

function OnMouseMove(self, x, y)
    local left, top, right, bottom = self:GetObjPos()
    local width, height = right - left, bottom - top
    
    local attr = self:GetAttribute()
    if not attr.UseValidPos then
        attr.ValidWidth = width
        attr.ValidHeight = height
    end
    if attr.Enable then
        if ((x >= attr.ValidLeft) and (x <= attr.ValidLeft + attr.ValidWidth) and (y >= attr.ValidTop) and (y <= attr.ValidTop + attr.ValidHeight)) then
            if attr.NowState==0 then
                if attr.BtnDown then
                    self:SetState(1)
                else
                    self:SetState(3)
                end
            end
        else
            if attr.ChangeStateWhenLeave then
                self:SetState(0)
            end
        end
        self:FireExtEvent("OnButtonMouseMove", x, y)
    end
    return 0, true
end

function OnMouseLeave(self, x, y)
    local attr = self:GetAttribute()
    if attr.Enable then
        self:SetState(0)
    else
        self:SetState(2)
    end
    self:FireExtEvent("OnButtonMouseLeave", x, y)
    return 0, true
end

function OnInitControl(self)
    local attr = self:GetAttribute()
    self:SetText(attr.Text)
	self:SetTextColor(attr.TextColor)
	self:SetTextFont(attr.TextFont)
    attr.NowState=0
    local bkg = self:GetControlObject("button.bkg")
    if attr.NormalBkgID == nil then
        return
    end
    bkg:SetTextureID(attr.NormalBkgID)
    self:SetEnable(attr.Enable)
    if attr.BtnLight then
        local light = self:GetControlObject("button.light")
        light:SetResID(attr.BtnLight)
    end
    
    local left, top, right, bottom = self:GetObjPos()
    
    if not attr.ValidLeft then
        attr.ValidLeft = 0
    end
    if not attr.ValidTop then
        attr.ValidTop = 0
    end
    if not attr.ValidWidth then
        attr.ValidWidth = right - left
    end
    if not attr.ValidHeight then
        attr.ValidHeight = bottom - top
    end
    self:Show(attr.Visible)
end

function SetTextColor(self, color)
    if color == nil then
        return
    end
    local textObj = self:GetControlObject("button.text")
    textObj:SetTextColorResID(color)
end

function SetTextFont(self, font)
    if font == nil then
        return
    end
    local textObj = self:GetControlObject("button.text")
    textObj:SetTextFontResID(font)
end

function Show(self, visible)
    local attr = self:GetAttribute()
    attr.Visible = visible
    self:SetVisible(visible)
    self:SetChildrenVisible(visible)
end

function IsShow(self)
    local attr = self:GetAttribute()
    return attr.Visible
end