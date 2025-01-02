CREATE TABLE IF NOT EXISTS `individual_xp` (
  `CharacterGUID` int(11) NOT NULL,
  `Rate` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`CharacterGUID`)
  );