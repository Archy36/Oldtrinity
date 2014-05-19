-- Childrens week quests hack-fixes
-- Blood Elf Orphan SAI
SET @ENTRY := 22817;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,10953,0,0,0,15,10953,0,0,0,0,0,7,0,0,0,0,0,0,0,"Blood Elf Orphan - On Quest 'Visit the Throne of the Elements' Taken - Quest Credit 'Visit the Throne of the Elements'"),
(@ENTRY,0,1,0,19,0,100,0,10963,0,0,0,15,10963,0,0,0,0,0,7,0,0,0,0,0,0,0,"Blood Elf Orphan - On Quest 'Time to Visit the Caverns' Taken - Quest Credit 'Time to Visit the Caverns'"),
(@ENTRY,0,2,0,19,0,100,0,11975,0,0,0,15,11975,0,0,0,0,0,7,0,0,0,0,0,0,0,"Blood Elf Orphan - On Quest 'Now, When I Grow Up...' Taken - Quest Credit 'Now, When I Grow Up...'"),
(@ENTRY,0,3,0,19,0,100,0,10945,0,0,0,15,10945,0,0,0,0,0,7,0,0,0,0,0,0,0,"Blood Elf Orphan - On Quest 'Hch'uu and the Mushroom People' Taken - Quest Credit 'Hch'uu and the Mushroom People'");
-- Draenei Orphan SAI
SET @ENTRY := 22818;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,10950,0,0,0,15,10950,0,0,0,0,0,7,0,0,0,0,0,0,0,"Draenei Orphan - On Quest 'Auchindoun and the Ring of Observance' Taken - Quest Credit 'Auchindoun and the Ring of Observance'"),
(@ENTRY,0,1,0,19,0,100,0,10954,0,0,0,15,10954,0,0,0,0,0,7,0,0,0,0,0,0,0,"Draenei Orphan - On Quest 'Jheel is at Aeris Landing!' Taken - Quest Credit 'Jheel is at Aeris Landing!'"),
(@ENTRY,0,2,0,19,0,100,0,10962,0,0,0,15,10962,0,0,0,0,0,7,0,0,0,0,0,0,0,"Draenei Orphan - On Quest 'Time to Visit the Caverns' Taken - Quest Credit 'Time to Visit the Caverns'"),
(@ENTRY,0,3,0,19,0,100,0,10956,0,0,0,15,10956,0,0,0,0,0,7,0,0,0,0,0,0,0,"Draenei Orphan - On Quest 'The Seat of the Naaru' Taken - Quest Credit 'The Seat of the Naaru'");
-- Farseer Nobundo SAI
SET @ENTRY := 17204;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,62,0,100,0,7377,1,0,0,15,10968,0,0,0,0,0,7,0,0,0,0,0,0,0,"Farseer Nobundo - On Gossip Option 1 Selected - Quest Credit 'Call on the Farseer'");
-- Orphan Matron Aria SAI
SET @ENTRY := 34365;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES(34365, 0, 1, 4, 62, 0, 100, 0, 10502, 1, 0, 0, 11, 65374, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orphan Matron Aria - On Gossip Option 1 Selected - Cast ''Matron Aria Credit''');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES(34365, 0, 2, 4, 62, 0, 100, 0, 10502, 2, 0, 0, 11, 65359, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orphan Matron Aria - On Gossip Option 2 Selected - Cast ''Oracle Orphan Whistle''');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES(34365, 0, 3, 4, 62, 0, 100, 0, 10502, 3, 0, 0, 11, 65360, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orphan Matron Aria - On Gossip Option 3 Selected - Cast ''Wolvar Orphan Whistle''');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES(34365, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orphan Matron Aria - On Gossip Option 3 Selected - Close Gossip');
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,5,0,20,0,100,0,13959,0,0,0,56,46545,1,0,0,0,0,7,0,0,0,0,0,0,0,"Orphan Matron Aria - On Quest 'Back To The Orphanage' Finished - Add Item 'Curious Oracle Hatchling' 1 Time"),
(@ENTRY,0,6,0,20,0,100,0,13960,0,0,0,56,46544,1,0,0,0,0,7,0,0,0,0,0,0,0,"Orphan Matron Aria - On Quest 'Back To The Orphanage' Finished - Add Item 'Curious Wolvar Pup' 1 Time");
-- Oracle Orphan SAI
SET @ENTRY := 33533;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,65357,0,0,0,33,36209,0,0,0,0,0,7,0,0,0,0,0,0,0,"Oracle Orphan - On Spellhit 'Throw Paper Zeppelin' - Quest Credit 'A Trip To The Wonderworks'"),
(@ENTRY,0,1,0,19,0,100,0,13929,0,0,0,15,13929,0,0,0,0,0,7,0,0,0,0,0,0,0,"Oracle Orphan - On Quest 'The Biggest Tree Ever!' Taken - Quest Credit 'The Biggest Tree Ever!'");
-- Wolvar Orphan SAI
SET @ENTRY := 33532;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,65357,0,0,0,33,36209,0,0,0,0,0,7,0,0,0,0,0,0,0,"Wolvar Orphan - On Spellhit 'Throw Paper Zeppelin' - Quest Credit 'A Trip To The Wonderworks'"),
(@ENTRY,0,1,0,19,0,100,0,13930,0,0,0,15,13930,0,0,0,0,0,7,0,0,0,0,0,0,0,"Wolvar Orphan - On Quest 'Home Of The Bear-Men' Taken - Quest Credit 'Home Of The Bear-Men'");