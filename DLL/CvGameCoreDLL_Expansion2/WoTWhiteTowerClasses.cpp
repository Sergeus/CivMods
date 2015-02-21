
#include "CvGameCoreDLLPCH.h"
#include "WoTWhiteTowerClasses.h"
#include "CvMinorCivAI.h"
#include "WoTSerialization.h"
#include "CvGameCoreUtils.h"

#if WOTMOD
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
	m_eSisterUnit = static_cast<UnitTypes>(GC.getInfoTypeForString(kResults.GetText("SisterUnitType")));

	const char* szAjahType = GetType();

	// Promotion Influence Tier Bonuses
	{
		m_aeTierPromotions.resize(kUtility.MaxRows("AjahInfluenceTiers"));
		std::string strKey("Ajah_InfluencePromotionBonus");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select AjahInfluenceTiers.ID, UnitPromotions.ID from Ajah_InfluencePromotionBonus inner join AjahInfluenceTiers on AjahInfluenceTiers.Type = TierType inner join UnitPromotions on UnitPromotions.Type = PromotionType where AjahType = ?");
		}
		pResults->Bind(1, szAjahType);

		while (pResults->Step())
		{
			AjahInfluenceTierTypes eTier = static_cast<AjahInfluenceTierTypes>(pResults->GetInt(0));
			PromotionTypes ePromotion = static_cast<PromotionTypes>(pResults->GetInt(1));

			m_aeTierPromotions[eTier].push_back(ePromotion);
		}
	}

	return true;
}
int WoTWhiteTowerAjahInfo::GetColorType() const
{
	return m_iDefaultColor;
}

UnitTypes WoTWhiteTowerAjahInfo::GetSisterUnitType() const
{
	return m_eSisterUnit;
}

const std::vector<PromotionTypes>& WoTWhiteTowerAjahInfo::GetTierPromotionBonuses(AjahInfluenceTierTypes eTier) const
{
	return m_aeTierPromotions[eTier];
}

//======================================================================================================
//					WoTWhiteTowerInfluenceTierInfo
//======================================================================================================

WoTWhiteTowerInfluenceTierInfo::WoTWhiteTowerInfluenceTierInfo()
	: m_iInfluenceThreshold(0)
	, m_iMaxPlayers(0)
{
}
WoTWhiteTowerInfluenceTierInfo::~WoTWhiteTowerInfluenceTierInfo()
{
}
bool WoTWhiteTowerInfluenceTierInfo::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	m_iInfluenceThreshold = kResults.GetInt("Threshold");
	m_iMaxPlayers = kResults.GetInt("MaxSimultaneousPlayers");

	return true;
}
int WoTWhiteTowerInfluenceTierInfo::GetInfluenceThreshold() const
{
	return m_iInfluenceThreshold;
}
int WoTWhiteTowerInfluenceTierInfo::GetMaxPlayers() const
{
	return m_iMaxPlayers;
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

	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); ++i)
	{
		kStream << m_piAjahInfluences[i];
		for (int j = 0; j < MAX_MAJOR_CIVS; ++j)
		{
			kStream << m_abHasAjahTierBonus[i][j];
		}
	}
}

void WoTMinorCivAjahs::Read(FDataStream& kStream)
{
	uint uiVersion;

	kStream >> uiVersion;

	kStream >> m_eMajorityAjah;

	kStream >> m_eAmyrlinAjah;

	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); ++i)
	{
		kStream >> m_piAjahInfluences[i];
		for (int j = 0; j < MAX_MAJOR_CIVS; ++j)
		{
			kStream >> m_abHasAjahTierBonus[i][j];
		}
	}
}

void WoTMinorCivAjahs::Uninit()
{
}

void WoTMinorCivAjahs::Init(CvMinorCivAI* pOwner)
{
	m_pOwner = pOwner;

	m_piAjahInfluences.resize(GC.GetNumWhiteTowerAjahInfos());
	m_abHasAjahTierBonus.resize(GC.GetNumWhiteTowerAjahInfos());

	for (int i = 0; i < GC.GetNumWhiteTowerAjahInfos(); ++i)
	{
		m_piAjahInfluences[i].resize(MAX_MAJOR_CIVS, 0);
		m_abHasAjahTierBonus[i].resize(MAX_MAJOR_CIVS);
		for (int j = 0; j < MAX_MAJOR_CIVS; ++j)
		{
			m_abHasAjahTierBonus[i][j].resize(GC.GetNumAjahInfluenceTierInfos(), false);
		}
	}

	m_eMajorityAjah = NO_AJAH;
	UpdateMajorityAjah();
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
	int iInfluence = 0;
	for (int i = 0; i < MAX_MAJOR_CIVS; ++i)
	{
		iInfluence += m_piAjahInfluences[eAjah][i];
	}
	return iInfluence;
}

int WoTMinorCivAjahs::GetAjahInfluence(AjahTypes eAjah, PlayerTypes ePlayer) const
{
	return m_piAjahInfluences[eAjah][ePlayer];
}

int WoTMinorCivAjahs::GetAjahInfluenceTimes100(AjahTypes eAjah) const
{
	return GetAjahInfluence(eAjah) * 100;
}

void WoTMinorCivAjahs::SetAjahInfluence(AjahTypes eAjah, PlayerTypes ePlayer, int iNewInfluence)
{
	CvAssertMsg(eAjah < GC.GetNumWhiteTowerAjahInfos(), "Index out of bounds");
	CvAssertMsg(eAjah > NO_AJAH, "Index out of bounds");
	m_piAjahInfluences[eAjah][ePlayer] = std::max(iNewInfluence, 0);

	UpdateMajorityAjah();

	DoUpdateAjahBonuses(eAjah);
}

void WoTMinorCivAjahs::ChangeAjahInfluence(AjahTypes eAjah, PlayerTypes ePlayer, int iChange)
{
	SetAjahInfluence(eAjah, ePlayer, GetAjahInfluence(eAjah, ePlayer) + iChange);
}

bool WoTMinorCivAjahs::HasInfluenceTier(PlayerTypes ePlayer, AjahTypes eAjah, AjahInfluenceTierTypes eTier) const
{
	WoTWhiteTowerInfluenceTierInfo* pInfo = GC.GetAjahInfluenceTierInfo(eTier);
	int playerInfluence = GetAjahInfluence(eAjah, ePlayer);
	if (playerInfluence < pInfo->GetInfluenceThreshold())
	{
		return false;
	}

	int numPlayersWithMoreInfluence = 0;
	for (int i = 0; i < MAX_MAJOR_CIVS; ++i)
	{
		if (GetAjahInfluence(eAjah, static_cast<PlayerTypes>(i)) > playerInfluence)
		{
			numPlayersWithMoreInfluence++;
		}
	}
	return numPlayersWithMoreInfluence < pInfo->GetMaxPlayers();
}

int WoTMinorCivAjahs::GetAjahInfluencePercent(AjahTypes eAjah) const
{
	int iAjahInfluence = GetAjahInfluenceTimes100(eAjah);
	int iInfluenceTotal = GetTotalInfluencePoints();

	if (iInfluenceTotal == 0)
	{
		return 0;
	}

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

void WoTMinorCivAjahs::DoUpdateAjahBonuses(AjahTypes eAjah)
{
	for (int i = 0; i < MAX_MAJOR_CIVS; ++i)
	{
		PlayerTypes ePlayer = static_cast<PlayerTypes>(i);
		CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
		if (kPlayer.isAlive())
		{
			for (int j = 0; j < GC.GetNumAjahInfluenceTierInfos(); ++j)
			{
				AjahInfluenceTierTypes eTier = static_cast<AjahInfluenceTierTypes>(j);
				bool bPlayerHasTier = HasInfluenceTier(ePlayer, eAjah, eTier);

				if (bPlayerHasTier != HasBeenGivenInfluenceTierBonus(ePlayer, eAjah, eTier))
				{
					SetHasTierBonus(ePlayer, eAjah, eTier, bPlayerHasTier);
				}
			}
		}
	}
}

bool WoTMinorCivAjahs::HasBeenGivenInfluenceTierBonus(PlayerTypes ePlayer, AjahTypes eAjah, AjahInfluenceTierTypes eTier) const
{
	return m_abHasAjahTierBonus[eAjah][ePlayer][eTier];
}

void WoTMinorCivAjahs::SetHasTierBonus(PlayerTypes ePlayer, AjahTypes eAjah, AjahInfluenceTierTypes eTier, bool bNewValue)
{
	m_abHasAjahTierBonus[eAjah][ePlayer][eTier] = bNewValue;

	WoTWhiteTowerAjahInfo* pInfo = GC.GetWhiteTowerAjahInfo(eAjah);

	const std::vector<PromotionTypes>& promotions = pInfo->GetTierPromotionBonuses(eTier);
	UnitTypes eSisterUnit = pInfo->GetSisterUnitType();
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

	if (promotions.size() > 0)
	{
		CvUnit* pUnit;
		int iLoop;
		for (pUnit = kPlayer.firstUnit(&iLoop); pUnit != NULL; pUnit = kPlayer.nextUnit(&iLoop))
		{
			if (pUnit->getUnitType() == eSisterUnit)
			{
				for (std::vector<PromotionTypes>::const_iterator it = promotions.begin(); it != promotions.end(); ++it)
				{
					pUnit->setHasPromotion(*it, bNewValue);
				}
			}
		}
	}
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

	ChangeAjahInfluence(availableAjahs[ajahIndex], pUnit->getOwner(), pUnit->GetAjahInfluenceChange());

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
	if (IsHostsAjahs())
	{
		if (GetAmyrlinAjah() == NO_AJAH)
		{
			DoChooseAmyrlin();
		}

		for (int i = 0; i < MAX_MAJOR_CIVS; i++)
		{
			PlayerTypes ePlayer = static_cast<PlayerTypes>(i);
			CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

			if (kPlayer.GetPublicSupportedAjah() != NO_AJAH)
			{
				ChangeAjahInfluence(kPlayer.GetPublicSupportedAjah(), ePlayer, kPlayer.GetAjahSupportPassiveInfluenceChange());
			}
		}
	}
}
#endif // WOTMOD