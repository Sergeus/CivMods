-- RangeSelection
-- Author: S3rgeus
-- DateCreated: 3/1/2015 10:13:43 PM
--------------------------------------------------------------
include("FLuaVector")

local blueColor = Vector4( 0.3, 0.5, 1, 1 );

local _selection = {}

function DoHighlight()
	local NO_DIRECTION = -1
	local iRange = _selection.Range
	for iDX = -iRange, iRange do
		for iDY = -iRange, iRange do
			local pTargetPlot = Map.GetPlotXY(_selection.CenterPlot:GetX(), _selection.CenterPlot:GetY(), iDX, iDY)
			if pTargetPlot then
				local pTargetX = pTargetPlot:GetX()
				local pTargetY = pTargetPlot:GetY()
				print("Checking plot at " .. pTargetX .. ", " .. pTargetY)
				local distance = Map.PlotDistance(_selection.CenterPlot:GetX(), _selection.CenterPlot:GetY(), pTargetX, pTargetY)
				if (distance <= _selection.Range) then
					print("It's in range")
					local hexID = ToHexFromGrid( Vector2( plotX, plotY) );
					if _selection.IsSelectable(pTargetPlot) then
						print("It's selectable")
						Events.SerialEventHexHighlight( hexID, true, blueColor);
					else
						print("It's just in range")
						Events.SerialEventHexHighlight( hexID, true, blueColor);
					end
				end
			end
		end
	end
end

function ClearUnitHexHighlights()
	Events.ClearHexHighlightStyle("");
end;

function DisplayArrow(hexX, hexY)
	if _selection.IsInRange(Map.GetPlot(hexX, hexY)) then
		Events.SpawnArrowEvent(_selection.CenterPlot:GetX(), _selection.CenterPlot:GetY(), hexX, hexY );
	else
		Events.RemoveAllArrowsEvent();
	end
end

function DoSelection(Selection)
	_selection = Selection
	DoHighlight()
	Events.SerialEventMouseOverHex.Add(DisplayArrow)
	return true
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
InputResponses[MouseEvents.LButtonDown] = function() return true end
InputResponses[MouseEvents.LButtonUp] = TrySelect
InputResponses[MouseEvents.RButtonDown] = function() return true end
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