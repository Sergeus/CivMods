-- MissionSubscribers
-- Author: S3rgeus
-- DateCreated: 3/11/2014 7:49:34 PM
--------------------------------------------------------------

function CanTrainAtTower(playerID, unitID, bTestVisible)
	local pPlayer = Players[playerID]
	local pUnit = pPlayer:GetUnitByID(unitID)

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
GameEvents.CanTrainWhiteTower.Add(CanTrainAtTower)

function UnitTrainingAtTower(playerID, unitID)
	local pUnit = Players[playerID]:GetUnitByID(unitID)

	pUnit:DoTrainAtTower()

	return true
end
GameEvents.TrainWhiteTower.Add(UnitTrainingAtTower)

function StartBondWarder(playerID, unitID)
	UI.SetInterfaceMode( InterfaceModeTypes.INTERFACEMODE_RANGE_ATTACK )
	return true
end
GameEvents.StartBondWarder.Add(StartBondWarder)

print("White Tower missions")