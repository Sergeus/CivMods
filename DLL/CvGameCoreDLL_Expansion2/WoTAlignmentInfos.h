// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#pragma once

#include "CvDatabaseUtility.h"
#include "CvInfos.h"
#include "Database.h"

#if WOTMOD
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  class : WoTAlignmentInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTAlignmentInfo : public CvBaseInfo
{
public:
	WoTAlignmentInfo();
	virtual ~WoTAlignmentInfo();

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	bool IsOpposing(AlignmentTypes eOtherAlignment) const;
	YieldTypes GetYield() const;

private:
	YieldTypes m_eYield;

	bool* m_pbAlignmentOpposing;
};

#endif // WOTMOD