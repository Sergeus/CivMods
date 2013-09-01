-- TurnsLeft
-- Author: S3rgeus
-- DateCreated: 8/22/2013 10:09:03 PM
--------------------------------------------------------------

gArgastFortCoords = {
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

gNumydiaExpansionCoords = {
	["X"] = 24,
	["Y"] = 20,
}

gCyatsExpansionCoords = {
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

gArgastExpansionCoords = {
	[0] = {
		["X"] = 18,
		["Y"] = 40,
	},
	[1] = {
		["X"] = 13,
		["Y"] = 40,
	},
}

gArgastReinforcementsFrequency = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_REINFORCEMENT_FREQUENCY.Value
gArgastReinforcementsProbability = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_REINFORCEMENT_PROBABILITY.Value
gArgastDeclareWarEarliest = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_DECLARE_WAR_EARLIEST.Value
gArgastDenouncePlayerTurn = GameInfo.SiegeModConstants.SIEGEMOD_ARGAST_DENOUNCE_PLAYER_NOT_AT_WAR.Value
gNumydiaDenounceArgastTurn = GameInfo.SiegeModConstants.SIEGEMOD_NUMYDIA_DENOUNCE_ARGAST_NOT_AT_WAR_TURN.Value
gNumydiaExpansionTurn = GameInfo.SiegeModConstants.SIEGEMOD_NUMYDIA_EXPANSION_TURN.Value
gCyatsExpansionTurnFirst = GameInfo.SiegeModConstants.SIEGEMOD_CYATS_EXPANSION_TURN_FIRST.Value
gCyatsExpansionTurnSecond = GameInfo.SiegeModConstants.SIEGEMOD_CYATS_EXPANSION_TURN_SECOND.Value
gCyatsExpansionTurnThird = GameInfo.SiegeModConstants.SIEGEMOD_CYATS_EXPANSION_TURN_THIRD.Value

gArgastHasDenouncedSvesta = false
gNumydiaHasDenouncedArgast = false

gArgastNextExpansion = 0
gCyatsNextExpansion = 0

gArgastReinforcements = {
	[0] = GameInfoTypes.UNIT_ARGAST_RAIDERS,
	[1] = GameInfoTypes.UNIT_CATAPULT,
}

function InitArgastWar(pArgast)
	local pArgastTeam = Teams[pArgast:GetTeam()]

	print("Initializing Argast's war with the player...")

	print("Giving Argast some free Raiders...")
	for i, coord in pairs(gArgastFortCoords) do
		pArgast:InitUnit(GameInfoTypes.UNIT_ARGAST_RAIDERS, coord.X, coord.Y)
	end

	print("Declaring war...")
	pArgastTeam:DeclareWar(Players[Game.GetActivePlayer()]:GetTeam())
end

function ReinforceArgast(pArgast)
	print("Reinforcing Argast's armies...")
	for i, coord in pairs(gArgastFortCoords) do
		local unitRand = Map.Rand(#gArgastReinforcements)

		local spawnRand = Map.Rand(100)
		
		if (spawnRand <= gArgastReinforcementsProbability) then
			print("Giving unit " .. gArgastReinforcements[unitRand] .. " to Argast at " .. coord.X .. "," .. coord.Y .. "...")

			pArgast:InitUnit(gArgastReinforcements[unitRand], coord.X, coord.Y)
		end
	end
end

function MaintainArgastWar(pArgast)
	local pArgastTeam = Teams[pArgast:GetTeam()]

	local pPlayer = Players[Game.GetActivePlayer()]

	if (not pArgastTeam:IsAtWar(pPlayer.GetTeam())) then
		print("ERROR: The MaintainArgastWar function has been called even though Argast isn't at war with the player...")
		return
	end

	print("Maintaining Argast's war with the player...")
	local currentTurn = Game.GetElapsedGameTurns()

	if math.fmod(currentTurn, gArgastReinforcementsFrequency) == 0 then
		ReinforceArgast(pArgast)
	end
end

function ArgastWar(playerID)
	if playerID ~= Game.GetActivePlayer() then
		return
	end

	print("Checking Argast's war status...")

	local pSvesta = Players[playerID]

	local pArgast
	for id, pPlayer in pairs(Players) do
		if pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_ARGAST_EMPIRE then
			print("Found Argast...")
			pArgast = pPlayer
		end
	end

	local pArgastTeam = Teams[pArgast:GetTeam()]

	local currentTurn = Game.GetElapsedGameTurns()

	print("Argast war check sees turn number " .. currentTurn .. "...")

	if currentTurn > gArgastDeclareWarEarliest and pArgastTeam:CanDeclareWar(pSvesta:GetTeam()) then
		InitArgastWar(pArgast)

	elseif currentTurn > gArgastDenouncePlayerTurn and not pArgastTeam:IsAtWar(pSvesta:GetTeam()) and not gArgastHasDenouncedSvesta then
		print("Denouncing active player...")
		pArgast:DoForceDenounce(playerID)
		gArgastHasDenouncedSvesta = true

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

	for i, pPlayer in pairs(Players) do
		if pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_NEMYDIAN_FEDERATION then
			pNumydia = pPlayer
			print("Found Numydia player...")
		elseif pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_ARGAST_EMPIRE then
			pArgast = pPlayer
			print("Found Argast player...")
		end
	end

	local pNumydiaTeam = Teams[pNumydia:GetTeam()]
	
	if not pNumydiaTeam:IsAtWar(pArgast:GetTeam()) and not gNumydiaHasDenouncedArgast and currentTurn > gNumydiaDenounceArgastTurn then
		print("Numydia is now denouncing Argast...")
		pNumydia:DoForceDenounce(pArgast:GetID())
		gNumydiaHasDenouncedArgast = true
	end

	if currentTurn == gNumydiaExpansionTurn then
		print("Numydia is now expanding...")

		print("Founding city...")
		pNumydia:Found(gNumydiaExpansionCoords.X, gNumydiaExpansionCoords.Y)

		print("Giving Numydia another worker for their new city...")
		pNumydia:InitUnit(GameInfoTypes.UNIT_WORKER, gNumydiaExpansionCoords.X, gNumydiaExpansionCoords.Y)

		Players[Game.GetActivePlayer()]:AddNotification(GameInfoTypes.NOTIFICATION_NUMYDIA_EXPANDED, nil, nil, gNumydiaExpansionCoords.X, gNumydiaExpansionCoords.Y, GameInfoTypes.CIVILIZATION_NUMYDIA)
	end
end
GameEvents.PlayerDoTurn.Add(NumydiaActions)

function CyatsActions(playerID)
	if playerID ~= Game.GetActivePlayer() then
		return
	end

	print("Managing the Cyats actions...")

	local pCyats

	for i, pPlayer in pairs(Players) do
		if pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_CYATS then
			pCyats = pPlayer
			print("Found Cyats player...")
		end
	end

	local currentTurn = Game.GetElapsedGameTurns()


end
GameEvents.PlayerDoTurn.Add(CyatsActions)

function ScaleConstantsBasedOnDifficulty()
	print("Scenario constants not yet scaled by difficulty...")
end

ScaleConstantsBasedOnDifficulty()