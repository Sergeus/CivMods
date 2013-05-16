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

	const char* GetType() const;
	const char* GetMessage() const;
	const char* GetSummary() const;
	const char* GetIconAtlas() const;
	const char* GetBlockMessage() const;
	const char* GetUIContext() const;

	bool IsBlocksEndTurn() const;
	bool IsMiniCivIcon() const;
	bool IsChecksKnown() const;
	bool IsDisplaysUnitIcon() const;
	bool IsDoubleCivNotification() const;
	bool IsExpiresAtTurnEnd() const;
	bool IsPlaysFXOnPlot() const;

	virtual bool CacheResults(Database::Results &kResults, CvDatabaseUtility& kUtility);

private:
	int m_iID;
	int m_iWelcomeness;
	int m_iIconIndex;

	CvString m_strType;
	CvString m_strMessage;
	CvString m_strSummary;
	CvString m_strIconAtlas;
	CvString m_strBlockMessage;
	CvString m_strUIContext;

	bool m_bBlocksEndTurn;
	bool m_bMiniCivIcon;
	bool m_bChecksKnown;
	bool m_bDisplaysUnitIcon;
	bool m_bDoubleCivNotification;
	bool m_bUrgent;
	bool m_bExpiresAtTurnEnd;
	bool m_bPlaysFXOnPlot;
};