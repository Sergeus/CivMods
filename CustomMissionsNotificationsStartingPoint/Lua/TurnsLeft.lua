-- TurnsLeft
-- Author: S3rgeus
-- DateCreated: 8/22/2013 10:09:03 PM
--------------------------------------------------------------

include("PlotIterators")
include("TableSaverLoader")

gT = {}

gT.gSvestaCoords = {
	["X"] = 36,
	["Y"] = 23,
}

gT.gArgastFortCoords = {
	[0] = { 
		["X"] = 32,
		["Y"] = 26,
	},
	[1] = {
		["X"] = 32,
		["Y"] = 27,
	},
	[2] = {
		["X"] = 33,
		["Y"] = 29,
	},
	[3] = {
		["X"] = 35,
		["Y"] = 28,
	},
	[4] = {
		["X"] = 36,
		["Y"] = 27,
	},
}

gT.gNumydiaExpansionCoords = {
	["X"] = 24,
	["Y"] = 20,
}

gT.gCyatsExpansionCoords = {
	[0] = {
		["X"] = 58,
		["Y"] = 28,
	},
	[1] = {
		["X"] = 61,
		["Y"] = 33,
	},
	[2] = {
		["X"] = 59,
		["Y"] = 38,
	},
}

gT.gArgastExpansionCoords = {
	[0] = {
		["X"] = 18,
		["Y"] = 40,
	},
	[1] = {
		["X"] = 13,
		["Y"] = 40,
	},
	[2] = {
		["X"] = 50,
		["Y"] = 38,
	},
}

gT.gArgastReinforcementsFrequency = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_REINFORCEMENT_FREQUENCY.Value
gT.gArgastReinforcementsProbability = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_REINFORCEMENT_PROBABILITY.Value
gT.gArgastDeclareWarEarliest = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_DECLARE_WAR_EARLIEST.Value
gT.gArgastDenouncePlayerTurn = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_DENOUNCE_PLAYER_NOT_AT_WAR.Value

gT.gNumydiaDenounceArgastTurn = GameInfo.SiegeModConstants.SIEGEMOD_NUMYDIA_DENOUNCE_ARGAST_NOT_AT_WAR_TURN.Value
gT.gNumydiaExpansionTurn = GameInfo.SiegeModConstants.SIEGEMOD_NUMYDIA_EXPANSION_TURN.Value

gT.gCyatsExpansionMinimum = GameInfo.SiegeModConstants.SIEGEMOD_CYATS_EXPANSION_MINIMUM.Value
gT.gCyatsExpansionInterval = GameInfo.SiegeModConstants.SIEGEMOD_CYATS_EXPANSION_INTERVAL.Value
gT.gCyatsExpansionVariance = GameInfo.SiegeModConstants.SIEGEMOD_CYATS_EXPANSION_VARIANCE.Value
gT.gCyatsFreeMissionaryNearSvestaInterval = GameInfo.SiegeModConstants.SIEGEMOD_CYATS_FREE_MISSIONARY_NEAR_SVESTA_INTERVAL.Value
gT.gCyatsNumPreachersPerSpawn = GameInfo.SiegeModConstants.SIEGEMOD_NUM_PREACHERS_PER_SPAWN.Value
gT.gCyatsMinPreacherSpawnDistance = GameInfo.SiegeModConstants.SIEGEMOD_MIN_PREACHER_SPAWN_DISTANCE.Value

gT.gCyatsNextExpansionTurn = gT.gCyatsExpansionMinimum + Map.Rand(gT.gCyatsExpansionVariance, "CyatsExpansion") - Map.Rand(gT.gCyatsExpansionVariance, "CyatsExpansion")

gT.gArgastHasDenouncedSvesta = false
gT.gNumydiaHasDenouncedArgast = false

gT.gArgastNextExpansion = 0
gT.gCyatsNextExpansion = 0

gT.gGoldRequiredForWin = GameInfo.SiegeModConstants.SIEGEMOD_GOLD_REQUIRED_FOR_VICTORY.Value

gArgastReinforcements = {
	[0] = GameInfoTypes.UNIT_ARGAST_RAIDERS,
	[1] = GameInfoTypes.UNIT_CATAPULT,
}

function InitArgastWar(pArgast)
	local pArgastTeam = Teams[pArgast:GetTeam()]

	print("Initializing Argast's war with the player...")

	print("Giving Argast some free Raiders...")
	for _, coord in pairs(gT.gArgastFortCoords) do
		pArgast:InitUnit(GameInfoTypes.UNIT_ARGAST_RAIDERS, coord.X, coord.Y)
	end

	print("Declaring war...")
	pArgastTeam:DeclareWar(Players[Game.GetActivePlayer()]:GetTeam())
end

function ReinforceArgast(pArgast)
	print("Reinforcing Argast's armies...")
	for _, coord in pairs(gT.gArgastFortCoords) do
		local unitRand = Map.Rand(#gArgastReinforcements, "ArgastReinforcements")

		local spawnRand = Map.Rand(100, "ArgastReinforcements")
		
		if (spawnRand <= gT.gArgastReinforcementsProbability) then
			print("Giving unit " .. gArgastReinforcements[unitRand] .. " to Argast at " .. coord.X .. "," .. coord.Y .. "...")

			pArgast:InitUnit(gArgastReinforcements[unitRand], coord.X, coord.Y)
		end
	end
end

function MaintainArgastWar(pArgast)
	local pArgastTeam = Teams[pArgast:GetTeam()]

	local pPlayer = Players[Game.GetActivePlayer()]

	if (not pArgastTeam:IsAtWar(pPlayer:GetTeam())) then
		print("ERROR: The MaintainArgastWar function has been called even though Argast isn't at war with the player...")
		return
	end

	print("Maintaining Argast's war with the player...")
	local currentTurn = Game.GetElapsedGameTurns()

	if math.fmod(currentTurn, gT.gArgastReinforcementsFrequency) == 0 then
		ReinforceArgast(pArgast)
	end
end

function ArgastWar(playerID)
	local pSvesta = Players[Game.GetActivePlayer()]

	local pArgast
	for _, pPlayer in pairs(Players) do
		if pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_ARGAST_EMPIRE then
			print("Found Argast...")
			pArgast = pPlayer
		end
	end

	if playerID ~= pArgast:GetID() then
		return
	end

	print("Checking Argast's war status...")

	local pArgastTeam = Teams[pArgast:GetTeam()]

	local currentTurn = Game.GetElapsedGameTurns()

	print("Argast war check sees turn number " .. currentTurn .. "...")

	if not pArgastTeam:CanDeclareWar(pSvesta:GetTeam()) then
		print("Argast cannot declare war on Svesta this turn.")
	end

	if currentTurn > gT.gArgastDeclareWarEarliest and pArgastTeam:CanDeclareWar(pSvesta:GetTeam()) then
		InitArgastWar(pArgast)

	elseif currentTurn > gT.gArgastDenouncePlayerTurn and not pArgastTeam:IsAtWar(pSvesta:GetTeam()) and not gT.gArgastHasDenouncedSvesta then
		print("Denouncing active player...")
		pArgast:DoForceDenounce(pSvesta:GetID())
		gT.gArgastHasDenouncedSvesta = true

	elseif pArgastTeam:IsAtWar(pSvesta:GetTeam()) then
		MaintainArgastWar(pArgast)
	end
end
GameEvents.PlayerDoTurn.Add(ArgastWar)

function NumydiaActions(playerID)
	if playerID ~= Game.GetActivePlayer() then
		return
	end

	print("Managing Numydia's actions...")

	local currentTurn = Game.GetElapsedGameTurns()

	local pNumydia
	local pArgast

	for _, pPlayer in pairs(Players) do
		if pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_NEMYDIAN_FEDERATION then
			pNumydia = pPlayer
			print("Found Numydia player...")
		elseif pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_ARGAST_EMPIRE then
			pArgast = pPlayer
			print("Found Argast player...")
		end
	end

	local pNumydiaTeam = Teams[pNumydia:GetTeam()]
	
	if not pNumydiaTeam:IsAtWar(pArgast:GetTeam()) and not gT.gNumydiaHasDenouncedArgast and currentTurn > gT.gNumydiaDenounceArgastTurn then
		print("Numydia is now denouncing Argast...")
		pNumydia:DoForceDenounce(pArgast:GetID())
		gT.gNumydiaHasDenouncedArgast = true
	end

	if currentTurn == gT.gNumydiaExpansionTurn then
		print("Numydia is now expanding...")

		print("Founding city...")
		pNumydia:Found(gT.gNumydiaExpansionCoords.X, gT.gNumydiaExpansionCoords.Y)

		print("Giving Numydia another worker for their new city...")
		pNumydia:InitUnit(GameInfoTypes.UNIT_WORKER, gT.gNumydiaExpansionCoords.X, gT.gNumydiaExpansionCoords.Y)

		local message = Locale.ConvertTextKey("TXT_KEY_NOTIFICATION_NUMYDIA_EXPANDED")
		local summary = Locale.ConvertTextKey("TXT_KEY_NOTIFICATION_ENEMY_EXPANDED_SUMMARY", pNumydia:GetNameKey())

		Players[Game.GetActivePlayer()]:AddNotification(GameInfoTypes.NOTIFICATION_ENEMY_EXPANDED, message, summary, gT.gNumydiaExpansionCoords.X, gT.gNumydiaExpansionCoords.Y, GameInfoTypes.CIVILIZATION_NUMYDIA)
	end
end
GameEvents.PlayerDoTurn.Add(NumydiaActions)

function IsValidForPreacher(pPlot)
	if pPlot:IsWater() or pPlot:IsUnit() or pPlot:IsImpassable() then
		return false
	end

	return true
end

function SpawnCyatPreacherNear(pCyats, iCenterX, iCenterY)
	print("Spawning a Cyat preacher somewhere near " .. iCenterX .. "," .. iCenterY .. "...")

	local pPlot = Map.GetPlot(iCenterX, iCenterY)

	local r = 2

	r = r + Map.Rand(2, "CyatPreacher")

	print("Searching plots at radius " .. r .. "...")

	local numSpawned = 0
	local distance = gT.gCyatsMinPreacherSpawnDistance

	for pEdgePlot in PlotRingIterator(pPlot, r) do
		if distance >= gT.gCyatsMinPreacherSpawnDistance and IsValidForPreacher(pEdgePlot) and numSpawned < gT.gCyatsNumPreachersPerSpawn then
			print("Spawning preacher at " .. pEdgePlot:GetX() .. "," .. pEdgePlot:GetY() .. "...")
			pCyats:InitUnit(GameInfoTypes.UNIT_CYAT_PREACHER, pEdgePlot:GetX(), pEdgePlot:GetY())
			numSpawned = numSpawned + 1
			distance = 0
		else
			distance = distance + 1
		end
	end
end

function CyatsActions(playerID)
	if playerID ~= Game.GetActivePlayer() then
		return
	end

	print("Managing the Cyats actions...")

	local pCyats

	for _, pPlayer in pairs(Players) do
		if pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_CYATS then
			pCyats = pPlayer
			print("Found Cyats player...")
		end
	end

	local currentTurn = Game.GetElapsedGameTurns()

	if math.fmod(currentTurn, gT.gCyatsFreeMissionaryNearSvestaInterval) == 0 then
		print("Time to spawn a preacher near Svesta...")
		SpawnCyatPreacherNear(pCyats, gT.gSvestaCoords.X, gT.gSvestaCoords.Y)
	end

	if currentTurn == gT.gCyatsNextExpansionTurn and gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion] ~= nil then
		print("Cyats are expanding now...")

		print("Founding city...")
		pCyats:Found(gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].X, gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].Y)

		if math.fmod(gT.gCyatsNextExpansion, 2) == 0 then
			print("Giving them a new worker to go with the city...")
			pCyats:InitUnit(GameInfoTypes.UNIT_WORKER, gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].X, gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].Y)
		end

		print("Notifying player...")

		local pCity = Map.GetPlot(gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].X, gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].Y):GetPlotCity()

		local message = Locale.ConvertTextKey("TXT_KEY_NOTIFICATION_ENEMY_EXPANDED", pCyats:GetNameKey(), pCity:GetNameKey())
		local summary = Locale.ConvertTextKey("TXT_KEY_NOTIFICATION_ENEMY_EXPANDED_SUMMARY", pCyats:GetNameKey())

		Players[Game.GetActivePlayer()]:AddNotification(GameInfoTypes.NOTIFICATION_ENEMY_EXPANDED, message, summary,
			gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].X, gT.gCyatsExpansionCoords[gT.gCyatsNextExpansion].Y, GameInfoTypes.CIVILIZATION_CYATS)

		print("Increasing city index...")
		gT.gCyatsNextExpansion = gT.gCyatsNextExpansion + 1

		gT.gCyatsNextExpansionTurn = gT.gCyatsNextExpansionTurn + gT.gCyatsExpansionInterval 
			+ Map.Rand(gT.gCyatsExpansionVariance, "CyatsExpansion") - Map.Rand(gT.gCyatsExpansionVariance, "CyatsExpansion")
	end
end
GameEvents.PlayerDoTurn.Add(CyatsActions)

function ScaleConstantsBasedOnDifficulty()
	print("Scenario constants not yet scaled by difficulty...")

	print("Telling other contexts about the gold required to win...")

	LuaEvents.SiegeModGoldRequiredChanged(gT.gGoldRequiredForWin)
end

function GetSiegeModGoldRequired()
	print("Return gold required" .. gT.gGoldRequiredForWin .. "...")
	LuaEvents.SiegeModGoldRequiredChanged(gT.gGoldRequiredForWin)
end
LuaEvents.SiegeModGoldRequired.Add(GetSiegeModGoldRequired)

ScaleConstantsBasedOnDifficulty()

function Init()
	local bNewGame = true
	local DBQuery = Modding.OpenSaveData().Query
	for row in DBQuery("SELECT name FROM sqlite_master WHERE name='SiegeMod_Info'") do
		if row.name then bNewGame = false end	-- presence of SiegeMod_Info tells us that game already in session
	end

	if bNewGame then
		print("Initializing for new game...")
	else
		print("Initializing for loaded game...")
		TableLoad(gT, "SiegeMod")
	end
end

Init()

function SaveGame()
	print("Saving the game...")
	TableSave(gT, "SiegeMod")
end
GameEvents.GameSaving.Add(SaveGame)