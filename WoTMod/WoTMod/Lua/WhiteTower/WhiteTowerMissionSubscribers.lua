-- MissionSubscribers
-- Author: S3rgeus
-- DateCreated: 3/11/2014 7:49:34 PM
--------------------------------------------------------------

function CanTrainAtTower(playerID, unitID, missionID, bTestVisible)
	if (missionID ~= GameInfoTypes.MISSION_TRAIN_WHITE_TOWER) then
		return false
	end

	pPlayer = Players[playerID]
	pUnit = pPlayer:GetUnitByID(unitID)

	if (not pUnit:IsHasPromotion(GameInfoTypes.PROMOTION_TOWER_TRAINEE)) then
		return false
	end

	local plotOwner = Players[pUnit:GetPlot():GetOwner()]

	if (bTestVisible or (plotOwner ~= nil and plotOwner:IsHostsAjahs() 
			and plotOwner:GetID() ~= playerID)) then
		return true
	end

	return false
end
GameEvents.UnitCanHandleMission.Add(CanTrainAtTower)

function UnitTrainingAtTower(playerID, unitID, missionID)
	if (missionID ~= GameInfoTypes.MISSION_TRAIN_WHITE_TOWER) then
		return false
	end

	local pUnit = Players[playerID]:GetUnitByID(unitID)

	pUnit:DoTrainAtTower()

	return true
end
GameEvents.UnitHandlingMission.Add(UnitTrainingAtTower)

print("White Tower missions")