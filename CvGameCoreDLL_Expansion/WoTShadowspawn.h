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

	static void Read(FDataStream& kStream);
	static void Write(FDataStream& kStream);

private:
	static bool CanShadowspawnSpawn();

	static int m_iSpawnDelay;
	static int m_iNumBlightPlots;
	static std::vector<int> m_aiBlightPlots;
	static int m_iSpawnRate;

	//static UnitTypes GetRandomShadowspawnUnitType(CvArea
};
