// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"
#include "CvGlobals.h"
#include "WoTShadowspawn.h"
#include "CvPlot.h"
#include "Database.h"
#include "CustomLog.h"

int WoTShadowspawn::m_iSpawnDelay = -1;
int WoTShadowspawn::m_iSpawnRate = -1;
int WoTShadowspawn::m_iSpawnVariance = -1;
int WoTShadowspawn::m_iNumBlightPlots = -1;
FeatureTypes WoTShadowspawn::m_eBlightFeatureType = FeatureTypes::NO_FEATURE;
short* WoTShadowspawn::m_aiPlotShadowspawnSpawnCounter = NULL;
short* WoTShadowspawn::m_aiPlotShadowspawnNumUnitsSpawned = NULL;
std::vector<UnitClassTypes> WoTShadowspawn::m_aeShadowspawnUnitClasses = std::vector<UnitClassTypes>();

void WoTShadowspawn::Init()
{
	Uninit();

	int numPlots = GC.getMap().numPlots();

	CvFeatureInfo* blightFeature = NULL;

	for (int i = 0; i < GC.getNumFeatureInfos(); i++)
	{
		CvFeatureInfo* loopFeature = GC.getFeatureInfo((FeatureTypes)i);
		if (strcmp(loopFeature->GetType(), "FEATURE_BLIGHT") == 0)
		{
			blightFeature = loopFeature;
			m_eBlightFeatureType = (FeatureTypes)loopFeature->GetID();
			CUSTOMLOG("Blight feature found to have id %i.", m_eBlightFeatureType);
		}
	}

	if (blightFeature == NULL)
	{
		// All is lost
		return;
	}

	Database::Results kResults;
	if (GC.GetGameDatabase()->Execute(kResults, "SELECT Value FROM WoTModConstants WHERE Type='SHADOWSPAWN_MINIMUM_SPAWN_DELAY'"))
	{
		if (kResults.Step())
		{
			m_iSpawnDelay = kResults.GetInt(0);
		}
	}

	if (GC.GetGameDatabase()->Execute(kResults, "SELECT Value FROM WoTModConstants WHERE Type='SHADOWSPAWN_SPAWN_RATE'"))
	{
		if (kResults.Step())
		{
			m_iSpawnRate = kResults.GetInt(0);
		}
	}

	if (GC.GetGameDatabase()->Execute(kResults, "SELECT Value FROM WoTModConstants WHERE Type='SHADOWSPAWN_SPAWN_VARIANCE'"))
	{
		if (kResults.Step())
		{
			m_iSpawnVariance = kResults.GetInt(0);
		}
	}

	int iNumWorldPlots = GC.getMap().numPlots();

	m_aiPlotShadowspawnSpawnCounter = FNEW(short[iNumWorldPlots], c_eCiv5GameplayDLL, 0);
	m_aiPlotShadowspawnNumUnitsSpawned = FNEW(short[iNumWorldPlots], c_eCiv5GameplayDLL, 0);

	for (int i = 0; i < iNumWorldPlots; i++)
	{
		m_aiPlotShadowspawnSpawnCounter[i] = GetSpawnCounter(GC.getMap().plotByIndex(i));
		m_aiPlotShadowspawnNumUnitsSpawned[i] = 0;
	}

	CacheShadowspawnUnitClasses();
}

void WoTShadowspawn::Uninit()
{
	SAFE_DELETE_ARRAY(m_aiPlotShadowspawnSpawnCounter);
	SAFE_DELETE_ARRAY(m_aiPlotShadowspawnNumUnitsSpawned);
}

void WoTShadowspawn::Read(FDataStream& kStream)
{
	// Map should be initialized before this
	Init();

	uint uiVersion;

	kStream >> uiVersion;

	kStream >> m_iSpawnDelay;
	kStream >> m_iSpawnRate;
	kStream >> m_iSpawnVariance;

	int iNumWorldPlots = GC.getMap().numPlots();

	kStream >> ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnSpawnCounter);
	kStream >> ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnNumUnitsSpawned);
}

void WoTShadowspawn::Write(FDataStream& kStream)
{
	uint uiVersion = 1;

	kStream << uiVersion;

	kStream << m_iSpawnDelay;
	kStream << m_iSpawnRate;
	kStream << m_iSpawnVariance;

	int iNumWorldPlots = GC.getMap().numPlots();

	kStream << ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnSpawnCounter);
	kStream << ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnNumUnitsSpawned);
}

bool WoTShadowspawn::CanShadowspawnSpawn()
{
	if (GC.getGame().getGameTurn() < m_iSpawnDelay)
	{
		CUSTOMLOG("Shadowspawn cannot yet spawn because of the %i spawn delay.", m_iSpawnDelay);
		return false;
	}

	// TODO game option to turn off shadowspawn here

	return true;
}

void WoTShadowspawn::BeginTurn()
{
	CUSTOMLOG("Shadowspawn beginning its turn.");
	if (!CanShadowspawnSpawn())
	{
		return;
	}

	int iNumWorldPlots = GC.getMap().numPlots();

	for (int i = 0; i < iNumWorldPlots; i++)
	{
		if (m_aiPlotShadowspawnSpawnCounter[i] > 0)
		{
			CvPlot* pkLoopPlot = GC.getMap().plotByIndex(i);

			if (IsValidShadowspawnSpawn(pkLoopPlot))
			{
				m_aiPlotShadowspawnSpawnCounter[i]--;
				CUSTOMLOG("Shadowspawn counter decreased to %i at plot index %i.", m_aiPlotShadowspawnSpawnCounter[i], i);
			}
			else
			{
				m_aiPlotShadowspawnSpawnCounter[i] = -1;
			}
		}
	}
}

void WoTShadowspawn::DoUnits()
{
	if (!CanShadowspawnSpawn())
	{
		return;
	}
	CUSTOMLOG("Shadowspawn spawning time.");

	int iNumWorldPlots = GC.getMap().numPlots();

	for (int i = 0; i < iNumWorldPlots; i++)
	{
		if (m_aiPlotShadowspawnSpawnCounter[i] == 0)
		{
			CUSTOMLOG("Spawning shadowspawn at plot index %i.", i);

			SpawnShadowspawnUnit(GC.getMap().plotByIndex(i));

			m_aiPlotShadowspawnSpawnCounter[i] = GetSpawnCounter(GC.getMap().plotByIndex(i));
		}
	}
}

void WoTShadowspawn::SpawnShadowspawnUnit(CvPlot* pPlot)
{
	CvPlayerAI& kShadowPlayer = GET_PLAYER(SHADOW_PLAYER);

	UnitTypes eUnit = GetRandomShadowSpawnUnitType();

	CUSTOMLOG("Placing unit %i at %i, %i.", eUnit, pPlot->getX(), pPlot->getY());

	kShadowPlayer.initUnit(eUnit, pPlot->getX(), pPlot->getY(), UNITAI_FAST_ATTACK);

	m_aiPlotShadowspawnNumUnitsSpawned[pPlot->GetPlotIndex()];
}

UnitTypes WoTShadowspawn::GetRandomShadowSpawnUnitType()
{
	CvPlayerAI& kShadowPlayer = GET_PLAYER(SHADOW_PLAYER);

	CUSTOMLOG("Choosing random shadowspawn unit.");

	int iNumShadowUnitClasses = m_aeShadowspawnUnitClasses.size();
	for (int i = 0; i < iNumShadowUnitClasses; i++)
	{
		UnitClassTypes eUnitClass = m_aeShadowspawnUnitClasses[i];

		CvUnitClassInfo* pClassInfo = GC.getUnitClassInfo(eUnitClass);

		if (pClassInfo == NULL)
			continue;

		const UnitTypes eUnit = ((UnitTypes)(kShadowPlayer.getCivilizationInfo().getCivilizationUnits(eUnitClass)));
		if (eUnit != NO_UNIT)
		{
			// TODO actual choosing
			CUSTOMLOG("Shadowspawn random unit selection has picked unit with ID %i", eUnit);
			return eUnit;
		}
	}
}

bool WoTShadowspawn::IsValidShadowspawnSpawn(CvPlot* pPlot)
{
	if (pPlot->getFeatureType() != GetBlightFeatureType())
	{
		//CUSTOMLOG("Plot at %i, %i is not valid for shadowspawn because %i != %i.",
		//	pPlot->getX(), pPlot->getY(), pPlot->getFeatureType(), GetBlightFeatureType());
		return false;
	}

	//CUSTOMLOG("Plot at %i, %i is valid for shadowspawn.", pPlot->getX(), pPlot->getY());
	return true;
}

bool WoTShadowspawn::IsValidShadowspawnArmySpawn(CvPlot* pPlot)
{
	for(int iX = (-1 * GetShadowspawnArmyWidth()); iX <= GetShadowspawnArmyWidth(); iX++)
	{
		for(int iY = (-1 * GetShadowspawnArmyWidth()); iY <= GetShadowspawnArmyWidth(); iY++)
		{
			CvPlot* pkLoopPlot = plotXYWithRangeCheck(pPlot->getX(), pPlot->getY(), iX, iY, GetShadowspawnArmyWidth());

			if(pkLoopPlot != NULL)
			{
				if (pkLoopPlot->getFeatureType() != GetBlightFeatureType())
				{
					return false;
				}
			}
		}
	}

	return true;
}

int WoTShadowspawn::GetShadowspawnArmyWidth()
{
	return 1;
}

FeatureTypes WoTShadowspawn::GetBlightFeatureType()
{
	return m_eBlightFeatureType;
}

short WoTShadowspawn::GetSpawnCounter(CvPlot* pPlot)
{
	int counter = GetBaseSpawnRate() + GC.getGame().getJonRandNum(GetBaseSpawnVariance(), "Shadowspawn spawn counter rand");

	CUSTOMLOG("Setting shadowspawn counter to %i for plot %i, %i.", counter, pPlot->getX(), pPlot->getY());

	return counter;
}

int WoTShadowspawn::GetBaseSpawnRate()
{
	return m_iSpawnRate;
}

int WoTShadowspawn::GetBaseSpawnVariance()
{
	return m_iSpawnVariance;
}

void WoTShadowspawn::CacheShadowspawnUnitClasses()
{
	Database::Results kResults;
	if (GC.GetGameDatabase()->Execute(kResults, "SELECT UnitClassType FROM ShadowspawnUnitClasses;"))
	{
		while(kResults.Step())
		{
			const char* szUnitClass = kResults.GetText(0);

			CUSTOMLOG("Discovered shadowspawn unit class: %s", szUnitClass);

			int iUnitClassId = GC.getInfoTypeForString(szUnitClass);

			m_aeShadowspawnUnitClasses.push_back((UnitClassTypes)iUnitClassId);
		}
	}
}