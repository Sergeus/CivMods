// ----------------------------------------------------------------
// WoTMod New File
// Created by: S3rgeus
// ----------------------------------------------------------------

#pragma once

#include "CvPlot.h"
#include "CvUnit.h"

// Horn of Valere
class HornOfValere
{
public:
	HornOfValere();
	HornOfValere(int iXPos, int iYPos);
	HornOfValere(CvPlot* pkPlot);
	~HornOfValere();

	CvPlot* GetPlot() const;
	int GetX() const;
	int GetY() const;
	int GetTurnsSinceHornBlown() const;
	int GetOwnerPlayerID() const;
	int GetOwnerUnitID() const;
	int GetDiscoveryRange() const;
	bool IsActive() const;
	bool IsFound() const;

	void SetPlot(CvPlot* pkPlot);
	void SetX(int iNewXPos);
	void SetY(int iNewYPos);
	void SetPosition(int iNewXPos, int iNewYPos);
	void SetTurnsSinceHornBlown(int iNewValue);
	void SetOwnerPlayerID(int iNewValue);
	void SetOwnerUnitID(int iNewValue);
	void SetDiscoveryRange(int iNewValue);
	void SetActive(bool bNewValue);
	void SetFound(bool bNewValue);

	void IncrementTurnsSinceHornBlown();

	void DoTurn();

	void FindHorn(CvUnit* pUnit);
	void DropHorn(CvUnit* pUnit);
	void MoveHorn(int iNewXPos, int iNewYPos);
	void MoveHorn(CvPlot* pkNewPlot);
	void TransferHorn(CvUnit* pUnit);

	bool IsHornBlower(CvUnit* pUnit) const;

	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream) const;

protected:
	int m_iXPosition;
	int m_iYPosition;
	int m_iTurnsSinceHornBlown;
	int m_iOwnerPlayerID;
	int m_iOwnerUnitID;

	int m_iDiscoveryRange;

	bool m_bFound;
	bool m_bActive;

	std::vector<CvPlot*> GetNeighboringPlots() const;
};