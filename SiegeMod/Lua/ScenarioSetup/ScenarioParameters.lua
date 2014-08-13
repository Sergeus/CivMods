----------------------------------------------------------------
-- Scenario Parameters - change these to match your requirements
----------------------------------------------------------------
ModId = "f1a3c52a-a236-4008-8bd3-7d03c95453eb"
ModVersion = Modding.GetActivatedModVersion(ModId);


ScenarioFixedMap = "Maps/Siegemod.Civ5Map"           -- Set to nil to disable option
--ScenarioRandomMap = "MAPS/ScenarioMapScript.lua"        -- Set to nil to disable option
--ScenarioMapSize = "WORLDSIZE_STANDARD"                  -- Only needed for random maps
--ScenarioMinorCivs = 16                                  -- Only needed for random maps


-- If using a WorldBuilder (.Civ5Map) map,
-- this table MUST match the placed playable civs
ScenarioCivilizations = {
  [0] = "CIVILIZATION_SVESTA",
}
ScenarioDefaultCivilization = "CIVILIZATION_SVESTA"  -- Set to nil to set initial value to "random"
--ScenarioDefaultCivilization = nil

-- Other AI (non-playable) civs - only needed for random maps
ScenarioAICivilizations = {
  [0] = "CIVILIZATION_ARGAST_EMPIRE",
  [1] = "CIVILIZATION_NEMYDIAN_FEDERATION",
  [2] = "CIVILIZATION_CYATS",
}


ScenarioDifficulties = {
  "HANDICAP_CHIEFTAIN",
  "HANDICAP_WARLORD",
  "HANDICAP_PRINCE",
  "HANDICAP_KING",
  "HANDICAP_EMPEROR",
  "HANDICAP_IMMORTAL",
  "HANDICAP_DEITY",
}
ScenarioDefaultDifficulty = "HANDICAP_PRINCE"


-- If using a script to determine victory, pick the one(s) that will make the AI behave the best
ScenarioVictories = {
  VICTORY_TIME       = false,
  VICTORY_SPACE_RACE = false,
  VICTORY_DOMINATION = true,
  VICTORY_CULTURAL   = false,
  VICTORY_DIPLOMATIC = false,
}


ScenarioGameSpeed = "GAMESPEED_STANDARD"


ScenarioTurns = 50
