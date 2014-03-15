-- WhiteTowerStatus
-- Author: S3rgeus
-- DateCreated: 3/2/2014 7:34:30 PM
--------------------------------------------------------------

include("FLuaVector")
include("IconSupport")

--------------------------------------------------------------
-- Game Logic
--------------------------------------------------------------

local m_CityStateId

local m_bFeedbackMode
local m_eChangedAjah
local m_iOldPercent

-- Expectation that this event will be called before the popup is displayed to tell
-- it what city state we're talking about (technical support for multiple competing
-- Tar Valons for eventual Salidar scenario?)
function InitTarValonStatus(cityStateId)
	m_CityStateId = cityStateId
end
LuaEvents.TarValonStatus.Add(InitTarValonStatus)

function AjahInfluenceChanged(playerID, unitID, towerID, ajahID, iOldInfluence)
	if (playerID == Game.GetActivePlayer()) then
		m_CityStateId = towerID

		m_bFeedbackMode = true
		m_eChangedAjah = ajahID
		m_iOldPercent = iOldInfluence

		UIManager:QueuePopup(ContextPtr, PopupPriority.eUtmost)
	end
end
GameEvents.TowerTraineeChangedAjahInfluence.Add(AjahInfluenceChanged)

function TowerNotificationActivated(playerID, notificationID, msg, summary, iX, iY, iGameData, iExtraData)
	if notificationID ~= GameInfoTypes.NOTIFICATION_TOWER_MAJORITY_AJAH_HAS_CHANGED
		and notificationID ~= GameInfoTypes.NOTIFICATION_TOWER_AMYRLIN_ELECTED 
	then
		return
	end

	m_CityStateId = iGameData

	UIManager:QueuePopup(ContextPtr, PopupPriority.eUtmost)
end
GameEvents.PlayerNotificationActivated.Add(TowerNotificationActivated)

function TowerPledgeNotificationActivated(playerID, notificationID, msg, summary, iX, iY, iGameData, iExtraData)
	if notificationID ~= GameInfoTypes.NOTIFICATION_PLAYER_PLEDGED_AJAH_SUPPORT then
		return
	end

	m_CityStateId = Players[iExtraData]:GetPublicSupportedTower()

	UIManager:QueuePopup(ContextPtr, PopupPriority.eUtmost)
end
GameEvents.PlayerNotificationActivated.Add(TowerPledgeNotificationActivated)

--------------------------------------------------------------
-- Visibility Management
--------------------------------------------------------------

ContextPtr:SetHide(true)

function OnCloseClicked()
	UIManager:DequeuePopup(ContextPtr)
end
Controls.CloseButton:RegisterCallback(Mouse.eLClick, OnCloseClicked)

function OnShowHide(bIsHiding, bIsInit)
	if not bIsHiding then
		OnDisplay()
	end
end
ContextPtr:SetShowHideHandler(OnShowHide)

function OnDisplay()
	local pPlayer = Players[m_CityStateId]
	local amyrlinAjah = pPlayer:GetAmyrlinAjah()

	CivIconHookup( pPlayer:GetID(), 80, Controls.CivIcon, Controls.CivIconBG, Controls.CivIconShadow, false, true );

	Controls.InfluenceStack:DestroyAllChildren()

	for pAjah in GameInfo.Ajahs() do
		if (pPlayer:IsAjahPermitted(pAjah.ID)) then
			local instance = {}
			local sAjahName = Locale.ConvertTextKey(pAjah.Description)
			local iAjahPercent = pPlayer:GetAjahInfluencePercent(pAjah.ID)
			local sAjahPercent = iAjahPercent .. "%"

			local ajahColorInfo = GameInfo.Colors[pAjah.Color]

			ContextPtr:BuildInstanceForControl("AjahInfluence", instance, Controls.InfluenceStack);

			instance.AjahLabel:SetText(sAjahName)
			instance.AjahPercent:SetText(sAjahPercent)

			local activePlayer = Players[Game.GetActivePlayer()]
			if (activePlayer:IsCanPledgeAjahSupport()) then
				-- and then there was scope!
				local onClick = function()
					Controls.Yes:RegisterCallback(Mouse.eLClick,
						function()
							m_bFeedbackMode = true
							m_eChangedAjah = pAjah.ID
							m_iOldPercent = iAjahPercent
							
							activePlayer:DoPledgeSupportForAjah(m_CityStateId, pAjah.ID)
							Controls.ChooseConfirm:SetHide(true)
							OnDisplay()
						end)

					Controls.ConfirmText:LocalizeAndSetText("TXT_KEY_CONFIRM_PLEDGE_SUPPORT", 
						pAjah.Description, pPlayer:GetCapitalCity():GetName())

					Controls.ChooseConfirm:SetHide(false)
				end

				instance.AjahButton:RegisterCallback(Mouse.eLClick, onClick)
			else
				instance.AjahButton:SetDisabled(true)

				instance.AjahButton:SetToolTipString(Locale.ConvertTextKey("TXT_KEY_PLEDGE_SUPPORT_DISABLED"))
			end

			if (m_bFeedbackMode and pAjah.ID == m_eChangedAjah) then
				instance.BGInfluenceBar:SetFGColor(Color(ajahColorInfo.Red, ajahColorInfo.Green, ajahColorInfo.Blue, 0.5))
				instance.BGInfluenceBar:SetHide(false)

				instance.InfluenceBar:SetPercent(m_iOldPercent / 100)
				instance.BGInfluenceBar:SetPercent(iAjahPercent / 100)

				m_bFeedbackMode = false
			else
				instance.InfluenceBar:SetPercent(iAjahPercent / 100)
			end
			
			instance.InfluenceBar:SetFGColor(Color(ajahColorInfo.Red, ajahColorInfo.Green, ajahColorInfo.Blue, ajahColorInfo.Alpha))

			instance.AjahContainer:SetToolTipString(Locale.ConvertTextKey(pAjah.Help))

			if (amyrlinAjah == pAjah.ID) then
				instance.AmyrlinSymbol:SetHide(false)
			else
				instance.AmyrlinSymbol:SetHide(true)
			end

			instance.AjahStack:CalculateSize()
			instance.AjahStack:ReprocessAnchoring()
		end
	end

	Controls.InfluenceStack:CalculateSize()
	Controls.InfluenceStack:ReprocessAnchoring()
end

function OnNoClicked()
	Controls.ChooseConfirm:SetHide(true)
end
Controls.No:RegisterCallback(Mouse.eLClick, OnNoClicked)