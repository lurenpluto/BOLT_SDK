function OnInitControl(self)
	local attr = self:GetAttribute()
	
	--local function OnSchemeChange(skinManager)
	--	self:LoadSkin(skinManager)
	--end
	
	--local skinManager = XLGetObject("Xunlei.Thunder.SkinManager")
	--attr.ChangeSchemeCookie = skinManager:AttachSchemeChangeEvent(OnSchemeChange)
	
	if attr.WhiteShadow ~= nil then
		local whiteShadowObj = self:GetControlObject("whiteshadow")
		whiteShadowObj:SetTextureID(attr.WhiteShadow)
	end
	
	if attr.UseLight then
		local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
		local lightTemplate = templateMananger:GetTemplate("Thunder.Bkg.Light", "ObjectTemplate")
		local obj = lightTemplate:CreateInstance("light")
		
		local imagebkg = self:GetControlObject("imagebkg")
		imagebkg:AddChild(obj)
	end
	
	if attr.Mask ~= nil and string.len(attr.Mask) > 0 then
		local maskObj = self:GetControlObject("mask")
		maskObj:SetMaskID("bitmap", attr.Mask, "r", false, nil)
		--maskObj:SetInheritMask(true, true)
	end
	
	--self:LoadSkin(skinManager)
end

function OnDestroy(self)
	local attr = self:GetAttribute()
	if attr.ChangeSchemeCookie ~= nil then
		--local skinManager = XLGetObject("Xunlei.Thunder.SkinManager")
		--skinManager:RemoveSchemeChangeEvent(attr.ChangeSchemeCookie)
	end	
end

function OnAbsPosChange(self)

end

function AdjustFillObjPos(self)
	local left,top,right,bottom = self:GetObjPos()
	
	local attr = self:GetAttribute()
	local fillPos = attr.FillPercent
	if fillPos < 0 then
		fillPos = 0
	elseif fillPos > 100 then
		fillPos = 100
	end
	
	local splitterPos = (bottom - top) * fillPos / 100
	
	local topfill = self:GetControlObject("topfill")
	topfill:SetObjPos(0,0,right - left,splitterPos)
	
	local bottomfill = self:GetControlObject("bottomfill")
	bottomfill:SetObjPos(0, splitterPos, right - left, bottom - top)
end

function SetBkgType(self, type)

	local attr = self:GetAttribute()
	if type == 0 then
		if attr.Type == 0 then
			return
		end
	else
		if attr.Type == 1 then
			return
		end
	end
	
	attr.Type = type
	
	local fillbkg = self:GetControlObject("fillbkg")
	local imagebkg = self:GetControlObject("imagebkg")
	
	if type == 0 then
		fillbkg:SetChildrenVisible(true)
		imagebkg:SetChildrenVisible(false)
	else
		fillbkg:SetChildrenVisible(false)
		imagebkg:SetChildrenVisible(true)
	end
end

function SetBkgImage(self, image, maincolor)
	local attr = self:GetAttribute()
	
	local imageObj = self:GetControlObject("image")
	local imageObj2 = self:GetControlObject("image2")
	
	if image == nil then
		imageObj:SetBitmap(nil)
		imageObj2:SetBitmap(nil)
		return
	end
	
	local colorFill = self:GetControlObject("colorfill")
	colorFill:SetSrcColor(maincolor)
	
	local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")	
	local ownerTree = self:GetOwner()
	
	if attr.CurImage == nil then
		attr.CurImage = imageObj:GetID()
		imageObj:SetBitmap(image)
		imageObj:SetAlpha(0)
		
		local aniAlphaShow = aniFactory:CreateAnimation("AlphaChangeAnimation")
		aniAlphaShow:BindRenderObj(imageObj)
		aniAlphaShow:SetTotalTime(300)
		aniAlphaShow:SetKeyFrameAlpha(0,255)
		ownerTree:AddAnimation(aniAlphaShow)
		aniAlphaShow:Resume()
	else
		local curImage, nextImage
		if attr.CurImage == imageObj:GetID() then
			curImage = imageObj
			nextImage = imageObj2
		else
			curImage = imageObj2
			nextImage = imageObj
		end
		
		nextImage:SetBitmap(image)
		attr.CurImage = nextImage:GetID()
		nextImage:SetAlpha(0)
		
		local aniAlphaHide = aniFactory:CreateAnimation("AlphaChangeAnimation")
		aniAlphaHide:BindRenderObj(curImage)
		aniAlphaHide:SetTotalTime(300)
		aniAlphaHide:SetKeyFrameAlpha(255,0)
		ownerTree:AddAnimation(aniAlphaHide)
		aniAlphaHide:Resume()
			
		local aniAlphaShow = aniFactory:CreateAnimation("AlphaChangeAnimation")
		aniAlphaShow:BindRenderObj(nextImage)
		aniAlphaShow:SetTotalTime(700)
		aniAlphaShow:SetKeyFrameAlpha(0,255)
		ownerTree:AddAnimation(aniAlphaShow)
		aniAlphaShow:Resume()
	end
end

function LoadSkin(self,skinManager)
	local attr = self:GetAttribute()
	
	local curScheme = skinManager:GetCurrentScheme()
	
	local type = 0
	
	if curScheme ~= nil then
		local bkgImage = curScheme:GetBkg()
		if bkgImage ~= nil then
			type = 1
			local mainColor = curScheme:GetBkgMainColor()
			self:SetBkgImage(bkgImage, mainColor)
		else
			self:SetBkgImage(nil)
		end
	else
		self:SetBkgImage(nil)
	end
	
	if type == 0 then
		local shadingBkgObj = self:GetControlObject("shading.bkg")
		if shadingBkgObj == nil then
			self:InitShadingBkg()
		end
	end
	
	if attr.UseLight then
		local bottomleftLight = self:GetControlObject("light.bottomleft")
		if bottomleftLight and type == 0 then
			bottomleftLight:SetVisible(false)
		else
			bottomleftLight:SetVisible(true)
		end
	end
	
	self:SetBkgType(type)
end

function InitShadingBkg(self)
	local attr = self:GetAttribute()
	if attr.BkgTemplate == nil then
		return
	end
	
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	
	local shadingTemplate = templateMananger:GetTemplate(attr.BkgTemplate, "ObjectTemplate")
	if shadingTemplate == nil then
		return
	end
	
	local obj = shadingTemplate:CreateInstance("shading.bkg")
	local fillBkgObj = self:GetControlObject("fillbkg")
	fillBkgObj:AddChild(obj)
end
