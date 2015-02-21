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
//  class : WoTWhiteTowerAjahInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTWhiteTowerAjahInfo : public CvBaseInfo
{
public:
	WoTWhiteTowerAjahInfo();
	~WoTWhiteTowerAjahInfo();

	int GetColorType() const;
	UnitTypes GetSisterUnitType() const;
	const std::vector<PromotionTypes>& GetTierPromotionBonuses(AjahInfluenceTierTypes eTier) const;

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

private:
	int m_iDefaultColor;
	UnitTypes m_eSisterUnit;
	std::vector<std::vector<PromotionTypes>> m_aeTierPromotions;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  class : WoTWhiteTowerInfluenceTierInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTWhiteTowerInfluenceTierInfo : public CvBaseInfo
{
public:
	WoTWhiteTowerInfluenceTierInfo();
	~WoTWhiteTowerInfluenceTierInfo();

	int GetInfluenceThreshold() const;
	int GetMaxPlayers() const;

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

private:
	int m_iInfluenceThreshold;
	int m_iMaxPlayers;
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  class : WoTMinorCivAjahs
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTMinorCivAjahs
{
public:
	WoTMinorCivAjahs();
	~WoTMinorCivAjahs();

	void Init(CvMinorCivAI* pOwner);
	void Uninit();

	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

	AjahTypes GetMajorityAjah() const;
	AjahTypes GetAmyrlinAjah() const;
	void SetAmyrlinAjah(AjahTypes eAjah);

	void UpdateMajorityAjah();

	int GetAjahInfluence(AjahTypes eAjah) const;
	int GetAjahInfluence(AjahTypes eAjah, PlayerTypes ePlayer) const;
	void SetAjahInfluence(AjahTypes eAjah, PlayerTypes ePlayer, int iNewInfluence);
	void ChangeAjahInfluence(AjahTypes eAjah, PlayerTypes ePlayer, int iChange);

	bool HasInfluenceTier(PlayerTypes ePlayer, AjahTypes eAjah, AjahInfluenceTierTypes eTier) const;

	int GetAjahInfluenceTimes100(AjahTypes eAjah) const;
	int GetAjahInfluencePercent(AjahTypes eAjah) const;

	bool IsAjahPermitted(AjahTypes eAjah) const;
	void SetAjahPermitted(AjahTypes eAjah, bool bNewValue);

	bool IsHostsAjahs() const;
	void SetHostsAjahs(bool bNewValue);

	void DoTraineeAdmitted(CvUnit* pUnit);
	void DoChooseAmyrlin();
	void DoTurn();

private:
	int GetTotalInfluencePoints() const;
	AjahTypes ChooseWeightedAjah() const;
	void DoUpdateAjahBonuses(AjahTypes eAjah);
	void SetHasTierBonus(PlayerTypes ePlayer, AjahTypes eAjah, AjahInfluenceTierTypes eTier, bool bNewValue);
	bool HasBeenGivenInfluenceTierBonus(PlayerTypes ePlayer, AjahTypes eAjah, AjahInfluenceTierTypes eTier) const;

	AjahTypes m_eMajorityAjah;
	AjahTypes m_eAmyrlinAjah;

	std::vector<std::vector<int>> m_piAjahInfluences;
	std::vector<std::vector<std::vector<bool>>> m_abHasAjahTierBonus;

	CvMinorCivAI* m_pOwner;
};
#endif // WOTMOD