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

int WoTShadowspawn::m_iSpawnDelay = -1;
int WoTShadowspawn::m_iSpawnRate = -1;
int WoTShadowspawn::m_iNumBlightPlots = -1;
int* WoTShadowspawn::m_aiBlightPlots = NULL;

void WoTShadowspawn::Init()
{
	if (m_aiBlightPlots != NULL)
	{
		SAFE_DELETE_ARRAY(m_aiBlightPlots);
	}

	int numPlots = GC.getMap().numPlots();

	CvFeatureInfo* blightFeature = NULL;

	for (int i = 0; i < GC.getNumFeatureInfos(); i++)
	{
		CvFeatureInfo* loopFeature = GC.getFeatureInfo((FeatureTypes)i);
		if (strcmp(loopFeature->GetType(), "FEATURE_BLIGHT"))
		{
			blightFeature = loopFeature;
			m_eBlightFeatureType = (FeatureTypes)i;
		}
	}

	if (blightFeature == NULL)
	{
		// All is lost
		return;
	}

	// Count all plots with Blight on them
	CvPlot* pkLoopPlot;
	for (int i = 0; i < numPlots; i++)
	{
		pkLoopPlot = GC.getMap().plotByIndexUnchecked(i);
		if (pkLoopPlot->getFeatureType() == blightFeature->GetID())
		{
			m_iNumBlightPlots++;
		}
	}

	// Create an array to deal with all current blight plots
	m_aiBlightPlots = FNEW(int[m_iNumBlightPlots], c_eCiv5GameplayDLL, 0);

	int plotsAdded = 0;
	for (int i = 0; i < numPlots; i++)
	{
		pkLoopPlot = GC.getMap().plotByIndexUnchecked(i);
		if (pkLoopPlot->getFeatureType() == blightFeature->GetID())
		{
			m_aiBlightPlots[plotsAdded] = i;
			plotsAdded++;
		}
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
}

void WoTShadowspawn::Uninit()
{
	SAFE_DELETE_ARRAY(m_aiBlightPlots);
}

void WoTShadowspawn::Read(FDataStream& kStream)
{
	// Map should be initialized before this
	Init();

	kStream >> m_iSpawnDelay;
	kStream >> m_iSpawnRate;

	int iNumWorldPlots = GC.getMap().numPlots();

	kStream >> ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnSpawnCounter);
	kStream >> ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnNumUnitsSpawned);
}

void WoTShadowspawn::Write(FDataStream& kStream)
{
	kStream << m_iSpawnDelay;
	kStream << m_iSpawnRate;

	int iNumWorldPlots = GC.getMap().numPlots();

	kStream << ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnSpawnCounter);
	kStream << ArrayWrapper<short>(iNumWorldPlots, m_aiPlotShadowspawnNumUnitsSpawned);
}

bool WoTShadowspawn::CanShadowspawnSpawn()
{
	if (GC.getGame().getGameTurn() < m_iSpawnDelay)
	{
		return false;
	}

	// TODO game option to turn off shadowspawn here

	return true;
}

void WoTShadowspawn::BeginTurn()
{

}

void WoTShadowspawn::DoUnits()
{

}

bool WoTShadowspawn::IsValidShadowspawnSpawn(CvPlot* pPlot)
{
	if (pPlot->getFeatureType() != GetBlightFeatureType())
	{
		return false;
	}

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
