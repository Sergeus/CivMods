ALTER TABLE Civilizations ADD SiegeMod boolean DEFAULT false;

UPDATE Civilizations
SET AIPlayable=false, Playable=false
WHERE SiegeMod=false;