#include "FDataStream.h"
#include "CvEnums.h"

#pragma once

// ----------------------------------------------------------------
// WoTMod Addition
// ----------------------------------------------------------------
FDataStream & operator>>(FDataStream&, GovernorTypes&);
FDataStream & operator<<(FDataStream&, const GovernorTypes&);
FDataStream & operator>>(FDataStream&, OnePowerTypes&);
FDataStream & operator<<(FDataStream&, const OnePowerTypes&);
FDataStream & operator>>(FDataStream&, WoTMinorCivPlotTypes&);
FDataStream & operator<<(FDataStream&, const WoTMinorCivPlotTypes&);
FDataStream & operator>>(FDataStream&, AjahTypes&);
FDataStream & operator<<(FDataStream&, const AjahTypes&);