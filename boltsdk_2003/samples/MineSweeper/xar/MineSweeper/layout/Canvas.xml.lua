function OnInitControl(self)
	
	local faceObject = self:GetControlObject("face")
	local mineNumberObj = self:GetControlObject("mineNumber")
	local timeObj = self:GetControlObject("timeNumber")
	
	local attr = self:GetAttribute()
	assert(attr)
	attr.face = faceObject
	attr.mineNumberObj = mineNumberObj
	attr.timeObj = timeObj
	
	attr.owner = self:GetOwner()
	
	-- XLPrint(string.format("Width=%d, Height=%d, MineNumber=%d\n", attr.Width, attr.Height, attr.MineNumber))
	
	attr.state = "nostart"
	
	CreateMineObject(self)
	
	Restart(self)
	
	-- local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	-- local xarFactory = xarManager:GetXARFactory()
	
	-- local bkg = self:GetControlObject("bkg")
	-- local left, top = 9, 53
	
	-- local imageObject = xarFactory:CreateUIObject("image.mine.0", "MineObject")
	-- bkg:AddChild(imageObject)
	-- imageObject:SetObjPos(left + (j - 1) * 16, top + (i - 1) * 16, left + j * 16, top + i * 16)
	
	-- local owner = self:GetOwner()
	-- local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory") 
	-- runningAni = aniFactory:CreateAnimation("PosChangeAnimation")

	-- runningAni:BindLayoutObj(imageObject)
	-- runningAni:SetLoop(false)
	-- runningAni:SetKeyFramePos(left, top, left+31*15, top)
	-- runningAni:SetTotalTime(3000)
	
	-- owner:AddAnimation(runningAni)
	-- runningAni:Resume()

	
	local function OnTimer()
		if attr.state == "running" then
			attr.currentTime = attr.currentTime + 1
			timeObj:SetNumber(attr.currentTime)
		end
	end
	local timerManager = XLGetObject("Xunlei.UIEngine.TimerManager")
	timerManager:SetTimer(OnTimer, 1000)
end

function InitMineMap(self)
	local attr = self:GetAttribute()
	for i=1, attr.Height do
		for j=1, attr.Width do
			attr.MineTab[i][j].number = -1 	-- 未初始化	
			attr.MineTab[i][j].opened = false
			attr.MineTab[i][j].state = 10 	-- 正常状态(默认), 雷, 问号
		end
	end
end

function CreateMineObject(self)
	local xarManager = XLGetObject("Xunlei.UIEngine.XARManager")
	local xarFactory = xarManager:GetXARFactory()
	
	local bkg = self:GetControlObject("MineHolder")
	
	local attr = self:GetAttribute()
	
	local function OnClick(self, eventName, x, y, t)
		if attr.state == "fail" then
			return 
		-- elseif attr.state == "nostart" then
			-- attr.state = "running"
		end
		
		local objattr = self:GetAttribute()
		-- XLPrint(string.format("image %d,%d %s\n", x, y, t))
		HitTest(attr, x, y, t)
		DrawGrid(attr)
	end
	
	local function OnLButtonDown(self)
		
		if attr.state == "nostart" then
			attr.state = "running"
		end
		
		if attr.state == "running" then
			attr.face:SetState("amazed")
			local objAttr = self:GetAttribute()
			
			if attr.MineTab[objAttr.x][objAttr.y].opened == false and 
				attr.MineTab[objAttr.x][objAttr.y].state == 10 then
				self:SetResID("bitmap.mine.0", false)
			end
		end
	end
	
	local function OnLButtonUp(self)
		if attr.state == "running" then
			attr.face:SetState()
		end
	end
	
	attr.MineTab = {}
	for i=1, attr.Height do
		attr.MineTab[i] = {}
		for j=1, attr.Width do
			local imageObject = xarFactory:CreateUIObject("image.mine."..(i*attr.Width+j), "MineObject")
			attr.MineTab[i][j] = {
				obj = imageObject,
			}
			bkg:AddChild(imageObject)
			imageObject:SetObjPos((j - 1) * 16, (i - 1) * 16, j * 16, i * 16)
			local objattr = imageObject:GetAttribute()
			objattr.x, objattr.y = i, j
			imageObject:AttachListener("OnClick", false, OnClick)
			imageObject:AttachListener("OnLButtonDown", false, OnLButtonDown)
			imageObject:AttachListener("OnLButtonUp", false, OnLButtonUp)
		end
	end
	
end

-- 入场动画
function RunEntranceAni(attr)
	local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory") 
	local owner = attr.owner
	for i=1, attr.Height do
		for j=1, attr.Width do
			local imageObj = attr.MineTab[i][j].obj
			local imageAttr = imageObj:GetAttribute()
			imageAttr.EntranceAni = aniFactory:CreateAnimation("PosChangeAnimation")
			
			local left, top, right, bottom = imageObj:GetObjPos()
			
			imageAttr.EntranceAni:BindLayoutObj(imageObj)
			imageAttr.EntranceAni:SetLoop(false)
			imageAttr.EntranceAni:SetKeyFramePos(left-31*16, top, left, top)
			imageAttr.EntranceAni:SetTotalTime(1500)
			
			owner:AddAnimation(imageAttr.EntranceAni)
			
			imageAttr.EntranceAni:Resume()
		end
	end
end

function FillGrid(attr)
	math.randomseed (os.time())
	for i=1, attr.MineNumber do
		while true do
			local left = math.random(attr.Height)
			local top = math.random(attr.Width)
			if attr.MineTab[left][top].number == -1 then
				attr.MineTab[left][top].number = 15  -- 雷
				break
			end
		end
	end
	
	for i=1, attr.Height do
		for j=1, attr.Width do
			if attr.MineTab[i][j].number ~= 15 then  -- 当前不是雷,统计周围雷的个数
				attr.MineTab[i][j].number = 0
				if attr.MineTab[i-1] then  -- 左侧三个
					if attr.MineTab[i-1][j] and attr.MineTab[i-1][j].number == 15 then
						attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
					end
					if attr.MineTab[i-1][j-1] and attr.MineTab[i-1][j-1].number == 15 then
						attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
					end
					if attr.MineTab[i-1][j+1] and attr.MineTab[i-1][j+1].number == 15 then
						attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
					end
				end
				if attr.MineTab[i][j-1] and attr.MineTab[i][j-1].number == 15 then
					attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
				end
				if attr.MineTab[i][j+1] and attr.MineTab[i][j+1].number == 15 then
					attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
				end
				if attr.MineTab[i+1] then  -- 右侧三个
					if attr.MineTab[i+1][j] and attr.MineTab[i+1][j].number == 15 then
						attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
					end
					if attr.MineTab[i+1][j-1] and attr.MineTab[i+1][j-1].number == 15 then
						attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
					end
					if attr.MineTab[i+1][j+1] and attr.MineTab[i+1][j+1].number == 15 then
						attr.MineTab[i][j].number = attr.MineTab[i][j].number + 1
					end
				end
				
				if attr.MineTab[i][j].number == 0 then
					attr.MineTab[i][j].number = 9
				end
			end
		end
	end
end

function RecursionOpen(attr, x, y)
	if attr.MineTab[x] and attr.MineTab[x][y] then
		attr.MineTab[x][y].opened = true
		if attr.MineTab[x][y].number == 9 then
			if attr.MineTab[x-1] and attr.MineTab[x-1][y] and attr.MineTab[x-1][y].opened == false then
				RecursionOpen(attr, x-1, y)		-- 上
			end
			if attr.MineTab[x+1] and attr.MineTab[x+1][y] and attr.MineTab[x+1][y].opened == false then
				RecursionOpen(attr, x+1, y)		-- 下
			end
			if attr.MineTab[x-1] and attr.MineTab[x-1][y-1] and attr.MineTab[x-1][y-1].opened == false then
				RecursionOpen(attr, x-1, y-1)	-- 左上
			end
			if attr.MineTab[x+1] and attr.MineTab[x+1][y+1] and attr.MineTab[x+1][y+1].opened == false then
				RecursionOpen(attr, x+1, y+1)	-- 右下		
			end
			if attr.MineTab[x+1] and attr.MineTab[x+1][y-1] and attr.MineTab[x+1][y-1].opened == false then
				RecursionOpen(attr, x+1, y-1)	-- 左下
			end
			if attr.MineTab[x-1] and attr.MineTab[x-1][y+1] and attr.MineTab[x-1][y+1].opened == false then
				RecursionOpen(attr, x-1, y+1)	-- 右上		
			end
			if attr.MineTab[x][y-1] and attr.MineTab[x][y-1].opened == false then
				RecursionOpen(attr, x, y-1)		-- 左
			end
			if attr.MineTab[x][y+1] and attr.MineTab[x][y+1].opened == false then
				RecursionOpen(attr, x, y+1)		-- 右
			end
		end
	end
end

function CheckHasFlag(attr, x, y)
	if attr.MineTab[x] and attr.MineTab[x][y] and attr.MineTab[x][y].opened == false and attr.MineTab[x][y].state == 11 then
		return true
	end
	
	return false
end

function CheckSuccess(attr)
	local notMine = attr.Height * attr.Width - attr.MineNumber
	for i=1, attr.Height do
		for j=1, attr.Width do
			if attr.MineTab[i][j].opened and attr.MineTab[i][j].number ~= 15 then
				notMine = notMine - 1
				if notMine == 0 then
					return true
				end
			end
		end
	end
	
	-- XLPrint(string.format("CheckSuccess notMine=%d", notMine))
	
	return false
end

function HitTest(attr, x, y, t)
	-- XLPrint("HitTest "..x.." "..y.." "..t)
	if t == "lclick" then
		if attr.MineTab[x] and attr.MineTab[x][y] and attr.MineTab[x][y].opened == false and attr.MineTab[x][y].state == 10 then
			attr.MineTab[x][y].opened = true
			if attr.MineTab[x][y].number == 9 then
				RecursionOpen(attr, x, y)
				
			elseif attr.MineTab[x][y].number == 15 then		-- 雷
				attr.state = "fail"
				attr.MineTab[x][y].number = 13		-- 踩雷了
				attr.face:SetState("fail")
				ShowAllMine(attr)
			end
			
			if CheckSuccess(attr) then
				attr.state = "success"
				attr.face:SetState("success")
			end
		end
	elseif t == "rclick" then
		if attr.MineTab[x][y].opened == false then
			if attr.MineTab[x][y].state == 10 then		-- 正常
				attr.MineTab[x][y].state = 11
				attr.currentMineNumber = attr.currentMineNumber - 1
			elseif attr.MineTab[x][y].state == 11 then	-- 红旗
				attr.MineTab[x][y].state = 12
				-- 更新剩余雷数
				attr.currentMineNumber = attr.currentMineNumber + 1
			elseif attr.MineTab[x][y].state == 12 then	-- 问号
				attr.MineTab[x][y].state = 10
			end
		end
	elseif t == "dbclick" then
		if attr.MineTab[x][y].opened == true and attr.MineTab[x][y].number ~= 9 then
			local number = 0
			if CheckHasFlag(attr, x-1, y-1) then
				number = number + 1
			end
			if CheckHasFlag(attr, x-1, y) then
				number = number + 1
			end
			if CheckHasFlag(attr, x-1, y+1) then
				number = number + 1
			end
			if CheckHasFlag(attr, x, y-1) then
				number = number + 1
			end
			if CheckHasFlag(attr, x, y+1) then
				number = number + 1
			end
			if CheckHasFlag(attr, x+1, y-1) then
				number = number + 1
			end
			if CheckHasFlag(attr, x+1, y) then
				number = number + 1
			end
			if CheckHasFlag(attr, x+1, y+1) then
				number = number + 1
			end
			-- XLPrint("number="..number)
			if number == attr.MineTab[x][y].number then
				HitTest(attr, x-1, y-1, "lclick")
				HitTest(attr, x-1, y, "lclick")
				HitTest(attr, x-1, y+1, "lclick")
				HitTest(attr, x, y-1, "lclick")
				HitTest(attr, x, y+1, "lclick")
				HitTest(attr, x+1, y-1, "lclick")
				HitTest(attr, x+1, y, "lclick")
				HitTest(attr, x+1, y+1, "lclick")
			end
		end
	end
end

local MineResIDMap = {
	"bitmap.mine.1", 				-- 1
	"bitmap.mine.2",  				-- 2
	"bitmap.mine.3",  				-- 3
	"bitmap.mine.4",  				-- 4
	"bitmap.mine.5",  				-- 5
	"bitmap.mine.6",  				-- 6
	"bitmap.mine.7",  				-- 7
	"bitmap.mine.8",  				-- 8
	"bitmap.mine.0",  				-- 9
	"bitmap.mine.normal",  			-- 10
	"bitmap.mine.flag",  			-- 11
	"bitmap.mine.unknown",  		-- 12
	"bitmap.mine.hitmine",  		-- 13
	"bitmap.mine.wrongmine",  		-- 14
	"bitmap.mine.mine",  			-- 15
	"bitmap.mine.unknown1",  		-- 16
}

function DrawGrid(attr)	
	for i=1, attr.Height do
		for j=1, attr.Width do
			if attr.MineTab[i][j].opened then
				attr.MineTab[i][j].obj:SetResID(MineResIDMap[attr.MineTab[i][j].number], true)
			else
				attr.MineTab[i][j].obj:SetResID(MineResIDMap[attr.MineTab[i][j].state])
			end
		end
	end
	
	-- XLPrint("update mine number ".. tostring(attr.currentMineNumber))
	attr.mineNumberObj:SetNumber(attr.currentMineNumber)
end

function face_OnClick(self)
	local owner = self:GetOwnerControl()
	Restart(owner)
end

function Restart(self)
	local attr = self:GetAttribute()
	attr.state = "nostart"
	attr.currentMineNumber = attr.MineNumber
	attr.currentTime = 0
	attr.timeObj:SetNumber(0)
	InitMineMap(self)
	FillGrid(attr)
	
	DrawGrid(attr)
	
	RunEntranceAni(attr)
end

function ShowAllMine(attr)
	for i=1, attr.Height do
		for j=1, attr.Width do
			if not attr.MineTab[i][j].opened then
				if attr.MineTab[i][j].number == 15 then
					attr.MineTab[i][j].opened = true
				end
				
				-- 标红旗但不是雷
				if attr.MineTab[i][j].state == 11 and attr.MineTab[i][j].number ~= 15 then
					attr.MineTab[i][j].number = 14
					attr.MineTab[i][j].opened = true
				end
			end
		end
	end
end