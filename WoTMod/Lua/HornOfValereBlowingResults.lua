-- HornOfValereBlowingResults
-- Author: S3rgeus
-- DateCreated: 4/24/2013 9:38:24 PM
--------------------------------------------------------------

include("PlotIterators")

-- Returns true if the parameter player can spawn a hero on the parameter plot
function CanSpawnHero(pPlayer, pPlot)
	if ((not pPlot:IsOwned() or pPlot:GetOwner() == pPlayer:GetID()) and not pPlot:IsUnit()) then
		return true
	end

	return false
end

-- Finds appropriate plots for heroes of the horn to spawn on around the parameter unit
-- Returns a table containing those plots
function FindHornHeroSpawns(pUnit)
	local pCenterPlot = pUnit:GetPlot()
	local iNumHeroesToSpawn = GameInfo.WoTModConstants["HORN_OF_VALERE_NUMBER_HEROES"].Value
	local tPlots = {}

	print("Supposed to spawn " .. iNumHeroesToSpawn .. " heroes.")

	local iNumberAdded = 0;
	-- Work your way out around the unit for spawn points, up to 2 hexes away
	for pNeighborPlot in PlotAreaSpiralIterator(pCenterPlot, 2, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
		if (CanSpawnHero(Players[pUnit:GetOwner()], pNeighborPlot)) then
			tPlots[iNumberAdded] = pNeighborPlot;
			iNumberAdded = iNumberAdded + 1
			print ("Adding " .. iNumberAdded .. " heroes.")
			if (iNumberAdded >= iNumHeroesToSpawn) then
				print("Returning plots.")
				return tPlots
			end
		end
	end

	-- If we didn't find enough plots, we'll just spawn fewer heroes
	return tPlots
end

-- Spawns a hero owned by the parameter player on the parameter plot
function SpawnHero(pPlayer, pPlot)
	pPlayer:InitUnit(GameInfoTypes.UNIT_HORN_HERO_HAWKWING, pPlot:GetX(), pPlot:GetY())
end

function ApplyHornOfValereEffects(playerID, unitID, iXPos, iYPos)
	print("Horn of Valere has been blown at " .. iXPos .. ", " .. iYPos .. ".")

	local pPlayer = Players[playerID]
	local pUnit = pPlayer:GetUnitByID(unitID)

	local tPlots = FindHornHeroSpawns(pUnit)

	for i = 0, #tPlots do
		print("Iterating over plots, on index " .. i .. ".")
		SpawnHero(pPlayer, tPlots[i])
	end
end

function EffectsCaller(playerID, unitID, iXPos, iYPos)
	print(pcall(ApplyHornOfValereEffects, playerID, unitID, iXPos, iYPos))
end
GameEvents.HornOfValereBlown.Add(EffectsCaller)