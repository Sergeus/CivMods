-- RangeSelection
-- Author: S3rgeus
-- DateCreated: 3/1/2015 10:13:43 PM
--------------------------------------------------------------
include("FLuaVector")

local redColor = Vector4( 0.7, 0, 0, 1 );
local highlightColor = Vector4( 0.7, 0.7, 0, 1 );

local _selection = {}

function DoHighlight(Selection)
	local NO_DIRECTION = -1
	local iRange = Selection.Range
	for iDX = -iRange, iRange do
		for iDY = -iRange, iRange do
			local pTargetPlot = Map.GetPlotXY(thisX, thisY, iDX, iDY)
			if pTargetPlot then
				local pTargetX = pTargetPlot:GetX()
				local pTargetY = pTargetPlot:GetY()
				local distance = Map.PlotDistance(Selection.CenterPlot:GetX(), Selection.CenterPlot:GetY(), pTargetX, pTargetY)
				if (distance <= Selection.Range) then
					local hexID = ToHexFromGrid( Vector2( plotX, plotY) );
					if Selection.IsValidTarget(pTargetPlot) then
						Events.SerialEventHexHighlight( hexID, true, highlightColor, "FireRangeBorder" );
						Events.SerialEventHexHighlight( hexID, true, redColor, "ValidFireTargetBorder");
					else
						Events.SerialEventHexHighlight( hexID, true, highlightColor, "FireRangeBorder" );
					end
				end
			end
		end
	end
end

function DisplayArrow(hexX, hexY)
	if _selection.IsValidTarget(Map.GetPlot(hexX, hexY)) then
		Events.SpawnArrowEvent( attacker:GetX(), attacker:GetY(), hexX, hexY );
	else
		Events.RemoveAllArrowsEvent();
	end
end

function DoSelection(Selection)
	DoHighlight(Selection)
	_selection = Selection
	UI.SetInterfaceMode(InterfaceModeTypes.INTERFACEMODE_RANGE_SELECTION)
	Events.SerialEventMouseOverHex.Add(DisplayArrow)
	return false
end

function EndSelection()
	_selection = {}
	UI.SetInterfaceMode(InterfaceModeTypes.INTERFACEMODE_SELECTION)
	Events.RemoveAllArrowsEvent()
	Events.SerialEventMouseOverHex.Remove(DisplayArrow)
	ClearUnitHexHighlights()
	return true
end

function TrySelect()
	local pPlot = Map.GetPlot(UI.GetMouseOverHex())

	if (_selection.Select(pPlot)) then
		return EndSelection()
	end

	return false
end

local InputResponses = {}
InputResponses[MouseEvents.LButtonUp] = TrySelect
InputResponses[MouseEvents.RButtonUp] = EndSelection

function InputHandler( uiMsg, wParam, lParam )
	local interfaceMode = UI.GetInterfaceMode();
	if interfaceMode == InterfaceModeTypes.INTERFACEMODE_RANGE_SELECTION and InputResponses[uiMsg] then
		return InputResponses[uiMsg]()
	end
	return false;
end
ContextPtr:SetInputHandler( InputHandler );

function InterfaceModeChanged(oldInterfaceMode, newInterfaceMode)
	if (oldInterfaceMode == InterfaceModeTypes.INTERFACEMODE_RANGE_SELECTION) then
		EndSelection()
	end
end

Events.InterfaceModeChanged.Add(InterfaceModeChanged)