-- HornOfValereFindingConditions
-- Author: S3rgeus
-- DateCreated: 4/24/2013 2:54:58 AM
--------------------------------------------------------------

function CheckIfCanFindHornOfValere(playerID, unitID)
	print("Testing if unit with ID " .. unitID .. " owned by player with ID " .. playerID .. " can discover the Horn of Valere.")

	local pPlayer = Players[playerID]

	local pUnit = pPlayer:GetUnitByID(unitID)

	if (pUnit:IsHasPromotion(GameInfoTypes.PROMOTION_HORN_HUNTER)) then
		print("This unit can discover the Horn Of Valere.")
		return true
	end

	print("This unit cannot discover the Horn of Valere.")

	return false
end
GameEvents.UnitCanDiscoverHornOfValere.Add(CheckIfCanFindHornOfValere)