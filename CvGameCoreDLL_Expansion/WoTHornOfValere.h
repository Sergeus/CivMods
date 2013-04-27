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

	void SetPlot(CvPlot* pkPlot);
	void SetX(int iNewXPos);
	void SetY(int iNewYPos);
	void SetPosition(int iNewXPos, int iNewYPos);
	void SetTurnsSinceHornBlown(int iNewValue);

	void IncrementTurnsSinceHornBlown();

	void DoTurn();

	void FindHorn(CvUnit* pUnit);
	void MoveHorn(int iNewXPos, int iNewYPos);
	void MoveHorn(CvPlot* pkNewPlot);

	bool CanFindHornOfValere(CvUnit* pUnit);

protected:
	CvPlot* m_pkPlot;
	int m_iXPosition;
	int m_iYPosition;
	bool m_bFound;
	int m_iTurnsSinceHornBlown;
};