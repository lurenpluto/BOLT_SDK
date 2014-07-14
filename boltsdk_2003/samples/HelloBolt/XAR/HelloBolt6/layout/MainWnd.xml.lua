--lua文件必须是UTF-8编码的(最好无BOM头)
function close_btn_OnLButtonDown(self)
	---创建内置动画的实例
	local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	local alphaAni = aniFactory:CreateAnimation("AlphaChangeAnimation")
	alphaAni:SetTotalTime(700)
	alphaAni:SetKeyFrameAlpha(255,0)
	local owner = self:GetOwner()
	local icon = owner:GetUIObject("icon")
	alphaAni:BindRenderObj(icon) 
	owner:AddAnimation(alphaAni)
	alphaAni:Resume()
	
	local posAni = aniFactory:CreateAnimation("PosChangeAnimation")
	posAni:SetTotalTime(700)
	posAni:SetKeyFrameRect(45,100,45+60,100+60,45-30,100-30,45+60+30,100+60+30)
	posAni:BindLayoutObj(icon)
	owner:AddAnimation(posAni)
	posAni:Resume()

	local alphaAni2 = aniFactory:CreateAnimation("AlphaChangeAnimation")
	alphaAni2:SetTotalTime(700)
	alphaAni2:SetKeyFrameAlpha(255,0)
	local msg = owner:GetUIObject("msg")
	alphaAni2:BindRenderObj(msg)
	owner:AddAnimation(alphaAni2)
	alphaAni2:Resume()
	
	---定义动画结束的回调函数
	local function onAniFinish(self,oldState,newState)
		if newState == 4 then
		----os.exit 效果等同于windows的exit函数，不推荐实际应用中直接使用
			os.exit()
		end
	end

	local posAni2 = aniFactory:CreateAnimation("PosChangeAnimation")
	posAni2:SetTotalTime(800)
	posAni2:BindLayoutObj(msg)
	posAni2:SetKeyFramePos(135,100,500,100)
	--当动画结束后，应用程序才退出
	posAni2:AttachListener(true,onAniFinish)
	owner:AddAnimation(posAni2)
	posAni2:Resume()
end

function OnInitControl(self)
	local owner = self:GetOwner()
	
	--动态创建一个ImageObject,这个Object在XML里没定义
	local objFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local newIcon = objFactory:CreateUIObject("icon2","ImageObject")
	local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	newIcon:SetResProvider(xarManager)
	newIcon:SetObjPos(45,165,45+70,165+70)
	newIcon:SetResID("app.icon2")
	local function onClickIcon()
		XLMessageBox("Don't touch me!")
	end
	--绑定鼠标事件的响应函数到对象
	newIcon:AttachListener("OnLButtonDown",true,onClickIcon)
	self:AddChild(newIcon)
	
	--创建一个自定义动画，作用在刚刚动态创建的ImageObject上
	local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
	myAni = aniFactory:CreateAnimation("HelloBolt.ani")
	--一直运行的动画就是一个TotalTime很长的动画
	myAni:SetTotalTime(9999999) 
	local aniAttr = myAni:GetAttribute()
	aniAttr.obj = newIcon
	owner:AddAnimation(myAni)
	myAni:Resume()
	
end

function MSG_OnMouseMove(self)
	self:SetTextFontResID ("msg.font.bold")
	self:SetCursorID ("IDC_HAND")
end

function MSG_OnMouseLeave(self)
	self:SetTextFontResID ("msg.font")
	self:SetCursorID ("IDC_ARROW")
end

function userdefine_btn_OnClick(self)
	XLMessageBox("Click on button")
end
