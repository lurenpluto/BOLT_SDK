--local function:
--NowState 0:Normal, 1:Down, 2:Disable, 3:Hover
local function UpdateRes(self, newState)
    local attr = self:GetAttribute()
    
	local boxBitmap, tickBitmap, TextColorID, TextFontID
	
	if newState == "down" then
	    boxBitmap = attr.BoxDownBitmapID
		TextColorID = attr.TextDownColorID
		TextFontID = attr.TextDownFontID
		tickBitmap = attr.TickDownBitmapID
	elseif newState == "disable" then
	    boxBitmap = attr.BoxDisableBitmapID
		TextColorID = attr.TextDisableColorID
		TextFontID = attr.TextDisableFontID
		tickBitmap = attr.TickDisableBitmapID
	elseif newState == "hover" then
	    boxBitmap = attr.BoxHoverBitmapID
		TextColorID = attr.TextHoverColorID
		TextFontID = attr.TextHoverFontID
		tickBitmap = attr.TickHoverBitmapID
	else 
	    boxBitmap = attr.BoxNormalBitmapID
		TextColorID = attr.TextNormalColorID
		TextFontID = attr.TextNormalFontID
		tickBitmap = attr.TickNormalBitmapID
		newState = "normal"
	end
	
	local boxBkg = self:GetControlObject("boximage")
	local tickimage = self:GetControlObject("tickimage")
	local textObj = self:GetControlObject("text")
	
	if boxBitmap ~= "" and boxBkg then
	    boxBkg:SetResID(boxBitmap)
	end
	
	if tickBitmap ~= "" and tickimage then
	    tickimage:SetResID(tickBitmap)
	end
	
	if TextColorID ~= "" and textObj then
	    textObj:SetTextColorResID(TextColorID)    
	end
	
	if TextFontID ~= "" and textObj then
	    textObj:SetTextFontResID(TextFontID)
	end
	
	attr.NowState = newState
end
--public function:

function SetCheck(self, check)
    local attr = self:GetAttribute()
    if check == nil then
        check = false
    end
    if check == self:GetCheck() then
        return
    end
    attr.Check = check
    
	local tickBkg = self:GetControlObject("tickimage")
	if check then
		tickBkg:SetVisible(true)
	else
		tickBkg:SetVisible(false)
	end        
    self:FireExtEvent("OnCheck",attr.Check, bClick)
end

function GetCheck(self)
    local attr = self:GetAttribute()
    return attr.Check
end

function SetText(self, text)
	local attr = self:GetAttribute()
	textobj = self:GetControlObject("text")
	textobj:SetText(attr.Text)
	attr.Text = text
end

function GetText(self)
	local attr = self:GetAttribute()
	return attr.Text;
end

function UpdateUI(self)
    local attr = self:GetAttribute()
	UpdateRes(self, attr.NowState)
end

-- private function:
function OnInitControl(self)
    local attr = self:GetAttribute()
	
	if attr.Text then
        SetText(self, attr.Text)
    end
	
	if not self:GetVisible() then
		local ctrlObj = self:GetControlObject("ctrl")
		ctrlObj:SetVisible(false)
		ctrlObj:SetChildrenVisible(false)
	end
	
	if not self:GetEnable() then
		local ctrlObj = self:GetControlObject("ctrl")
		ctrlObj:SetEnable(false)
		ctrlObj:SetChildrenEnable(false)
		attr.NowState = "disable"
	end

	UpdateRes(self, attr.NowState)
end

function OnLButtonDown(self)
    local attr = self:GetAttribute()
    UpdateRes(self,"down")
    self:SetCaptureMouse(true)
end

function OnLButtonUp(self)
    local attr = self:GetAttribute()
    if attr.NowState== "down" then
		local ret = self:FireExtEvent("OnBeforeClick")
		if ret~= false then
			self:SetCheck(not attr.Check)
			UpdateRes(self,"normal")
		end
    end
    self:SetCaptureMouse(false)
end

function OnMouseMove(self)
    local attr = self:GetAttribute()
    if attr.NowState == "normal" then
        UpdateRes(self,"hover")
    end
end

function OnMouseLeave(self)
    local attr = self:GetAttribute()
	UpdateRes(self,"normal")
	self:SetCaptureMouse(false)
end

function OnVisibleChange(self, visible)
	local ctrlObj = self:GetControlObject("ctrl")
	ctrlObj:SetVisible(visible)
	ctrlObj:SetChildrenVisible(visible)
end

function OnEnableChange(self, enable)
    local attr = self:GetAttribute()
	local ctrlObj = self:GetControlObject("ctrl")
	ctrlObj:SetEnable(enable)
	ctrlObj:SetChildrenEnable(enable)
	attr.Enable = enable
	if enable then
	    UpdateRes(self,"normal")
	else
	    UpdateRes(self,"disable")
	end
end
