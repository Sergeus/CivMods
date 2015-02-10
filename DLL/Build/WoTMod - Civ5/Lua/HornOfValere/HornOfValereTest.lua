-- HornOfValereTest
-- Author: S3rgeus
-- DateCreated: 4/23/2013 2:16:58 AM
--------------------------------------------------------------

print("Test code for Horn of Valere functionality running.")
print("This should not be active in deployed builds.")

function testHornPlacement(playerID)
	print("Mod starting...")
	if (playerID == 0 and Game.GetGameTurn() == Game.GetStartTurn() + 1) then
		print("Found player 1...")
		local pPlayer = Players[playerID]
		local pCity = pPlayer:GetCapitalCity()

		print("Got capital city")

		local X = pCity:GetX()
		local Y = pCity:GetY()

		X = X - 1;

		print("Placing Horn at " .. X .. ", " .. Y .. ".")

		Map.PlaceHornOfValere(X, Y);

		print("Placing Blight.")

		local pPlot = Map.GetPlot(X, Y)

		pPlot:SetFeatureType(GameInfoTypes.FEATURE_BLIGHT)

		--print("Refreshing graphics.")

		--pPlot:RefreshGraphics()

		print("done!")
	end
end

function HornHandler(playerID)
	print(pcall(testHornPlacement, playerID))
end
GameEvents.PlayerDoTurn.Add(HornHandler);