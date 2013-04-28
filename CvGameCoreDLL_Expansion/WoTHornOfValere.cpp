#include "WoTHornOfValere.h"
#include "CvGlobals.h"
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"

HornOfValere::HornOfValere(): m_bFound(false), m_bActive(false),
	m_iTurnsSinceHornBlown(50000) // arbitrarily large but not enough to overflow
{

}

HornOfValere::HornOfValere(int iXPos, int iYPos)
{
	HornOfValere(GC.getMap().plot(iXPos, iYPos));
}

HornOfValere::HornOfValere(CvPlot* pkPlot)
	: m_iXPosition(pkPlot->getX()), 
		m_iYPosition(pkPlot->getY())
{
	HornOfValere();
}

HornOfValere::~HornOfValere()
{
	m_iYPosition = -1;
	m_iXPosition = -1;
	m_iTurnsSinceHornBlown = 0;
}

void HornOfValere::Init()
{
	if (IsActive())
	{
		if (!m_bFound)
		{
			CvPlot* pkPlot = GetPlot();
			if (pkPlot)
			{
				pkPlot->SetHornOfValere(true);
			}
		}
		GC.getMap().SetHasHornOfValere(true);
	}
}

CvPlot* HornOfValere::GetPlot() const
{
	return GC.getMap().plot(m_iXPosition, m_iYPosition);
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

bool HornOfValere::IsActive() const
{
	return m_bActive;
}

void HornOfValere::SetActive(bool bNewValue)
{
	m_bActive = bNewValue;
}

void HornOfValere::SetPlot(CvPlot* pkPlot)
{
	MoveHorn(pkPlot);
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
	CvPlot* pkPlot = GetPlot();
	if (!m_bFound && pkPlot && pkPlot->getUnits(&currentUnits) > 0)
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
		GetPlot()->SetHornOfValere(false);

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
	CvPlot* pkPlot = GetPlot();
	if (pkPlot)
	{
		pkPlot->SetHornOfValere(false);
	}

	m_iXPosition = pkNewPlot->getX();
	m_iYPosition = pkNewPlot->getY();

	pkNewPlot->SetHornOfValere(true);
}

void HornOfValere::Read(FDataStream& kStream)
{
	kStream >> m_iXPosition;
	kStream >> m_iYPosition;

	kStream >> m_bFound;
	kStream >> m_bActive;
	kStream >> m_iTurnsSinceHornBlown;

	Init();
}

void HornOfValere::Write(FDataStream& kStream) const
{
	kStream << m_iXPosition;
	kStream << m_iYPosition;

	kStream << m_bFound;
	kStream << m_bActive;
	kStream << m_iTurnsSinceHornBlown;
}