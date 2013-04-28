-- HornOfValereBlowingConditions
-- Author: S3rgeus
-- DateCreated: 4/25/2013 5:08:21 AM
--------------------------------------------------------------

function CanBlowHornOfValere(playerID, unitID, iMission, bTestVisible)

	if (iMission ~= GameInfoTypes.MISSION_BLOW_HORN_OF_VALERE) then
		return false
	end

	pPlayer = Players[playerID]
	pUnit = pPlayer:GetUnitByID(unitID)

	if (not pUnit:IsHasPromotion(GameInfoTypes.PROMOTION_HORN_BLOWER)) then
		return false
	end

	local iCooldown = GameInfo.WoTModConstants["HORN_OF_VALERE_COOLDOWN"].Value
	local iTurnsSince = Map.GetTurnsSinceHornBlown();

	if (not bTestVisible and iTurnsSince < iCooldown) then
		print("The Horn of Valere has been used too recently.")
		print("We're supposed to wait " .. iCooldown .. " turns, but it's only been " .. iTurnsSince .. ".")
		return false
	end

	print("This guy can blow the Horn, no problem.")
	return true
end
GameEvents.UnitCanHandleMission.Add(CanBlowHornOfValere)