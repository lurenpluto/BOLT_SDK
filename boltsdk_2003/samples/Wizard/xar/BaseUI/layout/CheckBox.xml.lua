--NowState 0:Normal, 1:Down, 2:Disable, 3:Hover
local function SetState(self, newState)
    local attr = self:GetAttribute()
    local boxBkg = self:GetControlObject("boximage")
	if newState == 0 then
		boxBkg:SetResID(attr.BoxNormalIconID)
	elseif newState == 1 then
		boxBkg:SetResID(attr.BoxNormalIconID)
	elseif newState == 2 then
		boxBkg:SetResID(attr.BoxDisableIconID)
	elseif newState == 3 then
		boxBkg:SetResID(attr.BoxHoverIconID)
	end
	
	attr.NowState = newState
end

function SetCheck(self, check, bClick)	
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
    
	local tickBkg = self:GetControlObject("tickimage")
    local attr = self:GetAttribute()
    attr.Check = c
    if self:GetEnable() then
		if bClick then
			if attr.AniPos == nil then
				local left,top,right,bottom = tickBkg:GetObjPos()
				if attr.Check then
				    attr.curLeft,attr.curTop,attr.curRight,attr.curBottom = (left+right)/2,(top+bottom)/2,(left+right)/2,(top+bottom)/2
				else
				    attr.curLeft,attr.curTop,attr.curRight,attr.curBottom = left,top,right,bottom
				end
			else
				attr.AniPos:Stop()
				attr.AniPos = nil
				local left,top,right,bottom = tickBkg:GetObjPos()
				attr.curLeft,attr.curTop,attr.curRight,attr.curBottom = left,top,right,bottom
			end

			SetState(self,0)
			local ownerTree = self:GetOwner()
			tickBkg:SetResID(attr.TickNormalIconID)
			tickBkg:SetVisible(true)
			local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")	
			local posAniTemplate = templateManager:GetTemplate("checkbox.showani","AnimationTemplate")
			local instance = posAniTemplate:CreateInstance()
			if instance then
				if attr.Check then
					instance:SetKeyFrameRect(attr.curLeft,attr.curTop,attr.curRight,attr.curBottom,attr.initLeft,attr.initTop,attr.initRight,attr.initBottom)
				else
					instance:SetKeyFrameRect(attr.curLeft,attr.curTop,attr.curRight,attr.curBottom,(attr.initLeft+attr.initRight)/2,(attr.initTop+attr.initBottom)/2,(attr.initLeft+attr.initRight)/2,(attr.initTop+attr.initBottom)/2)
				end
				instance:BindLayoutObj(tickBkg)
				ownerTree:AddAnimation(instance)
				local function onAniFinish(self,old,new)
                    if new == 4 then
                        attr.AniPos = nil
                        --tickBkg:SetResID(attr.TickNormalIconID)
                        tickBkg:SetObjPos(attr.initLeft,attr.initTop,attr.initRight,attr.initBottom)
                        if attr.Check  then
                            tickBkg:SetVisible(true)
                        else
                            tickBkg:SetVisible(false)
                        end
                    end
                end
        
                instance:AttachListener(true ,onAniFinish)
				instance:Resume()
				attr.AniPos = instance
			end
		else
			SetState(self,0)
			--tickBkg:SetResID(attr.TickNormalIconID)
			if attr.Check  then
				tickBkg:SetVisible(true)
			else
				tickBkg:SetVisible(false)
			end
		end
    else
        SetState(self,2)
		--tickBkg:SetResID(attr.TickDisableIconID)
		if attr.Check  then
            tickBkg:SetVisible(true)
        else
            tickBkg:SetVisible(false)
        end
    end
	
    self:FireExtEvent("OnCheck",attr.Check, bClick)
end

function GetCheck(self)
    local attr = self:GetAttribute()
    return attr.Check
end

function SetText(self, text)
	local attr = self:GetAttribute()
	attr.Text = text
	textobj = self:GetControlObject("checktext")
	textobj:SetText(attr.Text)
end

function GetText(self)
	local attr = self:GetAttribute()
	return attr.Text;
end

function SetBitmap(self, bitmap)
	local attr = self:GetAttribute()
	attr.icon = bitmap
	if attr.icon ~= nil then
		local iconobj = self:GetControlObject("icon")
		if iconobj == nil then
			local objfactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
			local iconobj = objfactory:CreateUIObject("icon","ImageObject")
			local text = self:GetControlObject("checktext")
			local left,top,right,bottom = text:GetObjPos()
			iconobj:SetObjPos( left, ( bottom - top -16 )/2, left + 16, ( bottom - top -16 )/2+16 )
			iconobj:SetDrawMode(1)
			text:SetObjPos( left+19, top, right, bottom )
			iconobj:SetBitmap(attr.icon)
			self:AddChild(iconobj)
		else
			iconobj:SetBitmap(attr.icon)
		end
	end
end


function OnBind(self)
end

function OnInitControl(self)
    local attr = self:GetAttribute()

	
	local textObj = self:GetControlObject("checktext")
    if attr.Text then
        textObj:SetText(attr.Text)
    end

	textObj:SetTextColorResID(attr.TextColor)
	textObj:SetTextFontResID(attr.TextFont)
	
    local boxBkg = self:GetControlObject("boximage")
    boxBkg:SetResID(attr.BoxNormalIconID)
    local tickBkg = self:GetControlObject("tickimage")
    tickBkg:SetResID(attr.TickNormalIconID)
    tickBkg:SetVisible(attr.Check)
	local left,top,right,bottom = tickBkg:GetObjPos()
	attr.initLeft,attr.initTop,attr.initRight,attr.initBottom = left,top,right,bottom
end

function OnLButtonDown(self)
    if self:GetEnable() then
        SetState(self,1)
        self:SetCaptureMouse(true)
    end
end

function OnLButtonUp(self)
    local attr = self:GetAttribute()
    if self:GetEnable() then
        if attr.NowState==1 then
            self:SetCheck(not attr.Check, true)
        end
        self:SetCaptureMouse(false)
    end
end

function OnMouseMove(self)
    local attr = self:GetAttribute()
    if self:GetEnable() then
        if attr.NowState==0 then
            SetState(self,3)
        end
    end
end

function OnMouseLeave(self)
    local attr = self:GetAttribute()
	if self:GetEnable() then
		SetState(self,0)
	end
end
function OnChar(self, char)
    if char == 32 or char == 13 then
        local attr = self:GetAttribute()
        self:SetCheck(not attr.Check)
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

function AddTip(ctrl, text, type_)
    -- if not type_ then
        -- type_ = 1
    -- end
    -- local left, top, right, buttom = ctrl:GetObjPos()
    -- local width, height = right - left, buttom - top
    -- local tipid = ctrl:RemoveTip()
    -- local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
    -- local tip = objFactory:CreateUIObject(tipid,"Thunder.NewTask.Tip")
    -- tip:SetText(text)
    -- tip:SetType(type_)
    -- local tipwidth, tipheight = tip:GetSize(44, 35)
	
	-- tip:SetZorder(500)
    -- ctrl:AddChild(tip)
	-- if type_ == 1 then
        -- tip:SetObjPos(0, -height - 15, tipwidth + 2, -height + tipheight - 15)
    -- elseif type_ == 2 then
        -- tip:SetObjPos(0, height - 10, tipwidth + 2, height + tipheight - 10)
    -- elseif type_ == 3 then
        -- tip:SetObjPos(width - tipwidth, -height, width - 2, -height + tipheight)
    -- elseif type_ == 4 then
        -- tip:SetObjPos(width - tipwidth, height - 10, width - 2, height + tipheight - 10)
    -- end
 end

function RemoveTip(ctrl)
	-- return 
    -- local id = ctrl:GetID()
    -- local tipid = id..".tip"
    -- local tip = ctrl:GetControlObject(tipid)
    -- if tip ~= nil then
        -- ctrl:RemoveChild(tip)
    -- end
    -- return tipid
end

function OnFocusChange(self, focus)
	if not focus then
		self:RemoveTip()
	end
end

function OnEnableChange (self,isEnable)

	local tickBkg = self:GetControlObject("tickimage")
	local objText = self:GetControlObject("checktext")
    if not isEnable then
        SetState(self,2)
		objText:SetTextColorResID(attr.DisableTextColor)
		tickBkg:SetResID(attr.TickDisableIconID)
		if attr.Check  then
            tickBkg:SetVisible(true)
        else
            tickBkg:SetVisible(false)
        end
    else
        SetState(self,0)
		objText:SetTextColorResID(attr.TextColor)
		tickBkg:SetResID(attr.TickNormalIconID)
		if attr.Check  then
            tickBkg:SetVisible(true)
        else
            tickBkg:SetVisible(false)
        end
    end
end