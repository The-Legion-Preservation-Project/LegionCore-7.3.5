-- Add correct phase to rell nightwind. Prev had no phase. 
UPDATE `creature` SET `PhaseId`=1687 WHERE `guid`=369582 AND `id`=55789;

-- Phase Area
DELETE FROM `phase_area` WHERE `AreaId`=1519 AND `PhaseId`=1687;
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(1519, 1687, 'Cosmetic - See Rell Nightwind');

-- Conditions
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 26 AND `SourceGroup` = 1687 AND `SourceEntry` = 1519);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(26, 1687, 1519, 0, 0, 47, 0, 29547, 66, 0, 0, 'Allow Phase 1687 if Quest (29547) IS completed or Rewarded');
