// ----------------------------------------------------------------
// WoTMod Addition - Custom Notifications
// S3rgeus - 11/05/13
// ----------------------------------------------------------------

#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"
#include "CvGlobals.h"
#include "WoTCustomNotifications.h"

WoTCustomNotificationInfo::WoTCustomNotificationInfo()
	: m_iID(-1),
	m_iWelcomeness(0),
	m_iIconIndex(-1),
	m_bBlocksEndTurn(false),
	m_bMiniCivIcon(false),
	m_bChecksKnown(false),
	m_bDisplaysUnitIcon(false),
	m_bDoubleCivNotification(false),
	m_strType(),
	m_strMessage(),
	m_strSummary(),
	m_strBlockMessage(),
	m_strUIContext()
{

}

WoTCustomNotificationInfo::~WoTCustomNotificationInfo()
{
	
}

bool WoTCustomNotificationInfo::CacheResults(Database::Results &kResults, CvDatabaseUtility& kUtility)
{
	m_iID = kResults.GetInt("ID");
	m_iWelcomeness = kResults.GetInt("Welcomeness");
	m_iIconIndex = kResults.GetInt("IconIndex");

	m_bBlocksEndTurn = kResults.GetBool("BlocksEndTurn");
	m_bMiniCivIcon = kResults.GetBool("MiniCivIcon");
	m_bChecksKnown = kResults.GetBool("ChecksKnown");
	m_bDisplaysUnitIcon = kResults.GetBool("DisplaysUnitIcon");
	m_bDoubleCivNotification = kResults.GetBool("DoubleCivNotification");

	m_strType = kResults.GetText("Type");
	m_strMessage = kResults.GetText("Message");
	m_strSummary = kResults.GetText("Summary");
	m_strBlockMessage = kResults.GetText("BlockMessage");
	m_strUIContext = kResults.GetText("UIContext");

	return true;
}

int WoTCustomNotificationInfo::GetID() const
{
	return m_iID;
}

int WoTCustomNotificationInfo::GetWelcomeness() const
{
	return m_iWelcomeness;
}

int WoTCustomNotificationInfo::GetIconIndex() const
{
	return m_iIconIndex;
}

const char* WoTCustomNotificationInfo::GetType() const
{
	return m_strType;
}

const char* WoTCustomNotificationInfo::GetMessage() const
{
	return m_strMessage;
}

const char* WoTCustomNotificationInfo::GetSummary() const
{
	return m_strSummary;
}

const char* WoTCustomNotificationInfo::GetIconAtlas() const
{
	return m_strIconAtlas;
}

const char* WoTCustomNotificationInfo::GetBlockMessage() const
{
	return m_strBlockMessage;
}

const char* WoTCustomNotificationInfo::GetUIContext() const
{
	return m_strUIContext;
}

bool WoTCustomNotificationInfo::IsBlocksEndTurn() const
{
	return m_bBlocksEndTurn;
}

bool WoTCustomNotificationInfo::IsMiniCivIcon() const
{
	return m_bMiniCivIcon;
}

bool WoTCustomNotificationInfo::IsChecksKnown() const
{
	return m_bChecksKnown;
}

bool WoTCustomNotificationInfo::IsDisplaysUnitIcon() const
{
	return m_bDisplaysUnitIcon;
}

bool WoTCustomNotificationInfo::IsDoubleCivNotification() const
{
	return m_bDoubleCivNotification;
}