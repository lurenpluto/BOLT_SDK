function SetResID(self, resid, hasAni)
	local attr = self:GetAttribute()
	if attr.resid ~= resid then
		local image = self:GetControlObject("image")
		--image:SetResID(resid)
		if hasAni then
			if attr.resid and resid then
				RunTurnAni(self, attr.resid, resid)
			end
		else
			image:SetResID(resid)
		end
		
		
		attr.resid = resid
	end
end

function RunTurnAni(self, oldid, newid)
	XLPrint(string.format("RunTurnAni %s %s", oldid, newid))
	local imageAni = self:GetControlObject("imageAni")
	local image = self:GetControlObject("image")
	local owner = self:GetOwner()
	assert(owner)
	
	local left, top, right, bottom = image:GetObjPos()
	imageAni:SetObjPos(left, top, right, bottom)
	imageAni:SetResID(oldid)
	
	image:SetResID(newid)
	image:SetVisible(false)
	--imageAni:SetVisible(false)
	
	local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory") 
	local ani = aniFactory:CreateAnimation("TurnObjectAnimation")
--	ani:BindFrontObj(image, nil)
--	ani:BindBackObj(imageAni)
	ani:BindRenderObj(imageAni, image)
	ani:SetFlag("RoundX")
	ani:SetTotalTime(500)
	owner:AddAnimation(ani) 
	
	ani:Resume()
	
	
end

function OnLButtonDown(self)
	self:SetCaptureMouse(true)
	local attr = self:GetAttribute()
	attr.lbtndown = true
end

function OnLButtonUp(self)
	self:SetCaptureMouse(false)
	local attr = self:GetAttribute()
	if attr.lbtndown and attr.rbtndown then
		self:FireExtEvent("OnClick", attr.x, attr.y, "dbclick")
	elseif attr.lbtndown then
		self:FireExtEvent("OnClick", attr.x, attr.y, "lclick")
	end
	
	attr.lbtndown, attr.rbtndown = false, false
end

function OnRButtonDown(self)
	local attr = self:GetAttribute()
	attr.rbtndown = true
end

function OnRButtonUp(self)
	local attr = self:GetAttribute()
	if attr.lbtndown and attr.rbtndown then
		self:FireExtEvent("OnClick", attr.x, attr.y, "dbclick")
	elseif attr.rbtndown then
		self:FireExtEvent("OnClick", attr.x, attr.y, "rclick")
	end
	
	attr.lbtndown, attr.rbtndown = false, false
end