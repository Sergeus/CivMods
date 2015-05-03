// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#include "CvGameCoreDLLPCH.h"
#include "WoTThreadClasses.h"

#if WOTMOD

WoTThreadInfo::WoTThreadInfo()
{
}

WoTThreadInfo::~WoTThreadInfo()
{

}

bool WoTThreadInfo::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	m_ThreadTitle = kResults.GetText("ThreadTitle");
	m_FlavorBody = kResults.GetText("FlavorBody");

	// choices
	{
		std::vector<bool> hasChoices;
		kUtility.PopulateVectorByExistence(hasChoices, "ThreadChoices", "Thread_Choices", "ThreadChoiceType", "ThreadType", GetType());

		for (int i = 0; i < hasChoices.size(); ++i)
		{
			if (hasChoices[i])
			{
				m_Choices.push_back(static_cast<ThreadChoiceTypes>(i));
			}
		}
	}

	return true;
}

WoTThreadChoiceInfo::WoTThreadChoiceInfo()
{
}

WoTThreadChoiceInfo::~WoTThreadChoiceInfo()
{
}

bool WoTThreadChoiceInfo::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	kUtility.SetYields(m_YieldChanges, "ThreadChoice_Yields", "ThreadChoiceType", GetType());
}

#endif // WOTMOD