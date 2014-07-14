--lua文件必须是UTF-8编码的(最好无BOM头)
function close_btn_OnLButtonDown(self)
	----os.exit 效果等同于windows的exit函数，不推荐实际应用中直接使用
	os.exit()
end

function OnInitControl(self)
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
end

function MSG_OnMouseMove(self)
	self:SetTextFontResID ("msg.font.bold")
	self:SetCursorID ("IDC_HAND")
end

function MSG_OnMouseLeave(self)
	self:SetTextFontResID ("msg.font")
	self:SetCursorID ("IDC_ARROW")
end