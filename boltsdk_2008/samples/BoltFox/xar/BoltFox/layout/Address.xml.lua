function Edit_OnKeyDown(self, char)
	if char and char == 13 then
		self:GetOwnerControl():FireExtEvent("OnEnterAddress", self:GetText())
	end
end

function SetAddress(self, url)
	local edit = self:GetControlObject("edit")
	edit:SetText(url)
end

function GetAddress(self)
	local edit = self:GetControlObject("edit")
	return edit:GetText()
end

function go_OnClick(self)
	self:GetOwnerControl():FireExtEvent("OnGo")
end

function stop_OnClick(self)
	self:GetOwnerControl():FireExtEvent("OnStop")
end

function reload_OnClick(self)
	self:GetOwnerControl():FireExtEvent("OnReload")
end

-- function OnControlFocusChange(self, focus)
	-- XLPrint("address OnControlFocusChange focus="..tostring(focus))
	-- if focus then
		-- local edit = self:GetControlObject("edit")
		-- edit:SetFocus(true)
		-- edit:SetSelAll()
	-- end
-- end

-- 貌似没起作用
function OnFocusChange(self)
	self:SetSelAll()
end