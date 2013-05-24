
local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
local frameHostWndTemplate = templateMananger:GetTemplate("Spy.MainFrame","HostWndTemplate")
if frameHostWndTemplate then  
	local frameHostWnd = frameHostWndTemplate:CreateInstance("Spy.MainFrame")
	if frameHostWnd then
		local objectTreeTemplate = templateMananger:GetTemplate("SpyTree","ObjectTreeTemplate")
		if objectTreeTemplate then
			local uiObjectTree = objectTreeTemplate:CreateInstance("Spy.MainObjectTree")
			if uiObjectTree then
				frameHostWnd:BindUIObjectTree(uiObjectTree)
				frameHostWnd:Create()
			end
		end
	end
end

-- local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")
-- local frameHostWndTemplate = templateMananger:GetTemplate("Spy.ShareBmpFrame","HostWndTemplate")
-- if frameHostWndTemplate then  
	-- local frameHostWnd = frameHostWndTemplate:CreateInstance("Spy.ShareBmpFrame")
	-- if frameHostWnd then
		-- local objectTreeTemplate = templateMananger:GetTemplate("SpyShareBmpTree,"ObjectTreeTemplate")
		-- if objectTreeTemplate then
			-- local uiObjectTree = objectTreeTemplate:CreateInstance("Spy.ShareBmpObjectTree")
			-- if uiObjectTree then
				-- frameHostWnd:BindUIObjectTree(uiObjectTree)
				-- frameHostWnd:Create()
			-- end
		-- end
	-- end
-- end