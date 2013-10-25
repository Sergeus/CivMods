#include "FDataStream.h"
#include "CvEnums.h"

#pragma once

// ----------------------------------------------------------------
// WoTMod Addition
// ----------------------------------------------------------------
FDataStream & operator<<(FDataStream&, const GovernorTypes&);
FDataStream & operator>>(FDataStream&, GovernorTypes&);