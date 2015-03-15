ALTER TABLE Builds ADD Happiness integer DEFAULT 0;
ALTER TABLE Builds ADD OwnTerritoryMakesValid boolean DEFAULT false;
ALTER TABLE Units ADD BondBreakRecoversWithTime boolean DEFAULT false;
ALTER TABLE Units ADD BondBreakRecoveryDuration integer DEFAULT 0;
ALTER TABLE Units ADD BondBreakRecoversWhenBonded boolean DEFAULT false;