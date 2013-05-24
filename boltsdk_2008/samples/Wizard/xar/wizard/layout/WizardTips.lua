function OnHostWndSize(self, sizeType, x, y)
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	rootObject:SetObjPos(0, 0, x, y)
end

function OnLoadingInitControl(self)
	local objectTree = self:GetOwner()
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local popAniT = templateMananger:GetTemplate("webloading.animation","AnimationTemplate")
						
	local popAni = popAniT:CreateInstance()
	popAni:BindImageObj(self)
	objectTree:AddAnimation(popAni)
	popAni:Resume()
end

function OnPreCreateInstance(template, userdata)
end

function OnPostCreateInstance(template, obj, userdata)
end

function OnDelayPopup(self)
end