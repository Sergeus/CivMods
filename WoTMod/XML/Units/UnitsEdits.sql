ALTER TABLE Builds ADD Happiness integer DEFAULT 0;
ALTER TABLE Builds ADD OwnTerritoryMakesValid boolean DEFAULT false;

ALTER TABLE Units ADD BondsWarders boolean DEFAULT false;