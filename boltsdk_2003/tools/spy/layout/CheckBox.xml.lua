function SetState(self, newState, bShowAnimation)
	if bShowAnimation == nil then
		bShowAnimation = true	--true 是默认
	end

    local attr = self:GetAttribute()
    local owner = self:GetOwner()
    local bkg = self:GetControlObject("checkimage")
    local oldBkg = self:GetControlObject("oldimage")

	oldBkg:SetResID(bkg:GetResID())
	oldBkg:SetAlpha(255)
	
	if attr.Check then
		if newState == 0 then
			bkg:SetResID(attr.CheckNormalIconID)
		elseif newState == 1 then
			bkg:SetResID(attr.CheckDownIconID)
		elseif newState == 2 then
			bkg:SetResID(attr.CheckDisableIconID)
		elseif newState == 3 then
			bkg:SetResID(attr.CheckHoverIconID)
		end
	else
		if newState == 0 then
			bkg:SetResID(attr.UncheckNormalIconID)
		elseif newState == 1 then
			bkg:SetResID(attr.UncheckDownIconID)
		elseif newState == 2 then
			bkg:SetResID(attr.UncheckDisableIconID)
		elseif newState == 3 then
			bkg:SetResID(attr.UncheckHoverIconID)
		end
	end
	attr.NowState = newState
	
	if bShowAnimation then
		local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")	
		local aniAlpha = aniFactory:CreateAnimation("AlphaChangeAnimation")
		aniAlpha:BindRenderObj(oldBkg)
		aniAlpha:SetTotalTime(150)
		aniAlpha:SetKeyFrameAlpha(255,0)
		owner:AddAnimation(aniAlpha)
		aniAlpha:Resume()
	else
		oldBkg:SetAlpha(0)
	end
end

function SetCheck(self, check, bClick, bShowAnimation)	--bClick:是否是鼠标单击导致的状态改变
	if bClick == nil then
		bClick = false
	end
    local c = check
    if c == nil then
        c = false
    end
    
    if c == self:GetCheck() then
        return
    end
    
    local attr = self:GetAttribute()
    attr.Check = c
    if attr.Enable then
        self:SetState(0, bShowAnimation)
    else
        self:SetState(2, bShowAnimation)
    end
    self:FireExtEvent("OnCheck",attr.Check, bClick)
end

function GetCheck(self)
    local attr = self:GetAttribute()
    return attr.Check
end

function SetEnable(self, enable)
    if enable == nil or enable == self:GetEnable() then
        return
    end
    local attr = self:GetAttribute()
    attr.Enable = enable
    if not attr.Enable then
        self:SetState(2)
    else
        self:SetState(0)
    end
end

function GetEnable(self)
    local attr = self:GetAttribute()
    return attr.Enable
end

function OnBind(self)
    local attr = self:GetAttribute()
    attr.Check = false
    attr.NowState=0
    attr.Enable=true
    if attr.Text then
        text = self:GetControlObject("checktext")
        text:SetText(attr.Text)
    end
    local bkg = self:GetControlObject("checkimage")
    bkg:SetResID(attr.UncheckNormalIconID)
	local textObj = self:GetControlObject("checktext")
	textObj:SetTextColorResID(attr.TextColor)
	textObj:SetTextFontResID(attr.TextFont)
	
end

function OnInitControl(self)
    local attr = self:GetAttribute()
    self:Show(attr.Visible)
end

function OnLButtonDown(self)
    local attr = self:GetAttribute()
    if attr.Enable then
        self:SetState(1)
        self:SetCaptureMouse(true)
    end
end

function OnLButtonUp(self)
    local attr = self:GetAttribute()
    if attr.Enable then
        if attr.NowState==1 then
            self:SetCheck(not attr.Check, true)
        end
        self:SetCaptureMouse(false)
    end
end

function OnMouseMove(self)
    local attr = self:GetAttribute()
    if attr.Enable then
        if attr.NowState==0 then
            self:SetState(3)
        end
    end
end

function OnMouseLeave(self)
    local attr = self:GetAttribute()
    self:SetState(0)
end

function OnChar(self, char)
    if char == 32 or char == 13 then
        local attr = self:GetAttribute()
        self:SetCheck(not attr.Check, false)
    elseif char == 9 then
        self:RouteToFather()
    end
    return 0, true
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

function AddTip(ctrl, text, type)
    if not type then
        type = 1
    end
    local left, top, right, buttom = ctrl:GetObjPos()
    local width, height = right - left, buttom - top
    local tipid = ctrl:RemoveTip()
    local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
    local tip = objFactory:CreateUIObject(tipid,"Thunder.NewTask.Tip")
    tip:SetText(text)
    tip:SetType(type)
    local tipwidth, tipheight = tip:GetSize(44, 35)
	
	tip:SetZorder(500)
    ctrl:AddChild(tip)
	if type == 1 then
        tip:SetObjPos(0, -height - 15, tipwidth + 2, -height + tipheight - 15)
    elseif type == 2 then
        tip:SetObjPos(0, height - 10, tipwidth + 2, height + tipheight - 10)
    elseif type == 3 then
        tip:SetObjPos(width - tipwidth, -height, width - 2, -height + tipheight)
    elseif type == 4 then
        tip:SetObjPos(width - tipwidth, height - 10, width - 2, height + tipheight - 10)
    end
end

function RemoveTip(ctrl)
    local id = ctrl:GetID()
    local tipid = id..".tip"
    local tip = ctrl:GetControlObject(tipid)
    if tip ~= nil then
        ctrl:RemoveChild(tip)
    end
    return tipid
end