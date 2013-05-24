function SetEnable(self, enable)
	local attr = self:GetAttribute()
	
	if attr.Enable == enable then
		return
	end
	
	attr.Enable = enable
end

function GetEnable(self)
	local attr = self:GetAttribute()
	
	return attr.Enable
end

function SetSelect(self,select)
	local attr = self:GetAttribute()
	if attr.Select == select then
		return
	end
	
	attr.Select = select
	
	if select then
		self:FireExtEvent("OnSelect")
	end
end

function SetColorScheme(self, scheme)
	local attr = self:GetAttribute()
	
	attr.ColorScheme = scheme
	
	local colorObj = self:GetControlObject("color")
	colorObj:SetMonochromeColor(scheme:GetKeyColor())
end

function GetColorScheme(self)

	local attr = self:GetAttribute()
	
	return attr.ColorScheme
end

function OnColorSchemeLButtonDown(self)
	self:SetSelect(true)
end


function OnClose(self)
	--os.exit()
	local app = XLGetObject("XLTS_TestApp.Obj")
	app:Quit()
end

function OnBtnClose(self)
	--os.exit()
	local app = XLGetObject("XLTS_TestApp.Obj")
	app:Quit()
end

function OnInitControl(self)
	local list1 = self:GetControlObject("fontList")
	list1:AddString("宋体")
	list1:AddString("雅黑")
	list1:AddString("华文行楷")
	list1:AddString("华文琥珀")
	list1:SetCurSel(1)
	list1:UpdateUI()
	
	
	local list2 = self:GetControlObject("HeightList")
	list2:AddString("12")
	list2:AddString("16")
	list2:AddString("22")
	list2:AddString("26")
	list2:SetCurSel(1)
	list2:UpdateUI()
end


function OnSize(self, type_, width, height)

	if type_ == "min" then
		return
	elseif type_ == "max" then
		local tree = self:GetBindUIObjectTree()
		if tree ~= nil then
			local button = tree:GetUIObject( "mainwnd.bkg:mainwnd.sysbtn" )
			if button ~= nil then
				button:SetMaxState( false )
			end
		end
	elseif type_ == "restored" then
		local tree = self:GetBindUIObjectTree()
		if tree ~= nil then
			local button = tree:GetUIObject( "mainwnd.bkg:mainwnd.sysbtn" )
			if button ~= nil then
				button:SetMaxState( true )
			end
		end
	end
	
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	
	rootObject:SetObjPos(0, 0, width, height)
end

function RedBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local fmt = {textcolor="#FF0000FF"}
			richedit:SetSelectCharFormat(fmt)
		end
	end
end

function BlueBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local fmt = {textcolor="#0000FFFF"}
			richedit:SetSelectCharFormat(fmt)
		end
	end
end

function BlackBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local fmt = {textcolor="#000000FF"}
			richedit:SetSelectCharFormat(fmt)
		end
	end
end


function list1_OnSelectChanged(self,func, index)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
		    local text = self:GetStringByIndex(index)
			local fmt = {facename=text}
			richedit:SetSelectCharFormat(fmt)
		end
	end
end

function list2_OnSelectChanged(self,func, index)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
		    local text = self:GetStringByIndex(index)
			local fmt = {height=text}
			richedit:SetSelectCharFormat(fmt)
		end
	end
end

local objIndex = 0

function InsertObjectBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			--local tm = XLGetObject("Xunlei.UIEngine.TemplateManager")
			--local objTemplate = tm:GetTemplate("EmbedWebBrowseObject","ObjectTemplate")
			--local obj = objTemplate:CreateInstance("obj")
			--objIndex = objIndex + 1
			--richedit:InsertObject(-1, obj)
			--richedit:SetFocus(true)
			local objectFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
			local name = "webObj"
			objIndex = objIndex + 1
			local obj = objectFactory:CreateUIObject("obj"..objIndex, "EmbedWebBrowseObject")
			richedit:SetFocus(true)
			obj:SetObjPos(0, 0, 100, 100)
			richedit:InsertObject(-1, obj)
		end
	end
end

function InsertImgBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local objectFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
			local name = "imgObj"
			objIndex = objIndex + 1
			local obj = objectFactory:CreateUIObject("obj"..objIndex, "EmbedImageObject")
			richedit:SetFocus(true)
			obj:SetObjPos(0, 0, 48, 48)
			richedit:InsertObject(-1, obj)
		end
	end
end

function LinkBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local attr = self:GetAttribute()
			local fmt = nil
			if attr.isLink == 0 or attr.isLink == nil then
				attr.isLink = 1
				fmt = {link=1}
			else
				attr.isLink = 0
				fmt = {link=0}
			end
			--local fmt = {height=item.text}
			richedit:SetFocus(true)
			richedit:SetInsertCharFormat(fmt)
		end
	end
end

function SetSelForceLinkBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local fmt = {forcelink=1}
			richedit:SetFocus(true)
			richedit:SetSelectCharFormat(fmt)
		end
	end
end

function CancelSelForceLinkBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local fmt = {forcelink=0}
			richedit:SetFocus(true)
			richedit:SetSelectCharFormat(fmt)
		end
	end
end

function PasswordBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			--richedit:SetFocus(true)
			local text = "abc"
			local selStart, selEnd = richedit:GetSel()
			local x, y = richedit:GetCaretPos()
			XLMessageBox(x.." "..y)
			--richedit:ReplaceSel(text, true)
			--richedit:InsertText(-1, text)
			--richedit:SetFocus(true)
			--richedit:AppendText("aaaaaaaaa\naaaaaa\naaaaaaa\naaaaaaaaaaaaaaa\n")
		end
	end
end

function GetNaturalSizeBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			local cx, cy = richedit:GetNaturalSize(0, 100,100)
			XLMessageBox(cy)
		end
	end
end

function InsertAniBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			--local tm = XLGetObject("Xunlei.UIEngine.TemplateManager")
			--local objTemplate = tm:GetTemplate("EmbedWebBrowseObject","ObjectTemplate")
			--local obj = objTemplate:CreateInstance("obj")
			--objIndex = objIndex + 1
			--richedit:InsertObject(-1, obj)
			--richedit:SetFocus(true)
			local objectFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
			local name = "aniObj"
			objIndex = objIndex + 1
			local obj = objectFactory:CreateUIObject("obj"..objIndex, "EmbedAniObject")
			richedit:SetFocus(true)
			obj:SetObjPos(0, 0, 40, 40)
			richedit:InsertObject(-1, obj)
		end
	end
end

function OnAniObjectInitControl(self)
	local objectTree = self:GetOwner()
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local popAniT = templateMananger:GetTemplate("webloading.animation","AnimationTemplate")
	
	local popAni = popAniT:CreateInstance()
	popAni:BindImageObj(self)
	objectTree:AddAnimation(popAni)
	popAni:Resume()
end

function OnGetObjectUniqueID(self,event, obj)
	local userData = {}
	local objClass = obj:GetClass()
	if objClass == "EmbedWebBrowseObject" then
		userData["url"] = "www.bing.com"
	elseif objClass == "EmbedImageObject" then
		userData["image"] = "tree.bkg"
	end
	
	return objClass, userData, true
end

function OnCreateObjectFromUniqueID(self, event, strID, userData)
	if strID == nil then
		return nil,true
	end
	
	local objectFactory = XLGetObject("Xunlei.UIEngine.ObjectFactory")
	local obj
	if strID == "EmbedImageObject" then
		obj = objectFactory:CreateUIObject("", "EmbedImageObject")
		obj:SetObjPos(0, 0, 48, 48)
	elseif strID == "EmbedAniObject" then
		obj = objectFactory:CreateUIObject("", "EmbedAniObject")
		obj:SetObjPos(0, 0, 40, 40)
	elseif strID == "EmbedWebBrowseObject" then
		obj = objectFactory:CreateUIObject("", "EmbedWebBrowseObject")
		obj:SetObjPos(0, 0, 100, 100)
	end
	
	return obj, true
end

function OnLinkNotify(self, eventName, rangeMin, rangeMax)
	local richedit = self:GetControlObject("edit")
	
	local url = richedit:GetTextRange(rangeMin, rangeMax)
	XLMessageBox("ClickUrl:"..url)
end


function OnEditChange(self)
	--XLMessageBox("test")
end



function ftBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			richedit:SetTextType("freetype")
		end
	end
end

function gdiBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			richedit:SetTextType("gdi")
		end
	end
end

function fixedgdiBtn_OnClick(self)
	local ownerTree = self:GetOwnerControl()
    local editObj = ownerTree:GetControlObject("edit_simple")
	if editObj ~= nil then
		local richedit = editObj:GetControlObject("edit")
		if richedit ~= nil then
			richedit:SetTextType("fixedgdi")
		end
	end
end