-- DoubleUnits
-- Author: S3rgeus
-- DateCreated: 8/11/2013 12:13:45 AM
--------------------------------------------------------------
-- Note: This script requires the custom DLL included in SiegeMod to function correctly

function GiveDoubleUnits(iPlayer, iCity, iUnit)
	print("Give double units invoked")
	
	local pPlayer = Players[iPlayer]
	local iLeader = pPlayer:GetLeaderType()

	print("Got leader type " .. iLeader)

	-- TODO continue this script
end
GameEvents.CityTrained.Add(GiveDoubleUnits)
-- TODO DLL-side implementation for CityTrained event