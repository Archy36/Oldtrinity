-- ICC fixs...

   -- update immunity
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36612', '37957', '37958', '37959'); -- Lod Mark'gar
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36855', '38106', '38296', '38297'); -- Lady Deathwhisper
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('37813', '38402', '38582', '38583'); -- Deathbringer Saurfang
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36627', '38390', '38549', '38550'); -- Rotface
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36626', '37504', '37505', '37506'); -- Festergut
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36678', '38431', '38585', '38586'); -- Professor Putricide
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('37562', '38602', '38760', '38761'); -- Gas Cloud
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('37697', '38604', '38758', '38759'); -- Volatile Ooze
   UPDATE `creature_template` SET `mechanic_immune_mask` = 617299803 where entry IN ('36980', '38320', '38321', '38322'); -- Ice Tombs / Sindragosa

-- Update ICC: Rotface
-- Fix targetting for Ooze Flood ability in encounter Modermiene / Rotface in instance / Instanz ICC / Eiskronenzitadelle / Icecrown Citadel
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 3 AND `SourceEntry` IN (69783, 69797, 69799, 69802) AND `ConditionTypeOrReference` = 33;
INSERT INTO `conditions` (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorTextId, ScriptName, Comment) VALUES
(13, 3, 69783, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self'),
(13, 3, 69797, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self'),
(13, 3, 69799, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self'),
(13, 3, 69802, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self');
-- Add immunities to Little Ooze / Big Ooze in encounter Modermiene / Rotface in instance / Instanz ICC / Eiskronenzitadelle / Icecrown Citadel
UPDATE `creature_template` SET `mechanic_immune_mask` = 650853247 WHERE `entry` IN (36897, 36899, 38138, 38123);
- Add interrupt immunity to mini bosses (Rimefang / Raufang / Spinestalker / Wirbelpirscher) in encounter Sindragosa in instance / Instanz ICC / Eiskronenzitadelle / Icecrown Citadel
UPDATE `creature_template` SET `mechanic_immune_mask` = 650853247 WHERE `entry` IN (37533, 38220, 37534, 38219);
-- DB/NPCs: Little Ooze (Rotface in ICC) can not be taunted, 2012_09_10_06_world_creature_template.sql
UPDATE `creature_template` SET `flags_extra`=`flags_extra`|256 WHERE `entry` IN (36897, 38138); -- Little Ooze
-- Set speed values to database for Valkyr Shadowguard in encounter The Lich King in instance / Instanz ICC / Eiskronenzitadelle / Icecrown Citadel
UPDATE `creature_template` SET `speed_walk` = 0.642857, `speed_run` = 0.642857, `InhabitType` = 4 WHERE `entry` IN (36609, 39120, 39121, 39122);
-- Set speed values to database for Vile Spirits in encounter The Lich King in instance / Instanz ICC / Eiskronenzitadelle / Icecrown Citadel
UPDATE `creature_template` SET `speed_walk` = 0.5, `speed_run` = 0.5, `InhabitType` = 4 WHERE `entry` IN (37799, 39284, 39285, 39286);
