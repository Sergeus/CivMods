-- ThreadPopup
-- Author: S3rgeus
-- DateCreated: 5/4/2015 1:23:41 AM
--------------------------------------------------------------

include("FLuaVector")
include("IconSupport")

local m_ThreadId = -1

function DisplayThread()
	local threadInfo = GameInfo.Threads[m_ThreadId]
	Controls.ThreadTitleLabel:LocalizeAndSetText(threadInfo.ThreadTitle)
	Controls.ThreadFlavorBodyLabel:LocalizeAndSetText(threadInfo.FlavorBody)

	Controls.ChoiceStack:DestroyAllChildren()
	for i,choiceId in pairs(Game.GetChoicesForThread(m_ThreadId)) do
		local choiceInfo = GameInfo.ThreadChoices[choiceId]
		local instance = {}
		ContextPtr:BuildInstanceForControl("Choice", instance, Controls.ChoiceStack);

		if (i == 1) then
			instance.TopBar:SetHide(false)
		end

		instance.ChoiceDescription:LocalizeAndSetText(choiceInfo.Description)
		
		local bonusesDescription = ""
		for yieldInfo in GameInfo.Yields() do
			local yield = Game.GetYieldForThreadChoice(choiceId, yieldInfo.ID)
			if (yield ~= 0) then
				
				if (bonusesDescription ~= "") then
					bonusesDescription = bonusesDescription .. ", "
				end

				if (yield > 0) then
					bonusesDescription = bonusesDescription .. "[COLOR_POSITIVE_TEXT]"
				else
					bonusesDescription = bonusesDescription .. "[COLOR_NEGATIVE_TEXT]"
				end

				bonusesDescription = bonusesDescription .. "+" .. yield .. "[ENDCOLOR]" .. yieldInfo.IconString .. " " .. Locale.ConvertTextKey(yieldInfo.Description)
			end
		end

		instance.BonusesDescription:SetText(bonusesDescription)
		instance.ChooseButton:RegisterCallback(Mouse.eLClick, function() 
			Players[Game.GetActivePlayer()]:DoChooseThreadChoice(choiceId)
			UIManager:DequeuePopup(ContextPtr)
		end)
	end

	Controls.ChoiceStack:CalculateSize()
	Controls.ChoiceStack:ReprocessAnchoring()
end

--------------------------------------------------------------
-- Notification handler
--------------------------------------------------------------

function ThreadAvailableActivated(playerID, notificationID, msg, summary, iX, iY, iGameData, iExtraData)
	if notificationID ~= GameInfoTypes.NOTIFICATION_NEW_THREAD then
		return
	end

	m_ThreadId = iGameData

	UIManager:QueuePopup(ContextPtr, PopupPriority.eUtmost)
end
GameEvents.PlayerNotificationActivated.Add(ThreadAvailableActivated)

--------------------------------------------------------------
-- Visibility Management
--------------------------------------------------------------

ContextPtr:SetHide(true)

function OnClose()
	UIManager:DequeuePopup(ContextPtr)
end
Controls.CloseButton:RegisterCallback(Mouse.eLClick, OnClose)

function OnShowHide(bIsHiding, bIsInit)
	if not bIsHiding then
		OnDisplay()
	end
end
ContextPtr:SetShowHideHandler(OnShowHide)

function OnDisplay()
	if (m_threadId ~= -1) then
		DisplayThread()
	end
end

function InputHandler( uiMsg, wParam, lParam )
    if uiMsg == KeyEvents.KeyDown then
        if wParam == Keys.VK_ESCAPE then
            OnClose();
            return true;
		end
    end
end
ContextPtr:SetInputHandler( InputHandler );