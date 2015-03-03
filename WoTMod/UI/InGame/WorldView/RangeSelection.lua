-- RangeSelection
-- Author: S3rgeus
-- DateCreated: 3/1/2015 10:13:43 PM
--------------------------------------------------------------
include("FLuaVector")

local redColor = Vector4( 0.7, 0, 0, 1 );
local highlightColor = Vector4( 0.7, 0.7, 0, 1 );

local _selection = {}

function DoHighlight()
	local NO_DIRECTION = -1
	local iRange = _selection.Range
	for iDX = -iRange, iRange do
		for iDY = -iRange, iRange do
			local pTargetPlot = Map.GetPlotXY(thisX, thisY, iDX, iDY)
			if pTargetPlot then
				local pTargetX = pTargetPlot:GetX()
				local pTargetY = pTargetPlot:GetY()
				local distance = Map.PlotDistance(_selection.CenterPlot:GetX(), _selection.CenterPlot:GetY(), pTargetX, pTargetY)
				if (distance <= _selection.Range) then
					local hexID = ToHexFromGrid( Vector2( plotX, plotY) );
					if _selection.IsValidTarget(pTargetPlot) then
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
	_selection = Selection
	DoHighlight()
	Events.SerialEventMouseOverHex.Add(DisplayArrow)
	return false
end

function EndSelection()
	_selection = {}
	UI.SetInterfaceMode(GameInfoTypes.INTERFACEMODE_SELECTION)
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
	if _selection and interfaceMode == _selection.InterfaceMode and InputResponses[uiMsg] then
		return InputResponses[uiMsg]()
	end
	return false;
end
ContextPtr:SetInputHandler( InputHandler );

function InterfaceModeChanged(oldInterfaceMode, newInterfaceMode)
	if (_selection and oldInterfaceMode == _selection.InterfaceMode) then
		EndSelection()
	end
end
Events.InterfaceModeChanged.Add(InterfaceModeChanged)