SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;


UPDATE `creature_loot_template` SET `entry` = 39947,`item` = 49426,`ChanceOrQuestChance` = 100,`lootmode` = 1,`groupid` = 0,`mincountOrRef` = 1,`maxcount` = 1 WHERE `creature_loot_template`.`entry` = 39947 AND `creature_loot_template`.`item` = 49426;
UPDATE `creature_loot_template` SET `entry` = 39948,`item` = 49426,`ChanceOrQuestChance` = 100,`lootmode` = 1,`groupid` = 0,`mincountOrRef` = 1,`maxcount` = 1 WHERE `creature_loot_template`.`entry` = 39948 AND `creature_loot_template`.`item` = 49426;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

10g 39747 entry loot

UPDATE `creature_template` SET `lootid`=39947 WHERE `entry`=39747;

UPDATE `creature_template` SET `lootid`=39948 WHERE `entry`=39823;

65126