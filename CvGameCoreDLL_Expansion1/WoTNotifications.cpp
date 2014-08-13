// ----------------------------------------------------------------
// WoTMod Addition - Custom Notifications
// S3rgeus - 11/05/13
// ----------------------------------------------------------------
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"
#include "CvGlobals.h"
#include "WoTNotifications.h"

#if CUSTOM_NOTIFICATIONS
WoTNotificationInfo::WoTNotificationInfo()
	: m_iID(-1),
	m_iWelcomeness(0),
	m_iIconIndex(-1),
	m_bBlocksEndTurn(false),
	m_bLargeButton(false),
	m_bMiniCivIcon(false),
	m_bChecksKnown(false),
	m_bUsesRuntimeIndex(false),
	m_bDoubleCivIcon(false),
	m_bUrgent(false),
	m_bExpiresAtTurnEnd(true),
	m_bPlaysFXOnPlot(true),
	m_strType(),
	m_strMessage(),
	m_strSummary(),
	m_strBlockMessage(),
	m_strUIContext(),
	m_strCiv1Anchor(),
	m_strCiv2Anchor(),
	m_strExistingIconType(),
	m_strUnknownMessage()
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
	m_iIconSize = kResults.GetInt("IconSize");

	m_bBlocksEndTurn = kResults.GetBool("BlocksEndTurn");
	m_bLargeButton = kResults.GetBool("LargeButton");
	m_bMiniCivIcon = kResults.GetBool("MiniCivIcon");
	m_bChecksKnown = kResults.GetBool("ChecksKnown");
	m_bUsesRuntimeIndex = kResults.GetBool("UsesRuntimeIndex");
	m_bDoubleCivIcon = kResults.GetBool("DoubleCivIcon");
	m_bUrgent = kResults.GetBool("Urgent");
	m_bExpiresAtTurnEnd = kResults.GetBool("ExpiresAtTurnEnd");
	m_bPlaysFXOnPlot = kResults.GetBool("PlaysFXOnPlot");

	m_strType = kResults.GetText("Type");
	m_strMessage = kResults.GetText("Message");
	m_strSummary = kResults.GetText("Summary");
	m_strBlockMessage = kResults.GetText("BlockMessage");
	m_strBlockToolTip = kResults.GetText("BlockToolTip");
	m_strUIContext = kResults.GetText("UIContext");
	m_strCiv1Anchor = kResults.GetText("Civ1Anchor");
	m_strCiv2Anchor = kResults.GetText("Civ2Anchor");
	m_strExistingIconType = kResults.GetText("ExistingIconType");
	m_strUnknownMessage = kResults.GetText("UnknownMessage");

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

int WoTNotificationInfo::GetIconSize() const
{
	return m_iIconSize;
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

const char* WoTNotificationInfo::GetBlockToolTip() const
{
	return m_strBlockToolTip;
}

const char* WoTNotificationInfo::GetUIContext() const
{
	return m_strUIContext;
}

const char* WoTNotificationInfo::GetCiv1Anchor() const
{
	return m_strCiv1Anchor;
}

const char* WoTNotificationInfo::GetCiv2Anchor() const
{
	return m_strCiv2Anchor;
}

const char* WoTNotificationInfo::GetExistingIconType() const
{
	return m_strExistingIconType;
}

const char* WoTNotificationInfo::GetUnknownMessage() const
{
	return m_strUnknownMessage;
}

bool WoTNotificationInfo::IsBlocksEndTurn() const
{
	return m_bBlocksEndTurn;
}

bool WoTNotificationInfo::IsLargeButton() const
{
	return m_bLargeButton;
}

bool WoTNotificationInfo::IsMiniCivIcon() const
{
	return m_bMiniCivIcon;
}

bool WoTNotificationInfo::IsChecksKnown() const
{
	return m_bChecksKnown;
}

bool WoTNotificationInfo::IsUsesRuntimeIndex() const
{
	return m_bUsesRuntimeIndex;
}

bool WoTNotificationInfo::IsDoubleCivIcon() const
{
	return m_bDoubleCivIcon;
}

bool WoTNotificationInfo::IsExpiresAtTurnEnd() const
{
	return m_bExpiresAtTurnEnd;
}

bool WoTNotificationInfo::IsPlaysFXOnPlot() const
{
	return m_bPlaysFXOnPlot;
}
#endif // CUSTOM_NOTIFICATIONS
