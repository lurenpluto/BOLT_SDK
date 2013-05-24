local function UpdateRes(self, state)
	local attr = self:GetAttribute()
	local bkgTexture, textFont, textColor
	
	if state == "hover" then
		bkgTexture = attr.BkgHoverTextureID
	elseif state == "disable" then
		bkgTexture = attr.BkgDisableTextureID
		textFont = attr.DisableFont
		textColor = attr.DisableColor
	else
		bkgTexture = attr.BkgNormalTextureID
		textFont = attr.EnableFont
		textColor = attr.EnableColor
	end	
	
	if attr.ReadOnly then
		textFont = attr.ReadOnlyFont
		textColor = attr.ReadOnlyColor
	end
	
	local bkgObj = self:GetControlObject("ctrl")
	local textObj = self:GetControlObject("edit")
	if bkgTexture and bkgTexture ~= "" then
		bkgObj:SetTextureID(bkgTexture)
	end
	if textFont and textFont ~= "" then
		textObj:SetFontID(textFont)
	end
	if textColor and textColor ~= "" then
		textObj:SetTextColorResID(textColor)
	end
	attr.status = state
end

function OnHScrollPosChange(self)
	local edit = self:GetOwnerControl():GetControlObject("edit")
	local pos = self:GetScrollPos()
	edit:SetScrollPos(false, self:GetScrollPos())
end

function OnVScrollPosChange(self)
	local edit = self:GetOwnerControl():GetControlObject("edit")
	local pos = self:GetScrollPos()
	edit:SetScrollPos(true, self:GetScrollPos())
end

function SetReadOnly(self,readonly)
	local attr = self:GetAttribute()
	local edit = self:GetControlObject("edit")
	edit:SetReadOnly(readonly)
	UpdateRes(self, attr.status)
	attr.ReadOnly = readonly
end

function GetReadOnly(self)
	local edit = self:GetControlObject("edit")
	return edit:GetReadOnly()
end

function SetMultiline(self, multiline)
	local edit = self:GetControlObject("edit")
	edit:SetMultiline(multiline)
	attr.Multiline = multiline
end

function GetMultiline(self)
	local edit = self:GetControlObject("edit")
	return edit:GetMultiline()
end

function SetText(self, txt)
	local edit = self:GetControlObject("edit")
	edit:SetText(txt)
end

function GetText(self)
	local edit = self:GetControlObject("edit")
	return edit:GetText()
end

function Undo(self)
	local edit = self:GetControlObject("edit")
	edit:Undo()
end

function SetDisableIME(self, disable)
	local edit = self:GetControlObject("edit")
	edit:SetDisableIME(disable)
end

function GetDisableIME(self)
	local edit = self:GetControlObject("edit")
	return edit:GetDisableIME()
end

function OnInitControl(self)
	local attr = self:GetAttribute()	
	if not self:GetVisible() then
		local ctrlObj = self:GetControlObject("ctrl")
		ctrlObj:SetVisible(false)
		ctrlObj:SetChildrenVisible(false)
	end
	
	if not self:GetEnable() then
		local ctrlObj = self:GetControlObject("ctrl")
		ctrlObj:SetEnable(false)
		ctrlObj:SetChildrenEnable(false)
		
		attr.Status = "disable"
	end	
	
	UpdateRes(self, attr.Status)	
	
	local edit = self:GetControlObject("edit")
    --edit:SetReadOnly(attr.ReadOnly)
    --edit:SetMultiline(attr.Multiline)
	edit:SetAllowHSB(attr.AllowHSB )
	edit:SetAllowVSB(attr.AllowVSB )
	--edit:SetIsPassword(attr.Password)
	--edit:SetWordWrap(attr.WordWrap)
	--edit:SetMaxLength(attr.MaxLength)
	--edit:SetDisableIME(attr.DisableIME)
	--edit:SetText(attr.Text)
end

function OnVisibleChange(self, visible)
	local bkgObj = self:GetControlObject("ctrl")
	if bkgObj then
		bkgObj:SetVisible(visible)
		bkgObj:SetChildrenVisible(visible)
	end
end

function OnEnableChange(self, enable)
	local bkgObj = self:GetControlObject("ctrl")
	bkgObj:SetEnable(enable)
	bkgObj:SetChildrenEnable(enable)
	
	if enable then
		self:ChangeStatus("normal")
	else
		self:ChangeStatus("disable")
	end
end

function UpdateUI(self)
	local attr = self:GetAttribute()
    UpdateRes(self, attr.status)
end

function ChangeStatus(self,newStatus)
	local attr = self:GetAttribute()	
	if attr.Status == newStatus then
		return
	end	
	attr.Status = newStatus	
	UpdateRes(self, newStatus)
end

function GetEnable(self)
	local attr = self:GetAttribute()
	return (attr.Status ~= "disable")
end

function OnMouseMove(self)
	local attr = self:GetAttribute()
	if attr.Status ~= "disable" then
		self:ChangeStatus("hover")
	end
end

function OnMouseLeave(self)
	local attr = self:GetAttribute()
	if attr.Status ~= "disable" then
		self:ChangeStatus("normal")
	end
end

function Edit_OnMouseMove(self)
	self:RouteToFather()
end

function Edit_OnMouseLeave(self)
	self:RouteToFather()
end

function Edit_OnKeyDown(self, uChar, uRepeatCount, uFlags)
	local owner = self:GetOwnerControl()
	owner:FireExtEvent("OnEditKeyDown", uChar, uRepeatCount, uFlags)
end

function Edit_OnKeyUp(self, uChar, uRepeatCount, uFlags)
	local owner = self:GetOwnerControl()
	owner:FireExtEvent("OnEditKeyUp", uChar, uRepeatCount, uFlags)
end

function SetFocus(self,focus)
	local edit = self:GetControlObject("edit")
	edit:SetFocus(focus)
	self:FireExtEvent("OnEditFocusChange", focus)
end

function RemoveTip(self)
	local tree = self:GetOwner()
	local root = tree:GetRootObject()
    local tip = root:GetControlObject("edit.tip")
    if tip ~= nil then
        root:RemoveChild(tip)
    end
end

function AddTip(self, text)
	local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	local xarFactory = xarManager:GetXARFactory()
    local tipObj = xarFactory:CreateUIObject("edit.tip","IDE.CommonTips")
	local tipAttr = tipObj:GetAttribute()
	tipAttr.BkgTexture = "texture.mainwnd.tip.bkg"
	tipAttr.TextBeginH = 0
	tipAttr.TextBeginV = 0
	tipAttr.TrackMouse = false
	tipAttr.Multiline = false
	tipAttr.MultilineTextLimitWidth = 120
	tipObj:SetText(text)
	tipObj:SetType(4)
	tipObj:SetZorder(10000000)
	local tree = self:GetOwner()
	local root = tree:GetRootObject()
	root:AddChild(tipObj)
	local tipLeft,tipTop,tipRight,tipBottom = tipObj:GetTipRect(self)
	tipObj:SetObjPos(tipLeft, tipTop, tipRight,tipBottom)
end

function OnFocusChange(self, focus)
    local ownerattr = self:GetAttribute()
    if focus then
		ownerattr.Focus = true
    else
		self:RemoveTip()
		ownerattr.Focus = false
		if ownerattr.Status ~= "disable" then
			local text = self:GetText()
			local ownerattr = self:GetAttribute()
			if ownerattr.IsNumber then
				if text ~= nil then
					if ownerattr.Min ~= 0 or ownerattr.Max ~= 0 then
						if ownerattr.Min < ownerattr.Max then
							if tonumber( text ) == nil or tonumber( text ) < ownerattr.Min then
								text = ""..ownerattr.Min
								self:SetText( text )
								ownerattr.auto_set_text = true
							elseif  tonumber( text ) > ownerattr.Max then
								text = ""..ownerattr.Max
								ownerattr.auto_set_text = true
								self:SetText( text )
							end
						end
					end
				end
			end
			self:ChangeStatus("normal")
		else
			self:ChangeStatus("disable")
		end
		
		ownerattr.Text = text
    end
	
	self:FireExtEvent("OnEditFocusChange", focus)
end

function OnGetSBVisible(self, vertical)
	local var
	if vertical then
		var = self:GetOwnerControl():GetControlObject("vsb")	
	else
		var = self:GetOwnerControl():GetControlObject("hsb")	
	end
	--XLPrint(string.format("Edit:OnGetSBVisible() vertical=%s",tostring(vertical)))
	return var:GetVisible()
end

function OnGetSBEnable(self, vertical)
	--XLPrint(string.format("Edit:OnGetSBEnable() vertical=%s",tostring(vertical)))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")	
		return var:GetEnable()
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")	
		return var:GetEnable()
	end
end

function OnSetSBEnable(self, vertical, enable)
	--XLPrint(string.format("Edit:OnSetSBEnable() vertical=%s,enable=%s",tostring(vertical),tostring(enable)))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")
		var:SetEnable(enable)
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")
		var:SetEnable(enable)
	end
end

function OnGetSBPageSize(self, vertical)
	--XLPrint(string.format("Edit:OnGetSBPageSize() vertical=%s",tostring(vertical)))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")	
		return var:GetPageSize()
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")	
		return var:GetPageSize()
	end
end

function OnSetSBPageSize(self, vertical, pagesize)
	--XLPrint(string.format("Edit:OnSetSBPageSize() vertical=%s,pagesize=%s",tostring(vertical),tostring(pagesize)))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")	
		var:SetPageSize(pagesize)
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")	
		var:SetPageSize(pagesize)
	end
end

function OnSetSBRange(self, vertical, minrange, maxrange)
	--XLPrint(string.format("Edit:OnSetSBRange() vertical=%s,minrange=%d,maxrange=%d",tostring(vertical),minrange,maxrange))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")	
		var:SetScrollRange(minrange, maxrange)
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")	
		var:SetScrollRange(minrange, maxrange)
	end
	
end

function OnGetSBRange(self, vertical)
	--XLPrint(string.format("Edit:OnGetSBRange() vertical=%s",tostring(vertical)))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")
		return var:GetScrollRange()
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")
		return var:GetScrollRange()
	end
end

function OnSetSBPos(self, vertical, pos)
	--XLPrint(string.format("Edit:OnSetSBPos() vertical=%s,pos=%d",tostring(vertical),pos))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")
		var:SetScrollPos(pos, true)
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")
		var:SetScrollPos(pos, true)
	end
end

function OnGetSBPos(self, vertical)
	--XLPrint(string.format("Edit:OnGetSBPos() vertical=%s",tostring(vertical)))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")
		return var:GetScrollPos()
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")
		return var:GetScrollPos()
	end
end

function OnSetSBVisible(self, vertical, visible)
	--XLPrint(string.format("Edit:OnSetSBVisible() vertical=%s,visible=%s",tostring(vertical),tostring(visible)))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")	
		var:SetVisible(visible)
		if not visible then
			var:SetScrollRange( 0, 0 )
		end
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")	
		var:SetVisible(visible)
		if not visible then
			var:SetScrollRange( 0, 0 )
		end
	end
end

function OnSetSBRect(self, vertical, left, top, right, bottom)
	XLPrint(string.format("Edit:OnSetSBRect() vertical=%s,left, top, right, bottom=%d,%d,%d,%d",tostring(vertical),left, top, right, bottom))
	if vertical then
		local var = self:GetOwnerControl():GetControlObject("vsb")
		if self:GetScrollVisible( false ) then
			var:SetObjPos(left,top,right,bottom+10)
		else
			var:SetObjPos(left,top,right,bottom)
		end
	else
		local var = self:GetOwnerControl():GetControlObject("hsb")
		if self:GetScrollVisible( true ) then
			var:SetObjPos(left+4,top,right+4,bottom)
		else
			var:SetObjPos(left+4,top,right,bottom)
		end
	end
end

function OnGetSBIdealSize(self, vertical)
	--XLPrint(string.format("Edit:OnGetSBIdealSize() vertical=%s",tostring(vertical)))
	if vertical then
		return 10,0
	else
		return 0,10
	end	
end

function Edit_OnChange(self)
	local control = self:GetOwnerControl()
	control:RemoveTip()
    local text = self:GetText()
	local ownerattr = control:GetAttribute()
	ownerattr.Text = text
    control:FireExtEvent("OnEditChange", text)
end

function Edit_OnFocusChange(self, focus)
	local control = self:GetOwnerControl()
	control:RemoveTip()
end

function AppendText(self,text,scroll)
	local edit = self:GetControlObject("edit")
	edit:AppendText(text,scroll)
end

function OnGetObjectUniqueID(self, obj)
	local control = self:GetOwnerControl()

	local strID, userData, handled = control:FireExtEvent("OnGetObjectUniqueID", obj)
	
	return strID, userData, handled, true
end

function OnCreateObjectFromUniqueID(self, strID, userData)
	local control = self:GetOwnerControl()
	local obj, handled = control:FireExtEvent("OnCreateObjectFromUniqueID", strID, userData)
	
	return obj, handled, true
end

function OnLinkNotify(self, minRange, maxRange)
	local control = self:GetOwnerControl()
	return control:FireExtEvent("OnLinkNotify", minRange, maxRange)
end
