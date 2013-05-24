function OnDemoButton_Click(self)
	XLMessageBox("Click Demo Button!")
end

function DemoListBox_OnInitControl(self)
	for i=1,100 do
		self:AddString("String at "..i)
	end
	self:UpdateUI()
end

function OnTipsMouseEnter(self, x, y)
	local hostWndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local tipsHostWnd = hostWndManager:GetHostWnd("Wizard.Tips.Instance")
						
	tipsHostWnd:DelayPopup(600)
	tipsHostWnd:SetPositionByObject(50,30,self)
end

function OnTipsMouseLeave(self)
	local hostWndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local tipsHostWnd = hostWndManager:GetHostWnd("Wizard.Tips.Instance")
	
	tipsHostWnd:DelayCancel(0)
end

function OnTipsInitControl(self)
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local tipsHostWndTemplate = templateMananger:GetTemplate("Wizard.Tips","HostWndTemplate")
	tipsHostWndTemplate:CreateInstance("Wizard.Tips.Instance")
end

function OnTipsDestroy(self)
	local hostWndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	hostWndManager:RemoveHostWnd("Wizard.Tips.Instance")
end

function OnModalLButtonUp(self)
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local hostWndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	
	local modalHostWndTemplate = templateMananger:GetTemplate("Wizard.ModalDlg","HostWndTemplate")
	if modalHostWndTemplate then
		local modalHostWnd = modalHostWndTemplate:CreateInstance("Wizard.ModalDlg.Instance")
		if modalHostWnd then
			local objectTreeTemplate = templateMananger:GetTemplate("Wizard.ModalDlg","ObjectTreeTemplate")
			if objectTreeTemplate then
				local uiObjectTree = objectTreeTemplate:CreateInstance("Wizard.ModalDlg.Instance")
				if uiObjectTree then
					modalHostWnd:BindUIObjectTree(uiObjectTree)
					
					local mainWnd = hostWndManager:GetHostWnd("Wizard.MainWnd.Instance")
					modalHostWnd:DoModal(mainWnd:GetWndHandle())
				end
				
				local objtreeManager = XLGetObject("Xunlei.UIEngine.TreeManager")	
				objtreeManager:DestroyTree("Wizard.ModalDlg.Instance")
			end
			
			
			hostWndManager:RemoveHostWnd("Wizard.ModalDlg.Instance")
		end
	end
end

function OnCrashButton_Click(self)
	local app = XLGetObject("Wizard.App")
	app:Crash()
end

function OnDragTextUp(self)
    local ctrl = self:GetOwnerControl()
    local cattr = ctrl:GetAttribute()
    cattr.dragmouse = false
end

function OnDragTextDown(self)
    local ctrl = self:GetOwnerControl()
    local cattr = ctrl:GetAttribute()
    cattr.dragmouse = true
end

function OnDragTextMove(self)
    local ctrl = self:GetOwnerControl()
    local cattr = ctrl:GetAttribute()
    if cattr.dragmouse then
        local DataObjectHelper = XLGetObject("Wizard.DataObjectHelper")
        local dataObj = DataObjectHelper:CreateDataObjectFromText("XLUE界面引擎，Drag测试文字")
        if dataObj then
            local tree = self:GetOwner()
            local hostwnd = tree:GetBindHostWnd()
            hostwnd:DoDragDrop(dataObj, 1)
            cattr.dragmouse = false
        end
    end
end

function OnDragQuery(self, dataObject, keyState, x, y)
    local DataObjectHelper = XLGetObject("Wizard.DataObjectHelper")
    if DataObjectHelper:IsCFTEXTData(dataObject) then
        return 1, true
    end
    return 0, true
end

local effect

function OnDragEnter(self, dataObject, keyState, x, y)
    effect = 0
	
	local DataObjectHelper = XLGetObject("Wizard.DataObjectHelper")
	local ret = DataObjectHelper:IsCFTEXTData(dataObject)
	if not ret then
		return effect, true
	end
	
	effect = 1 --DROPEFFECT_COPY
	return effect, true
end

function OnDragOver(self)
    return effect, true
end

function OnDrop(self, dataObject, keyState, x, y)
    local DataObjectHelper = XLGetObject("Wizard.DataObjectHelper")
    local ret, text = DataObjectHelper:PraseDataObject(dataObject)
    if ret then
        local textObject = self:GetControlObject("droptext")
        textObject:SetText(text)
    end
end