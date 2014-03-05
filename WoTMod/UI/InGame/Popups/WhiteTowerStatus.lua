-- WhiteTowerStatus
-- Author: S3rgeus
-- DateCreated: 3/2/2014 7:34:30 PM
--------------------------------------------------------------

--------------------------------------------------------------
-- Game Logic
--------------------------------------------------------------

local m_CityStateId

-- Expectation that this event will be called before the popup is displayed to tell
-- it what city state we're talking about (technical support for multiple competing
-- Tar Valons for eventual Salidar scenario?)
function InitTarValonStatus(cityStateId)
	m_CityStateId = cityStateId
end
LuaEvents.TarValonStatus.Add(InitTarValonStatus)

--------------------------------------------------------------
-- Visibility Controls
--------------------------------------------------------------

ContextPtr:SetHide(true)

function OnOkClicked()
	UIManager:DequeuePopup(ContextPtr)
end
Controls.OkButton:RegisterCallback(Mouse.eLClick, OnOkClicked)

function OnShowHide(bIsHiding, bIsInit)
	if not bIsHiding then
		OnDisplay()
	end
end
ContextPtr:SetShowHideHandler(OnShowHide)

function OnDisplay()
	local pPlayer = Players[m_CityStateId]

	Controls.InfluenceStack:DestroyAllChildren()

	for pAjah in GameInfo.Ajahs() do
		print("Processing Ajah " .. pAjah.ID)

		if (pPlayer:IsAjahPermitted(pAjah.ID)) then
			local instance = {}
			local sAjahName = Locale.ConvertTextKey(pAjah.Description)
			local iAjahPercent = pPlayer:GetAjahInfluencePercent(pAjah.ID)
			local sAjahPercent = iAjahPercent .. "%"

			local colorInfo = GameInfo.Colors[pAjah.Color]

			local ajahColor = { x = colorInfo.Red, y = colorInfo.Green, z = colorInfo.Blue, w = colorInfo.Alpha }

			print ("Found color " .. colorInfo.Type .. " R:" .. colorInfo.Red .. " G:" .. colorInfo.Green .. " B:" .. colorInfo.Blue .. " A:" .. colorInfo.Alpha)

			ContextPtr:BuildInstanceForControl("AjahInfluence", instance, Controls.InfluenceStack);

			instance.AjahLabel:SetText(sAjahName)
			instance.AjahPercent:SetText(sAjahPercent)
			instance.InfluenceBar:SetPercent(iAjahPercent / 100)
			instance.InfluenceBar:SetColor(ajahColor)

			instance.AjahStack:SetToolTipString(Locale.ConvertTextKey(pAjah.Help))
		end
	end

	Controls.InfluenceStack:CalculateSize()
end

function GetColorInfo(colorType)
	for pColor in GameInfo.Colors() do
		if pColor.Type == colorType then
			return pColor
		end
	end

	return nil
end