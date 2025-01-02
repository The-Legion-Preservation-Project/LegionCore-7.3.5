SET @ENTRY:=100040;
DELETE FROM `trinity_string` WHERE `entry`=@ENTRY;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(@ENTRY+0, 'This server is running the |cff4CFF00IndividualXp |rmod.', '', '', '', '', '', 'Este servidor está ejecutando el mod |cff4CFF00 IndividualXp.', 'Este servidor está ejecutando el mod |cff4CFF00 IndividualXp.', ''),
(@ENTRY+1, '[XP] The Individual XP module is deactivated.', '', '', '', '', '', '[XP] El módulo XP individual está desactivado.', '[XP] El módulo XP individual está desactivado.', ''),
(@ENTRY+2, '[XP] Your Individual XP is currently disabled. Use .xp enable to re-enable it.', '', '', '', '', '', '[XP] Su XP individual está actualmente desactivado. Utilice .xp enable para volver a activarlo.', '[XP] Su XP individual está actualmente desactivado. Utilice .xp enable para volver a activarlo.', ''),
(@ENTRY+3, '[XP] Your current XP rate is %u.', '', '', '', '', '', '[XP] Su experiencia actual es %u.', '[XP] su experiencia actual es %u.', ''),
(@ENTRY+4, '[XP] The maximum rate limit is %u.', '', '', '', '', '', '[XP] El límite máximo de XP es %u.', '[XP] El límite máximo de XP es %u.', ''),
(@ENTRY+5, '[XP] The minimum rate limit is 1.', '', '', '', '', '', '[XP] El límite mínimo de XP es 1.', '[XP] El límite mínimo de XP es 1.', ''),
(@ENTRY+6, '[XP] You have updated your XP rate to %u.', '', '', '', '', '', '[XP] Has actualizado tu XP a %u', '[XP] Has actualizado tu XP a %u', ''),
(@ENTRY+7, '[XP] You have disabled your XP gain.', '', '', '', '', '', '[XP] Has desactivado tu ganancia de XP.', '[XP] Has desactivado tu ganancia de XP.', ''),
(@ENTRY+8, '[XP] You have enabled your XP gain.', '', '', '', '', '', '[XP] Has activado tu ganancia de XP.', '[XP] Has activado tu ganancia de XP.', ''),
(@ENTRY+9, '[XP] You have restored your XP rate to the default value of %u.', '', '', '', '', '', '[XP] Has restaurado tu tasa de XP al valor por defecto de %u.', '[XP] Has restaurado tu tasa de XP al valor por defecto de %u.', '');
