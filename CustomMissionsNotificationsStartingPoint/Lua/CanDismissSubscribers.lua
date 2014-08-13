-- CanDismissSubscribers
-- Author: S3rgeus
-- DateCreated: 5/19/2013 6:03:20 PM
--------------------------------------------------------------

-- Place your PlayerCanDismissNotification subscribers here (or in any InGameUIAddin lua script of your choice)

function CanDismissSiegeModNotifications(playerID, notifID, iLookupIndex)
	if notifID == GameInfoTypes.NOTIFICATION_CITY_FALLEN_UNDER_PUPPETING_RELIGION
		or notifID == GameInfoTypes.NOTIFICATION_CITY_FALLEN_UNDER_PUPPETING_RELIGION_UNKNOWN
		or notifID == GameInfoTypes.NOTIFICATION_CITY_LOST_TO_PUPPETING_RELIGION
		or notifID == GameInfoTypes.NOTIFICATION_CITY_STILL_UNDER_PUPPETING_RELIGION
		or notifID == GameInfoTypes.NOTIFICATION_CITY_STILL_UNDER_PUPPETING_RELIGION_UNKNOWN
		or notifID == GameInfoTypes.NOTIFICATION_CITY_CAPTURED_WITH_PUPPETING_RELIGION
		or notifID == GameInfoTypes.NOTIFICATION_CITY_GAINED_SWAY_WITH_PUPPETING_RELIGION
		or notifID == GameInfoTypes.NOTIFICATION_CITY_LOST_SWAY_WITH_PUPPETING_RELIGION
		or notifID == GameInfoTypes.NOTIFICATION_CITY_WAR_BLOCKED_PUPPETING_RELIGION
		then
			return true
	end
end
GameEvents.PlayerCanDismissNotification.Add(CanHornOfValereDiscoveryBeDismissed)