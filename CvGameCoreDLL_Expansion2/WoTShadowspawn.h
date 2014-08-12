// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#pragma once

#if WOTMOD
class WoTShadowspawn
{
public:
	static void Init();
	static void Uninit();
	static void BeginTurn();
	static void DoBlightSpread();
	static void DoUnits();

	static FeatureTypes GetBlightFeatureType();

	static void SpawnShadowspawnUnit(CvPlot* pPlot);
	static void SpawnShadowspawnArmy(CvPlot* pPlot);

	static void Read(FDataStream& kStream);
	static void Write(FDataStream& kStream);

private:
	static bool CanShadowspawnSpawn();
	static bool IsValidShadowspawnSpawn(CvPlot* pPlot);
	static bool IsValidShadowspawnArmySpawn(CvPlot* pPlot);

	static int GetShadowspawnArmyWidth();
	static short GetSpawnCounter(CvPlot* pPlot);

	static void CacheShadowspawnUnitClasses();

	static int GetBaseSpawnRate();
	static int GetBaseSpawnVariance();
	static UnitTypes GetRandomShadowSpawnUnitType();

	static int m_iSpawnDelay;
	static int m_iNumBlightPlots;
	static int m_iSpawnRate;
	static int m_iSpawnVariance;

	static FeatureTypes m_eBlightFeatureType;

	static short* m_aiPlotShadowspawnSpawnCounter;
	static short* m_aiPlotShadowspawnNumUnitsSpawned;

	static std::vector<UnitClassTypes> m_aeShadowspawnUnitClasses;

	//static UnitTypes GetRandomShadowspawnUnitType(CvArea
};
#endif // WOTMOD