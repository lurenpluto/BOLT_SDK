-- 一个对象的动画
--显示对象
--对象消失
--渐变的改变对象的状态

-- 两个对象的动画
-- 从对象A切换到对象B

--destObj    目标对象  出现对象
--bDestRender   true/false  是否进行渲染   如果是非ImageObject  则需要置为true
--srcObj     源对象   消失对象
--bSrcRender    true/false  是否进行渲染   如果是非ImageObject 则需要置为true
--templateName   模板名称
local function AppearDestObjDisappearSrcObj(self, destObj, bDestRender, srcObj, bSrcRender, lpSrcfunc, lpDestfunc, templateName)
    
	if destObj == nil or srcObj == nil then
	    return false
	end
	
	local tree = destObj:GetOwner()
	if tree then
		local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
		if templateMananger then
		    local animationTemplate = nil
		    if templateName == "" or templateName == nil then
			    animationTemplate = templateMananger:GetTemplate("system.animation.helper.alphachange","AnimationTemplate")
			else
			    animationTemplate = templateMananger:GetTemplate(templateName,"AnimationTemplate")
			end
			
			if animationTemplate then
				local srcinstance = animationTemplate:CreateInstance()
				if srcinstance then
				    srcinstance:SetKeyFrameAlpha(255, 0)
					if bSrcRender == true then
						srcObj:SetVisible(true)
						srcObj:SetChildrenVisible(true)
						srcinstance:BindRenderObj(srcObj, true)
						srcObj:SetVisible(false)
						srcObj:SetChildrenVisible(false)
					else
					    srcinstance:BindRenderObj(srcObj)
					end
					if lpSrcfunc then
						srcinstance:AttachListener(true, lpSrcfunc)
					end
					tree:AddAnimation(srcinstance)
					srcinstance:Resume()
				end
				
				local destinstance = animationTemplate:CreateInstance()
				if destinstance then
				    destinstance:SetKeyFrameAlpha(100, 255)
					if bDestRender == true then
						destObj:SetVisible(true)
						destObj:SetChildrenVisible(true)
						destinstance:BindRenderObj(destObj, true)
						destObj:SetVisible(false)
						destObj:SetChildrenVisible(false)
					else
					    destinstance:BindRenderObj(destObj)
					end
					if lpDestfunc then
						destinstance:AttachListener(true, lpDestfunc)
					end
					tree:AddAnimation(destinstance)
					destinstance:Resume()
					return true
				end
			end
		end
	end
	
end

--bkg 是窗口的背景图片，需要保证该对象有一个father在窗口上
--templateName   模板名称
--bShow   true ：出现动画   false : 消失动画
--lpfunc  是回调函数，用于获得动画的状态

local function ShowWindow(self, bkg, bShow, lpfunc, templateName)
    local rootLayoutObj = nil
	if bkg == nil then
	    return false
	end
	
	local bindTree = bkg:GetOwner()
	if bkg then
	    local left, top, right, bottom = bkg:GetObjPos()
		rootLayoutObj = bkg
		if rootLayoutObj then
		    local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
			local animationTemplate = nil
			if templateName == nil or tmeplateName == "" then
			    if bShow == true then
					animationTemplate = templateMananger:GetTemplate("system.animation.helper.showwindow","AnimationTemplate")
				else
					animationTemplate = templateMananger:GetTemplate("system.animation.helper.hidewindow","AnimationTemplate")
				end
			else
			    animationTemplate = templateMananger:GetTemplate(templateName,"AnimationTemplate")
			end
			
		    if bShow == true then
			    local ShowAnimation = animationTemplate:CreateInstance()
				if ShowAnimation then
				    local attr = ShowAnimation:GetAttribute()
					rootLayoutObj:SetVisible(true)
					rootLayoutObj:SetChildrenVisible(true)
					ShowAnimation:BindObj(rootLayoutObj, true)
					rootLayoutObj:SetVisible(false)
					rootLayoutObj:SetChildrenVisible(false)
					
					attr.BindObj = ShowAnimation:GetRenderObj()
					attr.BindObj:SetDrawMode(1)
					local imageleft, imagetop, imageright, imagebottom = attr.BindObj:GetObjPos()
					attr.ZoomType = 1
					attr.left = left
					attr.top = top
					attr.right = right
					attr.bottom = bottom
					if lpfunc then
					    ShowAnimation:AttachListener(true, lpfunc)
					end
					
					bindTree:AddAnimation(ShowAnimation)
				    ShowAnimation:Resume()
				end
			else
			    local ShowAnimation = animationTemplate:CreateInstance()
				if ShowAnimation then
				    local attr = ShowAnimation:GetAttribute()
					rootLayoutObj:SetVisible(true)
					rootLayoutObj:SetChildrenVisible(true)
					ShowAnimation:BindObj(rootLayoutObj, true)
					rootLayoutObj:SetVisible(false)
					rootLayoutObj:SetChildrenVisible(false)
					attr.BindObj = ShowAnimation:GetRenderObj()
					attr.BindObj:SetDrawMode(1)
					attr.ZoomType = 0
					attr.left = left
					attr.top = top
					attr.right = right
					attr.bottom = bottom
					if lpfunc then
						ShowAnimation:AttachListener(true, lpfunc)
					end
					bindTree:AddAnimation(ShowAnimation)
					ShowAnimation:Resume()
				end
			    
			end
		end
	end
end

--srcObj   源对象 离开区域的对象
--bSrcRender   true/false  是否进行渲染 如果不是一个ImageObject的情况下，需要设置为true
--destObj  目标对象  进入区域的对象
--bDestRender  true/false  是否进行渲染 如果不是一个ImageObject的情况下，需要设置为true
--rectLeft, rectTop, rectRight, rectBottom 区域的大小
--moveType   有四种情况 left: 源对象从区域左侧离开区域，目标对象从区域右侧进入区域
--                      right: 源对象从区域右侧离开区域，目标对象从区域左侧进入区域
--                      top : 源对象从区域顶侧离开区域，目标对象从区域底侧进入区域
--                      bottom: 源对象从区域底册离开区域， 目标对象从区域丁册进入区域
--templateName  动画模板的名称
--lpfunc        AttachListener的回调函数，用于判断动画的状态

local function DestObjIntoAreaSrcObjAwayArea(self, srcObj, bSrcRender, destObj, bDestRender, rectLeft, rectTop, rectRight, rectBottom, moveType, lpfunc , templateName)
    if destObj == nil or srcObj == nil or moveType == nil then
	    return false
	end
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	if templateMananger then
	    local animationTemplate = nil
		if templateName == nil or templateName == "" then
			animationTemplate = templateMananger:GetTemplate("system.animation.helper.cycleobject","AnimationTemplate")
		else
			animationTemplate = templateMananger:GetTemplate(templateName,"AnimationTemplate")
		end
	    
		if animationTemplate then
		    local hAnimation = animationTemplate:CreateInstance()
			if hAnimation then
			    local tree = srcObj:GetOwner()
				local bindSrcObj = srcObj
				local bindDestObj = destObj
				if bSrcRender == true then
				    srcObj:SetVisible(true)
					srcObj:SetChildrenVisible(true)
				    hAnimation:BindSrcObj(srcObj, true)
					srcObj:SetVisible(false)
					srcObj:SetChildrenVisible(false)
					bindSrcObj = hAnimation:GetRenderSrcObj()
				end
				if bDestRender == true then
				    destObj:SetVisible(true)
					destObj:SetChildrenVisible(true)
				    hAnimation:BindDestObj(destObj, true)
					destObj:SetVisible(false)
					destObj:SetChildrenVisible(false)
					bindDestObj = hAnimation:GetRenderDestObj()
					
				end
				hAnimation:BindSrcAndDestObj(bindSrcObj, bindDestObj, moveType, rectLeft, rectTop, rectRight, rectBottom)
				if lpfunc then
				    hAnimation:AttachListener(true, lpfunc)
				end
				tree:AddAnimation(hAnimation)
				hAnimation:Resume()
				return true
			end
		end
	end
end

-- bkgObj   是对象的根对象
--templateName   动画模板名称
--lightWidth    是光环的宽度 默认是26

local function FocusLight(self, bkgObj, templateName, lightWidth)
    if self == nil or bkgObj == nil then
	    return false
	end
	local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	if xarManager then
	    local xarFactory = xarManager:GetXARFactory()
		if xarFactory then
		    local defaultLight = xarFactory:CreateUIObject("","Focus.Light")
			if defaultLight then
			    local attr = defaultLight:GetAttribute()
				attr.animationTemplateName = templateName
				attr.lightWidth = lightWidth
			    defaultLight:SetZorder(200)
				local bkgleft,bkgtop,bkgright,bkgbottom = bkgObj:GetObjPos()
				defaultLight:SetObjPos(-3,-3,bkgright-bkgleft + 3,bkgbottom-bkgtop)
				bkgObj:AddChild(defaultLight)
			end
		end
	end
end

--出现动态效果
--滑落效果，从指定区域的顶端滑落到区域中
--obj 滑落对象
--rectLeft rectTop rectRight rectBottom 指定的区域
--lpfunc   响应动画状态改变的方法
--templateName  模板名称，如果为nil，则利用系统默认的模板
local function AppearObjectSlideDown(self, obj, rectLeft, rectTop, rectRight, rectBottom, lpfunc, templateName)
    if self == nil or obj == nil or rectLeft == nil or rectTop == nil or rectRight == nil or rectBottom == nil then
	    return false
	end
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	if templateMananger then
	    local animationTemplate =  nil
	    if templateName == nil or templateName == "" then
		    animationTemplate = templateMananger:GetTemplate("system.animation.helper.slidedownobject","AnimationTemplate")
		else
		    animationTemplate = templateMananger:GetTemplate(templateName,"AnimationTemplate")
		end
		if animationTemplate then
		    local hAnimation = animationTemplate:CreateInstance()
			if hAnimation then
			    local tree = obj:GetOwner()
				if lpfunc then
				    hAnimation:AttachListener(true, lpfunc)
				end
				local objLeft, objTop, objRight, objBottom = obj:GetObjPos()
				local objWidth = objRight - objLeft
				local objHeight = objBottom - objTop
				hAnimation:SetKeyFrameRect(rectLeft + math.floor((rectRight - rectLeft - objWidth)/2), rectTop - objHeight, rectLeft + math.floor((rectRight - rectLeft - objWidth)/2) + objWidth ,rectTop, rectLeft + math.floor((rectRight - rectLeft - objWidth)/2), rectBottom - objHeight, rectLeft + math.floor((rectRight - rectLeft - objWidth)/2) + objWidth ,rectBottom)
			    tree:AddAnimation(hAnimation)
				hAnimation:BindObj(obj)
				hAnimation:Resume()
			end
		end
	end
end

local function CreateAniFromTypeName(typeName)
	return nil
end
--totalTime如果不是nil,则一定会生效而无视templateName.当templateName为nil时，会尝试解读aniType
local function AniChangeObjectState(changeFunction,obj,templateName,isIncludeChild,totalTime,aniType)
	local render = XLGetObject("Xunlei.Render")
	local objFactory = XLGetObject("Xunlei.ObjectFactory")
	local oldImage = objFactory:CreateObject("ImageObject")
	local newImage = objFactory:CreateObject("ImageObject")
	local objTree = obj:GetOwner()
	if objTree == nil then
		return
	end
	
	obj:GetFather():AddChild(oldImage)
	obj:GetFather():AddChild(newImage)
	
	render:RenderObject(obj,oldObj)
	changeFunction(obj)
	render:RenderObject(obj,newImage)
	obj:SetVisible(false)
	if isIncludeChild then
		obj:SetChildrenVisible(false)
	end
	
	local templateManager = nil
	local ani = nil
	if templateName ~= nil then
		templateManager = XLGetObject("Xunlei.Ani");
		aniTemplate = templateManager:GetTemplate(templateName)
		--模板必须是一个从对象A过渡到对象B类型的动画
		ani = aniTemplate:CreateInstance()
	else
		ani = CreateAniFromTypeName(aniType)
		ani:SetTotalTime(300)
	end
	
	ani:BindObj(oldImage,newImage)
	if totalTime then
		ani:SetTotalTime(totalTime)
	end
	
	ani:AttachListener(function (self,oldState,newState)
						 if newState == 4 then
							obj:SetVisible(true)
							if isIncludeChild then
								obj:SetChildrenVisible(true)
							end
						 end
		               end,true)
	objTree:AddAnimation(ani)
	ani:Resume()
	return ani	
end

--弹出效果，要求obj一定要有一个father对象

function RegisterAnimationObject()
	local obj = {}
	obj.ShowDestHideSrcAtTheSamePlace = ShowDestHideSrcAtTheSamePlace      -- ShowDestHideSrcAtTheSamePlace(destObj, srcObj, templateName)
	obj.AppearDestObjDisappearSrcObj = AppearDestObjDisappearSrcObj
	obj.DestObjIntoAreaSrcObjAwayArea = DestObjIntoAreaSrcObjAwayArea
	obj.ShowWindow = ShowWindow                                            -- ShowWindow(hHandle, templateName)
	obj.FocusLight = FocusLight
	obj.AppearPopObject = AppearPopObject
	obj.AppearObjectSlideDown = AppearObjectSlideDown                      --滑落效果
	
	--obj.ObjIntoArea = ObjIntoArea                                        --进入
	--obj.ObjAwayArea = ObjAwayArea                                        --离开
	--obj.ObjAppear = ObjAppear                                            --出现
	--obj.ObjDisappear = ObjDisappear                                      --消失 
	--obj.ObjRotate = ObjRotate                                            --旋转
	--以上动画的组合可以实现位置和透明度的动态效果
	XLSetGlobal("UIBase.AnimationHelper", obj)
end

function AnimationShowWindowBindObj(self, obj)
    local attr = self:GetAttribute()
	if attr and obj then
	    attr.BindObj = obj
	end
end

function AnimationShowWindowAction(self)
    local attr = self:GetAttribute()
	local curve = self:GetCurve()
	local totalTime = self:GetTotalTime()
	local runningTime = self:GetRuningTime()
	local progress = curve:GetProgress(runningTime / totalTime)
	
	local left,right,top,bottom
	if attr.ZoomType == 0 then -- Hide
		left = attr.left - attr.ChangeX * progress
		right = attr.right + attr.ChangeX * progress
		top = attr.top - attr.ChangeY * progress
		bottom = attr.bottom + attr.ChangeY* progress
		attr.BindObj:SetObjPos(left,top,right,bottom)

		local alpha = 255 - 255*(runningTime / totalTime)
		attr.BindObj:SetAlpha(alpha)
	elseif attr.ZoomType == 1 then  -- Show
	    left = (attr.left + attr.ChangeX) - attr.ChangeX * progress
		right = (attr.right - attr.ChangeX) + attr.ChangeX * progress
		top = (attr.top + attr.ChangeY) - attr.ChangeY * progress
		bottom = (attr.bottom - attr.ChangeY) + attr.ChangeY* progress
		attr.BindObj:SetObjPos(left,top,right,bottom)

		local alpha = 255*(runningTime / totalTime)
		attr.BindObj:SetAlpha(alpha)    
	end
	return true
end

function AnimationCycleObjectAction(self)
    local attr = self:GetAttribute()
	local curve = self:GetCurve()
	local totalTime = self:GetTotalTime()
	local runningTime = self:GetRuningTime()
	local progress = curve:GetProgress(runningTime / totalTime)
	local left,right,top,bottom
	
	local srcLeft, srcTop, srcRight, srcBottom = attr.srcBindObj:GetObjPos()
	local srcWidth = srcRight - srcLeft
	local srcHeight = srcBottom - srcTop
	local destLeft, destTop, destRight, destBottom = attr.destBindObj:GetObjPos()
	local destWidth = destRight - destLeft
	local destHeight = destBottom - destTop
	
	if attr.MoveType == "left" then
	    left = attr.srcLeft + (0 - srcWidth) * progress
		top = srcTop
		right = attr.srcRight + (0 - srcWidth) * progress
		bottom = srcBottom
		attr.srcBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255 - 255*(runningTime / totalTime)
		attr.srcBindObj:SetAlpha(alpha)
		
		left = attr.destLeft + (0 - destWidth) * progress
		top = destTop
		right = attr.destRight + (0 - destWidth) * progress
		bottom = destBottom
		attr.destBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255*(runningTime / totalTime)
		attr.destBindObj:SetAlpha(alpha)
		
	elseif attr.MoveType == "right" then
	    left = attr.srcLeft + (0 + srcWidth) * progress
		top = srcTop
		right = attr.srcRight + (0 + srcWidth) * progress
		bottom = srcBottom
		attr.srcBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255 - 255*(runningTime / totalTime)
		attr.srcBindObj:SetAlpha(alpha)
		
		left = attr.destLeft + (0 + destWidth) * progress
		top = destTop
		right = attr.destRight + (0 + destWidth) * progress
		bottom = destBottom
		attr.destBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255*(runningTime / totalTime)
		attr.destBindObj:SetAlpha(alpha)
	elseif attr.MoveType == "top" then
	    left = srcLeft
		top = attr.srcTop + ( 0 - srcHeight) * progress
		right = srcRight
		bottom = attr.srcBottom + ( 0 - srcHeight) * progress
		attr.srcBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255 - 255*(runningTime / totalTime)
		attr.srcBindObj:SetAlpha(alpha)
		
		left = destLeft
		top = attr.destTop + ( 0 - destHeight) * progress
		right = destRight
		bottom = attr.destBottom + ( 0 - destHeight) * progress
		attr.destBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255*(runningTime / totalTime)
		attr.destBindObj:SetAlpha(alpha)
	elseif attr.MoveType == "bottom" then
	    left = srcLeft
		top = attr.srcTop + ( 0 + srcHeight) * progress
		right = srcRight
		bottom = attr.srcBottom + ( 0 + srcHeight) * progress
		attr.srcBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255 - 255*(runningTime / totalTime)
		attr.srcBindObj:SetAlpha(alpha)
		
		left = destLeft
		top = attr.destTop + ( 0 + destHeight) * progress
		right = destRight
		bottom = attr.destBottom + ( 0 + destHeight) * progress
		attr.destBindObj:SetObjPos(left, top, right, bottom)
		local alpha = 255*(runningTime / totalTime)
		attr.destBindObj:SetAlpha(alpha)
	end
	return true
end

function AnimationCycleObjectBindObj(self , srcObj, destObj, moveType, rectLeft, rectTop, rectRight, rectBottom)
    if srcObj == nil or destObj == nil then
	    return false
	end
	
	local attr = self:GetAttribute()
	if attr then
	    attr.srcBindObj = srcObj
		attr.destBindObj = destObj
		attr.MoveType = moveType
		if moveType == "left" then
		    local srcLeft, srcTop, srcRight, srcBottom = srcObj:GetObjPos()
			local destLeft, destTop, destRight, destBottom = destObj:GetObjPos()
			local destWidth = destRight - destLeft
			local destHeight = destBottom - destTop
			destObj:SetObjPos(srcRight, destTop, srcRight + destWidth, destBottom)
			attr.srcLeft = srcLeft
			attr.srcTop = srcTop
			attr.srcRight = srcRight
			attr.srcBottom = srcBottom
			attr.destLeft = rectRight
			attr.destTop = rectTop
			attr.destRight = rectRight + destWidth
			attr.destBottom = rectBottom
		elseif moveType == "right" then
		    local srcLeft, srcTop, srcRight, srcBottom = srcObj:GetObjPos()
			local destLeft, destTop, destRight, destBottom = destObj:GetObjPos()
			local destWidth = destRight - destLeft
			local destHeight = destBottom - destTop
			destObj:SetObjPos(srcLeft - destWidth, destTop, srcLeft, destBottom)
			attr.srcLeft = srcLeft
			attr.srcTop = srcTop
			attr.srcRight = srcRight
			attr.srcBottom = srcBottom
			attr.destLeft = rectLeft - destWidth
			attr.destTop = rectTop
			attr.destRight = rectLeft
			attr.destBottom = rectBottom
		elseif moveType == "top" then
		    local srcLeft, srcTop, srcRight, srcBottom = srcObj:GetObjPos()
			local destLeft, destTop, destRight, destBottom = destObj:GetObjPos()
			local destWidth = destRight - destLeft
			local destHeight = destBottom - destTop
			destObj:SetObjPos(destLeft, srcBottom, destRight, srcBottom + destHeight)
			attr.srcLeft = srcLeft
			attr.srcTop = srcTop
			attr.srcRight = srcRight
			attr.srcBottom = srcBottom
			attr.destLeft = rectLeft
			attr.destTop = rectBottom
			attr.destRight = rectRight
			attr.destBottom = rectBottom + destHeight
		elseif moveType == "bottom" then
		    local srcLeft, srcTop, srcRight, srcBottom = srcObj:GetObjPos()
			local destLeft, destTop, destRight, destBottom = destObj:GetObjPos()
			local destWidth = destRight - destLeft
			local destHeight = destBottom - destTop
			destObj:SetObjPos(destLeft, srcTop - destHeight, destRight, srcTop)
			attr.srcLeft = srcLeft
			attr.srcTop = srcTop
			attr.srcRight = srcRight
			attr.srcBottom = srcBottom
			attr.destLeft = rectLeft
			attr.destTop = rectTop - destHeight
			attr.destRight = rectRight
			attr.destBottom = rectTop
		end
		return true
	end
end

--焦点动画
function FocusLight_Action(self)
	local attr = self:GetAttribute()
	if attr.BindObj == nil then
		return false
	end
	
	local curve = self:GetCurve()
	local totalTime = self:GetTotalTime()
	local runningTime = self:GetRuningTime()
	
	local bindObjleft, bindObjtop,bindObjright, bindObjbottom = attr.BindObj:GetObjPos()
	
	local maskWidth = bindObjright - bindObjleft
	local maskHeight = bindObjbottom - bindObjtop
	local s = (maskWidth + maskHeight) * 2
	
	local progress = math.floor(curve:GetProgress(runningTime / totalTime) * s)
	local light = attr.BindObj:GetControlObject("light")
	local left, top, right, bottom = light:GetObjPos()
	local width = 26
	local height = 26
	if attr.lightWidth then
	    width = attr.lightWidth
		height = attr.lightWidth
	end
	
	if progress <= maskWidth then
	    local lightCenterX = progress + 0
		local lightCenterY = 0
		light:SetObjPos(lightCenterX - width, lightCenterY - height, lightCenterX + width, lightCenterY + height)
	elseif progress <= (maskWidth + maskHeight) then
	    local lightCenterX = bindObjright - bindObjleft
		local lightCenterY = progress - maskWidth + 0
		light:SetObjPos(lightCenterX - width, lightCenterY - height, lightCenterX + width, lightCenterY + height)
	elseif progress <= (maskWidth + maskHeight + maskWidth) then
	    local lightCenterX = bindObjright - bindObjleft - (progress - maskWidth - maskHeight) 
		local lightCenterY = bindObjbottom - bindObjtop
		light:SetObjPos(lightCenterX - width, lightCenterY - height, lightCenterX + width, lightCenterY + height)
	elseif progress <= s then
	    local lightCenterX = 0 
		local lightCenterY = bindObjbottom - bindObjtop - (progress - maskWidth - maskHeight - maskWidth)
		light:SetObjPos(lightCenterX - width, lightCenterY - height, lightCenterX + width, lightCenterY + height)
	end
	
	return true
end

function FocusLight_BindObj(self,obj)
	local attr = self:GetAttribute()
	attr.BindObj = obj
	
end

function FocusLight_GetBindObj(self)
	local attr = self:GetAttribute()
	return attr.BindObj
end


function FocusLight_OnInitControl(self)
	local attr = self:GetAttribute()
	
	local ownerTree = self:GetOwner()
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	if templateMananger then
	    local animationTemplate = nil
	    if attr.animationTemplateName == "" or attr.animationTemplateName == nil then
			animationTemplate = templateMananger:GetTemplate("system.animation.helper.focus.light","AnimationTemplate")
		else
			animationTemplate = templateMananger:GetTemplate(attr.animationTemplateName,"AnimationTemplate")
		end
		if animationTemplate then
		    local hAnimation = animationTemplate:CreateInstance()
			if hAnimation then
			    hAnimation:BindObj(self)
				local animationAttr = hAnimation:GetAttribute()
				animationAttr.lightWidth = attr.lightWidth
			    ownerTree:AddAnimation(hAnimation)
				hAnimation:Resume()
				attr.BindAni = hAnimation
			end
		end
	end
end

function FocusLight_OnDestroy(self)
	local attr = self:GetAttribute()
	attr.BindAni:BindObj(nil)
end
