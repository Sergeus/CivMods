-- GovernorsMissionsSubscribers
-- Author: S3rgeus
-- DateCreated: 10/26/2013 6:14:42 PM
--------------------------------------------------------------

function CanGovernCity(playerID, unitID, bTestVisible)
	local pUnit = Players[playerID]:GetUnitByID(unitID)

	if (not pUnit:IsCanGovernCities()) then
		return false
	end

	if (not bTestVisible and not pUnit:GetPlot():IsCity()) then
		return false
	end

	return true
end
GameEvents.CanCreateGovernor.Add(CanGovernCity)

function UnitGoverningCity(playerID, unitID)
	local pUnit = Players[playerID]:GetUnitByID(unitID)

	if not pUnit:GetPlot():IsCity() then
		print("This unit (" .. pUnit:GetName() .. ") is trying to become a governor while not standing on a city")
		return false
	end

	pUnit:DoGovernCity()

	return true
end
GameEvents.CreateGovernor.Add(UnitGoverningCity)