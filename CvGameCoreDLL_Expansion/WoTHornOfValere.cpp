#include "WoTHornOfValere.h"
#include "CvGlobals.h"
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"

HornOfValere::HornOfValere(): m_bFound(false), m_bActive(false),
	m_iTurnsSinceHornBlown(50000), // arbitrarily large but not enough to overflow
	m_iOwnerPlayerID(-1), m_iOwnerUnitID(-1)
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

int HornOfValere::GetOwnerPlayerID() const
{
	return m_iOwnerPlayerID;
}

int HornOfValere::GetOwnerUnitID() const
{
	return m_iOwnerUnitID;
}

bool HornOfValere::IsActive() const
{
	return m_bActive;
}

bool HornOfValere::IsFound() const
{
	return m_bFound;
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

void HornOfValere::SetOwnerPlayerID(int iNewValue)
{
	m_iOwnerPlayerID = iNewValue;
}

void HornOfValere::SetOwnerUnitID(int iNewValue)
{
	m_iOwnerUnitID = iNewValue;
}

void HornOfValere::IncrementTurnsSinceHornBlown() 
{
	m_iTurnsSinceHornBlown++;
}

void HornOfValere::SetFound(bool bNewValue)
{
	m_bFound = bNewValue;
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
		GetPlot()->SetHasHornOfValere(false);
		m_iOwnerPlayerID = pUnit->getOwner();
		m_iOwnerUnitID = pUnit->GetID();

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

void HornOfValere::DropHorn(CvUnit* pUnit)
{
	if (pUnit && pUnit->GetID() == GetOwnerUnitID() && pUnit->getOwner() == GetOwnerPlayerID())
	{
		SetFound(false);
		SetOwnerPlayerID(-1);
		SetOwnerUnitID(-1);

		MoveHorn(pUnit->plot());
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
		pkPlot->SetHasHornOfValere(false);
	}

	m_iXPosition = pkNewPlot->getX();
	m_iYPosition = pkNewPlot->getY();

	pkNewPlot->SetHasHornOfValere(true);
}

bool HornOfValere::IsHornBlower(CvUnit* pUnit) const
{
	return pUnit && pUnit->GetID() == GetOwnerUnitID() && pUnit->getOwner() == GetOwnerPlayerID();
}

void HornOfValere::Read(FDataStream& kStream)
{
	kStream >> m_iXPosition;
	kStream >> m_iYPosition;
	kStream >> m_iTurnsSinceHornBlown;
	kStream >> m_iOwnerPlayerID;
	kStream >> m_iOwnerUnitID;

	kStream >> m_bFound;
	kStream >> m_bActive;
}

void HornOfValere::Write(FDataStream& kStream) const
{
	kStream << m_iXPosition;
	kStream << m_iYPosition;
	kStream << m_iTurnsSinceHornBlown;
	kStream << m_iOwnerPlayerID;
	kStream << m_iOwnerUnitID;

	kStream << m_bFound;
	kStream << m_bActive;
}