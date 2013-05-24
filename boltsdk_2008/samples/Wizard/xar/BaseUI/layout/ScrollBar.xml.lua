function PosAni_Action(ani)
	local aniattr = ani:GetAttribute()
	local cnt = aniattr.Counter
	local bar = aniattr.ScrollBar
	local spd = aniattr.Speed + cnt * aniattr.Acceleration
	local pos = bar:GetScrollPos()
	if aniattr.rangebegin and pos < aniattr.rangebegin then
		return 
	end
	pos = pos + spd
	if aniattr.rangeend and pos > aniattr.rangeend then
		return 
	end
	bar:SetScrollPos(pos)
	bar:FireExtEvent("OnScrollPosChange")
	aniattr.Counter = cnt + 1
end

function PosAni_Bind(ani,bar)
	local aniattr = ani:GetAttribute()
	aniattr.ScrollBar = bar
	aniattr.Counter = 0
	ani:SetTotalTime(1000000000)
end

function PosAni_SetSpeed(ani,spd,acc)
	local aniattr = ani:GetAttribute()
	aniattr.Speed = spd
	aniattr.Acceleration = acc
end

function PosAni_SetRange(ani,rbegin,rend)
	local aniattr = ani:GetAttribute()
	aniattr.rangebegin = rbegin
	aniattr.rangeend = rend
end

function getMouseThumbRelationV(self,x,y)
	local attr = self:GetAttribute()
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local l,t,r,b = thumbbtn:GetObjPos()
	if y < t then
		return "pre"
	elseif y > b then
		return "next"
	else 
		return "on"
	end
end

function getMouseThumbRelationH(self,x,y)
	local attr = self:GetAttribute()
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local l,t,r,b = thumbbtn:GetObjPos()
	if x < l then
		return "pre"
	elseif x > r then
		return "next"
	else 
		return "on"
	end
end

function getThumbEndV(self,bkgright,bkgbottom)
	local attr = self:GetAttribute()
	return bkgbottom - attr.NavigationBtnLength
end

function getThumbEndH(self,bkgright,bkgbottom)
	local attr = self:GetAttribute()
	return bkgright - attr.NavigationBtnLength
end

function onMouseWheelV(self,distance)
	local attr = self:GetAttribute()
	attr.TypeImplMethods.offsetThumbPos(self,0,distance)
	updateScrollPos(self)
end

function onMouseWheelH(self,distance)
end

function rawSetProp(self,rbegin,rend,pgsize)
	local attr = self:GetAttribute()
	local bkg = self:GetControlObject("bkg")
	local l,t,r,b = bkg:GetObjPos()
	local tb = attr.NavigationBtnLength
	local te = attr.TypeImplMethods.getThumbEnd(self,r,b)
	local tr = te - tb
	attr.Range = rend - rbegin
	local tl = tr * attr.PageSize / attr.Range 
	if tl < attr.ThumbBtnMinLength then
		tl = attr.ThumbBtnMinLength
	end
	attr.ThumbPosBegin = tb
	attr.ThumbPosEnd = te - tl
	attr.ThumbLength = tl
end

function initBtnPosV(self)
	local attr = self:GetAttribute()
	local bkg = self:GetControlObject("bkg")
	local l,t,r,b = bkg:GetObjPos()
	local w = r - l
	local prebtn = bkg:GetControlObject("prebtn")
	prebtn:SetObjPos(0,0,w,attr.NavigationBtnLength)
	local nextbtn = bkg:GetControlObject("nextbtn")
	nextbtn:SetObjPos(0,b - attr.NavigationBtnLength,w,b)
	attr.BtnWidth = w
end

function initBtnPosH(self)
	local attr = self:GetAttribute()
	local bkg = self:GetControlObject("bkg")
	local l,t,r,b = bkg:GetObjPos()
	local h = b - t
	local prebtn = bkg:GetControlObject("prebtn")
	prebtn:SetObjPos(0,0,attr.NavigationBtnLength,h)
	local nextbtn = bkg:GetControlObject("nextbtn")
	nextbtn:SetObjPos(r - attr.NavigationBtnLength,0,r,h)
	attr.BtnHeight = h
end

function rawSetThumbPos(self,pos)
	local attr = self:GetAttribute()
	if pos < attr.ThumbPosBegin then
		pos = attr.ThumbPosBegin
	elseif pos > attr.ThumbPosEnd then
		pos = attr.ThumbPosEnd
	end
	attr.ThumbPos = pos
	attr.TypeImplMethods.setThumbPos(self,pos)
	attr.TypeImplMethods.updateBkgDownArea(self)
end

function offsetThumbPosV(self,offsetx,offsety)
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local attr = self:GetAttribute()
	local l,t,r,b = thumbbtn:GetObjPos()
	t = t + offsety
	rawSetThumbPos(self,t)
end

function offsetThumbPosH(self,offsetx,offsety)
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local attr = self:GetAttribute()
	local l,t,r,b = thumbbtn:GetObjPos()
	l = l + offsetx
	rawSetThumbPos(self,l)
end


function setThumbPosV(self,pos)
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local attr = self:GetAttribute()
	thumbbtn:SetObjPos(0,pos,attr.BtnWidth,pos + attr.ThumbLength)
end

function setThumbPosH(self,pos)
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local attr = self:GetAttribute()
	thumbbtn:SetObjPos(pos,0,pos + attr.ThumbLength,attr.BtnHeight)
end

function getThumbPosV(self)
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local l,t,r,b = thumbbtn:GetObjPos()
	local attr = self:GetAttribute()
	return t - attr.ThumbPosBegin
end

function getThumbPosH(self)
	local thumbbtn = (self:GetControlObject("bkg")):GetControlObject("thumbbtn")
	local l,t,r,b = thumbbtn:GetObjPos()
	local attr = self:GetAttribute()
	return l - attr.ThumbPosBegin
end

function getScrollPosFromPointV(self,x,y)
	local attr = self:GetAttribute()
	local posrate = (y - attr.ThumbPosBegin) / (attr.ThumbPosEnd - attr.ThumbPosBegin)
	return attr.RangeBegin + posrate * (attr.Range - attr.PageSize)
end

function getScrollPosFromPointH(self,x,y)
	local attr = self:GetAttribute()
	local posrate = (x - attr.ThumbPosBegin) / (attr.ThumbPosEnd - attr.ThumbPosBegin)
	return attr.RangeBegin + posrate * (attr.Range - attr.PageSize)
end

function updateScrollPos(self)
	local attr = self:GetAttribute()
	local scrollpos = attr.TypeImplMethods.getScrollPosFromPoint(self,attr.ThumbPos,attr.ThumbPos)
	rawSetScrollPos(self,scrollpos)
end

function updateThumbPos(self)
	local attr = self:GetAttribute()
	local posrate = (attr.ScrollPos - attr.RangeBegin) / (attr.Range - attr.PageSize)
	local thumbpos = attr.ThumbPosBegin + (attr.ThumbPosEnd - attr.ThumbPosBegin) * posrate 
	rawSetThumbPos(self,thumbpos)
end


function rawForceRefresh(self)
	local attr = self:GetAttribute()
	attr.TypeImplMethods.initBtnPos(self)
	rawSetProp(self,attr.RangeBegin,attr.RangeEnd,attr.PageSize)
	updateThumbPos(self)
end


function updateBkgDownAreaV(self)
	local attr = self:GetAttribute()
	local bkg = self:GetControlObject("bkg")
	local bkgdown = bkg:GetControlObject("bkgdownarea")
	local l,t,r,b = bkg:GetObjPos()
	local w = r - l
	if attr.BkgDown == true then
		if attr.BkgDownPos == "pre" then
			bkgdown:SetObjPos(0,attr.ThumbPosBegin,w,attr.ThumbPos)
		elseif attr.BkgDownPos == "next" then
			bkgdown:SetObjPos(0,attr.ThumbPosBegin + attr.ThumbLength,w,attr.ThumbPosEnd + attr.ThumbLength)
		end
	else
		bkgdown:SetObjPos(0,0,0,0)
	end
	l,t,r,b = bkgdown:GetObjPos()
end

function updateBkgDownAreaH(self)
	local attr = self:GetAttribute()
	local bkg = self:GetControlObject("bkg")
	local bkgdown = bkg:GetControlObject("bkgdownarea")
	local l,t,r,b = bkg:GetObjPos()
	local h = b - t
	if attr.BkgDown == true then
		if attr.BkgDownPos == "pre" then
			bkgdown:SetObjPos(attr.ThumbPosBegin,0,attr.ThumbPos,h)
		elseif attr.BkgDownPos == "next" then
			bkgdown:SetObjPos(attr.ThumbPosBegin + attr.ThumbLength,0,attr.ThumbPosEnd + attr.ThumbLength,h)
		end
	else
		bkgdown:SetObjPos(0,0,0,0)
	end
end

function OnBind(self)
	local attr = self:GetAttribute()
	attr.TypeImplMethods = {}
	if attr.Type == "v" then
		attr.TypeImplMethods.getThumbEnd = getThumbEndV
		attr.TypeImplMethods.initBtnPos = initBtnPosV
		attr.TypeImplMethods.setThumbPos = setThumbPosV
		attr.TypeImplMethods.getThumbPos = getThumbPosV
		attr.TypeImplMethods.offsetThumbPos = offsetThumbPosV
		attr.TypeImplMethods.getMouseThumbRelation = getMouseThumbRelationV
		attr.TypeImplMethods.getScrollPosFromPoint = getScrollPosFromPointV
		attr.TypeImplMethods.onMouseWheel = onMouseWheelV
		attr.TypeImplMethods.updateBkgDownArea = updateBkgDownAreaV
	elseif attr.Type == "h" then
		attr.TypeImplMethods.getThumbEnd = getThumbEndH
		attr.TypeImplMethods.initBtnPos = initBtnPosH
		attr.TypeImplMethods.setThumbPos = setThumbPosH
		attr.TypeImplMethods.getThumbPos = getThumbPosH
		attr.TypeImplMethods.offsetThumbPos = offsetThumbPosH
		attr.TypeImplMethods.getMouseThumbRelation = getMouseThumbRelationH
		attr.TypeImplMethods.getScrollPosFromPoint = getScrollPosFromPointH
		attr.TypeImplMethods.onMouseWheel = onMouseWheelH
		attr.TypeImplMethods.updateBkgDownArea = updateBkgDownAreaH
	end
	local tm = XLGetObject("Xunlei.UIEngine.TemplateManager")
	attr.ScrollAniTemplate = tm:GetTemplate("BaseUI.ScrollBar.ScrollPosAni.Template","AnimationTemplate")
	
	local bkg = self:GetControlObject("bkg")
	local bkgattr = bkg:GetStyle()
	bkgattr.Type = attr.Type
	bkgattr.bkg_normal_texture = attr.BkgNormalBitmapID
    bkgattr.bkg_hover_texture = attr.BkgHoverBitmapID
	bkgattr.bkg_down_texture = attr.BkgDownBitmapID
	bkgattr.bkg_disable_texture = attr.BkgDisableBitmapID
	
	local prebtn = bkg:GetControlObject("prebtn") 
    local prebtnattr = prebtn:GetStyle()
    prebtnattr.bkg_normal_texture = attr.PreBtnNormalBitmapID
    prebtnattr.bkg_hover_texture = attr.PreBtnHoverBitmapID
    prebtnattr.bkg_down_texture = attr.PreBtnDownBitmapID
    prebtnattr.bkg_disable_texture = attr.PreBtnDisableBitmapID

	local nextbtn = bkg:GetControlObject("nextbtn")
	local nextbtnattr = nextbtn:GetStyle()
	nextbtnattr.bkg_normal_texture = attr.NextBtnNormalBitmapID
    nextbtnattr.bkg_hover_texture = attr.NextBtnHoverBitmapID
    nextbtnattr.bkg_down_texture = attr.NextBtnDownBitmapID
    nextbtnattr.bkg_disable_texture = attr.NextBtnDisableBitmapID

    local thumbbtn = bkg:GetControlObject("thumbbtn")
    local thumbbtnattr = thumbbtn:GetStyle()
	thumbbtnattr.bkg_normal_texture = attr.ThumbBtnNormalBitmapID
    thumbbtnattr.bkg_hover_texture = attr.ThumbBtnHoverBitmapID
    thumbbtnattr.bkg_down_texture = attr.ThumbBtnDownBitmapID
    thumbbtnattr.bkg_disable_texture = attr.ThumbBtnDisableBitmapID
	
	bkg:UpdateUI()
	prebtn:UpdateUI()
	nextbtn:UpdateUI()
	thumbbtn:UpdateUI()
	
	local bkgdown = bkg:GetControlObject("bkgdownarea")
	bkgdown:SetTextureID(attr.BkgDownBitmapID)
end


function OnInitControl(self)
	local attr = self:GetAttribute()
	attr.ScrollPos = attr.RangeBegin
	rawForceRefresh(self)
	if not self:GetVisible() then
		OnVisibleChange(self,false)
	end
	if not self:GetEnable() then
		OnEnableChange(self,false)
	end
end


function rawSetScrollPos(self,pos)
	local attr = self:GetAttribute()
	if pos < attr.RangeBegin then
		pos = attr.RangeBegin
	elseif pos > (attr.RangeEnd - attr.PageSize) then
		pos = (attr.RangeEnd - attr.PageSize)
	end
	attr.ScrollPos = pos
end

function SetScrollPos(self,pos)
	local attr = self:GetAttribute()
	if attr.ScrollPos ~= pos then
		rawSetScrollPos(self,pos)
		updateThumbPos(self)
	end
end

function GetScrollPos(self)
	local attr = self:GetAttribute()
	return attr.ScrollPos
end
			
function SetScrollRange(self,rbegin,rend)
	local attr = self:GetAttribute()
	if rbegin ~= attr.RangeBegin or rend ~= attr.RangeEnd then
		attr.RangeBegin = rbegin
		attr.RangeEnd = rend
		rawSetScrollPos(self,attr.ScrollPos)
		rawForceRefresh(self)
	end
end

function GetScrollRange(self)
	local attr = self:GetAttribute()
	return attr.RangeBegin, attr.RangeEnd
end
			
function GetPageSize(self)
	local attr = self:GetAttribute()
	return attr.PageSize
end

function SetPageSize(self,pgsize)
	local attr = self:GetAttribute()
	if pgsize ~= attr.PageSize then
		if pgsize > attr.Range then
			pgsize = attr.Range
		elseif pgsize <= 0 then
			pgsize = 1
		end
		attr.PageSize = pgsize
		rawForceRefresh(self)
	end
end
			
function SetNavigationSpeed(self,speed)
	local attr = self:GetAttribute()
	attr.NavigationSpeed = speed
end

function GetNavigationSpeed(self)
	local attr = self:GetAttribute()
	return attr.NavigationSpeed
end


function beginScroll(self,spd,acc,rbegin,rend)
	local attr = self:GetAttribute()
	local ani = attr.ScrollAniTemplate:CreateInstance()
	ani:Bind(self)
	ani:SetRange(rbegin,rend)
	ani:SetSpeed(spd,acc)
	local tr = self:GetOwner()
	tr:AddAnimation(ani)
	ani:Resume()
	attr.ScrollAni = ani
end

function stopScroll(self)
	local attr = self:GetAttribute()
	local ani = attr.ScrollAni
	if ani ~= nil then
		ani:Stop()
		attr.ScrollAni = nil
	end
end

function PreButton_OnClick(btn)
end

function PreBtn_OnLButtonDown(btn,x,y)
	local bkg = btn:GetFather()
	local bar = (bkg:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	beginScroll(bar,-attr.NavigationSpeed,-attr.NavigationAcceleration)
	attr.PreBtnDown = true
end

function PreBtn_OnLButtonDBClick(btn,x,y)
	PreBtn_OnLButtonDown(btn,x,y)
end

function PreBtn_OnLButtonUp(btn)
	local bkg = btn:GetFather()
	local bar = (bkg:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	stopScroll(bar)
	attr.PreBtnDown = false
end

function PreBtn_OnMouseMove(btn,x,y)
	
end

function NextBtn_OnClick(btn)
	
end

function NextBtn_OnLButtonDown(btn,x,y)
	local bkg = btn:GetFather()
	local bar = (bkg:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	beginScroll(bar,attr.NavigationSpeed,attr.NavigationAcceleration)
	attr.NextBtnDown = true
end

function NextBtn_OnLButtonDBClick(btn,x,y)
	NextBtn_OnLButtonDown(btn,x,y)
end

function NextBtn_OnLButtonUp(btn)
	local bkg = btn:GetFather()
	local bar = (bkg:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	stopScroll(bar)
	attr.NextBtnDown = false
end

function NextBtn_OnMouseMove(btn,x,y)
end

function ThumbBtn_OnMouseMove(btn,x,y,flag)
	local bkg = btn:GetFather()
	local bar = (bkg:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	if attr.ThumbBtnDown == true then
		local offsetx = x - attr.ThumbBtnMousePosX
		local offsety = y - attr.ThumbBtnMousePosY
		attr.TypeImplMethods.offsetThumbPos(bar,offsetx,offsety)
		updateScrollPos(bar)
		bar:FireExtEvent("OnScrollPosChange")
	end
end

function ThumbBtn_OnMouseLeave(btn)
end

function ThumbBtn_OnLButtonDown(btn,x,y)
	local bkg = btn:GetFather()
	local bar = (bkg:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	attr.ThumbBtnDown = true
	attr.ThumbBtnMousePosX = x
	attr.ThumbBtnMousePosY = y
end

function ThumbBtn_OnLButtonUp(btn)
	local bkg = btn:GetFather()
	local bar = (bkg:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	attr.ThumbBtnDown = false
end

function DownArea_OnLButtonDown(bkgdown)
	bkgdown:RouteToFather()
end

function DownArea_OnLButtonUp(bkgdown)
	bkgdown:RouteToFather()
end

function DownArea_OnMouseWheel(bkgdown)
	bkgdown:RouteToFather()
end

function DownArea_OnLButtonDBClick(bkgdown)
	bkgdown:RouteToFather()
end

function Bkg_OnClick(btn)
end

function Bkg_OnLButtonDown(btn,x,y)
	local bar = (btn:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	local pos = attr.TypeImplMethods.getMouseThumbRelation(bar,x,y)
	local scrollpos = attr.TypeImplMethods.getScrollPosFromPoint(bar,x,y)
	if pos ~= "on" then
		if pos == "pre" then
			beginScroll(bar,-attr.PageSize,0,scrollpos)
		elseif pos == "next" then
			beginScroll(bar,attr.PageSize,0,nil,scrollpos)
		end
		attr.BkgDown = true
		attr.BkgDownPos = pos
	end
end

function Bkg_OnLButtonUp(btn)
	local bar = (btn:GetFather()):GetOwnerControl()
	local attr = bar:GetAttribute()
	if attr.BkgDown == true then
		stopScroll(bar)
		attr.BkgDown = false
		attr.TypeImplMethods.updateBkgDownArea(bar)
	end
end

function Bkg_OnMouseWheel(btn,x,y,distance)
	local bar = btn:GetOwnerControl()
	OnMouseWheel(bar,x,y,distance)
end

function ThumbBtn_OnMouseWheel(btn,x,y,distance)
	local bar = (btn:GetFather()):GetOwnerControl()
	OnMouseWheel(bar,x,y,distance)
end

function PreBtn_OnMouseWheel(btn,x,y,distance)
	local bar = (btn:GetFather()):GetOwnerControl()
	OnMouseWheel(bar,x,y,distance)
end

function NextBtn_OnMouseWheel(btn,x,y,distance)
	local bar = (btn:GetFather()):GetOwnerControl()
	OnMouseWheel(bar,x,y,distance)
end

function OnMouseWheel(self,x,y,distance)
	local attr = self:GetAttribute()
	attr.TypeImplMethods.onMouseWheel(self,-(attr.WheelSpeed * distance / 120) )
	self:FireExtEvent("OnScrollPosChange")
end

function OnEnableChange(self,enable)
	local ctrl = self:GetControlObject("ctrl")
	ctrl:SetEnable(enable)
	ctrl:SetChildrenEnable(enble)
	if self:GetVisible() then
		local thumb = (ctrl:GetControlObject("bkg")):GetControlObject("thumbbtn")
		thumb:SetVisible(enable)
	end
end

function OnVisibleChange(self,visible)
	local ctrl = self:GetControlObject("ctrl")
	ctrl:SetVisible(visible)
	ctrl:SetChildrenVisible(visible)
end

function OnPosChange(self)
	rawForceRefresh(self)
end
