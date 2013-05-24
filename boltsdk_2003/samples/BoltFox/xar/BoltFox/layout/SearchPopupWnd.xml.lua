function OnLButtonDown(self)
	local tree = self:GetOwner()	CloseWindow(tree)
end

function OnInitControl(self)

end

function OnCreate(self)
	local tree = self:GetBindUIObjectTree()	local list = tree:GetUIObject("list")		local data = self:GetUserData()	for i=1, #data.textArray do		list:AddString(data.textArray[i], data.iconArray[i])	end		list:UpdateUI()
end

function OnSize(self)	XLPrint("OnSize")
	local tree = self:GetBindUIObjectTree()	local bkg = tree:GetUIObject("bkg")		local left, top, right, bottom = self:GetWindowRect(void)	bkg:SetObjPos(0, 0, right-left, bottom-top)
end

function OnFocusChange(self, focus)	if not focus then		local data = self:GetUserData()		if data.edit:GetFocus() then			return 		end		
		local tree = self:GetBindUIObjectTree()		CloseWindow(tree)	end
endfunction CloseWindow(tree)	AsynCall(function ()			local hostwnd = tree:GetBindHostWnd()						if hostwnd then				hostwnd:Destroy()								local hostwndMgr = XLGetObject("Xunlei.UIEngine.HostWndManager")				hostwndMgr:RemoveHostWnd("BoltFox.SearchPopupWnd")								local treeMgr = XLGetObject("Xunlei.UIEngine.TreeManager")				treeMgr:DestroyTree("BoltFox.SearchPopupTree")			end		end	)end

function OnSelectChange(self, func, text)
	XLPrint("OnSelectChange(self, func, text) "..text)

	local tree = self:GetOwner()
	local hostwnd = tree:GetBindHostWnd()
	local data = hostwnd:GetUserData()
	data.text = text
	data.closeByClick = true
	
	CloseWindow(tree)
end