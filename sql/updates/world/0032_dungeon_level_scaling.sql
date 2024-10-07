-- the deadmines
update creature_template set ScaleLevelMin = 15, ScaleLevelMax = 60, ScaleLevelDelta = 0
where entry IN (47404, 48230, 48262, 48279, 48280, 48338, 48351, 48417, 48418, 48421, 48439, 48445, 48502, 48505, 48521, 48522);

update creature_template set ScaleLevelMin = 15, ScaleLevelMax = 60, ScaleLevelDelta = -1
where entry IN (48229, 48278, 48284, 48419, 48420, 48440, 48441, 48442);

update creature_template set ScaleLevelMin = 15, ScaleLevelMax = 60, ScaleLevelDelta = 1
where entry IN (1732, 43778, 47162, 47296, 47626, 47739);

