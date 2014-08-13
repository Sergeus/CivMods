-- SiegeModDifficultyConstants
-- Author: S3rgeus
-- DateCreated: 10/3/2013 9:30:24 PM
--------------------------------------------------------------

DifficultyConstants = {
	["GoldForVictory"] = {
		[GameInfoTypes.HANDICAP_CHIEFTAIN]	= 4000,
		[GameInfoTypes.HANDICAP_WARLORD]	= 4500,
		[GameInfoTypes.HANDICAP_PRINCE]		= 5000,
		[GameInfoTypes.HANDICAP_KING]		= 5500,
		[GameInfoTypes.HANDICAP_EMPEROR]	= 6000,
		[GameInfoTypes.HANDICAP_IMMORTAL]	= 6500,
		[GameInfoTypes.HANDICAP_DEITY]		= 7500, 
	},
	["CyatsPreachersPerSpawn"] = {
		[GameInfoTypes.HANDICAP_CHIEFTAIN]	= 1,
		[GameInfoTypes.HANDICAP_WARLORD]	= 1,
		[GameInfoTypes.HANDICAP_PRINCE]		= 1,
		[GameInfoTypes.HANDICAP_KING]		= 2,
		[GameInfoTypes.HANDICAP_EMPEROR]	= 2,
		[GameInfoTypes.HANDICAP_IMMORTAL]	= 3,
		[GameInfoTypes.HANDICAP_DEITY]		= 4, 
	},
	["CyatsMinPreacherSpawnDistance"] = {
		[GameInfoTypes.HANDICAP_CHIEFTAIN]	= 4,
		[GameInfoTypes.HANDICAP_WARLORD]	= 3,
		[GameInfoTypes.HANDICAP_PRINCE]		= 2,
		[GameInfoTypes.HANDICAP_KING]		= 2,
		[GameInfoTypes.HANDICAP_EMPEROR]	= 2,
		[GameInfoTypes.HANDICAP_IMMORTAL]	= 1,
		[GameInfoTypes.HANDICAP_DEITY]		= 1, 
	},
	["CyatsPreacherSpawnNearSvestaInterval"] = {
		[GameInfoTypes.HANDICAP_CHIEFTAIN]	= 10,
		[GameInfoTypes.HANDICAP_WARLORD]	= 8,
		[GameInfoTypes.HANDICAP_PRINCE]		= 6,
		[GameInfoTypes.HANDICAP_KING]		= 4,
		[GameInfoTypes.HANDICAP_EMPEROR]	= 3,
		[GameInfoTypes.HANDICAP_IMMORTAL]	= 2,
		[GameInfoTypes.HANDICAP_DEITY]		= 1, 
	},
	["ArgastDeclareWarEarliestTurn"] = {
		[GameInfoTypes.HANDICAP_CHIEFTAIN]	= 4,
		[GameInfoTypes.HANDICAP_WARLORD]	= 3,
		[GameInfoTypes.HANDICAP_PRINCE]		= 2,
		[GameInfoTypes.HANDICAP_KING]		= 2,
		[GameInfoTypes.HANDICAP_EMPEROR]	= 2,
		[GameInfoTypes.HANDICAP_IMMORTAL]	= 1,
		[GameInfoTypes.HANDICAP_DEITY]		= 0, 
	},
	["ArgastReinforcementFrequency"] = {
		[GameInfoTypes.HANDICAP_CHIEFTAIN]	= 8,
		[GameInfoTypes.HANDICAP_WARLORD]	= 6,
		[GameInfoTypes.HANDICAP_PRINCE]		= 4,
		[GameInfoTypes.HANDICAP_KING]		= 3,
		[GameInfoTypes.HANDICAP_EMPEROR]	= 3,
		[GameInfoTypes.HANDICAP_IMMORTAL]	= 2,
		[GameInfoTypes.HANDICAP_DEITY]		= 1, 
	},
	["ArgastReinforcementProbability"] = {
		[GameInfoTypes.HANDICAP_CHIEFTAIN]	= 25,
		[GameInfoTypes.HANDICAP_WARLORD]	= 35,
		[GameInfoTypes.HANDICAP_PRINCE]		= 50,
		[GameInfoTypes.HANDICAP_KING]		= 65,
		[GameInfoTypes.HANDICAP_EMPEROR]	= 75,
		[GameInfoTypes.HANDICAP_IMMORTAL]	= 85,
		[GameInfoTypes.HANDICAP_DEITY]		= 100, 
	},
}