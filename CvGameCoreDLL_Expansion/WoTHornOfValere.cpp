#include "WoTHornOfValere.h"
#include "CvGlobals.h"
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"

HornOfValere::HornOfValere(): m_bFound(false), m_bActive(false),
	m_iTurnsSinceHornBlown(50000), // arbitrarily large but not enough to overflow
	m_iOwnerPlayerID(-1), m_iOwnerUnitID(-1),
	m_iDiscoveryRange(1)
{

}

HornOfValere::HornOfValere(int iXPos, int iYPos)
	: m_iXPosition(iXPos), m_iYPosition(iYPos)
{
	HornOfValere();
}

HornOfValere::HornOfValere(CvPlot* pkPlot)
{
	HornOfValere(pkPlot->getX(), pkPlot->getY());
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

int HornOfValere::GetDiscoveryRange() const
{
	return m_iDiscoveryRange;
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

void HornOfValere::SetDiscoveryRange(int iNewValue)
{
	m_iDiscoveryRange = iNewValue;
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
	if (!m_bFound)
	{
		for(int iX = -m_iDiscoveryRange; iX <= m_iDiscoveryRange; iX++)
		{
			for(int iY = -m_iDiscoveryRange; iY <= m_iDiscoveryRange; iY++)
			{
				CvPlot* pkPlot = plotXYWithRangeCheck(GetX(), GetY(), iX, iY, m_iDiscoveryRange);

				if(pkPlot != NULL)
				{
					IDInfoVector currentUnits;
					if (pkPlot->getUnits(&currentUnits) > 0)
					{
						for (IDInfoVector::const_iterator itr = currentUnits.begin(); itr != currentUnits.end(); ++itr)
						{
							CvUnit* pUnit = ::getUnit(*itr);

							if(pUnit && pUnit->CanDiscoverHornOfValere())
							{
								FindHorn(pUnit);
								return;
							}
						}
					}
				}
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
		CvPlot* pkPlot = GetPlot();
		if (pkPlot)
		{
			pkPlot->SetHasHornOfValere(false);
		}
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

		GET_PLAYER((PlayerTypes)pUnit->getOwner()).GetNotifications()->Add(NotificationTypes::NUM_NOTIFICATION_TYPES,
			GetLocalizedText("TXT_KEY_HORN_DISCOVERED", pUnit->getNameKey()), 
			GetLocalizedText("TXT_KEY_HORN_DISCOVERED", pUnit->getNameKey()),
			pUnit->getX(), pUnit->getY(), pUnit->GetID(), pUnit->getOwner());
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

void HornOfValere::TransferHorn(CvUnit* pUnit)
{
	// TODO UI popup and notification
	if (m_bFound && pUnit)
	{
		m_iOwnerPlayerID = pUnit->getOwner();
		m_iOwnerUnitID = pUnit->GetID();

		FindHorn(pUnit);
	}
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

	kStream >> m_iDiscoveryRange;

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

	kStream << m_iDiscoveryRange;

	kStream << m_bFound;
	kStream << m_bActive;
}