-- MissionSubscribers
-- Author: S3rgeus
-- DateCreated: 3/11/2014 7:49:34 PM
--------------------------------------------------------------

include("RangeSelection")

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

function StartBondWarder()
	local pUnit = UI.GetHeadSelectedUnit()
	local bShift = UIManager:GetShift()

	local IsValidTarget = function (pPlot)
		return (Map.PlotDistance(pPlot:GetX(), pPlot:GetY(), pUnit:GetX(), pUnit:GetY()) <= 1)
	end

	local SelectionData = {
		InterfaceMode = GameInfoTypes.INTERFACEMODE_BOND_WARDER,
		CenterPlot = pUnit:GetPlot(),
		Range = 1,
		IsValidTarget = IsValidTarget,
		Select = function (pPlot)
			if (not pUnit:CanStartMission(GameInfoTypes.MISSION_BOND_WARDER, pPlot:GetX(), pPlot:GetY(), pUnit:GetPlot(), false)) then
				return false
			else
				Game.SelectionListGameNetMessage(GameMessageTypes.GAMEMESSAGE_PUSH_MISSION, MissionTypes.MISSION_BOND_WARDER, 
					pPlot:GetX(), pPlot:GetY(), pUnit:GetPlot(), false, bShift)
				return true
			end
		end
	}

	DoSelection(SelectionData)

	return true
end

function InterfaceModeChanged(oldInterfaceMode, newInterfaceMode)
	if (newInterfaceMode == GameInfoTypes.INTERFACEMODE_BOND_WARDER) then
		StartBondWarder()
	end
end
Events.InterfaceModeChanged.Add(InterfaceModeChanged)

print("White Tower missions")