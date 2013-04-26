-- HornOfValereDiscoverResults
-- Author: S3rgeus
-- DateCreated: 4/26/2013 2:13:07 AM
--------------------------------------------------------------

function GiveHornPromotion(playerID, unitID)
	pPlayer = Players[playerID]
	pUnit = pPlayer:GetUnitByID(unitID)

	pUnit:SetHasPromotion(GameInfoTypes.PROMOTION_HORN_BLOWER, true)
end

function HornPromotionCaller(playerID, unitID)
	print(pcall(GiveHornPromotion(playerID, unitID)))
end
GameEvents.UnitDiscoveredHornOfValere.Add(HornPromotionCaller)