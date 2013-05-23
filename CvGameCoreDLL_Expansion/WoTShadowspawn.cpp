#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"
#include "CvGlobals.h"
#include "WoTShadowspawn.h"
#include "CvPlot.h"
#include "Database.h"

void WoTShadowspawn::Init()
{
	int numPlots = GC.getMap().numPlots();

	CvFeatureInfo* blightFeature;

	for (int i = 0; i < GC.getNumFeatureInfos(); i++)
	{
		CvFeatureInfo* loopFeature = GC.getFeatureInfo((FeatureTypes)i);
		if (strcmp(loopFeature->GetType(), "FEATURE_BLIGHT"))
		{
			blightFeature = loopFeature;
		}
	}

	// Store a list of all plots that have the Blight feature (possible spawn points)
	CvPlot* pkLoopPlot;
	for (int i = 0; i < numPlots; i++)
	{
		pkLoopPlot = GC.getMap().plotByIndexUnchecked(i);
		if (pkLoopPlot->getFeatureType() == blightFeature->GetID())
		{
			m_aiBlightPlots[m_iNumBlightPlots] = i;
			m_iNumBlightPlots++;
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

}

void WoTShadowspawn::Read(FDataStream& kStream)
{
	kStream >> m_iSpawnDelay;
	kStream >> m_iNumBlightPlots;
	kStream >> m_aiBlightPlots;
	kStream >> m_iSpawnRate;
}

void WoTShadowspawn::Write(FDataStream& kStream)
{
	kStream << m_iSpawnDelay;
	kStream << m_iNumBlightPlots;
	kStream << m_aiBlightPlots;
	kStream << m_iSpawnRate;
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