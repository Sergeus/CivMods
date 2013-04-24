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

	CvPlot* GetPlot() const;
	int GetX() const;
	int GetY() const;

	void SetPlot(CvPlot* pkPlot);
	void SetX(int iNewXPos);
	void SetY(int iNewYPos);
	void SetPosition(int iNewXPos, int iNewYPos);

	void DoTurn();

	void FindHorn(CvUnit* pUnit);
	void MoveHorn(int iNewXPos, int iNewYPos);
	void MoveHorn(CvPlot* pkNewPlot);

protected:
	CvPlot* m_pkPlot;
	int m_iXPosition;
	int m_iYPosition;
	bool m_bFound;
};