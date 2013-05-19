-- CustomNotificationsUtil
-- Author: S3rgeus
-- DateCreated: 5/18/2013 5:58:53 PM
--------------------------------------------------------------
function IsValid(notificationID)
	local notificationInfo = GameInfo.Notifications[notificationID]

	
end

function GetNameForNotification(notificationID)

	local notificationInfo = GameInfo.Notifications[notificationID]

	if (notificationInfo.LargeButton == true) then
		print("Large button name")
		return "CustomLarge"
	elseif (notificationInfo.DoubleCivIcon == true) then
		print("Double civ icon name")
		return "CustomDoubleCiv"
	else
		print("Custom generic name")
		return "CustomGeneric"
	end
end