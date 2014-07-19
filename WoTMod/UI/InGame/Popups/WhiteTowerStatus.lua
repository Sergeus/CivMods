-- WhiteTowerStatus
-- Author: S3rgeus
-- DateCreated: 3/2/2014 7:34:30 PM
--------------------------------------------------------------

include("FLuaVector")
include("IconSupport")

--------------------------------------------------------------
-- Game Logic
--------------------------------------------------------------

local m_CityStateId = -1

local m_bFeedbackMode -- if true, we're displaying a change in influence, rather than just the current status
local m_bChangedAjahs = {} -- table of bools, indexed by Ajah DB ID, indicating whether the corresponding Ajah has changed
local m_iOldPercent = {} -- table of ints, indexed by Ajah DB ID, indicating the lower percent to be rendered with a solid color

-- Calling this lua event brings up the Tower summary window, where the Tower
-- is the parameter player ID (technical support for multiple competing
-- Tar Valons for eventual Salidar scenario?)
function InitTarValonStatus(cityStateId)
	m_CityStateId = cityStateId
	UIManager:QueuePopup(ContextPtr, PopupPriority.eUtmost)
end
LuaEvents.DisplayTarValonStatus.Add(InitTarValonStatus)

function AjahInfluenceChanged(playerID, unitID, towerID, ajahID, iSolidInfluence)
	if (playerID == Game.GetActivePlayer()) then
		m_CityStateId = towerID

		m_bFeedbackMode = true
		m_bChangedAjahs[ajahID] = true
		m_iOldPercent[ajahID] = iSolidInfluence

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
	if (m_CityStateId ~= -1) then
		DisplayAjahInfluences()
	else
		Controls.InfluenceStack:SetHide(true)
		Controls.WhiteTowerStatusLabel:SetText(Locale.ConvertTextKey("TXT_KEY_NO_AJAH_HOST_MET"))
	end
end

function DisplayAjahInfluences()
	local pPlayer = Players[m_CityStateId]
	local amyrlinAjah = pPlayer:GetAmyrlinAjah()

	CivIconHookup( pPlayer:GetID(), 80, Controls.CivIcon, Controls.CivIconBG, Controls.CivIconShadow, false, true );

	Controls.InfluenceStack:SetHide(false)
	Controls.WhiteTowerStatusLabel:SetText(Locale.ConvertTextKey("TXT_KEY_WHITE_TOWER_STATUS"))
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
			local activePlayerSupportedAjah = activePlayer:GetPublicSupportedAjah()
			print("Supported Tower is " .. activePlayer:GetPublicSupportedTower() .. " and Ajah is " .. pAjah.ID)
			if (activePlayer:GetPublicSupportedTower() == m_CityStateId and activePlayerSupportedAjah == pAjah.ID) then
				instance.AjahButton:SetDisabled(true)

				instance.AjahButton:SetToolTipString(Locale.ConvertTextKey("TXT_KEY_PLEDGE_SUPPORT_DISABLED_ALREADY"))

				instance.AjahLabel:SetColor(Color(ajahColorInfo.Red, ajahColorInfo.Green, ajahColorInfo.Blue, ajahColorInfo.Alpha), 0)
			elseif (activePlayer:IsCanPledgeAjahSupport()) then
				-- and then there was scope!
				local onClick = function()
					Controls.Yes:RegisterCallback(Mouse.eLClick,
						function()
							m_bFeedbackMode = true
							m_bChangedAjahs[pAjah.ID] = true
							m_iOldPercent[pAjah.ID] = iAjahPercent

							if (activePlayerSupportedAjah ~= -1) then -- possibly need a better condition?
								m_bChangedAjahs[activePlayerSupportedAjah] = true
								m_iOldPercent[activePlayerSupportedAjah] = pPlayer:GetAjahInfluencePercent(activePlayerSupportedAjah)
							end
							
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

			if (m_bFeedbackMode and m_bChangedAjahs[pAjah.ID]) then
				instance.BGInfluenceBar:SetFGColor(Color(ajahColorInfo.Red, ajahColorInfo.Green, ajahColorInfo.Blue, 0.5))
				instance.BGInfluenceBar:SetHide(false)

				local solidPercent = math.min(m_iOldPercent[pAjah.ID] / 100, iAjahPercent / 100)
				local transparentPercent = math.max(m_iOldPercent[pAjah.ID] / 100, iAjahPercent / 100)

				instance.InfluenceBar:SetPercent(solidPercent)
				instance.BGInfluenceBar:SetPercent(transparentPercent)

				m_bFeedbackMode = false
				m_bChangedAjahs[pAjah.ID] = false
			else
				instance.InfluenceBar:SetPercent(iAjahPercent / 100)
				-- TODO: set default text color
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

LuaEvents.AdditionalInformationDropdownGatherEntries.Add(function(entries)
	local anyAjahs = false
	for _, pPlayer in pairs(Players) do
		if (pPlayer:IsHostsAjahs() and pPlayer:GetCapitalCity() ~= nil
				and Teams[Players[Game.GetActivePlayer()]:GetTeam()]:IsHasMet(pPlayer:GetTeam())) then
			anyAjahs = true
			print("Adding met tower")
			table.insert(entries, {
				text = Locale.ConvertTextKey("TXT_KEY_TOWER_AT_CITY", pPlayer:GetCapitalCity():GetName()),
				call = function() 
					LuaEvents.DisplayTarValonStatus(pPlayer:GetID())
				end
			})
		end
	end

	if (not anyAjahs) then
		print("Adding unknown tower")
		table.insert(entries, {
			text = Locale.Lookup("TXT_KEY_TOWER_UNKNOWN"),
			call = function() 
				LuaEvents.DisplayTarValonStatus(-1)
			end
		})	
	end
end)

LuaEvents.RequestRefreshAdditionalInformationDropdownEntries();