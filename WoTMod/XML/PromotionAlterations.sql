ALTER TABLE UnitPromotions ADD TurnDamage integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD BlocksHealing boolean DEFAULT false;
ALTER TABLE UnitPromotions ADD AdjacentEnemyDamage integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD RangedAttackSelfDamageChance integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD OnResearchCombatModifier integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD OnResearchRangedCombatModifier integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD OnResearchModifiersDuration integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD BondsWardersChange integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD WoundedDamageModifier integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD DetectionRange integer DEFAULT 0;
ALTER TABLE UnitPromotions ADD CannotBondWarders boolean DEFAULT false;
ALTER TABLE UnitPromotions ADD AllowGiftToTower boolean DEFAULT false;
ALTER TABLE UnitPromotions ADD CanOnlyAttackThreatening boolean DEFAULT false;
