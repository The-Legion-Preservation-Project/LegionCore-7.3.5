-- The Mission - Alliance Pandaria

-- Condition for terrainswap
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=25 AND `SourceEntry`=1066 AND `ConditionValue1`=29548;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(25, 0, 1066, 0, 0, 47, 0, 29548, 8, 0, 0, 0, '', 'Stormwind terrainswap if quest The Mission is in progress'),
(25, 0, 1066, 0, 0, 47, 0, 29548, 64, 0, 1, 0, '', 'Stormwind terrainswap if quest The Mission is NOT rewarded');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=25 AND `SourceEntry`=1066 AND `ConditionValue1`=469;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(25, 0, 1066, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Stormwind terrainswap if player is alliance');

-- Remove Old Terrainswap Data
DELETE FROM FROM `lc_phase_definitions` WHERE `TerrainSwapMap`=1066;