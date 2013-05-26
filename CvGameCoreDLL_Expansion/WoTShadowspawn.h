// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#pragma once

class WoTShadowspawn
{
public:
	static void Init();
	static void Uninit();
	static void BeginTurn();
	static void DoBlightSpread();
	static void DoUnits();

	static FeatureTypes GetBlightFeatureType();

	static void Read(FDataStream& kStream);
	static void Write(FDataStream& kStream);

private:
	static bool CanShadowspawnSpawn();
	static bool IsValidShadowspawnSpawn(CvPlot* pPlot);
	static bool IsValidShadowspawnArmySpawn(CvPlot* pPlot);
	static int GetShadowspawnArmyWidth();

	static int m_iSpawnDelay;
	static int m_iNumBlightPlots;
	static int* m_aiBlightPlots;
	static int m_iSpawnRate;

	static FeatureTypes m_eBlightFeatureType;

	static short* m_aiPlotShadowspawnSpawnCounter;
	static short* m_aiPlotShadowspawnNumUnitsSpawned;

	//static UnitTypes GetRandomShadowspawnUnitType(CvArea
};
