-- WhiteTowerStatus
-- Author: S3rgeus
-- DateCreated: 3/2/2014 7:34:30 PM
--------------------------------------------------------------

include("FLuaVector")

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

			local ajahColorInfo = GameInfo.Colors[pAjah.Color]

			ContextPtr:BuildInstanceForControl("AjahInfluence", instance, Controls.InfluenceStack);

			instance.AjahLabel:SetText(sAjahName)
			instance.AjahPercent:SetText(sAjahPercent)
			instance.InfluenceBar:SetPercent(iAjahPercent / 100)
			instance.InfluenceBar:SetFGColor(Color(ajahColorInfo.Red, ajahColorInfo.Green, ajahColorInfo.Blue, ajahColorInfo.Alpha))

			instance.AjahStack:SetToolTipString(Locale.ConvertTextKey(pAjah.Help))
		end
	end

	Controls.InfluenceStack:CalculateSize()
end