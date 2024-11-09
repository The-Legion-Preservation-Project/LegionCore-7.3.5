-- Hellfire Ramparts
-- Watchkeeper Gargolmar
DELETE FROM `creature_loot_template` WHERE `Entry`=17306 AND (`Chance` = 0 OR `QuestRequired` = 1) AND `LootMode` & 1 = 1 AND `GroupId` IN (0, 1) AND `MinCount`=1;
DELETE FROM `creature_loot_template` WHERE `Entry`=17306 AND `Item` = 27450;

INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`)
VALUES
    (17306, 23881, 100, 1, 3, 1, 1, 1, 0);

INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`)
VALUES
    (17306, 24021, 0, 0,1, 0, 1, 1, 0),
    (17306, 24024, 0, 0,1, 0, 1, 1, 0),
    (17306, 24022, 0, 0,1, 0, 1, 1, 0),
    (17306, 24020, 0, 0,1, 0, 1, 1, 0),
    (17306, 24023, 0, 0,1, 0, 1, 1, 0),
    (17306, 27448, 0, 0,1, 0, 1, 1, 0),
    (17306, 27450, 0, 0,1, 0, 1, 1, 0),
    (17306, 27449, 0, 0,1, 0, 1, 1, 0),
    (17306, 27447, 0, 0,1, 0, 1, 1, 0);

-- Hellfire Ramparts
-- Omor the Unscarred
DELETE FROM `creature_loot_template` WHERE `Entry` = 17308 AND (`Chance` = 0 OR `QuestRequired` = 1) AND `LootMode` & 1 = 1 AND `MinCount`=1;
DELETE FROM `creature_loot_template` WHERE `Entry` = 17308 AND `Item` = 23886;

INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`)
VALUES
    (17308, 23886, 100, 1, 3, 1, 1, 1, 0);

INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`)
VALUES
    (17308, 24090, 0, 0,1, 0, 1, 1, 0),
    (17308, 24073, 0, 0,1, 0, 1, 1, 0),
    (17308, 24094, 0, 0,1, 0, 1, 1, 0),
    (17308, 24096, 0, 0,1, 0, 1, 1, 0),
    (17308, 24091, 0, 0,1, 0, 1, 1, 0),
    (17308, 24069, 0, 0,1, 0, 1, 1, 0);

-- Hellfire Ramparts
-- Reinforced Fel Iron Chest
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 185168 AND (`Chance` = 0 OR `QuestRequired` = 1) AND `LootMode` & 1 = 1 AND `MinCount`=1;
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 185168 AND `Item` = 71636;

INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`)
VALUES
    (185168, 71636, 1.7448, 0, 1, 1, 1, 1);

INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`)
VALUES
    (185168, 24151, 0, 0,1, 0, 1, 1),
    (185168, 24064, 0, 0,1, 0, 1, 1),
    (185168, 24154, 0, 0,1, 0, 1, 1),
    (185168, 24045, 0, 0,1, 0, 1, 1),
    (185168, 24044, 0, 0,1, 0, 1, 1),
    (185168, 24150, 0, 0,1, 0, 1, 1),
    (185168, 24046, 0, 0,1, 0, 1, 1),
    (185168, 24083, 0, 0,1, 0, 1, 1),
    (185168, 24063, 0, 0,1, 0, 1, 1),
    (185168, 24155, 0, 0,1, 0, 1, 1);

-- The Blood Furnace
-- The Maker
DELETE FROM `creature_loot_template` WHERE `Entry` = 17381 AND (`Chance` = 0 OR `QuestRequired` = 1) AND `LootMode` & 1 = 1 AND `MinCount`=1;
DELETE FROM `creature_loot_template` WHERE `Entry` = 17381 AND `Item` = 23886;

INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`)
VALUES
    (17381, 73200, 100, 1, 3, 1, 1, 1, 0),
    (17381, 23894, 100, 1, 3, 2, 1, 1, 0);

INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`)
VALUES
    (17381, 24384, 0, 0,1, 0, 1, 1, 0),
    (17381, 24385, 0, 0,1, 0, 1, 1, 0),
    (17381, 24388, 0, 0,1, 0, 1, 1, 0),
    (17381, 24387, 0, 0,1, 0, 1, 1, 0);

-- Iron Deposit
DELETE FROM `gameobject_loot_template` WHERE `Entry` IN (51300, 51299, 51297, 51303, 51301, 51305, 51306, 51307, 51308, 51309, 51296, 1502, 1503, 51302, 51295);
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES
(51300, 3858, 0, 100, 0, 1, 0, 2, 4, 'Mithril Deposit'),
(51300, 3864, 0, 5, 0, 1, 1, 1, 1, 'Mithril Deposit'),
(51300, 7909, 0, 5, 0, 1, 1, 1, 1, 'Mithril Deposit'),
(51300, 7910, 0, 5, 0, 1, 1, 1, 1, 'Mithril Deposit'),
(51300, 7912, 0, 80, 0, 1, 0, 1, 8, 'Mithril Deposit'),
(51300, 9262, 0, 5, 0, 1, 1, 1, 1, 'Mithril Deposit'),
(51299, 3864, 0, 5, 0, 1, 1, 1, 1, 'Truesilver Deposit'),
(51299, 7909, 0, 5, 0, 1, 1, 1, 1, 'Truesilver Deposit'),
(51299, 7910, 0, 5, 0, 1, 1, 1, 1, 'Truesilver Deposit'),
(51299, 7911, 0, 100, 0, 1, 0, 2, 4, 'Truesilver Deposit'),
(51297, 1529, 0, 5, 0, 1, 1, 1, 1, 'Iron Deposit'),
(51297, 1705, 0, 5, 0, 1, 1, 1, 1, 'Iron Deposit'),
(51297, 2772, 0, 100, 0, 1, 0, 2, 4, 'Iron Deposit'),
(51297, 2838, 0, 100, 0, 1, 0, 1, 5, 'Iron Deposit'),
(51297, 3864, 0, 5, 0, 1, 1, 1, 1, 'Iron Deposit'),
(51297, 7909, 0, 5, 0, 1, 1, 1, 1, 'Iron Deposit'),
(51303, 1, 12904, 5, 0, 1, 0, 1, 1, 'Rich Adamantite Deposit'),
(51303, 2, 12902, 1, 0, 1, 0, 1, 1, 'Rich Adamantite Deposit'),
(51303, 22573, 0, 10, 0, 1, 0, 4, 8, 'Rich Adamantite Deposit'),
(51303, 23425, 0, 100, 0, 1, 0, 5, 7, 'Rich Adamantite Deposit'),
(51303, 23427, 0, 10, 0, 1, 0, 2, 3, 'Rich Adamantite Deposit'),
(51301, 1, 12901, 5, 0, 1, 0, 1, 1, 'Fel iron Deposit'),
(51301, 22573, 0, 10, 0, 1, 0, 1, 2, 'Fel iron Deposit'),
(51301, 22574, 0, 10, 0, 1, 0, 1, 2, 'Fel iron Deposit'),
(51301, 23424, 0, 100, 0, 1, 0, 2, 4, 'Fel iron Deposit'),
(51301, 23427, 0, 5, 0, 1, 0, 1, 2, 'Fel iron Deposit'),
(51305, 1, 12901, 5, 0, 1, 0, 1, 1, 'Khorium Vein'),
(51305, 2, 12902, 1, 0, 1, 0, 1, 1, 'Khorium Vein'),
(51305, 22573, 0, 35, 0, 1, 0, 2, 5, 'Khorium Vein'),
(51305, 22574, 0, 10, 0, 1, 0, 2, 5, 'Khorium Vein'),
(51305, 23426, 0, 100, 0, 1, 0, 2, 4, 'Khorium Vein'),
(51305, 23427, 0, 10, 0, 1, 0, 1, 3, 'Khorium Vein'),
(51306, 1, 12905, 5, 0, 1, 0, 1, 1, 'Cobalt Deposit'),
(51306, 36909, 0, 100, 0, 1, 0, 2, 4, 'Cobalt Deposit'),
(51306, 37701, 0, 30, 0, 1, 0, 1, 2, 'Cobalt Deposit'),
(51306, 37705, 0, 30, 0, 1, 0, 1, 2, 'Cobalt Deposit'),
(51307, 1, 12905, 5, 0, 1, 0, 1, 1, 'Rich Cobalt Deposit'),
(51307, 2, 12906, 5, 0, 1, 0, 1, 1, 'Rich Cobalt Deposit'),
(51307, 36909, 0, 100, 0, 1, 0, 4, 8, 'Rich Cobalt Deposit'),
(51307, 37701, 0, 30, 0, 1, 0, 1, 2, 'Rich Cobalt Deposit'),
(51307, 37705, 0, 30, 0, 1, 0, 1, 2, 'Rich Cobalt Deposit'),
(51308, 1, 12905, 5, 0, 1, 0, 1, 1, 'Saronite Deposit'),
(51308, 36912, 0, 100, 0, 1, 0, 2, 4, 'Saronite Deposit'),
(51308, 37701, 0, 40, 0, 1, 0, 1, 2, 'Saronite Deposit'),
(51308, 37703, 0, 40, 0, 1, 0, 1, 2, 'Saronite Deposit'),
(51309, 1, 12905, 5, 0, 1, 0, 1, 1, 'Rich Saronite Deposit'),
(51309, 2, 12906, 5, 0, 1, 0, 1, 1, 'Rich Saronite Deposit'),
(51309, 36912, 0, 100, 0, 1, 0, 5, 7, 'Rich Saronite Deposit'),
(51309, 37701, 0, 40, 0, 1, 0, 1, 8, 'Rich Saronite Deposit'),
(51309, 37703, 0, 40, 0, 1, 0, 1, 8, 'Rich Saronite Deposit'),
(51296, 1705, 0, 5, 0, 1, 1, 1, 1, 'Gold Vein'),
(51296, 2776, 0, 100, 0, 1, 0, 2, 4, 'Gold Vein'),
(51296, 3864, 0, 5, 0, 1, 1, 1, 1, 'Gold Vein'),
(51296, 7909, 0, 5, 0, 1, 1, 1, 1, 'Gold Vein'),
(1502, 774, 0, 5, 0, 1, 1, 1, 1, 'Copper Vein'),
(1502, 818, 0, 5, 0, 1, 1, 1, 1, 'Copper Vein'),
(1502, 1210, 0, 5, 0, 1, 1, 1, 1, 'Copper Vein'),
(1502, 2770, 0, 100, 0, 1, 0, 1, 4, 'Copper Vein'),
(1502, 2835, 0, 25, 0, 1, 0, 1, 11, 'Copper Vein'),
(1502, 22634, 0, 100, 1, 1, 0, 1, 2, 'Copper Vein'),
(1503, 1206, 0, 5, 0, 1, 1, 1, 1, 'Tin Vein'),
(1503, 1210, 0, 5, 0, 1, 1, 1, 1, 'Tin Vein'),
(1503, 1529, 0, 5, 0, 1, 1, 1, 1, 'Tin Vein'),
(1503, 1705, 0, 5, 0, 1, 1, 1, 1, 'Tin Vein'),
(1503, 2771, 0, 100, 0, 1, 0, 1, 9, 'Tin Vein'),
(1503, 2798, 0, 100, 1, 1, 0, 1, 2, 'Tin Vein'),
(1503, 2836, 0, 25, 0, 1, 0, 1, 13, 'Tin Vein'),
(1503, 22634, 0, 100, 1, 1, 0, 1, 2, 'Tin Vein'),
(51302, 21929, 0, 0.7, 0, 1, 1, 1, 1, 'Adamantite Deposit'),
(51302, 22573, 0, 30, 0, 1, 0, 1, 5, 'Adamantite Deposit'),
(51302, 23077, 0, 0.7, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23079, 0, 0.7, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23107, 0, 0.7, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23112, 0, 0.7, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23117, 0, 0.7, 0, 1, 1, 1, 1, 'Adamantite Deposit'),
(51302, 23425, 0, 100, 0, 1, 0, 1, 4, 'Adamantite Deposit'),
(51302, 23427, 0, 30, 0, 1, 0, 1, 3, 'Adamantite Deposit'),
(51302, 23436, 0, 0.3, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23437, 0, 0.3, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23438, 0, 0.3, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23439, 0, 0.3, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23440, 0, 0.3, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 23441, 0, 0.3, 0, 1, 0, 1, 1, 'Adamantite Deposit'),
(51302, 35229, 0, 40, 1, 1, 0, 1, 2, 'Adamantite Deposit'),
(51295, 1206, 0, 5, 0, 1, 1, 1, 1, 'Silver Vein'),
(51295, 1210, 0, 5, 0, 1, 1, 1, 1, 'Silver Vein'),
(51295, 1705, 0, 5, 0, 1, 1, 1, 1, 'Silver Vein'),
(51295, 2775, 0, 100, 0, 1, 0, 2, 4, 'Silver Vein');
