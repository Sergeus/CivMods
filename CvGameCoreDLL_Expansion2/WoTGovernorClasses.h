// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#pragma once

#include "CvDatabaseUtility.h"
#include "CvInfos.h"
#include "Database.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  class : WoTGovernorClassInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTGovernorClassInfo : public CvBaseInfo
{
public:
	WoTGovernorClassInfo();
	virtual ~WoTGovernorClassInfo();

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

private:
	int m_iDefaultGovernorIndex;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      WoTGovernorEntry
//!  \brief		A single governor available in the game
//
//!  Key Attributes:
//!  - Populated from XML\Governors\Governors.xml
//!  - Array of these contained in WoTGovernorXMLEntries class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class WoTGovernorEntry : public CvBaseInfo
{
public:
	WoTGovernorEntry();
	~WoTGovernorEntry();

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	int GetGovernorClassType() const;
	const WoTGovernorClassInfo& GetGovernorClassInfo() const;

	int GetYieldChange(int i) const;
	int* GetYieldChangeArray() const;

private:
	int m_iGovernorClassType;
	const WoTGovernorClassInfo* m_pkGovernorClassInfo;

	int* m_piYieldChange;
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      WoTGovernorXMLEntries
//!  \brief		Game-wide information about governors
//
//! Key Attributes:
//! - Plan is it will be contained in CvGameRules object within CvGame class
//! - Populated from XML\Governors\Governors.xml
//! - Contains an array of WoTGovernorEntry from the above XML file
//! - One instance for the entire game
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTGovernorXMLEntries
{
public:
	WoTGovernorXMLEntries();
	~WoTGovernorXMLEntries();

	std::vector<WoTGovernorEntry*>& GetGovernorEntries();
	int GetNumGovernors();
	_Ret_maybenull_ WoTGovernorEntry* GetEntry(int index);

	void DeleteArray();

private:
	std::vector<WoTGovernorEntry*> m_paGovernorEntries;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  class : WoTCityGovernors
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class WoTCityGovernors
{
public:
	WoTCityGovernors();
	~WoTCityGovernors();

	GovernorTypes GetGovernorType();
	void SetGovernorType(GovernorTypes eNewType);
	int GetYieldChange(YieldTypes eYieldType);
	void SetYieldChange(YieldTypes eYieldType, int iYield);

	void Init(WoTGovernorXMLEntries* pGovernors, CvCity* pCity);
	void Uninit();
	void Reset();

	bool IsHasGovernor();

	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

private:
	GovernorTypes m_eGovernorType;

	int* m_aiYieldChange;

	WoTGovernorXMLEntries* m_pGovernors;
	CvCity* m_pCity;
};