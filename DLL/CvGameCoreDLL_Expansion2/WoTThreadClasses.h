// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#pragma once

#include "CvDatabaseUtility.h"
#include "CvInfos.h"
#include "Database.h"

#if WOTMOD

class WoTThreadInfo : public CvBaseInfo
{
public:
	WoTThreadInfo();
	~WoTThreadInfo();

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	const std::vector<ThreadChoiceTypes>& GetChoices() const;

private:
	CvString m_ThreadTitle;
	CvString m_FlavorBody;
	std::vector<ThreadChoiceTypes> m_Choices;
};

class WoTThreadChoiceInfo : public CvBaseInfo
{
public:
	WoTThreadChoiceInfo();
	~WoTThreadChoiceInfo();

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	int GetYield(YieldTypes eYield) const;

private:
	std::vector<int> m_YieldChanges;
};

#endif // WOTMOD