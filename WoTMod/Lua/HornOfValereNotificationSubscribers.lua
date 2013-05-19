-- HornOfValereNotificationSubscribers
-- Author: S3rgeus
-- DateCreated: 5/19/2013 4:29:14 PM
--------------------------------------------------------------

function CanHornOfValereDiscoveryBeDismissed(playerID, notificationID, notificationLookupIndex)
	-- You can always dismiss the Horn of Valere discovery notification
	if (notificationID == GameInfoTypes.NOTIFICATION_HORN_OF_VALERE_DISCOVERED) then
		return true
	end

	return false
end
GameEvents.PlayerCanDismissNotification.Add(CanHornOfValereDiscoveryBeDismissed)


function CanHornOfValereBlownBeDismissed(playerID, notificationID, notificationLookupIndex)
	-- You can always dismiss a Horn of Valere blown notification
	if (notificationID == GameInfoTypes.NOTIFICATION_HORN_OF_VALERE_BLOWN) then
		return true
	end

	return false
end
GameEvents.PlayerCanDismissNotification.Add(CanHornOfValereBlownBeDismissed)