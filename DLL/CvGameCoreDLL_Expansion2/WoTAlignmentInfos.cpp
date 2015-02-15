// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------
#include "CvGameCoreDLLPCH.h"
#include "WoTAlignmentInfos.h"

#if WOTMOD

WoTAlignmentInfo::WoTAlignmentInfo()
	: m_eYield(NO_YIELD),
	m_pbAlignmentOpposing(NULL)
{

}

WoTAlignmentInfo::~WoTAlignmentInfo()
{
	SAFE_DELETE_ARRAY(m_pbAlignmentOpposing);
}

bool WoTAlignmentInfo::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	m_eYield = static_cast<YieldTypes>(GC.getInfoTypeForString(kResults.GetText("YieldType")));

	const char* szAlignmentType = GetType();

	kUtility.PopulateArrayByExistence(m_pbAlignmentOpposing, "Alignments", "Alignment_Oppositions", "OpposingType", "AlignmentType", szAlignmentType);
}

#endif // WOTMOD

