#include "WoTHornOfValere.h"
#include "CvGlobals.h"
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"

HornOfValere::HornOfValere(): m_bFound(false)
{

}

HornOfValere::HornOfValere(int iXPos, int iYPos)
	: m_iXPosition(iXPos), m_iYPosition(iYPos), m_bFound(false)
{
	m_pkPlot = GC.getMap().plot(iXPos, iYPos);
}

HornOfValere::HornOfValere(CvPlot* pkPlot)
	: m_pkPlot(pkPlot), m_iXPosition(pkPlot->getX()), 
		m_iYPosition(pkPlot->getY()), m_bFound(false)
{

}

CvPlot* HornOfValere::GetPlot() const
{
	return m_pkPlot;
}

int HornOfValere::GetX() const
{
	return m_iXPosition;
}

int HornOfValere::GetY() const
{
	return m_iYPosition;
}

void HornOfValere::SetPlot(CvPlot* pkPlot)
{
	m_pkPlot = pkPlot;
}

void HornOfValere::SetX(int iNewXPos)
{
	m_iXPosition = iNewXPos;
}

void HornOfValere::SetY(int iNewYPos)
{
	m_iYPosition = iNewYPos;
}

void HornOfValere::SetPosition(int iNewXPos, int iNewYPos)
{
	SetX(iNewXPos);
	SetY(iNewYPos);
}

void HornOfValere::DoTurn() 
{
	IDInfoVector currentUnits; 
	if (m_pkPlot->getUnits(&currentUnits) > 0)
	{
		for (IDInfoVector::const_iterator itr = currentUnits.begin(); itr != currentUnits.end(); ++itr)
		{
			CvUnit* pUnit = ::getUnit(*itr);

			if(pUnit && pUnit->CanDiscoverHornOfValere())
			{
				FindHorn(pUnit);
			}
		}
	}
}

void HornOfValere::FindHorn(CvUnit* pUnit)
{
	// TODO UI popup, actual stuff that's useful
	m_bFound = true;
	pUnit->kill(false);
}