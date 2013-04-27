#include "WoTHornOfValere.h"
#include "CvGlobals.h"
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"

HornOfValere::HornOfValere(): m_bFound(false)
{

}

HornOfValere::HornOfValere(int iXPos, int iYPos)
{
	HornOfValere(GC.getMap().plot(iXPos, iYPos));
}

HornOfValere::HornOfValere(CvPlot* pkPlot)
	: m_pkPlot(pkPlot), m_iXPosition(pkPlot->getX()), 
		m_iYPosition(pkPlot->getY()), m_bFound(false),
		m_iTurnsSinceHornBlown(50000) // arbitrarily large but not enough to overflow
{

}

HornOfValere::~HornOfValere()
{
	m_pkPlot = NULL;

	m_iYPosition = 0;
	m_iXPosition = 0;
	m_iTurnsSinceHornBlown = 0;
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

int HornOfValere::GetTurnsSinceHornBlown() const
{
	return m_iTurnsSinceHornBlown;
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

void HornOfValere::SetTurnsSinceHornBlown(int iNewValue)
{
	m_iTurnsSinceHornBlown = iNewValue;
}

void HornOfValere::IncrementTurnsSinceHornBlown() 
{
	m_iTurnsSinceHornBlown++;
}

void HornOfValere::DoTurn() 
{
	IDInfoVector currentUnits; 
	if (!m_bFound && m_pkPlot->getUnits(&currentUnits) > 0)
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

	IncrementTurnsSinceHornBlown();
}

void HornOfValere::FindHorn(CvUnit* pUnit)
{
	// TODO UI popup
	if (pUnit)
	{
		m_bFound = true;
		m_pkPlot->SetHornOfValere(false);

		ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();

		if (pkScriptSystem)
		{
			CvLuaArgsHandle args;
			args->Push(pUnit->getOwner());
			args->Push(pUnit->GetID());

			bool bResult;
			LuaSupport::CallHook(pkScriptSystem, "UnitDiscoveredHornOfValere", args.get(), bResult);
		}
	}
}

void HornOfValere::MoveHorn(int iNewXPos, int iNewYPos)
{
	CvPlot* pkNewPlot = GC.getMap().plot(iNewXPos, iNewYPos);

	MoveHorn(pkNewPlot);
}

void HornOfValere::MoveHorn(CvPlot* pkNewPlot)
{
	m_pkPlot->SetHornOfValere(false);
	
	m_iXPosition = pkNewPlot->getX();
	m_iYPosition = pkNewPlot->getY();

	pkNewPlot->SetHornOfValere(true);

	m_pkPlot = pkNewPlot;
}