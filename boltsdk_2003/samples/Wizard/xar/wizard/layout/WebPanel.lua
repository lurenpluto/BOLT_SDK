function OnNewWindow3(self, flags, urlContext, url)
	local ownerCtrl = self:GetOwnerControl()
	local obj = ownerCtrl:GetControlObject("web.bottom")
	
	--强制在下侧的webbrowse里面打开，这里可以额外处理
	return 0, obj:GetRawWebBrowser(), false 
end

function OnChar(self, char)
	if char == 0x0D then
		local ctrl = self:GetOwnerControl()
		local web = ctrl:GetControlObject("web.top")
		
		local url = self:GetText()
		web:Navigate(url)
	end
end

function OnInitControl(self)
	local app = XLGetObject("Wizard.App")
	local disp = app:GetWebBrowserDisp()
	if disp then
		self:SetExternal(disp)
	end
end