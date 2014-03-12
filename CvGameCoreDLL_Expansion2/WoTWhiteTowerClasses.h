// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#pragma once

#include "CvDatabaseUtility.h"
#include "CvInfos.h"
#include "Database.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  class : WoTWhiteTowerAjahInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTWhiteTowerAjahInfo : public CvBaseInfo
{
public:
	WoTWhiteTowerAjahInfo();
	~WoTWhiteTowerAjahInfo();

	int GetColorType() const;

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

private:
	int m_iDefaultColor;
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

	void Reset();

	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

	AjahTypes GetMajorityAjah() const;
	AjahTypes GetAmyrlinAjah() const;
	void SetAmyrlinAjah(AjahTypes eAjah);

	void UpdateMajorityAjah();

	int GetAjahInfluence(AjahTypes eAjah) const;
	void SetAjahInfluence(AjahTypes eAjah, int iNewInfluence);
	void ChangeAjahInfluence(AjahTypes eAjah, int iChange);

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

	AjahTypes m_eMajorityAjah;
	AjahTypes m_eAmyrlinAjah;

	int* m_piAjahInfluences;

	CvMinorCivAI* m_pOwner;
};