ALTER TABLE Improvements ADD ExplodesOnEnemyEnterHex boolean DEFAULT false;
ALTER TABLE Improvements ADD ExplosionDamageToThisHex integer DEFAULT 0;
ALTER TABLE Improvements ADD ExplosionDamageToOuterHexes integer DEFAULT 0;
ALTER TABLE Improvements ADD ExplosionDamageRange integer DEFAULT 0;
ALTER TABLE Improvements ADD ExplosionDestroyedAfter boolean DEFAULT false;
ALTER TABLE Improvements ADD ExplosionPillagedAfter boolean DEFAULT false;