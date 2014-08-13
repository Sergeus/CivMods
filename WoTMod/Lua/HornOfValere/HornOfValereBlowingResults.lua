-- HornOfValereBlowingResults
-- Author: S3rgeus
-- DateCreated: 4/24/2013 9:38:24 PM
--------------------------------------------------------------

include("PlotIterators")

-- Returns true if the parameter player can spawn a hero on the parameter plot
function CanSpawnHero(pPlayer, pPlot)
	-- Heroes can spawn on non-hostile hexes that don't already have a unit on them.
	if ((not pPlot:IsOwned() or pPlot:GetOwner() == pPlayer:GetID()) and not pPlot:IsUnit() and not pPlot:IsWater()) then
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

-- Returns a table of heroes based on the XML defined probabilities and the parameter telling it how many are needed
function FindHeroes(iNumToSpawn)
	local tHeroes = {}
	local iSpawned = 0

	print("Finding " .. iNumToSpawn .. " heroes...")

	while iSpawned < iNumToSpawn do
		print("Current iSpawned " .. iSpawned)
		for pHero in GameInfo.HornHeroes() do
			local chance = pHero.HeroChance
			print("Chance of " .. chance .. " to spawn " .. pHero.HeroType)
			if iSpawned <= iNumToSpawn then
				tHeroes[iSpawned] = GameInfoTypes[pHero.HeroType]
				iSpawned = iSpawned + 1
				print("Adding " .. pHero.HeroType .. " to the table.")
			end
		end
	end

	print("Returning heroes table.")

	return tHeroes
end

-- Spawns the heroes in the tHeroes table onto the plots in the tPlots table
function SpawnHeroes(pPlayer, tPlots, tHeroes)
	for i = 0, #tPlots do
		local pPlot = tPlots[i]
		local iHero = tHeroes[i]
		print("Spawning hero with ID " .. iHero .. "!")
		pPlayer:InitUnit(iHero, pPlot:GetX(), pPlot:GetY())
	end
end

-- Sends notifications to all other players that the parameter player/unit combo has blown the Horn of Valere
function SendNotifications(pPlayer, pUnit)
	local pTeam = Teams[pPlayer:GetTeam()]
	local pOtherTeam

	-- loop over all players
	for id, pOtherPlayer in pairs(Players) do
		-- don't need to tell the person who actually blew the horn
		if (id ~= pPlayer:GetID()) then
			pOtherTeam = Teams[pOtherPlayer:GetTeam()]

			-- If this player can see the plot with the unit, then notify them of the exact unit that blew the Horn
			if (pUnit:GetPlot():GetVisibilityCount(id) > 0) then
				local message = Locale.ConvertTextKey("TXT_KEY_NOTIFICATION_HORN_OF_VALERE_BLOWN_KNOWN_UNIT", pPlayer:GetNameKey(), pUnit:GetNameKey())

				pOtherPlayer:AddNotification(GameInfoTypes.NOTIFICATION_HORN_OF_VALERE_BLOWN, message, nil, pUnit:GetX(), pUnit:GetY(), 0, pPlayer:GetID())
			-- Otherwise we just tell them which Civ has blown the horn, and don't move to the plot
			else
				local message = Locale.ConvertTextKey("TXT_KEY_NOTIFICATION_HORN_OF_VALERE_BLOWN_UNKNOWN_UNIT", pPlayer:GetNameKey())

				pOtherPlayer:AddNotification(GameInfoTypes.NOTIFICATION_HORN_OF_VALERE_BLOWN, message, nil, -1, -1, 0, pPlayer:GetID())
			end

			-- If the players haven't met, the notification is replaced automatically
		end
	end
end

function ApplyHornOfValereEffects(playerID, unitID, iMission)
	
	if (iMission ~= GameInfoTypes.MISSION_BLOW_HORN_OF_VALERE) then
		return false
	end

	local pPlayer = Players[playerID]
	local pUnit = pPlayer:GetUnitByID(unitID)

	local tPlots = FindHornHeroSpawns(pUnit)
	print("Found spawns")
	local tHeroes = FindHeroes(#tPlots)

	SpawnHeroes(pPlayer, tPlots, tHeroes)

	print("Resetting turns since Horn blown.")
	Map.SetTurnsSinceHornBlown(0)
	pUnit:SetMoves(0)

	SendNotifications(pPlayer, pUnit)

	return true
end
GameEvents.UnitHandlingMission.Add(ApplyHornOfValereEffects)