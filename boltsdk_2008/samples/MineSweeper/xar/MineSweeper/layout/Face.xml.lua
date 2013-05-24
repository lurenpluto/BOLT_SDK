function OnLButtonDown(self)
	local image = self:GetControlObject("image")
	image:SetResID("bitmap.face.push")
end

function OnLButtonUp(self)
	local image = self:GetControlObject("image")
	image:SetResID("bitmap.face.normal")
	
	self:FireExtEvent("OnClick")
end

function SetState(self, state)
	local image = self:GetControlObject("image")
	if state == "success" then
		image:SetResID("bitmap.face.winner")
	elseif state == "amazed" then
		image:SetResID("bitmap.face.amazed")
	elseif state == "normal" then
		image:SetResID("bitmap.face.normal")
	elseif state == "fail" then
		image:SetResID("bitmap.face.fail")
	else
		image:SetResID("bitmap.face.normal")
	end
end