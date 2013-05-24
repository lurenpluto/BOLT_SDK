
function GetText(self)
    local attr = self:GetAttribute()
    return attr.Text
end

function SetText(self, text)
	if not text then
		return
	end
    local attr = self:GetAttribute()
    attr.Text = text
    local t = self:GetControlObject("frame.text")
    t:SetText(text)
	local line = self:GetControlObject("frame.line")
	if text == "" then
		line:SetObjPos(10, 0, "father.right-10", 1)
	else
		local textwidth, textheight = t:GetTextExtent()
		local tleft, ttop, tright, tbottom = t:GetObjPos()
		line:SetObjPos(tleft + textwidth, 0, "father.right-10", 1)
	end
end

function GetTextColorID(self)
    local attr = self:GetAttribute()
    return attr.TextColor
end

function SetTextColorID(self, textColorID)
	if not textColorID then
		return
	end
    local attr = self:GetAttribute()
    attr.TextColor = textColorID
    local t = self:GetControlObject("frame.text")
    t:SetTextColorResID(attr.TextColor)
end

function GetTextFont(self)
    local attr = self:GetAttribute()
    return attr.Font
end

function SetTextFont(self, font)
	if not font then
		return
	end
    local attr = self:GetAttribute()
    attr.Font = font
    local t = self:GetControlObject("frame.text")
    t:SetTextColorResID(attr.Font)
end

function OnInitControl(self)
	local owner = self:GetOwnerControl()
    local attr = owner:GetAttribute()
	self:SetTextFontResID(attr.Font)
	self:SetTextColorResID(attr.TextColor)
	if attr.Text then
		owner:SetText(attr.Text)
	end
end