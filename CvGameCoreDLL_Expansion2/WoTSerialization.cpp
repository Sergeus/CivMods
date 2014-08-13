#include "CvGameCoreDLLPCH.h"
#include "WoTSerialization.h"

#if WOTMOD
FDataStream& operator<<(FDataStream& kStream, const GovernorTypes& egovernorType)
{
	kStream << static_cast<int>(egovernorType);
	return kStream;
}

FDataStream& operator>>(FDataStream& kStream, GovernorTypes& egovernorType)
{
	int igovernorType;
	kStream >> igovernorType;

	egovernorType = static_cast<GovernorTypes>(igovernorType);
	return kStream;
}

FDataStream& operator<<(FDataStream& kStream, const OnePowerTypes& eOnePowerType)
{
	kStream << static_cast<int>(eOnePowerType);
	return kStream;
}

FDataStream& operator>>(FDataStream& kStream, OnePowerTypes& eOnePowerType)
{
	int iOnePowerType;
	kStream >> iOnePowerType;

	eOnePowerType = static_cast<OnePowerTypes>(iOnePowerType);
	return kStream;
}

FDataStream& operator<<(FDataStream& kStream, const WoTMinorCivPlotTypes& eMinorCivPlotType)
{
	kStream << static_cast<int>(eMinorCivPlotType);
	return kStream;
}

FDataStream& operator>>(FDataStream& kStream, WoTMinorCivPlotTypes& eMinorCivPlotType)
{
	int iMinorCivPlotType;
	kStream >> iMinorCivPlotType;

	eMinorCivPlotType = static_cast<WoTMinorCivPlotTypes>(iMinorCivPlotType);
	return kStream;
}

FDataStream& operator<<(FDataStream& kStream, const AjahTypes& eAjah)
{
	kStream << static_cast<int>(eAjah);
	return kStream;
}

FDataStream& operator>>(FDataStream& kStream, AjahTypes& eAjah)
{
	int iAjah;
	kStream >> iAjah;

	eAjah = static_cast<AjahTypes>(iAjah);
	return kStream;
}
#endif // WOTMOD