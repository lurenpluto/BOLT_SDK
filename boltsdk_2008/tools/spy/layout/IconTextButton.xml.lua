function SetEnable(self, enable)
    local attr = self:GetAttribute()
    attr.Enable = enable
    local bkg = self:GetControlObject("icontextbtn.bkg")
    if enable then
        bkg:SetTextureID(attr.NormalBkgID)
    else
        bkg:SetTextureID(attr.DisableBkgID)
    end
end

function GetEnable(self)
    local attr = self:GetAttribute()
    return attr.Enable
end

function OnInitControl(self)
    local attr = self:GetAttribute()
    local text = self:GetControlObject("icontextbtn.text")
    if attr.TextFont then
        text:SetTextFontResID(attr.TextFont)
    end
	
    if attr.TextColor then
        text:SetTextColorResID(attr.TextColor)
    end
    local attr = self:GetAttribute()
    self:SetText(attr.Text)
    attr.NowState=0
    attr.OpState=0;
    local bkg = self:GetControlObject("icontextbtn.bkg")
    bkg:SetTextureID(attr.NormalBkgID)
    local icon = self:GetControlObject("icontextbtn.icon")
    if attr.IconResID then
        icon:SetResID(attr.IconResID)
    end
    
    local left,top,right,bottom = self:GetObjPos()
    local width = right - left
    local height = bottom - top
    
    local text = self:GetControlObject("icontextbtn.text")
    if attr.OnlyIcon then
        icon:SetObjPos(attr.LeftMagrin + (width - attr.IconWidth) / 2 - 1, (height - attr.IconHeight) / 2, attr.LeftMagrin + (width + attr.IconWidth) / 2, (height + attr.IconHeight) / 2)
        text:SetObjPos(0, 0, 0, 0)
    else
        if attr.IconWidth then
            if attr.IconHeight ~= 0 then
                icon:SetObjPos(attr.LeftMagrin, (height - attr.IconHeight) / 2, attr.LeftMagrin + attr.IconWidth, (height + attr.IconHeight) / 2)
            else
                icon:SetObjPos(0, 0, attr.IconWidth, height)
            end
        end
        text:SetObjPos(attr.LeftMagrin + attr.IconWidth, 0, width, height)
    end
    
end

function SetIcon(self, icon)
    local image = self:GetControlObject("icontextbtn.icon")
    image:SetResID(icon)
end

function SetState(self, newState)
    local attr = self:GetAttribute()
    if attr == nil then
		return
	end
	
    if attr.NowState ~= nil and newState ~= attr.NowState then
    local ownerTree = self:GetOwner()
    local oldBkg = self:GetControlObject("icontextbtn.oldbkg")
    local bkg = self:GetControlObject("icontextbtn.bkg")
    
    oldBkg:SetTextureID(bkg:GetTextureID())
    oldBkg:SetAlpha(255)
    if newState == 0 then
    bkg:SetTextureID(attr.NormalBkgID)
    elseif newState == 1 then
    bkg:SetTextureID(attr.DownBkgID)
    elseif newState == 2 then
    bkg:SetTextureID(attr.DisableBkgID)
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

function SetText(self, text)
    if text == nil then
        return
    end
    local textObj = self:GetControlObject("icontextbtn.text")
    textObj:SetText(text)
    textObj:SetAlpha(254)
end

function OnLButtonDown(self)
    local attr = self:GetAttribute()
    if attr.Enable then
        self:SetState(1)
        self:SetCaptureMouse(true)
        attr.BtnDown = true
    end
end

function OnLButtonUp(self, x, y)
    local attr = self:GetAttribute()
    if attr.Enable then
        if attr.NowState==1 then
            self:SetState(0)
            self:FireExtEvent("OnClick", x, y)
        end
        self:SetCaptureMouse(false)
        attr.BtnDown = false
    end
end

function OnMouseMove(self, x, y)
    local left, top, right, bottom = self:GetObjPos()
    local width, height = right - left, bottom - top
    
    local attr = self:GetAttribute()
    if attr.Enable then
        if ((x >= 0) and (x <= width) and (y >= 0) and (y <= height)) then
            if attr.NowState==0 then
                if attr.BtnDown then
                    self:SetState(1)
                else
                    self:SetState(3)
                end
                
            end
        else
            self:SetState(0)
        end
    end
end

function OnMouseLeave(self)
    local attr = self:GetAttribute()
    self:SetState(0)
end

function OnBind(self)
end

function OnPosChange(self)
    local attr = self:GetAttribute()

    local left,top,right,bottom = self:GetObjPos()
    local width = right - left
    local height = bottom - top
    
    local bkn = self:GetControlObject("icontextbtn.bkg")
    bkn:SetObjPos(0, 0, width, height)

    local oldBkn = self:GetControlObject("icontextbtn.oldbkg")
    oldBkn:SetObjPos(0, 0, width, height)
    
    --local icon = self:GetControlObject("icontextbtn.icon")
    local text = self:GetControlObject("icontextbtn.text")
    --icon:SetObjPos(0, 0, attr.IconWidth, height)
    text:SetObjPos(attr.IconWidth, 0, width, height)
end