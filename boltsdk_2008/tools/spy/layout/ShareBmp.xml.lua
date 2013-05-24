function OnClose(self)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Spy.ShareBmpFrame")
	local mainhost = hostwndManager:GetHostWnd("Spy.MainFrame")
	local objtree = mainhost:GetBindUIObjectTree()
	local bkg = objtree:GetUIObject("mainwnd.bkg")
	local check = bkg:GetControlObject("Main.CheckBox.Preview")
	check:SetCheck(false)
	hostwnd:SetVisible(false)
end

function OnLButtonDown(self,x,y)
	local owner = self:GetOwner()
	local resize = owner:GetUIObject("status_bar.resize.bottomright")
	local left1,top1,right1,bottom1 = resize:GetObjPos()
	xx = x
	yy = y
	lbuttondown = true
	
	local owner = self:GetOwner()
	local preview = owner:GetUIObject("Preview")
	left,top,right,bottom = preview:GetObjPos()
end

function OnLButtonUp(self)
	lbuttondown = false
	self:SetCursorID("IDC_ARROW")
end

function OnMouseMove(self,x,y)
	if lbuttondown then
		x1 = x - xx
		y1 = y - yy
		
		local owner = self:GetOwner()
		local preview = owner:GetUIObject("Preview")
		
		preview:SetObjPos(left+x1,top+y1,right+x1,bottom+y1)
		self:SetCursorID("IDC_HAND")
	end
end

function OnCheck(self, check1, check2)
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd("Spy.ShareBmpFrame")
	hostwnd:SetTopMost(check2)
end

function OnSize(self, type, x, y)
	if type == "min" then return end
	
	local objectTree = self:GetBindUIObjectTree()
	local rootObject = objectTree:GetRootObject()
	rootObject:SetObjPos(0, 0, x, y)
	lbuttondown = false
end