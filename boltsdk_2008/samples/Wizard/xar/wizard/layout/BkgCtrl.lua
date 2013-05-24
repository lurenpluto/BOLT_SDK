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

function SetBkgFile(self, file)
	
	local fillBkg = self:GetControlObject("fillbkg")
	local imageBkg = self:GetControlObject("imagebkg")
	
	if not file or file == "" then
		fillBkg:SetChildrenVisible(true)
		imageBkg:SetChildrenVisible(false)
		return
	end
	
	local imageCore = XLGetObject("Wizard.ImageCore")
	
	local bitmap = imageCore:LoadImage(file)
	if not bitmap then
		fillBkg:SetChildrenVisible(true)
		imageBkg:SetChildrenVisible(false)
		return
	end
	
	fillBkg:SetChildrenVisible(false)
	imageBkg:SetChildrenVisible(true)
		
	local imageObj = self:GetControlObject("image")
	imageObj:SetBitmap(bitmap)
		
	local mainColor = imageCore:GetBitmapMainColor(bitmap)
	local colorFill = self:GetControlObject("colorfill")
	colorFill:SetMonochromeColor(mainColor)
	
	ChangeColor(bitmap)
end

function OnInitControl(self)
	
	local app = XLGetObject("Wizard.App")
	local ret, curImageFile = app:GetString("ConfigGraphics", "ConfigGraphics_BkgFile", "")
	
	SetBkgFile(self, curImageFile)
	
	
	local function OnConfigChange(section, key, value)
		if section == "ConfigGraphics" then
			if key == "ConfigGraphics_MainWndOpaque" then
				if value == 1 then
					self:SetBkgAlpha(255)
				else
					self:SetBkgAlpha(app:GetInt("ConfigGraphics", "ConfigGraphics_MainWndAlpha", 200))
				end
			elseif key =="ConfigGraphics_MainWndAlpha" then
				local opaque = app:GetInt("ConfigGraphics", "ConfigGraphics_MainWndOpaque", 0)
				if opaque == 0 then
					self:SetBkgAlpha(value)
				end
			elseif key =="ConfigGraphics_BkgFile" then
				SetBkgFile(self, value)
			end
		end
	end
	
	local attr = self:GetAttribute()
	
	local app = XLGetObject("Wizard.App")
	attr.cookie = app:AttachListener("ConfigChange", OnConfigChange)
	
end

function OnDestroy(self)
	local attr = self:GetAttribute()
	
	local app = XLGetObject("Wizard.App")
	app:RemoveListener("ConfigChange", attr.cookie)

end

function SetBkgAlpha(self, alpha)
	local shading = self:GetControlObject("mainwnd.shading")
	shading:SetAlpha(alpha)
	
	local colorFill = self:GetControlObject("colorfill")
	colorFill:SetAlpha(alpha)
	
	local image = self:GetControlObject("image")
	image:SetAlpha(alpha)
end


