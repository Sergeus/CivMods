// ----------------------------------------------------------------
// WoTMod Addition - Custom Notifications
// S3rgeus - 11/05/13
// ----------------------------------------------------------------

#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"
#include "CvGlobals.h"
#include "WoTNotifications.h"

WoTNotificationInfo::WoTNotificationInfo()
	: m_iID(-1),
	m_iWelcomeness(0),
	m_iIconIndex(-1),
	m_bBlocksEndTurn(false),
	m_bMiniCivIcon(false),
	m_bChecksKnown(false),
	m_bDisplaysUnitIcon(false),
	m_bDoubleCivNotification(false),
	m_bUrgent(false),
	m_bExpiresAtTurnEnd(true),
	m_strType(),
	m_strMessage(),
	m_strSummary(),
	m_strBlockMessage(),
	m_strUIContext()
{

}

WoTNotificationInfo::~WoTNotificationInfo()
{
	
}

bool WoTNotificationInfo::CacheResults(Database::Results &kResults, CvDatabaseUtility& kUtility)
{
	m_iID = kResults.GetInt("ID");
	m_iWelcomeness = kResults.GetInt("Welcomeness");
	m_iIconIndex = kResults.GetInt("IconIndex");

	m_bBlocksEndTurn = kResults.GetBool("BlocksEndTurn");
	m_bMiniCivIcon = kResults.GetBool("MiniCivIcon");
	m_bChecksKnown = kResults.GetBool("ChecksKnown");
	m_bDisplaysUnitIcon = kResults.GetBool("DisplaysUnitIcon");
	m_bDoubleCivNotification = kResults.GetBool("DoubleCivNotification");
	m_bUrgent = kResults.GetBool("Urgent");
	m_bExpiresAtTurnEnd = kResults.GetBool("ExpiresAtTurnEnd");

	m_strType = kResults.GetText("NotificationType");
	m_strMessage = kResults.GetText("Message");
	m_strSummary = kResults.GetText("Summary");
	m_strBlockMessage = kResults.GetText("BlockMessage");
	m_strUIContext = kResults.GetText("UIContext");

	return true;
}

int WoTNotificationInfo::GetID() const
{
	return m_iID;
}

int WoTNotificationInfo::GetWelcomeness() const
{
	return m_iWelcomeness;
}

int WoTNotificationInfo::GetIconIndex() const
{
	return m_iIconIndex;
}

const char* WoTNotificationInfo::GetType() const
{
	return m_strType;
}

const char* WoTNotificationInfo::GetMessage() const
{
	return m_strMessage;
}

const char* WoTNotificationInfo::GetSummary() const
{
	return m_strSummary;
}

const char* WoTNotificationInfo::GetIconAtlas() const
{
	return m_strIconAtlas;
}

const char* WoTNotificationInfo::GetBlockMessage() const
{
	return m_strBlockMessage;
}

const char* WoTNotificationInfo::GetUIContext() const
{
	return m_strUIContext;
}

bool WoTNotificationInfo::IsBlocksEndTurn() const
{
	return m_bBlocksEndTurn;
}

bool WoTNotificationInfo::IsMiniCivIcon() const
{
	return m_bMiniCivIcon;
}

bool WoTNotificationInfo::IsChecksKnown() const
{
	return m_bChecksKnown;
}

bool WoTNotificationInfo::IsDisplaysUnitIcon() const
{
	return m_bDisplaysUnitIcon;
}

bool WoTNotificationInfo::IsDoubleCivNotification() const
{
	return m_bDoubleCivNotification;
}

bool WoTNotificationInfo::IsExpiresAtTurnEnd() const
{
	return m_bExpiresAtTurnEnd;
}