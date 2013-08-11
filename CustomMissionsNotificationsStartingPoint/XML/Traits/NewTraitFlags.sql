ALTER TABLE Traits ADD ExtraUnitsWhenTrained integer DEFAULT 0;
ALTER TABLE Traits ADD TradeStopsWars boolean DEFAULT false;
ALTER TABLE Traits ADD ReqTradeRoutesForPeace integer DEFAULT -1;
ALTER TABLE Traits ADD PuppetsReligiousFollowers boolean DEFAULT false;
ALTER TABLE Traits ADD ReligionTakeoverTurns integer DEFAULT -1;