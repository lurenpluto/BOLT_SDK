function SetNumber(self, number)
	if number > 999 then
		return
	end
	
	local attr = self:GetAttribute()
	local owner = self:GetOwner()
	
	
	local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory") 
	
	local first = math.fmod(number, 10)
	number = math.floor(number / 10)
	local second = math.fmod(number, 10)
	number = math.floor(number / 10)
	local third = number
	
	if not attr.first then
		attr.first = 0
		attr.second = 0
		attr.third = 0
	end
	
	local firstObj = self:GetControlObject("number3")
	local firstObjAni = self:GetControlObject("number3Ani")
	local secondObj = self:GetControlObject("number2")
	local secondObjAni = self:GetControlObject("number2Ani")
	local thirdObj = self:GetControlObject("number1")
	local thirdObjAni = self:GetControlObject("number1Ani")
	
	firstObj:SetResID("bitmap.number."..attr.first)
	firstObjAni:SetResID("bitmap.number."..first)
	
	
	secondObj:SetResID("bitmap.number."..attr.second)
	secondObjAni:SetResID("bitmap.number."..second)
	
	if attr.second ~= second then
		local SecondAni = aniFactory:CreateAnimation("PosChangeAnimation")
		SecondAni:BindLayoutObj(secondObj)
		SecondAni:SetLoop(false)
		SecondAni:SetKeyFramePos(13, 0, 13, -23)
		SecondAni:SetTotalTime(1000)
		
		owner:AddAnimation(SecondAni)
		
		local SecondObjAni = aniFactory:CreateAnimation("PosChangeAnimation")
		SecondObjAni:BindLayoutObj(secondObjAni)
		SecondObjAni:SetLoop(false)
		SecondObjAni:SetKeyFramePos(13, 23, 13, 0)
		SecondObjAni:SetTotalTime(1000)
		
		owner:AddAnimation(SecondObjAni)
		
		SecondAni:Resume()
		SecondObjAni:Resume()
	end
	
	
	thirdObj:SetResID("bitmap.number."..attr.third)
	thirdObjAni:SetResID("bitmap.number."..third)
	
	if attr.third ~= third then
		local ThirdAni = aniFactory:CreateAnimation("PosChangeAnimation")
		ThirdAni:BindLayoutObj(thirdObj)
		ThirdAni:SetLoop(false)
		ThirdAni:SetKeyFramePos(13, 0, 13, -23)
		ThirdAni:SetTotalTime(1000)
		
		owner:AddAnimation(ThirdAni)
		
		local ThirdObjAni = aniFactory:CreateAnimation("PosChangeAnimation")
		ThirdObjAni:BindLayoutObj(thirdObjAni)
		ThirdObjAni:SetLoop(false)
		ThirdObjAni:SetKeyFramePos(13, 23, 13, 0)
		ThirdObjAni:SetTotalTime(1000)
		
		owner:AddAnimation(ThirdObjAni)
		
		ThirdAni:Resume()
		ThirdObjAni:Resume()
	end
	
	
	local FristAni = aniFactory:CreateAnimation("PosChangeAnimation")
	FristAni:BindLayoutObj(firstObj)
	FristAni:SetLoop(false)
	FristAni:SetKeyFramePos(26, 0, 26, -23)
	FristAni:SetTotalTime(1000)
	
	owner:AddAnimation(FristAni)
	
	local FristObjAni = aniFactory:CreateAnimation("PosChangeAnimation")
	FristObjAni:BindLayoutObj(firstObjAni)
	FristObjAni:SetLoop(false)
	FristObjAni:SetKeyFramePos(26, 23, 26, 0)
	FristObjAni:SetTotalTime(1000)
	
	owner:AddAnimation(FristObjAni)
	
	FristAni:Resume()
	FristObjAni:Resume()
	
	
	attr.first = first
	attr.second = second
	attr.third = third
	
	
	-- XLPrint(string.format("SetNumber %d %d %d", first, second, third))
end