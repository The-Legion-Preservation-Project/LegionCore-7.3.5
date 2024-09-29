-- Fix creature levels in Stormwind Stockade
-- 1708 = Defias Inmate
-- 1720 = Bruegal Ironknuckle
-- 46248 = Riverpaw Basher
-- 46249 = Riverpaw Slayer
-- 46250 = Riverpaw Poacher
-- 46251 = Riverpaw Looter
-- 46252 = Riverpaw Shaman
-- 46254 = Hogger
-- 46260 = Searing Destroyer
-- 46261 = Enraged Fire Elemental
-- 46262 = Rumbling Earth
-- 46263 = Slag Fury
-- 46264 = Lord Overheat
-- 46375 = Rowdy Troublemaker
-- 46379 = Vicious Thug
-- 46381 = Shifty Thief
-- 46382 = Petty Criminal
-- 46383 = Randolph Moloch
UPDATE `creature_template` SET `minlevel` = 20, `maxlevel` = 20, `ScaleLevelMin` = 20, `ScaleLevelMax` = 60 WHERE `entry` IN (1708, 1720, 46248, 46249, 46250, 46251, 46252, 46254, 46260, 46261, 46262, 46263, 46264, 46375, 46379, 46381, 46382, 46383);

-- Fix creature levels in Gnomeregan
-- 6206 = Caverndeep Burrower
-- 6207 = Caverndeep Ambusher
-- 6212 = Dark Iron Agent
-- 6215 = Chomper
-- 6218 = Irradiated Slime
-- 6219 = Corrosive Lurker
-- 6220 = Irradiated Horror
-- 6222 = Leprous Technician
-- 6223 = Leprous Defender
-- 6225 = Mechano-Tank
-- 6229 = Crowd Pummeler 9-60
-- 6230 = Peacekeeper Security Suit
-- 6232 = Arcane Nullifier X-21
-- 6233 = Mechanized Sentry
-- 6234 = Mechanized Guardian
-- 6235 = Electrocutioner 6000
-- 6329 = Irradiated Pillager
-- 7079 = Viscous Fallout
-- 7361 = Grubbis
-- 7603 = Leprous Assistant
-- 7800 = Mekgineer Thermaplugg
-- 7849 = Mobile Alert System
UPDATE `creature_template` SET `minlevel` = 24, `maxlevel` = 24, `ScaleLevelMin` = 24, `ScaleLevelMax` = 60 WHERE `entry` IN (6206, 6207, 6212, 6215, 6218, 6219, 6220, 6222, 6223, 6225, 6229, 6230, 6232, 6233, 6234, 6235, 6329, 7079, 7361, 7603, 7800, 7849);

-- Fix creature levels in Scarlet Halls
-- 58632 = Armsmaster Harlan
-- 58676 = Scarlet Defender
-- 58683 = Scarlet Myrmidon
-- 58684 = Scarlet Scourge Hewer
-- 58685 = Scarlet Evangelist
-- 58756 = Scarlet Evoker
-- 58876 = Starving Hound
-- 58898 = Vigilant Watchman
-- 59150 = Flameweaver Koegler
-- 59175 = Master Archer
-- 59191 = Commander Lindon
-- 59240 = Scarlet Hall Guardian
-- 59241 = Scarlet Treasurer
-- 59293 = Scarlet Cannoneer
-- 59299 = Scarlet Guardian
-- 59302 = Sergeant Verdone
-- 59303 = Houndmaster Braun
-- 59309 = Obedient Hound
-- 59372 = Scarlet Scholar
-- 59373 = Scarlet Pupil
-- TODO For heroic may want to change these later to 29, 90, 26, 90 for values!
UPDATE `creature_template` SET `minlevel` = 26, `maxlevel` = 26, `ScaleLevelMin` = 26, `ScaleLevelMax` = 60 WHERE `entry` IN (58632, 58676, 58683, 58684, 58685, 58756, 58876, 58898, 59150, 59175, 59191, 59240, 59241, 59299, 59302, 59303, 59309, 59372, 59373);
