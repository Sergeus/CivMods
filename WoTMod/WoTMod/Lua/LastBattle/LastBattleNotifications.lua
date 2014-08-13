-- LastBattleNotifications
-- Author: S3rgeus
-- DateCreated: 10/19/2013 7:41:56 PM
--------------------------------------------------------------

function PlayerChosenSide(playerID, side)

	print("A civilization has chosen a side in the Last Battle")
	
	local notifID

	if side == GameInfoTypes.SIDE_LIGHT then
		notifID = GameInfoTypes.NOTIFICATION_OTHER_CIV_JOINED_LIGHT

	elseif side == GameInfoTypes.SIDE_SHADOW then
		notifID = GameInfoTypes.NOTIFICATION_OTHER_CIV_JOINED_SHADOW

	else
		print("Unknown side chosen")
		return
	end

	local message = Locale.ConvertTextKey(GameInfo.Notifications[notifID].Message,
		Players[playerID]:GetNameKey(), GameInfo.LastBattleSides[side].Description)

	local summary = Locale.ConvertTextKey(GameInfo.Notifications[notifID].Summary)

	for id, pPlayer in pairs(Players) do
		if (id ~= playerID) then
			pPlayer:AddNotification(notifID, message, summary, -1, -1, 0, playerID)
		end
	end
end
GameEvents.PlayerChosenLastBattleSide.Add(PlayerChosenSide)

function CanDismiss(playerID, notificationID, hookupIndex)
	if notificationID == GameInfoTypes.NOTIFICATION_OTHER_CIV_JOINED_LIGHT
		or notificationID == GameInfoTypes.NOTIFICATION_OTHER_CIV_JOINED_SHADOW then
		
		return true
	end

	return false
end
GameEvents.PlayerCanDismissNotification.Add(CanDismiss)

print("Last Battle notifications loaded")