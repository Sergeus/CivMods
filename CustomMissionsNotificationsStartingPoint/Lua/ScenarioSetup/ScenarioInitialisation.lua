----------------------------------------------------------------
--
-- Scenario Initialisation
--
----------------------------------------------------------------
include("\ScenarioUtilities.lua")

----------------------------------------------------------------
function ScenarioInitialise() 
  if (IsScenarioNotInitialised()) then
	print("Initializing players...")
    -- Loop over each player and initialise them
    for iPlayerLoop = 0, GameDefines.MAX_MAJOR_CIVS-1, 1 do
      local pPlayer = Players[iPlayerLoop];
      if (pPlayer:IsAlive()) then
        InitPlayer(pPlayer);
      end
    end

    -- Add random goody huts to World Builder maps
    --if (IsScenarioWBMap()) then
    --  ClearGoodies()
    --  AddGoodies()
    --end
  end
end

function InitCyats(pPlayer)
	print("Initializing Cyats civ...")

	print("Founding Cyat pantheon...")
	Game.FoundPantheon(pPlayer:GetID(), GameInfoTypes.BELIEF_DESERT_FOLKLORE)
	pPlayer:SetFaith(1000)

	print("Found Cirran Orthodoxy...")
	Game.FoundReligion(pPlayer:GetID(), GameInfoTypes.RELIGION_CIRRAN_ORTHODOXY, nil, GameInfoTypes.BELIEF_TITHE, 
		GameInfoTypes.BELIEF_RELIGIOUS_COMMUNITY, -1, -1, pPlayer:GetCapitalCity())

	print("Enhancing Cirran Orthodoxy...")
	Game.EnhanceReligion(pPlayer:GetID(), GameInfoTypes.RELIGION_CIRRAN_ORTHODOXY, GameInfoTypes.BELIEF_ITINERANT_PREACHERS, GameInfoTypes.BELIEF_GURUSHIP)

	print("Adding reformation belief...")
	Game.AddReformationBelief(pPlayer:GetID(), GameInfoTypes.RELIGION_CIRRAN_ORTHODOXY, GameInfoTypes.BELIEF_UNITY_OF_PROPHETS)

	print("Spreading religion to all Cyat cities...")
	for pCity in pPlayer:Cities() do
		pCity:AdoptReligionFully(GameInfoTypes.RELIGION_CIRRAN_ORTHODOXY)
	end
end

function InitSvesta(pPlayer)
	print("Initializing Svesta...")

	print("Founding Svestan pantheon...")
	Game.FoundPantheon(pPlayer:GetID(), GameInfoTypes.BELIEF_FAITH_HEALERS)
	pPlayer:SetFaith(0)

	print("Founding Advarn...")
	Game.FoundReligion(pPlayer:GetID(), GameInfoTypes.RELIGION_ADVARN, nil, GameInfoTypes.BELIEF_CHURCH_PROPERTY,
		GameInfoTypes.BELIEF_SWORD_PLOWSHARES, -1, -1, pPlayer:GetCapitalCity())

	print("Enhancing Advarn...")
	Game.EnhanceReligion(pPlayer:GetID(), GameInfoTypes.RELIGION_ADVARN, GameInfoTypes.BELIEF_HOLY_ORDER, GameInfoTypes.BELIEF_FEED_WORLD)
end

function InitArgast(pPlayer)
	print("Initializing Argastinium...")

	print("Spreading religions to cities...")
	for pCity in pPlayer:Cities() do
		local name = pCity:GetName()

		if name == "Pashnis" or name == "Kavrat" or name == "Preshets" then
			pCity:AdoptReligionFully(GameInfoTypes.RELIGION_KLONOA)
		elseif name == "Juslik" or name == "Argastus" or name == "Argasia" then
			pCity:AdoptReligionFully(GameInfoTypes.RELIGION_ADVARN)
		elseif name == "Supal" or name == "Suman" or name == "Argan" or name == "Krade" or name == "Hyronium" then
			pCity:AdoptReligionFully(GameInfoTypes.RELIGION_CIRRAN_ORTHODOXY)
		end
	end
end

function InitNumydia(pPlayer)
	print("Initializing Numydia...")
	
	print("Founding Numydian pantheon...")
	Game.FoundPantheon(pPlayer:GetID(), GameInfoTypes.BELIEF_GOD_SEA)
	pPlayer:SetFaith(0)

	print("Founding Klonoan...")
	Game.FoundReligion(pPlayer:GetID(), GameInfoTypes.RELIGION_KLONOA, nil, GameInfoTypes.BELIEF_INITIATION_RITES,
		GameInfoTypes.BELIEF_CATHEDRALS, -1, -1, pPlayer:GetCapitalCity())

	print("Enhancing Klonoan...")
	Game.EnhanceReligion(pPlayer:GetID(), GameInfoTypes.RELIGION_KLONOA, GameInfoTypes.BELIEF_MISSIONARY_ZEAL, GameInfoTypes.BELIEF_PAGODAS)

	print("Spreading Klonoan to other Numydian cities...")
	for pCity in pPlayer:Cities() do
		pCity:AdoptReligionFully(GameInfoTypes.RELIGION_KLONOA)
	end
end

----------------------------------------------------------------
function InitPlayer(pPlayer)
  --local pStartPlot = GetPlayerStartPlot(pPlayer);

  --local scout = pPlayer:InitUnit(GameInfoTypes["UNIT_SCOUT"], pStartPlot:GetX(), pStartPlot:GetY(), UNITAI_EXPLORE);
  --scout:JumpToNearestValidPlot();
  --local ship = pPlayer:InitUnit(GameInfoTypes["UNIT_TRIREME"], pStartPlot:GetX(), pStartPlot:GetY(), UNITAI_EXPLORE_SEA, DirectionTypes.DIRECTION_WEST);
  --ship:JumpToNearestValidPlot();

	pPlayer:ChangeGold(50);

	if pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_SVESTA then
		InitSvesta(pPlayer)
	elseif pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_ARGAST_EMPIRE then
		InitArgast(pPlayer)
	elseif pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_NEMYDIAN_FEDERATION then
		InitNumydia(pPlayer)
	elseif pPlayer:GetCivilizationType() == GameInfoTypes.CIVILIZATION_CYATS then
		InitCyats(pPlayer)
	end
end

----------------------------------------------------------------
ScenarioInitialise()