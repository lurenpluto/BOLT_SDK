local lcSystemButtonIndex = 0

function System_Button_GetMaxState(self)
    local attr = self:GetAttribute()
	return attr.MaxState
end

function System_Button_SetMaxState(self, bMax)
    local attr = self:GetAttribute()
	local maxObj = self:GetControlObject("IDE.System.Button.Max")
	local maxObjattr = maxObj:GetAttribute()  
	if bMax then
	    attr.MaxState = true
		if attr.Max_NormalImageID then		
			maxObjattr.BkgNormalImageID = attr.Max_NormalImageID
			maxObj:SetImageID(attr.Max_NormalImageID)
			maxObj:SetInitImage(attr.Max_NormalImageID, attr.Max_NormalImageID)
		end
		if attr.Max_HoverImageID then		
			maxObjattr.BkgHoverImageID = attr.Max_HoverImageID
		end
		if attr.Max_DownImageID then		
			maxObjattr.BkgDownImageID = attr.Max_DownImageID
		end
		if attr.Max_DisableImageID then		
			maxObjattr.BkgDisableImageID = attr.Max_DisableImageID
		end
		
	else
	    attr.MaxState = false
		if attr.Restore_NormalImageID then		
			maxObjattr.BkgNormalImageID = attr.Restore_NormalImageID
			maxObj:SetImageID(attr.Restore_NorImageID)
			maxObj:SetInitImage(attr.Restore_NormalImageID, attr.Restore_NormalImageID)
		end
		if attr.Restore_HoverImageID then		
			maxObjattr.BkgHoverImageID = attr.Restore_HoverImageID
		end
		if attr.Restore_DownImageID then		
			maxObjattr.BkgDownImageID = attr.Restore_DownImageID
		end
		if attr.Restore_DisableImageID then		
			maxObjattr.BkgDisableImageID = attr.Restore_DisableImageID
		end    
	end
	
	maxObj:SetState(0, true, false)
end

function System_Button_MiniIsShow(self)
    local attr = self:GetAttribute()
	return attr.MiniShow
end

function System_Button_MaxIsShow(self)
    local attr = self:GetAttribute()
	return attr.MaxShow
end

function System_Button_CloseIsShow(self)
    local attr = self:GetAttribute()
	return attr.CloseShow
end

function System_Button_MiniIsEnable(self)
    local attr = self:GetAttribute()
	return attr.MiniEnable
end

function System_Button_MaxIsEnable(self)
    local attr = self:GetAttribute()
	return attr.MaxEnable
end

function System_Button_CloseIsEnable(self)
    local attr = self:GetAttribute()
	return attr.CloseEnable
end

function System_Button_SetMiniEnable(self, bEnable)
    local miniobj = self:GetControlObject("IDE.System.Button.Mini")
    miniobj:SetEnable(bEnable)	
end

function System_Button_SetMaxEnable(self, bEnable)
    local maxobj = self:GetControlObject("IDE.System.Button.Max")
    maxobj:SetEnable(bEnable)	
end

function System_Button_SetCloseEnable(self, bEnable)
    local closeobj = self:GetControlObject("IDE.System.Button.Close")
    closeobj:SetEnable(bEnable)
end

function System_Button_Mini_OnClick(self)
	local owner = self:GetOwnerControl()
    owner:FireExtEvent("OnMinisize")
end

function System_Button_Max_OnClick(self)
    local owner = self:GetOwnerControl()
    local attr = owner:GetAttribute()
	local maxObj = owner:GetControlObject("IDE.System.Button.Max")
	local maxObjattr = maxObj:GetAttribute()
	if attr.MaxState then
	    attr.MaxState = false
		if attr.Restore_NormalImageID then		
			maxObjattr.BkgNormalImageID = attr.Restore_NormalImageID
			maxObj:SetImageID(attr.Restore_NorImageID)
		end
		if attr.Restore_HoverImageID then		
			maxObjattr.BkgHoverImageID = attr.Restore_HoverImageID
		end
		if attr.Restore_DownImageID then		
			maxObjattr.BkgDownImageID = attr.Restore_DownImageID
		end
		if attr.Restore_DisableImageID then		
			maxObjattr.BkgDisableImageID = attr.Restore_DisableImageID
		end
		
		owner:FireExtEvent("OnMaxSize")
	else
	    attr.MaxState = true
		if attr.Max_NormalImageID then		
			maxObjattr.BkgNormalImageID = attr.Max_NormalImageID
			maxObj:SetImageID(attr.Max_NormalImageID)
		end
		if attr.Max_HoverImageID then		
			maxObjattr.BkgHoverImageID = attr.Max_HoverImageID
		end
		if attr.Max_DownImageID then		
			maxObjattr.BkgDownImageID = attr.Max_DownImageID
		end
		if attr.Max_DisableImageID then		
			maxObjattr.BkgDisableImageID = attr.Max_DisableImageID
		end
		owner:FireExtEvent("OnReStore")
	end
	
end

function System_Button_Close_OnClick(self)
    local owner = self:GetOwnerControl()
    owner:FireExtEvent("OnClose")
end



function System_Button_Ctrl_OnBind(self)
    local attr = self:GetAttribute()
	
	local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	local xarFactory = xarManager:GetXARFactory()
	
	local miniobj = nil
	local maxobj = nil
	local closeobj = nil
	
	local lcIndex = 0
	
	if attr.MiniShow then
	    miniobj = xarFactory:CreateUIObject("IDE.System.Button.Mini", "IDE.System.ItemEx")
		
		local objattr = miniobj:GetAttribute()
		objattr.type = 0
		
		if attr.Mini_NormalImageID then
		    objattr.BkgNormalImageID = attr.Mini_NormalImageID
		end
		if attr.Mini_HoverImageID then
		    objattr.BkgHoverImageID = attr.Mini_HoverImageID
		end
		if attr.Mini_DownImageID then
		    objattr.BkgDownImageID = attr.Mini_DownImageID
		end
		if attr.Mini_DisableImageID then
		    objattr.BkgDisableImageID = attr.Mini_DisableImageID
		end
		miniobj:AttachListener("OnClick", true, self.System_Button_Mini_OnClick)
		
		miniobj:SetEnable(attr.MiniEnable)
		self:AddChild(miniobj)
		lcIndex = lcIndex + 1
	end
	
	if attr.MaxShow then
	    maxobj = xarFactory:CreateUIObject("IDE.System.Button.Max", "IDE.System.ItemEx")
		
		local objattr = maxobj:GetAttribute()
		objattr.type = 1
		
		if attr.MaxState then --显示最大化
			if attr.Max_NormalImageID then
				objattr.BkgNormalImageID = attr.Max_NormalImageID
			end
			if attr.Max_HoverImageID then
				objattr.BkgHoverImageID = attr.Max_HoverImageID
			end
			if attr.Max_DownImageID then
				objattr.BkgDownImageID = attr.Max_DownImageID
			end
			if attr.Max_DisableImageID then
				objattr.BkgDisableImageID = attr.Max_DisableImageID
			end
		    
		else--显示reset
			if attr.Restore_NormalImageID then
				objattr.BkgNormalImageID = attr.Restore_NormalImageID
			end
			if attr.Restore_HoverImageID then
				objattr.BkgHoverImageID = attr.Restore_HoverImageID
			end
			if attr.Restore_DownImageID then
				objattr.BkgDownImageID = attr.Restore_DownImageID
			end
			if attr.Restore_DisableImageID then
				objattr.BkgDisableImageID = attr.Restore_DisableImageID
			end
		end
		maxobj:AttachListener("OnClick", true, self.System_Button_Max_OnClick)
		
		maxobj:SetEnable(attr.MaxEnable)
		self:AddChild(maxobj)
		lcIndex = lcIndex + 1
	end
	
	if attr.CloseShow then
	    closeobj = xarFactory:CreateUIObject("IDE.System.Button.Close", "IDE.System.ItemEx")
		
		local objattr = closeobj:GetAttribute()
		objattr.type = 2
        if attr.MiniShow == false and attr.MaxShow == false then
		    if attr.SingleClose_NormalImageID then
				objattr.BkgNormalImageID = attr.SingleClose_NormalImageID
			end
			if attr.SingleClose_HoverImageID then
				objattr.BkgHoverImageID = attr.SingleClose_HoverImageID
			end
			if attr.SingleClose_DownImageID then
				objattr.BkgDownImageID = attr.SingleClose_DownImageID
			end
			if attr.SingleClose_DisableImageID then
				objattr.BkgDisableImageID = attr.SingleClose_DisableImageID
			end
            		
		else
			if attr.Close_NormalImageID then
				objattr.BkgNormalImageID = attr.Close_NormalImageID
			end
			if attr.Close_HoverImageID then
				objattr.BkgHoverImageID = attr.Close_HoverImageID
			end
			if attr.Close_DownImageID then
				objattr.BkgDownImageID = attr.Close_DownImageID
			end
			if attr.Close_DisableImageID then
				objattr.BkgDisableImageID = attr.Close_DisableImageID
			end
        end		
		
		
		closeobj:AttachListener("OnClick", true, self.System_Button_Close_OnClick)
		
		closeobj:SetEnable(attr.CloseEnable)
		self:AddChild(closeobj)
		lcIndex = lcIndex + 1
	end
	
	local lcLeft = 0
	local left, top, right, bottom = self:GetObjPos()
	local SelfWidth = right - left
	local SelfHeight = bottom - top
	local ItemWidth = SelfWidth / lcIndex
	if miniobj then
	    if attr.ItemWidthIsSame then
			miniobj:SetObjPos(0, 0, ItemWidth, SelfHeight)
			lcLeft = lcLeft + ItemWidth
		else
		    miniobj:SetObjPos(0, 0, attr.MiniWidth, SelfHeight)
			lcLeft = lcLeft + attr.MiniWidth
		end
	end
	if maxobj then
	    if attr.ItemWidthIsSame then
			maxobj:SetObjPos(lcLeft, 0, lcLeft + ItemWidth, SelfHeight)
			lcLeft = lcLeft + ItemWidth
		else
		    maxobj:SetObjPos(lcLeft, 0, lcLeft + attr.MaxWidth, SelfHeight)
			lcLeft = lcLeft + attr.MaxWidth
		end
	end
	if closeobj then
	    if attr.ItemWidthIsSame then
			closeobj:SetObjPos(lcLeft, 0, lcLeft + ItemWidth, SelfHeight)
			lcLeft = lcLeft + ItemWidth
		else
		    closeobj:SetObjPos(lcLeft, 0, lcLeft + attr.CloseWidth, SelfHeight)
			lcLeft = lcLeft + attr.CloseWidth
		end
	end
	if attr.MiniShow == false and attr.MaxShow == false then
		closeobj:SetOldImagePos(1)
    end	
end

function SetInitImage(self, bkgid, oldbkgid)
    local oldBkg = self:GetControlObject("old.image")
    local bkg = self:GetControlObject("item.bkg.image")
	if bkgid then
	     bkg:SetResID(bkgid)
	end
	if oldbkgid then
	     oldBkg:SetResID(oldbkgid)
	end
end

function SetState(self, newState, forceUpdate, noAni)
    if self == nil then
	    return
	end
    local attr = self:GetAttribute()
	if attr == nil then
	    return 
	end
    if forceUpdate and newState ~= attr.NowState then
        attr.NowState = newState
        local ownerTree = self:GetOwner()
        local oldBkg = self:GetControlObject("old.image")
        local bkg = self:GetControlObject("item.bkg.image")
		if bkg then
			if newState == 0 then		
			elseif newState == 1 then
				oldBkg:SetResID(attr.BkgHoverImageID)
			elseif newState == 2 then
				oldBkg:SetResID(attr.BkgDownImageID)				
			elseif newState == 3 then
				oldBkg:SetResID(attr.BkgDisableImageID)
			end	
			
			local function OnFinish(self, old, new)
				if new == 4 then
					attr.Animation = nil
					if attr.NowState == 0 then
						oldBkg:SetAlpha(0)
					elseif attr.NowState ~= 3 then
						oldBkg:SetAlpha(255)
						
					end					
				end
			end
           	
			if oldBkg  and attr.Animation == nil then
			    
				local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")	
				local aniAlpha = aniFactory:CreateAnimation("AlphaChangeAnimation")
				aniAlpha:BindRenderObj(oldBkg)
				aniAlpha:SetTotalTime(200)
				if newState == 0 then
					aniAlpha:SetKeyFrameAlpha(255,0)
			    elseif newState == 2 then
			        aniAlpha:SetKeyFrameAlpha(150,255)
				elseif newState ~= 3 then
				    aniAlpha:SetKeyFrameAlpha(0,255)
				 
				end
				ownerTree:AddAnimation(aniAlpha)
				aniAlpha:AttachListener(true,OnFinish)
				attr.Animation = aniAlpha
				aniAlpha:Resume()
			else
			    
			end			
		end
		
    end
end

function SetOldImagePos(self, pos)
    local oldImageObj = self:GetControlObject("old.image")
	local left, top, right, bottom = oldImageObj:GetObjPos()
	oldImageObj:SetObjPos(left + pos, top, right + pos, bottom )
end

function System_Item_OnBind(self)
    local attr = self:GetAttribute()
	local itembkgobj = self:GetControlObject("item.bkg.image")
	local oldImageObj = self:GetControlObject("old.image")
	if itembkgobj then
	    if attr.State == 0 and attr.Enable then
		    if attr.BkgNormalImageID then
			    itembkgobj:SetResID(attr.BkgNormalImageID)
				oldImageObj:SetResID(attr.BkgNormalImageID)
				oldImageObj:SetAlpha(0)
			end
			oldImageObj:SetEnable(true)
			itembkgobj:SetEnable(true)
		elseif attr.State == 1 and attr.Enable then
		    if attr.BkgHoverImageID then
			    oldImageObj:SetResID(attr.BkgHoverImageID)
			end
			oldImageObj:SetEnable(true)
			itembkgobj:SetEnable(true)
		elseif attr.State == 2 and attr.Enable then
		    if attr.BkgDownImageID then
			    oldImageObj:SetResID(attr.BkgDownImageID)
			end
			oldImageObj:SetEnable(true)
			itembkgobj:SetEnable(true)
		else
		    if attr.BkgDisableImageID then
			    oldImageObj:SetResID(attr.BkgDisableImageID)
				oldImageObj:SetAlpha(255)
				oldImageObj:SetEnable(false)
				itembkgobj:SetEnable(false)
			end
		end
	end
end

function System_Item_OnInitControl(self)
    SetState(self, 0, true, true)
end

function System_Item_OnLButtonDown(self)
   -- local owner = self:GetOwnerControl()
    local attr = self:GetAttribute()
	local itembkgimageobj = self:GetControlObject("item.bkg.image")
	
	if attr.Enable and attr.NowState ~= 2 then
	    if attr.BkgDownImageID then
		    local oldbkgid = itembkgimageobj:GetResID()
			self:SetState(2, true, true)
		end
	end
end

function System_Item_OnLButtonUp(self)
    --local owner = self:GetOwnerControl()
    local attr = self:GetAttribute()
	local itembkgimageobj = self:GetControlObject("item.bkg.image")
	local oldImageObj = self:GetControlObject("old.image")
	if attr.Enable then
	    if attr.NowState == 2 then
			self:FireExtEvent("OnClick")
			oldImageObj:SetResID(attr.BkgNormalImageID)
			itembkgimageobj:SetResID(attr.BkgNormalImageID)
		end
		if attr.BkgNormalImageID then
		    local oldbkgid = itembkgimageobj:GetResID()
			self:SetState(0, true, true)
			
		end
	end
end

function System_Item_OnMouseMove(self, x, y)

    local attr = self:GetAttribute()
	local itembkgimageobj = self:GetControlObject("item.bkg.image")
	local layouttextureobj = self:GetControlObject("layout.texture")
	local left, top, right, bottom = layouttextureobj:GetObjPos()
	if attr.Enable and attr.NowState ~= 2 and attr.NowState ~= 1 then
	    if attr.BkgHoverImageID then
			self:SetState(1, true, true)
		end
	end
end

function System_Item_OnMouseLeave(self, x, y)

    local attr = self:GetAttribute()
	local itembkgimageobj = self:GetControlObject("item.bkg.image")
	local layouttextureobj = self:GetControlObject("layout.texture")
	local left, top, right, bottom = layouttextureobj:GetObjPos()
	if attr.Enable then
	    if attr.BkgNormalImageID then
			self:SetState(0, true, true)
		end
	end
	
end

function System_Item_SetEnable(self, bEnable)
    local attr = self:GetAttribute()
	local itembkgimageobj = self:GetControlObject("item.bkg.image")
	local oldImageObj = self:GetControlObject("old.image")
	attr.Enable = bEnable
    if bEnable then  
        if attr.BkgNormalImageID then
			itembkgimageobj:SetResID(attr.BkgNormalImageID)
			oldImageObj:SetResID(attr.BkgNormalImageID)
			oldImageObj:SetAlpha(0)
		end	
		itembkgimageobj:SetEnable(true)
		oldImageObj:SetEnable(true)
		attr.NowState = 0
	else
	    if attr.BkgDisableImageID then
			itembkgimageobj:SetResID(attr.BkgDisableImageID)
			oldImageObj:SetResID(attr.BkgDisableImageID)
			oldImageObj:SetAlpha(255)
			itembkgimageobj:SetEnable(false)
			oldImageObj:SetEnable(false)
			attr.NowState = 3
			
		end
	end
end

function System_Item_SetImageID(self, ImgeID)
    local itembkgobj = self:GetControlObject("item.bkg")
	if itembkgobj then
	    if ImgeID then
		    itembkgobj:SetResID(ImgeID)
		end
	end
end