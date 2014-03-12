
#include "CvGameCoreDLLPCH.h"
#include "WoTWhiteTowerClasses.h"
#include "CvMinorCivAI.h"
#include "WoTSerialization.h"
#include "CvGameCoreUtils.h"

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
	m_eAmyrlinAjah(NO_AJAH),
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

	kStream << m_eAmyrlinAjah;

	kStream << ArrayWrapper<int>(GC.GetNumWhiteTowerAjahInfos(), m_piAjahInfluences);
}

void WoTMinorCivAjahs::Read(FDataStream& kStream)
{
	uint uiVersion;

	kStream >> uiVersion;

	kStream >> m_eMajorityAjah;

	kStream >> m_eAmyrlinAjah;

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

AjahTypes WoTMinorCivAjahs::GetAmyrlinAjah() const
{
	return m_eAmyrlinAjah;
}

void WoTMinorCivAjahs::SetAmyrlinAjah(AjahTypes eAjah)
{
	m_eAmyrlinAjah = eAjah;
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
	int iInfluenceTotal = GetTotalInfluencePoints();

	return iAjahInfluence / iInfluenceTotal;
}

int WoTMinorCivAjahs::GetTotalInfluencePoints() const
{
	int iInfluenceTotal = 0;
	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); i++)
	{
		AjahTypes eLoopAjah = static_cast<AjahTypes>(i);
		iInfluenceTotal += max(0, GetAjahInfluence(eLoopAjah));
	}
	return iInfluenceTotal;
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

	AjahTypes eOldMajority = m_eMajorityAjah;
	m_eMajorityAjah = eHighestAjah;

	if (eOldMajority != eHighestAjah)
	{
		WoTWhiteTowerAjahInfo* pOldInfo = GC.GetWhiteTowerAjahInfo(eOldMajority);
		WoTWhiteTowerAjahInfo* pNewInfo = GC.GetWhiteTowerAjahInfo(eHighestAjah);
	
		CvCity* pCapital = m_pOwner->GetPlayer()->getCapitalCity();
	
		if (pCapital)
		{
			CvString capitalName = pCapital->getName();
			int capitalX = pCapital->getX();
			int capitalY = pCapital->getY();

			for (int i = 0; i < MAX_MAJOR_CIVS; i++)
			{
				PlayerTypes ePlayer = static_cast<PlayerTypes>(i);

				CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

				if (kPlayer.isAlive() && GET_TEAM(kPlayer.getTeam()).isHasMet(m_pOwner->GetPlayer()->getTeam()))
				{
					CvString strMessage;
					CvNotifications* pNotification = kPlayer.GetNotifications();
					if(pNotification)
					{
						strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_TOWER_MAJORITY_AJAH_HAS_CHANGED", capitalName, pOldInfo->GetDescription(), pNewInfo->GetDescription());
						Localization::String strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_TOWER_MAJORITY_AJAH_HAS_CHANGED_SUMMARY");
						pNotification->Add(static_cast<NotificationTypes>(GC.getInfoTypeForString("NOTIFICATION_TOWER_MAJORITY_AJAH_HAS_CHANGED")), strMessage, strSummary.toUTF8(), capitalX, capitalY, pCapital->getOwner());
					}
				}
			}
		}
	}
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

bool WoTMinorCivAjahs::IsHostsAjahs() const
{
	CvMinorCivTraitInfo* pInfo = GC.GetMinorCivTraitInfo(static_cast<MinorCivTraitTypes>(GC.getMinorCivInfo(m_pOwner->GetMinorCivType())->GetMinorCivTrait()));
	return pInfo->IsHostsAjahs();
}

void WoTMinorCivAjahs::SetHostsAjahs(bool bNewValue)
{
	CvMinorCivTraitInfo* pInfo = GC.GetMinorCivTraitInfo(static_cast<MinorCivTraitTypes>(GC.getMinorCivInfo(m_pOwner->GetMinorCivType())->GetMinorCivTrait()));
	pInfo->SetHostsAjahs(bNewValue);
}

void WoTMinorCivAjahs::DoTraineeAdmitted(CvUnit* pUnit)
{
	std::vector<AjahTypes> availableAjahs;
	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); i++)
	{
		AjahTypes eAjah = static_cast<AjahTypes>(i);
		if (IsAjahPermitted(eAjah))
		{
			availableAjahs.push_back(eAjah);
		}
	}

	int ajahIndex = GC.getGame().getJonRandNum(availableAjahs.size(), "Choosing an Ajah to give influence to");
	
	int oldInfluencePercent = GetAjahInfluencePercent(availableAjahs[ajahIndex]);

	ChangeAjahInfluence(availableAjahs[ajahIndex], pUnit->GetAjahInfluenceChange());

	UpdateMajorityAjah();

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();

	if (pkScriptSystem)
	{
		CvLuaArgsHandle args;

		args->Push(pUnit->getOwner());
		args->Push(pUnit->GetID());
		args->Push(m_pOwner->GetPlayer()->GetID());
		args->Push(availableAjahs[ajahIndex]);
		args->Push(oldInfluencePercent);

		bool bResult;
		LuaSupport::CallHook(pkScriptSystem, "TowerTraineeChangedAjahInfluence", args.get(), bResult);
	}
}

AjahTypes WoTMinorCivAjahs::ChooseWeightedAjah() const
{
	int totalPoints = GetTotalInfluencePoints();

	int rand = GC.getGame().getJonRandNum(totalPoints, "Choosing Amyrlin");

	for (int i = 0; i < totalPoints; i++)
	{
		AjahTypes eAjah = static_cast<AjahTypes>(i);

		if (GetAjahInfluence(eAjah) > rand)
		{
			return eAjah;
		}
		rand -= max(0, GetAjahInfluence(eAjah));
	}
	CvAssertMsg(false, "Random selection of an Ajah should not fall out of the selection loop.");
}

void WoTMinorCivAjahs::DoChooseAmyrlin()
{
	AjahTypes eAjah = ChooseWeightedAjah();

	SetAmyrlinAjah(eAjah);

	WoTWhiteTowerAjahInfo* pNewInfo = GC.GetWhiteTowerAjahInfo(eAjah);
	
	CvCity* pCapital = m_pOwner->GetPlayer()->getCapitalCity();
	
	if (pCapital)
	{
		CvString capitalName = pCapital->getName();
		int capitalX = pCapital->getX();
		int capitalY = pCapital->getY();

		for (int i = 0; i < MAX_MAJOR_CIVS; i++)
		{
			PlayerTypes ePlayer = static_cast<PlayerTypes>(i);

			CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

			if (kPlayer.isAlive() && GET_TEAM(kPlayer.getTeam()).isHasMet(m_pOwner->GetPlayer()->getTeam()))
			{
				CvString strMessage;
				CvNotifications* pNotification = kPlayer.GetNotifications();
				if(pNotification)
				{
					strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_TOWER_AMYRLIN_ELECTED", capitalName, pNewInfo->GetDescription());
					Localization::String strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_TOWER_AMYRLIN_ELECTED_SUMMARY");
					pNotification->Add(static_cast<NotificationTypes>(GC.getInfoTypeForString("NOTIFICATION_TOWER_AMYRLIN_ELECTED")), strMessage, strSummary.toUTF8(), capitalX, capitalY, pCapital->getOwner());
				}
			}
		}
	}

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();

	if (pkScriptSystem)
	{
		CvLuaArgsHandle args;

		args->Push(m_pOwner->GetPlayer()->GetID());

		bool bResult;
		LuaSupport::CallHook(pkScriptSystem, "TowerAmyrlinElected", args.get(), bResult); 
	}
}

void WoTMinorCivAjahs::DoTurn()
{
	if (IsHostsAjahs() && GetAmyrlinAjah() == NO_AJAH)
	{
		DoChooseAmyrlin();
	}
}