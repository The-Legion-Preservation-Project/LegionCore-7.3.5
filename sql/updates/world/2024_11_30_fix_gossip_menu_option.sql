--
ALTER TABLE `gossip_menu_option`
    DROP `OptionType`,
    CHANGE COLUMN `OptionIcon` `OptionNpc` MEDIUMINT UNSIGNED NOT NULL DEFAULT '0';

-- Drop "npc_experience" script use
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry` IN (35364, 35365);
