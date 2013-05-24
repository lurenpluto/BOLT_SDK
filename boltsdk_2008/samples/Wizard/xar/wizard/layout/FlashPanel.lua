
function OnChar(self, char)
	if char == 0x0D then
		local ctrl = self:GetOwnerControl()
		local flashObj = ctrl:GetControlObject("flash.top")
		
		local flashFile = self:GetText()
		if not flashObj:LoadMovie(flashFile) then
			XLMessageBox("load movie failed! file="..flashFile)
		end
		
		flashObj = ctrl:GetControlObject("flash.bottom")
		if not flashObj:LoadMovie(flashFile) then
			XLMessageBox("load movie failed! file="..flashFile)
		end
		
	end
end

function OnInitControl(self)
	
	local app = XLGetObject("Wizard.App")
	local file = app:GetFlashFile()
	
	local topObj = self:GetControlObject("flash.top")
	local bottomObj = self:GetControlObject("flash.bottom")
	
	--topObj:SetFlashModule("C:\\Windows\\SysWOW64\\Macromed\\Flash\\Flash11c.ocx", "{D27CDB6E-AE6D-11CF-96B8-444553540000}")
	
	topObj:LoadMovie(file)
	bottomObj:LoadMovie(file)
end