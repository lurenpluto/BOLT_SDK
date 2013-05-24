function OnAlphaAnimBtnClick(self)
    local ctrl = self:GetOwnerControl()
    local objTree = self:GetOwner()
    local image = ctrl:GetControlObject("simple_image")
    if image then
        local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
        if aniFactory then
            local alphaAnim = aniFactory:CreateAnimation("AlphaChangeAnimation")
            alphaAnim:BindObj(image)
            alphaAnim:SetKeyFrameAlpha(255, 0)
            alphaAnim:SetTotalTime(1000)
            local cookie = alphaAnim:AttachListener(true,function (self,oldState,newState)
                    if newState == 4 then
                        image:SetAlpha(255)
                    end
                end)
            objTree:AddAnimation(alphaAnim)
            alphaAnim:Resume()
        end
    end
end

function OnPosAnimBtnClick(self)
    local ctrl = self:GetOwnerControl()
    local objTree = self:GetOwner()
    local image = ctrl:GetControlObject("simple_image")
    local left, top, right, bottom = image:GetObjPos()
    if image then
        local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
        if aniFactory then
            local posAnim = aniFactory:CreateAnimation("PosChangeAnimation")
            posAnim:BindObj(image)
            posAnim:SetKeyFramePos(left, top, left+200, top)
            posAnim:SetTotalTime(1000)
            local cookie = posAnim:AttachListener(true,function (self,oldState,newState)
                    if newState == 4 then
                        image:SetObjPos(left, top, right, bottom)
                    end
                end)
            objTree:AddAnimation(posAnim)
            posAnim:Resume()
        end
    end
end

function OnSeqAnimBtnClick(self)
    local ctrl = self:GetOwnerControl()
    local objTree = self:GetOwner()
    local image = ctrl:GetControlObject("simple_image")
    local left, top, right, bottom = image:GetObjPos()
    if image then
        local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
        if aniFactory then
            local seqAnim = aniFactory:CreateAnimation("SeqFrameAnimation")
            seqAnim:BindImageObj(image)
            seqAnim:SetResID("simpleseq")
            seqAnim:SetTotalTime(1000)
            local cookie = seqAnim:AttachListener(true,function (self,oldState,newState)
                    if newState == 4 then
                        image:SetResID("animation.simple.image")
                    end
                end)
            objTree:AddAnimation(seqAnim)
            seqAnim:Resume()
        end
    end
end

function OnAngleAnimBtnClick(self)
    local ctrl = self:GetOwnerControl()
    local objTree = self:GetOwner()
    local image = ctrl:GetControlObject("simple_image")
    local left, top, right, bottom = image:GetObjPos()
    if image then
        local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
        if aniFactory then
            local angleAnim = aniFactory:CreateAnimation("AngleChangeAnimation")
            angleAnim:BindObj(image)
            angleAnim:SetKeyFrameAngle(0, 180, 0, 0, 0, 0)
            angleAnim:SetTotalTime(1000)
            objTree:AddAnimation(angleAnim)
            angleAnim:Resume()
        end
    end
end

function OnTurnAnimBtnClick(self)
    local ctrl = self:GetOwnerControl()
    local objTree = self:GetOwner()
    local image = ctrl:GetControlObject("simple_image")
    local left, top, right, bottom = image:GetObjPos()
    if image then
        local aniFactory = XLGetObject("Xunlei.UIEngine.AnimationFactory")
        if aniFactory then
            local turnAnim = aniFactory:CreateAnimation("TurnObjectAnimation")
            turnAnim:BindObj(image, image)
            turnAnim:SetTotalTime(1000)
            objTree:AddAnimation(turnAnim)
            turnAnim:Resume()
        end
    end
end