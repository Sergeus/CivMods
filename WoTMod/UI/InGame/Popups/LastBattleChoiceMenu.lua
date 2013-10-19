-- LastBattleChoiceMenu
-- Author: S3rgeus
-- DateCreated: 10/17/2013 8:14:07 PM
--------------------------------------------------------------

ContextPtr:SetHide(true)

function OnDisplay()
	ContextPtr:SetHide(false)
end
GameEvents.LastBattleStart.Add(OnDisplay)

function OnChooseLightCallback()
	print("Player has chosen the Light!")

	Game.ChooseLastBattleSide(Game.GetActivePlayer(), GameInfoTypes.SIDE_LIGHT)
	ContextPtr:SetHide(true)
end
Controls.LightButton:RegisterCallback(Mouse.eLClick, OnChooseLightCallback)

function OnChooseShadowCallback()
	print("Player has chosen the Shadow!")

	Game.ChooseLastBattleSide(Game.GetActivePlayer(), GameInfoTypes.SIDE_SHADOW)
	ContextPtr:SetHide(true)
end
Controls.ShadowButton:RegisterCallback(Mouse.eLClick, OnChooseShadowCallback)