// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------
#include "CvGameCoreDLLPCH.h"
#include "WoTGovernorClasses.h"
#include "WoTSerialization.h"

#if WOTMOD
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

	return true;
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

int WoTGovernorEntry::GetYieldChange(int i) const
{
	return m_piYieldChange[i];
}

int* WoTGovernorEntry::GetYieldChangeArray() const
{
	return m_piYieldChange;
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

	return true;
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
	m_aiYieldChange(NULL),
	m_pCity(NULL),
	m_pGovernors(NULL)
{

}

WoTCityGovernors::~WoTCityGovernors()
{
	
}

void WoTCityGovernors::Init(WoTGovernorXMLEntries* pGovernors, CvCity* pCity)
{
	m_pCity = pCity;
	m_pGovernors = pGovernors;

	m_aiYieldChange = FNEW(int[GC.GetNumYieldInfos()], c_eCiv5GameplayDLL, 0);

	Reset();
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

GovernorTypes WoTCityGovernors::GetGovernorType() const
{
	return m_eGovernorType;
}
GovernorClassTypes WoTCityGovernors::GetGovernorClassType() const
{
	if (!IsHasGovernor())
	{
		return NO_GOVERNORCLASS;
	}

	return static_cast<GovernorClassTypes>(GC.GetGovernorInfo(GetGovernorType())->GetGovernorClassType());
}
void WoTCityGovernors::SetGovernorType(GovernorTypes eNewGovernorType)
{
	if (eNewGovernorType != GetGovernorType())
	{
		if (eNewGovernorType == NO_GOVERNOR)
		{
			Reset();
		}
		else
		{
			m_eGovernorType = eNewGovernorType;

			WoTGovernorEntry* pInfo = GC.GetGovernorInfo(eNewGovernorType);

			for (int i = 0; i < GC.GetNumYieldInfos(); i++)
			{
				int iBase = 0;

				iBase += pInfo->GetYieldChange(i);

				// building changes
				for (int j = 0; j < GC.getNumBuildingInfos(); j++)
				{
					BuildingTypes eBuilding = static_cast<BuildingTypes>(j);
					if (m_pCity->GetCityBuildings()->GetNumActiveBuilding(eBuilding) > 0)
					{
						CvBuildingEntry* pBuildingInfo = GC.getBuildingInfo(eBuilding);
						iBase += pBuildingInfo->GetGovernorYieldChange(i);
						iBase += pBuildingInfo->GetGovernorClassYieldChange(m_eGovernorType, i);
					}
				}

				m_aiYieldChange[i] = iBase;
			}
		}
	}
}

int WoTCityGovernors::GetYieldChange(YieldTypes eYieldType) const
{
	CvAssertMsg(eYieldType > -1, "Index out of bounds");
	CvAssertMsg(eYieldType < GC.GetNumYieldInfos(), "Index out of bounds");
	if (IsHasGovernor())
	{
		return m_aiYieldChange[eYieldType];
	}
	else
	{
		return 0;
	}
}

void WoTCityGovernors::SetYieldChange(YieldTypes eYieldType, int iYield)
{
	CvAssertMsg(eYieldType > -1, "Index out of bounds");
	CvAssertMsg(eYieldType < GC.GetNumYieldInfos(), "Index out of bounds");
	m_aiYieldChange[eYieldType] = iYield;
}

void WoTCityGovernors::ChangeYieldChange(YieldTypes eYieldType, int iYield)
{
	CvAssertMsg(eYieldType > -1, "Index out of bounds");
	CvAssertMsg(eYieldType < GC.GetNumYieldInfos(), "Index out of bounds");
	SetYieldChange(eYieldType, GetYieldChange(eYieldType) + iYield);
}

bool WoTCityGovernors::IsHasGovernor() const
{
	return m_eGovernorType != NO_GOVERNOR;
}

void WoTCityGovernors::Read(FDataStream& kStream)
{
	uint uiVersion;
	kStream >> uiVersion;

	kStream >> m_eGovernorType;

	kStream >> ArrayWrapper<int>(GC.GetNumYieldInfos(), m_aiYieldChange);
}

void WoTCityGovernors::Write(FDataStream& kStream)
{
	uint uiVersion = 1;

	kStream << uiVersion;

	kStream << m_eGovernorType;

	kStream << ArrayWrapper<int>(GC.GetNumYieldInfos(), m_aiYieldChange);
}
#endif // WOTMOD