DELETE FROM `areatrigger_scripts` WHERE `ScriptName` IN ('at_twiggy_flathead','at_madrigosa','at_eye_of_eternity_improvised_floor');

UPDATE `gameobject_template` SET `ScriptName`='' WHERE `ScriptName` IN ('go_defias_cannon','go_door_lever_dm','go_kael_orb','go_movie_orb');

UPDATE `item_template` SET `ScriptName`='' WHERE `ScriptName`='internalitemhandler';

UPDATE `creature_template` SET `ScriptName`='' WHERE `ScriptName` IN ('do_nothing','npc_aran_blizzard','npc_bladespire_ogre','npc_blaze','npc_crystalline_tangler','npc_demon_fire','npc_flame_crash','npc_generic_harpoon_cannon','npc_homunculus','npc_invis_legion_teleporter','npc_magnetic_core','npc_mindless_skeleton','npc_nether_vapor','npc_novos_minion','npc_sliver','npc_thuzadin_acolyte','npc_tracy_proudwell','npc_vereth_the_cunning','npc_void_zone','npc_yauj_brood');