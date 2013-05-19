// ----------------------------------------------------------------
// WoTMod Addition - Custom Notifications
// S3rgeus - 11/05/13
// ----------------------------------------------------------------

#pragma once

#include "CvString.h"
#include "Database.h"
#include <vector>

class WoTNotificationInfo
{
public:
	WoTNotificationInfo();
	~WoTNotificationInfo();

	int GetID() const;
	int GetWelcomeness() const;
	int GetIconIndex() const;
	int GetIconSize() const;

	const char* GetType() const;
	const char* GetMessage() const;
	const char* GetSummary() const;
	const char* GetIconAtlas() const;
	const char* GetBlockMessage() const;
	const char* GetBlockToolTip() const;
	const char* GetUIContext() const;
	const char* GetCiv1Anchor() const;
	const char* GetCiv2Anchor() const;
	const char* GetExistingIconType() const;

	bool IsBlocksEndTurn() const;
	bool IsLargeButton() const;
	bool IsMiniCivIcon() const;
	bool IsChecksKnown() const;
	bool IsUsesRuntimeIndex() const;
	bool IsDoubleCivIcon() const;
	bool IsExpiresAtTurnEnd() const;
	bool IsPlaysFXOnPlot() const;

	virtual bool CacheResults(Database::Results &kResults, CvDatabaseUtility& kUtility);

private:
	int m_iID;
	int m_iWelcomeness;
	int m_iIconIndex;
	int m_iIconSize;

	CvString m_strType;
	CvString m_strMessage;
	CvString m_strSummary;
	CvString m_strIconAtlas;
	CvString m_strBlockMessage;
	CvString m_strBlockToolTip;
	CvString m_strUIContext;
	CvString m_strCiv1Anchor;
	CvString m_strCiv2Anchor;
	CvString m_strExistingIconType;

	bool m_bBlocksEndTurn;
	bool m_bLargeButton;
	bool m_bMiniCivIcon;
	bool m_bChecksKnown;
	bool m_bUsesRuntimeIndex;
	bool m_bDoubleCivIcon;
	bool m_bUrgent;
	bool m_bExpiresAtTurnEnd;
	bool m_bPlaysFXOnPlot;
};