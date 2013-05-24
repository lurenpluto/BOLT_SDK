function SetText(self, text)
	local edit = self:GetControlObject("edit")
	edit:SetText(text)
end

function GetText(self)
	local edit = self:GetControlObject("edit")
	return edit:GetText()
end