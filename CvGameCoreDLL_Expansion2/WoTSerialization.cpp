#include "CvGameCoreDLLPCH.h"
#include "WoTSerialization.h"

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
