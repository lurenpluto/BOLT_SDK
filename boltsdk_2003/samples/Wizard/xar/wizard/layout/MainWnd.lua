local function ChangeColor(hBitmap)
    local SkinManager = XLGetObject("Wizard.SkinManager")
	if SkinManager then
	    if hBitmap then
	        local function ModifyWith(h,s,v,bepaint)
                local function Pretreat(resId,resObj)               
                    resObj:ModifyColor(h,s,v,bepaint)
                end
                return Pretreat
            end
    
	        local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	        local h,s,v = SkinManager:GetBitmapHSLColor(hBitmap)
	        local resID
	        
	        count = SkinManager:GetColorableBitmapCount()
            for index = 0, count - 1 do
                resID = SkinManager:GetBitmapIdByIndex(index)
                xarManager:SetBitmapPretreater(resID,ModifyWith(h,s,v,1))
            end
            
            count = SkinManager:GetColorableColorCount()
            for index = 0, count - 1 do
                resID = SkinManager:GetColorIdByIndex(index)
                xarManager:SetColorPretreater(resID,ModifyWith(h,s,v,1))
            end
            
            count = SkinManager:GetColorableTextureCount()
            for index = 0, count - 1 do
                resID = SkinManager:GetTextureIdByIndex(index)
                xarManager:SetTexturePretreater(resID,ModifyWith(h,s,v,1))
            end
            
            xarManager:CommitPretreat()
	    end
	end
end


function OnSize(self, sizetype, width, height)

	if type_ == "min" then
		return
	elseif type_ == "max" then
		local tree = self:GetBindUIObjectTree()
		if tree ~= nil then
			local button = tree:GetUIObject( "mainwnd.bkg:mainwnd.sysbtn" )
			if button ~= nil then
				--button:SetMaxState( false )
			end
		end
	elseif type_ == "restored" then
		local tree = self:GetBindUIObjectTree()
		if tree ~= nil then
			local button = tree:GetUIObject( "mainwnd.bkg:mainwnd.sysbtn" )
			if button ~= nil then
				--button:SetMaxState( true )
			end
		end
	end
	
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	rootObject:SetObjPos(0, 0, width, height)
	
end

function OnDragQuery(self, dataObject, keyState, x, y)

	local imageCore = XLGetObject("Wizard.ImageCore")
	local ret, file = imageCore:ParseDataObject(dataObject)
	if not ret then
		return 0, true
	end
	
	return 1, true
end

local effect

function OnDragEnter(self, dataObject, keyState, x, y)
	
	effect = 0
	
	local imageCore = XLGetObject("Wizard.ImageCore")
	local ret, file = imageCore:ParseDataObject(dataObject)
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
	
	local imageCore = XLGetObject("Wizard.ImageCore")
	local ret, file = imageCore:ParseDataObject(dataObject)
	if not ret then
		return 0, true
	end
	
	local app = XLGetObject("Wizard.App")
	app:SetString("ConfigGraphics", "ConfigGraphics_BkgFile", file)
	
	return effect, true
end

function OnClose(self)
	
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Wizard.MainWnd.Instance")
	
	hostwndManager:RemoveHostWnd("Wizard.MainWnd.Instance")
	
	local objtreeManager = XLGetObject("Xunlei.UIEngine.TreeManager")	
	objtreeManager:DestroyTree("Wizard.MainObjTree.Instance")
	
	local app = XLGetObject("Wizard.App")
	app:Quit(0)
end

function OnSysBtnInitControl(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Wizard.MainWnd.Instance")
	local state = hostwnd:GetWindowState()
	if state == "max" then
		self:SetMaxState( false )
	else
		self:SetMaxState( true )
	end
end

function OnMinisize(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Wizard.MainWnd.Instance")
	hostwnd:Min() 
end

function OnMaxBtnClick(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Wizard.MainWnd.Instance")
	local state = hostwnd:GetWindowState()
	if state == "max" then
		if start_max then
			hostwnd:Restore()
			local defaultWidth, defaultHeight = GetDefaultWindowSize()
			hostwnd:Move(0, 0, defaultWidth, defaultHeight)
			hostwnd:Center()
			start_max = false
		else
			hostwnd:Restore()
		end
		self:SetMaxState( true )
	else
		hostwnd:Max()
		self:SetMaxState( false )
	end
end

function GetScreenPos(menuRoot, x, y)
	local left,top,right,bottom = menuRoot:GetObjPos()
	
	local width = right - left
	local height = bottom - top
	
	local menuTree = menuRoot:GetOwner()
	local menuHostWnd = menuTree:GetBindHostWnd()
	
	local sleft, stop, sright, sbottom = menuHostWnd:GetMonitorRect(x, y)
	if x + width > sright then
		x = x - width
	end
	
	if y + height > sbottom then
		y = y - height
	end
	
	return x, y, width, height
end	
function OnMainWndRButtonUp(self, x, y)
		
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")				
	local menuTreeTemplate = templateMananger:GetTemplate("Wizard.Menu","ObjectTreeTemplate")
	local menuTree = menuTreeTemplate:CreateInstance("Wizard.Menu.Instance")
	
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local parent = hostwndManager:GetHostWnd("Wizard.MainWnd.Instance")
	local parentHandle = parent:GetWndHandle()

	local menuHostWndTemplate = templateMananger:GetTemplate("Wizard.Menu", "HostWndTemplate")
	local menuHostWnd = menuHostWndTemplate:CreateInstance("Wizard.Menu.Instance")
	
	menuHostWnd:BindUIObjectTree(menuTree)
	local menuContext = menuTree:GetUIObject("menu.root")
	
	x, y = parent:ClientPtToScreenPt(x,y)
	local left, top, width, height = GetScreenPos(menuContext, x, y)
	
	menuHostWnd:TrackPopupMenu(parentHandle, left, top, width, height)
	
	local objtreeManager = XLGetObject("Xunlei.UIEngine.TreeManager")	
	objtreeManager:DestroyTree("Wizard.Menu.Instance")
	hostwndManager:RemoveHostWnd("Wizard.Menu.Instance")
end

function TabHeader_OnInitControl(self)
	self:AddTabItem("MainPanel","常见控件","",true)
	self:AddTabItem("RichEdit","富文本")
	self:AddTabItem("IE","Web")
	self:AddTabItem("Flash","Flash")
	self:AddTabItem("Res", "Resource")
	self:AddTabItem("Animation", "Animation")
end

function OnActiveTabChanged(self, eventName, newid, oldid)
	local ownerTree = self:GetOwner()
	local mainPanel = ownerTree:GetUIObject("app.bkg:MainPanel")
	local iePanel = ownerTree:GetUIObject("app.bkg:WebPanel")
	local flashPanel = ownerTree:GetUIObject("app.bkg:FlashPanel")
	local resPanel = ownerTree:GetUIObject("app.bkg:ResPanel")
	local richeditPanel = ownerTree:GetUIObject("app.bkg:RichEditPanel")
	local animPanel = ownerTree:GetUIObject("app.bkg:AnimationPanel")
	
	mainPanel:SetVisible(false)
    mainPanel:SetChildrenVisible(false)
	
	richeditPanel:SetVisible(false)
    richeditPanel:SetChildrenVisible(false)
    
    iePanel:SetVisible(false)
    iePanel:SetChildrenVisible(false)
    
    flashPanel:SetVisible(false)
    flashPanel:SetChildrenVisible(false)
    
    resPanel:SetVisible(false)
    resPanel:SetChildrenVisible(false)
    
    animPanel:SetVisible(false)
    animPanel:SetChildrenVisible(false)
	
	if newid =="MainPanel" then
		mainPanel:SetVisible(true)
		mainPanel:SetChildrenVisible(true)
	elseif newid == "RichEdit" then	
		richeditPanel:SetVisible(true)
		richeditPanel:SetChildrenVisible(true)
	elseif newid == "IE" then
		iePanel:SetVisible(true)
		iePanel:SetChildrenVisible(true)
	elseif newid == "Flash" then
		flashPanel:SetVisible(true)
		flashPanel:SetChildrenVisible(true)
	elseif newid =="Res" then
		resPanel:SetVisible(true)
		resPanel:SetChildrenVisible(true)
	elseif newid == "Animation" then
	    animPanel:SetVisible(true)
        animPanel:SetChildrenVisible(true)
	end
end