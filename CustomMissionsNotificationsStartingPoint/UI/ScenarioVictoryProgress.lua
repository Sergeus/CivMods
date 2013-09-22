----------------------------------------------------------------
--
-- ScenarioVictoryProgress.lua
--
----------------------------------------------------------------
include("\ScenarioUtilities.lua")

gGoldRequired = 1000000

function OnGoldRequiredUpdated(goldRequired)
	print("Required gold changed to " .. goldRequired .. "...")
	gGoldRequired = goldRequired
end
LuaEvents.SiegeModGoldRequiredChanged.Add(OnGoldRequiredUpdated)

function GetGoldRequired() 
	print("Getting gold required...")
	LuaEvents.SiegeModGoldRequired()
end
GetGoldRequired()

---------------------------------------------------------------------
function OnUpdate()
	local pSvesta = Players[Game.GetActivePlayer()]

	local svestaGold = pSvesta:GetGold()

	if svestaGold >= gGoldRequired then
		ScenarioWin(ContextPtr, pSvesta, "VICTORY_MERCANTILE")
	end

	-- Check for a domination victory or out of turns
	if (IsScenarioLastManStanding()) then
		ScenarioWin(ContextPtr, Players[Game.GetActivePlayer()], "VICTORY_DOMINATION")
	elseif (IsScenarioOutOfTurns()) then
		ScenarioLose(ContextPtr)
	else
		-- Update the victory progress banner
		Controls.VictoryProgressLabel:LocalizeAndSetText("TXT_KEY_SCENARIO_VICTORY_PROGRESS", gGoldRequired - svestaGold);
		-- Controls.VictoryProgressLabel:LocalizeAndSetText("TXT_KEY_SCENARIO_TURNS_REMAINING", ScenarioTurnsRemaining());
		-- Controls.VictoryProgressLabel:LocalizeAndSetText("TXT_KEY_SCENARIO_TURNS_AND_OPPONENTS_REMAINING", ScenarioTurnsRemaining(), ScenarioCountOpponents());
		Controls.Grid:DoAutoSize();
	end
end
ContextPtr:SetUpdate(OnUpdate)

---------------------------------------------------------------------
function OnBriefingButton()
    UI.AddPopup( { Type = ButtonPopupTypes.BUTTONPOPUP_TEXT,
                   Data1 = 800,
                   Option1 = true,
                   Text = "TXT_KEY_SCENARIO_BRIEFING_TEXT" } );
end
Controls.BriefingButton:RegisterCallback(Mouse.eLClick, OnBriefingButton);

---------------------------------------------------------------------
Events.SerialEventEnterCityScreen.Add(function() ContextPtr:SetHide(true) end);
Events.SerialEventExitCityScreen.Add(function() ContextPtr:SetHide(false) end);

---------------------------------------------------------------------