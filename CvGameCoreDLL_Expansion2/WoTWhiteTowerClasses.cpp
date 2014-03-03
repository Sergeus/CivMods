
#include "CvGameCoreDLLPCH.h"
#include "WoTWhiteTowerClasses.h"
#include "CvMinorCivAI.h"
#include "WoTSerialization.h"

//======================================================================================================
//					WoTWhiteTowerAjahInfo
//======================================================================================================

WoTWhiteTowerAjahInfo::WoTWhiteTowerAjahInfo() :
	m_iDefaultColor(-1)
{

}
WoTWhiteTowerAjahInfo::~WoTWhiteTowerAjahInfo()
{

}
bool WoTWhiteTowerAjahInfo::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	const char* szTextVal = kResults.GetText("Color");
	m_iDefaultColor = GC.getInfoTypeForString(szTextVal, true);

	return true;
}
int WoTWhiteTowerAjahInfo::GetColorType() const
{
	return m_iDefaultColor;
}

//======================================================================================================
//					WoTMinorCivAjahs
//======================================================================================================

WoTMinorCivAjahs::WoTMinorCivAjahs() :
	m_eMajorityAjah(NO_AJAH),
	m_piAjahInfluences(NULL),
	m_pOwner(NULL)
{

}

WoTMinorCivAjahs::~WoTMinorCivAjahs()
{
	Uninit();
}

void WoTMinorCivAjahs::Write(FDataStream& kStream)
{
	uint uiVersion = 0;
	kStream << uiVersion;

	kStream << m_eMajorityAjah;

	kStream << ArrayWrapper<int>(GC.GetNumWhiteTowerAjahInfos(), m_piAjahInfluences);
}

void WoTMinorCivAjahs::Read(FDataStream& kStream)
{
	uint uiVersion;

	kStream >> uiVersion;

	kStream >> m_eMajorityAjah;

	kStream >> ArrayWrapper<int>(GC.GetNumWhiteTowerAjahInfos(), m_piAjahInfluences);
}

void WoTMinorCivAjahs::Uninit()
{
	SAFE_DELETE_ARRAY(m_piAjahInfluences);
}

void WoTMinorCivAjahs::Init(CvMinorCivAI* pOwner)
{
	m_pOwner = pOwner;

	m_piAjahInfluences = FNEW(int[GC.GetNumWhiteTowerAjahInfos()], c_eCiv5GameplayDLL, 0);

	Reset();

	if (m_pOwner->GetPlayer()->isMinorCiv())
	{
		CvMinorCivInfo* pOwnerInfo = GC.getMinorCivInfo(m_pOwner->GetMinorCivType());

		for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); i++)
		{
			AjahTypes eAjah = static_cast<AjahTypes>(i);
			m_piAjahInfluences[i] = pOwnerInfo->GetAjahStartingInfluence(eAjah);
		}
	}
	UpdateMajorityAjah();
}

void WoTMinorCivAjahs::Reset()
{
	m_eMajorityAjah = NO_AJAH;

	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); i++)
	{
		m_piAjahInfluences[i] = 0;
	}
}

AjahTypes WoTMinorCivAjahs::GetMajorityAjah() const
{
	return m_eMajorityAjah;
}

int WoTMinorCivAjahs::GetAjahInfluence(AjahTypes eAjah) const
{
	CvAssertMsg(eAjah < GC.GetNumWhiteTowerAjahInfos(), "Index out of bounds");
	CvAssertMsg(eAjah > NO_AJAH, "Index out of bounds");
	return m_piAjahInfluences[eAjah];
}

int WoTMinorCivAjahs::GetAjahInfluenceTimes100(AjahTypes eAjah) const
{
	return GetAjahInfluence(eAjah) * 100;
}

void WoTMinorCivAjahs::SetAjahInfluence(AjahTypes eAjah, int iNewInfluence)
{
	CvAssertMsg(eAjah < GC.GetNumWhiteTowerAjahInfos(), "Index out of bounds");
	CvAssertMsg(eAjah > NO_AJAH, "Index out of bounds");
	m_piAjahInfluences[eAjah] = iNewInfluence;
}

void WoTMinorCivAjahs::ChangeAjahInfluence(AjahTypes eAjah, int iChange)
{
	SetAjahInfluence(eAjah, GetAjahInfluence(eAjah) + iChange);
}

int WoTMinorCivAjahs::GetAjahInfluencePercent(AjahTypes eAjah) const
{
	int iAjahInfluence = GetAjahInfluenceTimes100(eAjah);
	int iInfluenceTotal = 0;
	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); i++)
	{
		AjahTypes eLoopAjah = static_cast<AjahTypes>(i);
		iInfluenceTotal += max(0, GetAjahInfluence(eLoopAjah));
	}

	return iAjahInfluence / iInfluenceTotal;
}

void WoTMinorCivAjahs::UpdateMajorityAjah()
{
	int iHighestInfluence = 0;
	AjahTypes eHighestAjah = NO_AJAH;
	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); i++)
	{
		AjahTypes eAjah = static_cast<AjahTypes>(i);

		// TODO: deal with ties
		int iInfluence = GetAjahInfluence(eAjah);
		if (iInfluence > iHighestInfluence)
		{
			iHighestInfluence = iInfluence;
			eHighestAjah = eAjah;
		}
	}

	CvAssertMsg(eHighestAjah != NO_AJAH, "No Ajah has majority!");
	m_eMajorityAjah = eHighestAjah;
}

bool WoTMinorCivAjahs::IsAjahPermitted(AjahTypes eAjah) const
{
	CvMinorCivInfo* pInfo = GC.getMinorCivInfo(m_pOwner->GetMinorCivType());
	return pInfo->IsAjahPermitted(eAjah);
}

void WoTMinorCivAjahs::SetAjahPermitted(AjahTypes eAjah, bool bNewValue)
{
	CvMinorCivInfo* pInfo = GC.getMinorCivInfo(m_pOwner->GetMinorCivType());
	pInfo->SetAjahPermitted(eAjah, bNewValue);
}