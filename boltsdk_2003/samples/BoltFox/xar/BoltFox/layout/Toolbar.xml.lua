function back_OnClick(self)
	self:GetOwnerControl():FireExtEvent("OnBack")
end

function forward_OnClick(self)
	self:GetOwnerControl():FireExtEvent("OnForward")
end