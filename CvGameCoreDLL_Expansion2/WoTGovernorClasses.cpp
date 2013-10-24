// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------
#include "CvGameCoreDLLPCH.h"
#include "WoTGovernorClasses.h"

//======================================================================================================
//					WoTGovernorClassInfo
//======================================================================================================
WoTGovernorClassInfo::WoTGovernorClassInfo() :
	m_iDefaultGovernorIndex(NO_GOVERNOR)
{

}

WoTGovernorClassInfo::~WoTGovernorClassInfo()
{

}

bool WoTGovernorClassInfo::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	const char* szDefaultGovernor = kResults.GetText("DefaultGovernor");
	m_iDefaultGovernorIndex = GC.getInfoTypeForString(szDefaultGovernor, true);
}

//======================================================================================================
//					WoTGovernorEntry
//======================================================================================================
WoTGovernorEntry::WoTGovernorEntry() :
	m_iGovernorClassType(-1),
	m_piYieldChange(NULL),
	m_pkGovernorClassInfo(NULL)
{

}

WoTGovernorEntry::~WoTGovernorEntry()
{
	SAFE_DELETE_ARRAY(m_piYieldChange);
}

int WoTGovernorEntry::GetGovernorClassType() const
{
	return m_iGovernorClassType;
}

const WoTGovernorClassInfo& WoTGovernorEntry::GetGovernorClassInfo() const
{
	return *m_pkGovernorClassInfo;
}

bool WoTGovernorEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	const char* szGovernorClass = kResults.GetText("GovernorClass");
	m_iGovernorClassType = GC.getInfoTypeForString(szGovernorClass, true);
	
	m_pkGovernorClassInfo = GC.GetGovernorClassInfo(static_cast<GovernorClassTypes>(m_iGovernorClassType));

	const char* szGovernorType = GetType();

	kUtility.SetYields(m_piYieldChange, "Governor_YieldChanges", "GovernorType", szGovernorType);
}

//======================================================================================================
//					WoTGovernorXMLEntries
//======================================================================================================

WoTGovernorXMLEntries::WoTGovernorXMLEntries()
{

}

WoTGovernorXMLEntries::~WoTGovernorXMLEntries()
{
	DeleteArray();
}

int WoTGovernorXMLEntries::GetNumGovernors()
{
	return m_paGovernorEntries.size();
}

WoTGovernorEntry* WoTGovernorXMLEntries::GetEntry(int iIndex)
{
	return m_paGovernorEntries[iIndex];
}

std::vector<WoTGovernorEntry*>& WoTGovernorXMLEntries::GetGovernorEntries()
{
	return m_paGovernorEntries;
}

void WoTGovernorXMLEntries::DeleteArray()
{
	for (std::vector<WoTGovernorEntry*>::iterator it = m_paGovernorEntries.begin(); it != m_paGovernorEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paGovernorEntries.clear();
}

//======================================================================================================
//					WoTCityGovernors
//======================================================================================================

WoTCityGovernors::WoTCityGovernors() :
	m_eGovernorType(NO_GOVERNOR),
	m_aiYieldChange(NULL)
{

}

WoTCityGovernors::~WoTCityGovernors()
{
	
}

void WoTCityGovernors::Init(WoTGovernorXMLEntries* pGovernors, CvCity* pCity)
{

}

void WoTCityGovernors::Reset()
{
	m_eGovernorType = NO_GOVERNOR;

	for (int i = 0; i < GC.GetNumYieldInfos(); i++)
	{
		m_aiYieldChange[i] = 0;
	}
}

void WoTCityGovernors::Uninit()
{
	SAFE_DELETE_ARRAY(m_aiYieldChange);
}

GovernorTypes WoTCityGovernors::GetGovernorType()
{
	return m_eGovernorType;
}

int WoTCityGovernors::GetYieldChange(YieldTypes yield)
{
	return m_aiYieldChange[yield];
}

void WoTCityGovernors::Read(FDataStream& kStream)
{
	uint uiVersion = 1;
	kStream >> uiVersion;

	kStream >> m_eGovernorType;

	for (int i = 0;
}