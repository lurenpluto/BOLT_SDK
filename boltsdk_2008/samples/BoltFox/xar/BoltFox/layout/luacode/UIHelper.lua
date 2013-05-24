
function Initialize(self, mainObjTree)
	self.mainObjTree = mainObjTree
end

function PopupMainMenu(self,x,y)
	local templateMananger = XLGetObject("Xunlei.UIEngine.TemplateManager")				
	local menuTreeTemplate = templateMananger:GetTemplate("BoltFox.MainMenu","ObjectTreeTemplate")
	local menuTree = menuTreeTemplate:CreateInstance("BoltFox.MainMenu.Instance")
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local parent = hostwndManager:GetHostWnd("BoltFox.MainFrame")
	local parentHandle = parent:GetWndHandle()

	local menuHostWndTemplate = templateMananger:GetTemplate("BoltFox.MainMenuHostwnd", "HostWndTemplate")
	local menuHostWnd = menuHostWndTemplate:CreateInstance("BoltFox.MainMenuHostWnd.Instance")
	
	menuHostWnd:BindUIObjectTree(menuTree)
	local menuContext = menuTree:GetUIObject("root")
	
	x, y = parent:ClientPtToScreenPt(x,y)
	
	menuHostWnd:TrackPopupMenu(parentHandle, x, y, x + 400, y + 400)
	
	local objtreeManager = XLGetObject("Xunlei.UIEngine.TreeManager")	
	objtreeManager:DestroyTree("BoltFox.MainMenu.Instance")
	hostwndManager:RemoveHostWnd("BoltFox.MainMenuHostWnd.Instance")
end

function NavigateToUrl(self,url,addToRecent)
	local webbrowser = self.mainObjTree:GetUIObject("Web")
	webbrowser:Navigate(url)
	if addToRecent then
		local explorerHistory = XLGetGlobal("BoltFox.History")
		explorerHistory:PushFrontRecentUrl(url)
	end
end

function QuitApp(self)
	local explorerHistory = XLGetGlobal("BoltFox.History")
	explorerHistory:Save()
	local app = XLGetObject("BoltFox.App")
	app:Quit(0)	
end

function RegisterObject(self)
	local UIHelper = {}
	UIHelper.Initialize = Initialize
	UIHelper.PopupMainMenu = PopupMainMenu
	UIHelper.NavigateToUrl = NavigateToUrl
	UIHelper.QuitApp = QuitApp
	XLSetGlobal("BoltFox.UIHelper",UIHelper)
end
