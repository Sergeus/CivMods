-- BlightUtils
-- Author: S3rgeus
-- DateCreated: 5/27/2013 10:06:23 PM
--------------------------------------------------------------

function IsValidForBlight(pPlot)
	if (pPlot:IsMountain() or pPlot:IsWater() or pPlot:IsStartingPlot()) then
		return false
	end

	return true
end

function PlaceBlightBasic()
	local iNumPlots = Map.GetNumPlots()
	local pLoopPlot

	for i = 0, iNumPlots do
		
		pLoopPlot = Map.GetPlotByIndex(i)
		
		if (pLoopPlot ~= nil) then

			local latitude = pLoopPlot:GetLatitude()

			if (latitude > 65 and IsValidForBlight(pLoopPlot)) then
				pLoopPlot:SetFeatureType(GameInfoTypes.FEATURE_BLIGHT)
			end
		end
	end
end

PlaceBlightBasic()