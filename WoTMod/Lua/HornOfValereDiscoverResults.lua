-- HornOfValereDiscoverResults
-- Author: S3rgeus
-- DateCreated: 4/26/2013 2:13:07 AM
--------------------------------------------------------------

function GiveHornPromotion(playerID, unitID)
	print("Giving Horn promotion to player " .. playerID .. " unit ID " .. unitID .. ".")
	pPlayer = Players[playerID]
	pUnit = pPlayer:GetUnitByID(unitID)

	pUnit:SetHasPromotion(GameInfoTypes.PROMOTION_HORN_BLOWER, true)

	local message = Locale.ConvertTextKey("TXT_KEY_HORN_DISCOVERED", pUnit:GetNameKey())

	pPlayer:AddNotification(GameInfoTypes.NOTIFICATION_HORN_OF_VALERE_DISCOVERED, message, nil, pUnit:GetX(), pUnit:GetY(), pUnit:GetUnitType())
end

function HornPromotionCaller(playerID, unitID)
	print(pcall(GiveHornPromotion(playerID, unitID)))
end
GameEvents.UnitDiscoveredHornOfValere.Add(HornPromotionCaller)