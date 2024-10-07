ALTER TABLE `character_pet` CHANGE `slot` `slot` smallint NOT NULL DEFAULT '-1';

UPDATE `character_pet` SET `slot` = -1 WHERE `slot` = 100;

ALTER TABLE `characters` DROP `stable_slots`;

ALTER TABLE `characters` ADD `summonedPetNumber` int unsigned NOT NULL DEFAULT '0' AFTER `extra_flags`;

UPDATE `characters` SET `summonedPetNumber` = COALESCE((SELECT `id` FROM `character_pet` WHERE `owner` = `guid` AND `slot` = 0 AND `curhealth` > 0), 0);