#include "ScriptPCH.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "WorldSession.h"
#include "Chat.h"
#include "CreatureTextMgr.h"
#include "Language.h"
#include "ObjectMgr.h"

#define MSG_GOSSIP_TEXT_ALCHEMY "Изучить Алхимию."
#define MSG_GOSSIP_TEXT_BLACKSMITHING "Изучить Кузнечное Дело."
#define MSG_GOSSIP_TEXT_ENCNANTING "Изучить Наложение Чар."
#define MSG_GOSSIP_TEXT_ENGINEERING "Изучить Инженерное дело."
#define MSG_GOSSIP_TEXT_HERBALISM "Изучить Травничество."
#define MSG_GOSSIP_TEXT_INSCRIPTION "Изучить Начертание."
#define MSG_GOSSIP_TEXT_JEWELCRAFTING "Изучить Ювелирное дело."
#define MSG_GOSSIP_TEXT_LEATHERWORKING "Изучить Кожевничество."
#define MSG_GOSSIP_TEXT_MINING "Изучить Горное дело."
#define MSG_GOSSIP_TEXT_SKINNING "Изучить Снятие Шкур."
#define MSG_GOSSIP_TEXT_TAILORING "Изучить Портняжное дело."
#define MSG_GOSSIP_READY "[Готово...]"

//head, shoulder, chest, gloves, legs
uint32 pvpset[16][5] =
{
    { 40826, 40866, 40789, 40807, 40847 }, //warrior 0
    { 40828, 40869, 40788, 40808, 40849 }, //ret pally 1
    { 40933, 40963, 40907, 40927, 40939 }, //holy pally 2
    { 41678, 41715, 41661, 41773, 41667 }, //feral dru 3
    { 41327, 41281, 41316, 41293, 41304 }, //moonkin dru 4
    { 41321, 41275, 41310, 41287, 41298 }, //restore dru 5
    { 41854, 41869, 41859, 41874, 41864 }, //healer priest 6
    { 41915, 41934, 41921, 41940, 41927 }, //shadow priest 7
    { 41019, 41044, 40993, 41007, 41033 }, //elem shammy 8
    { 41013, 41038, 40992, 41001, 41027 }, //restore shammy 9
    { 41151, 41211, 41081, 41137, 41199 }, //enh shammy 10
    { 41157, 41217, 41087, 41143, 41205 }, //hunter 11
    { 41993, 42011, 41998, 42017, 42005 }, //warlock 12
    { 41946, 41965, 41953, 41971, 41959 }, //mage 13
    { 41672, 41683, 41650, 41767, 41655 }, //rogue 14
    { 40827, 40868, 40787, 40809, 40848 } //dk 15
};

//bracer, waist, feet, ringa, ringb
uint32 offset[11][5] =
{
    { 51364, 51362, 51363, 51358, 42119 }, //plate melee 0
    { 51361, 51359, 51360, 51336, 42118 }, //plate heal 1
    { 51329, 51327, 51328, 51336, 42118 }, //cloth crit 2
    { 51367, 51365, 51366, 51336, 42118 }, //cloth spirit 3
    { 51339, 51337, 51338, 51336, 42118 }, //cloth haste 4
    { 51376, 51374, 51375, 51336, 42118 }, //mail caster spd 5
    { 51373, 51371, 51372, 51336, 42118 }, //mail caster mp5 6
    { 51352, 51350, 51351, 51358, 42119 }, //mail agility 7
    { 51345, 51343, 51344, 51336, 42118 }, //leather caster crit 8
    { 51342, 51340, 51341, 51336, 42118 }, //leather caster spirit 9
    { 51370, 51368, 51369, 51358, 42119 }, //leather melee 10
};
uint32 badge = 47241; //Emblem of triumph

uint32 factionset[6] = { 29468, 25473, 51377, 29469, 25533, 51378 }; //horse,gryphon,amedallion; wolf,windrider,hmedallion 

uint32 backmelee[2] = { 51354, 51356 }; //crit,hit
uint32 backcaster[5] = { 51334, 51348, 51330, 51346, 51332 }; //hit,spirit,crit,mp5,haste
uint32 neckmelee[3] = { 51353, 51355, 51357 }; //arp,crit,hit
uint32 neckcaster[5] = { 51335, 51349, 51331, 51347, 51333 }; //hit,spirit,crit,mp5,haste
uint32 battlemaster[5] = { 42133, 42134, 42136, 42137, 42135 }; //crit,hit,ap,spd,haste

uint32 books[4] = { 51407, 51396, 51408, 51409 }; //haste,crit,penetration,spirit
uint32 shields[2] = { 51452, 51455 }; //crit,mp5
uint32 ranged[3] = { 47659, 47658, 47660 }; //agi,wand,tank
uint32 weapon[3] = { 51384, 51011, 50776 }; //dagger 1&2, bow

enum spells
{
    SPELL_LEARN_INSCRIPTION = 65287,
    SPELL_LEARN_JEWELCRAFTING = 65286,
    SPELL_LEARN_BLACKSMITHING = 65282,
    SPELL_LEARN_LEATHERWORKING = 65284,
    SPELL_LEARN_ALCHEMY = 65281,
    SPELL_LEARN_TAILORING = 65283,
    SPELL_LEARN_ENGINEERING = 61464,
    SPELL_LEARN_MINING = 65289,
    SPELL_LEARN_HERBALISM = 65288,
    SPELL_LEARN_SKINNING = 65290,
    SPELL_LEARN_ENCHANTING = 65285
};

class npc_transfer : public CreatureScript
{
public:
    npc_transfer() : CreatureScript("npc_transfer") { }

    void GiveSet(Player *player, int i, bool delonly = false)
    {
        int j;
        for (j = 0; j < 5; ++j)
        {
            player->DestroyItemCount(pvpset[i][j], 1, true);
            if (!delonly)
                player->AddItem(pvpset[i][j], 1);
        }
    }

    bool HasFreeSpace(Player* player, uint32 itemID)
    {
        ItemPosCountVec dest;
        uint32 noSpaceForCount = 0;
        uint32 count = 25;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemID, count, &noSpaceForCount);
        if (msg != EQUIP_ERR_OK)
            count -= noSpaceForCount;
        if (count < 25 || dest.empty())
            return false;
        else
            return true;
    }

    void LearnSkills(Player *player)
    {
        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            player->LearnSpell(750, false);
            player->LearnSpell(196, false);
            player->LearnSpell(197, false);
            player->LearnSpell(198, false);
            player->LearnSpell(199, false);
            player->LearnSpell(200, false);
            player->LearnSpell(201, false);
            player->LearnSpell(202, false);
            player->LearnSpell(264, false);
            player->LearnSpell(5011, false);
            player->LearnSpell(266, false);
            player->LearnSpell(2567, false);
            player->LearnSpell(1180, false);
            player->LearnSpell(15590, false);
            break;
        case CLASS_PALADIN:
            player->LearnSpell(750, false);
            player->LearnSpell(196, false);
            player->LearnSpell(197, false);
            player->LearnSpell(198, false);
            player->LearnSpell(199, false);
            player->LearnSpell(200, false);
            player->LearnSpell(201, false);
            player->LearnSpell(202, false);
            break;
        case CLASS_DRUID:
            player->LearnSpell(198, false);
            player->LearnSpell(199, false);
            player->LearnSpell(200, false);
            player->LearnSpell(227, false);
            player->LearnSpell(15590, false);
            break;
        case CLASS_PRIEST:
            player->LearnSpell(198, false);
            player->LearnSpell(227, false);
            player->LearnSpell(1180, false);
            player->LearnSpell(5009, false);
            break;
        case CLASS_SHAMAN:
            player->LearnSpell(8737, false);
            player->LearnSpell(196, false);
            player->LearnSpell(197, false);
            player->LearnSpell(198, false);
            player->LearnSpell(199, false);
            player->LearnSpell(227, false);
            player->LearnSpell(1180, false);
            player->LearnSpell(15590, false);
            break;
        case CLASS_HUNTER:
            player->LearnSpell(15590, false);
            player->LearnSpell(8737, false);
            player->LearnSpell(196, false);
            player->LearnSpell(197, false);
            player->LearnSpell(200, false);
            player->LearnSpell(201, false);
            player->LearnSpell(202, false);
            player->LearnSpell(264, false);
            player->LearnSpell(266, false);
            player->LearnSpell(5011, false);
            break;
        case CLASS_WARLOCK:
            player->LearnSpell(201, false);
            player->LearnSpell(227, false);
            player->LearnSpell(1180, false);
            player->LearnSpell(5009, false);
            break;
        case CLASS_MAGE:
            player->LearnSpell(201, false);
            player->LearnSpell(227, false);
            player->LearnSpell(1180, false);
            player->LearnSpell(5009, false);
            break;
        case CLASS_ROGUE:
            player->LearnSpell(196, false);
            player->LearnSpell(198, false);
            player->LearnSpell(201, false);
            player->LearnSpell(1180, false);
            player->LearnSpell(2567, false);
            player->LearnSpell(264, false);
            player->LearnSpell(266, false);
            player->LearnSpell(5011, false);
            player->LearnSpell(15590, false);
            break;
        case CLASS_DEATH_KNIGHT:
            player->LearnSpell(750, false);
            player->LearnSpell(196, false);
            player->LearnSpell(197, false);
            player->LearnSpell(198, false);
            player->LearnSpell(199, false);
            player->LearnSpell(200, false);
            player->LearnSpell(201, false);
            player->LearnSpell(202, false);
            break;
        default:
            break;
        }
        player->SaveToDB();
    }

    void GiveOffSet(Player *player, int i, bool delonly = false)
    {
        int j;
        for (j = 0; j < 5; ++j)
        {
            player->DestroyItemCount(offset[i][j], 1, true);
            if (!delonly)
                player->AddItem(offset[i][j], 1);
        }
    }
    void GiveFactionSet(Player *player, int i)
    {
        int j, k = 6;
        if (i == 0)
            k = 3;
        for (j = i; j < k; ++j)
            player->AddItem(factionset[j], 1);
    }

    bool PlayerAlreadyHasNineProfessions(const Player *player) const
    {
        uint32 skillCount = 0;

        if (player->HasSkill(SKILL_MINING))
            skillCount++;
        if (player->HasSkill(SKILL_SKINNING))
            skillCount++;
        if (player->HasSkill(SKILL_HERBALISM))
            skillCount++;

        if (skillCount >= 2)
            return true;

        for (uint32 i = 0; i < sSkillLineStore.GetNumRows(); ++i)
        {
            SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
            if (!SkillInfo)
                continue;

            if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
                continue;

            if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
                continue;

            const uint32 skillID = SkillInfo->id;
            if (player->HasSkill(skillID))
                skillCount++;

            if (skillCount >= 2)
                return true;
        }
        return false;
    }

    bool LearnProfession(Player* player, Creature* creature, SkillType skill)
    {
        if (!PlayerAlreadyHasNineProfessions(player))
        {
            switch (skill)
            {
            case SKILL_ALCHEMY:
                player->CastSpell(player, SPELL_LEARN_ALCHEMY, true);
                break;
            case SKILL_BLACKSMITHING:
                player->CastSpell(player, SPELL_LEARN_BLACKSMITHING, true);
                break;
            case SKILL_ENCHANTING:
                player->CastSpell(player, SPELL_LEARN_ENCHANTING, true);
                player->AddItem(44452, 1); // runed titanium rod
                break;
            case SKILL_ENGINEERING:
                player->CastSpell(player, SPELL_LEARN_ENGINEERING, true);
                break;
            case SKILL_HERBALISM:
                player->CastSpell(player, SPELL_LEARN_HERBALISM, true);
                break;
            case SKILL_INSCRIPTION:
                player->CastSpell(player, SPELL_LEARN_INSCRIPTION, true);
                break;
            case SKILL_JEWELCRAFTING:
                player->CastSpell(player, SPELL_LEARN_JEWELCRAFTING, true);
                break;
            case SKILL_LEATHERWORKING:
                player->CastSpell(player, SPELL_LEARN_LEATHERWORKING, true);
                break;
            case SKILL_MINING:
                player->CastSpell(player, SPELL_LEARN_MINING, true);
                break;
            case SKILL_SKINNING:
                player->CastSpell(player, SPELL_LEARN_SKINNING, true);
                break;
            case SKILL_TAILORING:
                player->CastSpell(player, SPELL_LEARN_TAILORING, true);
                break;
            default:
                break;
            }
            SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
            uint16 maxLevel = player->GetPureMaxSkillValue(SkillInfo->id);
            player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), maxLevel, maxLevel);
            player->SaveToDB();
            QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM char_transfers WHERE guid=%u;", player->GetGUIDLow());
            Field *fieldsDB = resultDB->Fetch();
            uint32 state = fieldsDB[1].GetUInt32();
            if (PlayerAlreadyHasNineProfessions(player) && (state == 53))
            {
                CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,55);", player->GetGUIDLow());
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cFF006400[ Завершить перенос ]|h|r", GOSSIP_SENDER_MAIN, 122);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else
            {
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[ == Выберите 2 профессии == ]", GOSSIP_SENDER_MAIN, 99);
                if (!PlayerAlreadyHasNineProfessions(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ALCHEMY, GOSSIP_SENDER_MAIN, 110);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BLACKSMITHING, GOSSIP_SENDER_MAIN, 111);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENCNANTING, GOSSIP_SENDER_MAIN, 112);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENGINEERING, GOSSIP_SENDER_MAIN, 113);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_HERBALISM, GOSSIP_SENDER_MAIN, 114);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_INSCRIPTION, GOSSIP_SENDER_MAIN, 115);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_JEWELCRAFTING, GOSSIP_SENDER_MAIN, 116);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_LEATHERWORKING, GOSSIP_SENDER_MAIN, 117);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_MINING, GOSSIP_SENDER_MAIN, 118);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_SKINNING, GOSSIP_SENDER_MAIN, 119);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_TAILORING, GOSSIP_SENDER_MAIN, 120);
                }
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,53);", player->GetGUIDLow()); //one prof choosed
                creature->Whisper("|cFF00FFFFВЫБЕРИТЕ ЕЩЁ ОДНУ ПРОФЕССИЮ!|h|r", LANG_UNIVERSAL, player, false);
            }
        }
        else
        {
            creature->Whisper("Ошибка. Вы уже выбрали 2 профессии, жмите Готово!", LANG_UNIVERSAL, player, false);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM char_transfers WHERE guid=%u;", player->GetGUIDLow());
        if (resultDB)
        {
            Field *fieldsDB = resultDB->Fetch();
            uint32 state = fieldsDB[1].GetUInt32();
            switch (state)
            {
            case 1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Здравствуйте, уважаемый игрок! Рады приветствовать вас на нашем сервере - WOW.PG.UZ! Данный нпц поможет вам перенести персонажа с другого сервера!", GOSSIP_SENDER_MAIN, 99);
                if (player->getClass() == CLASS_DEATH_KNIGHT && (!player->IsQuestRewarded(13188) && !player->IsQuestRewarded(13189)))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Я смогу вам помочь с переносом только после того, как вы выполните все начальные квесты ДК.", GOSSIP_SENDER_MAIN, 126);
                else
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cFF006400[ Начать перенос ]|h|r", GOSSIP_SENDER_MAIN, 2);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|cFF0000CD[ Информация о переносе ]|r", GOSSIP_SENDER_MAIN, 124);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 2:
                if (!HasFreeSpace(player, 6712))
                {
                    player->PlayerTalkClass->ClearMenus();
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "У вас недостаточно свободного места в инвентаре для того что бы продолжить. Оденьте сумку или освободите место!", GOSSIP_SENDER_MAIN, 126);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                }
                else
                {
                    switch (player->getClass())
                    {
                    case CLASS_WARRIOR:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет воина]", GOSSIP_SENDER_MAIN, 3);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_PALADIN:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ретри паладина]", GOSSIP_SENDER_MAIN, 4);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет холи паладина]", GOSSIP_SENDER_MAIN, 5);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_DRUID:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ферал друида]", GOSSIP_SENDER_MAIN, 7);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет баланс друида]", GOSSIP_SENDER_MAIN, 8);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор друида]", GOSSIP_SENDER_MAIN, 9);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_PRIEST:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет приста хила]", GOSSIP_SENDER_MAIN, 10);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ШП]", GOSSIP_SENDER_MAIN, 11);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_SHAMAN:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет элем шамана]", GOSSIP_SENDER_MAIN, 12);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор шамана]", GOSSIP_SENDER_MAIN, 13);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_HUNTER:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет охотника]", GOSSIP_SENDER_MAIN, 14);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_WARLOCK:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет варлока]", GOSSIP_SENDER_MAIN, 15);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_MAGE:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет мага]", GOSSIP_SENDER_MAIN, 16);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_ROGUE:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет разбойника]", GOSSIP_SENDER_MAIN, 17);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    case CLASS_DEATH_KNIGHT:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ДК]", GOSSIP_SENDER_MAIN, 130);
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                        break;
                    default:
                        break;
                    }
                    LearnSkills(player);
                };
                break;
            case 5: //pally not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ретри паладина]", GOSSIP_SENDER_MAIN, 4);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет холи паладина]", GOSSIP_SENDER_MAIN, 5);;
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 7: //dru not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ферал друида]", GOSSIP_SENDER_MAIN, 7);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет баланс друида]", GOSSIP_SENDER_MAIN, 8);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор друида]", GOSSIP_SENDER_MAIN, 9);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 9: //priest not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет приста хила]", GOSSIP_SENDER_MAIN, 10);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ШП]", GOSSIP_SENDER_MAIN, 11);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 11: //shammy not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет элем шамана]", GOSSIP_SENDER_MAIN, 12);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор шамана]", GOSSIP_SENDER_MAIN, 13);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 10: //offset menus
                switch (player->getClass())
                {
                case CLASS_WARLOCK:
                case CLASS_MAGE:
                case CLASS_PRIEST:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с критом]", GOSSIP_SENDER_MAIN, 18);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с духом]", GOSSIP_SENDER_MAIN, 19);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет со скоростью]", GOSSIP_SENDER_MAIN, 20);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                case CLASS_PALADIN:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ретрика]", GOSSIP_SENDER_MAIN, 21);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на холи пала]", GOSSIP_SENDER_MAIN, 22);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                case CLASS_WARRIOR:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на воина]", GOSSIP_SENDER_MAIN, 23);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                case CLASS_DEATH_KNIGHT:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ДК]", GOSSIP_SENDER_MAIN, 23);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                case CLASS_SHAMAN:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на элема]", GOSSIP_SENDER_MAIN, 24);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор шамана]", GOSSIP_SENDER_MAIN, 26);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                case CLASS_DRUID:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на баланс друида]", GOSSIP_SENDER_MAIN, 27);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор друида]", GOSSIP_SENDER_MAIN, 28);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ферал друида]", GOSSIP_SENDER_MAIN, 29);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                case CLASS_HUNTER:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет охотника]", GOSSIP_SENDER_MAIN, 30);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                case CLASS_ROGUE:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет разбойника]", GOSSIP_SENDER_MAIN, 31);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                    break;
                default:
                    break;
                }
                break;
            case 15: //cloth not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с критом]", GOSSIP_SENDER_MAIN, 18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с духом]", GOSSIP_SENDER_MAIN, 19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет со скоростью]", GOSSIP_SENDER_MAIN, 20);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 16: //plate not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ретрика]", GOSSIP_SENDER_MAIN, 21);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на холи пала]", GOSSIP_SENDER_MAIN, 22);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 17: //mail not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на элема]", GOSSIP_SENDER_MAIN, 24);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор шамана]", GOSSIP_SENDER_MAIN, 26);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 18: //leather not confirmed
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на баланс друида]", GOSSIP_SENDER_MAIN, 27);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор друида]", GOSSIP_SENDER_MAIN, 28);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ферал друида]", GOSSIP_SENDER_MAIN, 29);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 20: //backs
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Плащи ===== ]|h|r", GOSSIP_SENDER_MAIN, 100);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 30: //necks
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 35: //battlemasters crit,hit,ap,spd,haste
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 102);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 36: //battlemasters crit,hit,ap,haste melee
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 103);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 40: //books & shields haste,crit,penetration,spirit OR crit,mp5
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Книги и щиты === ]|h|r", GOSSIP_SENDER_MAIN, 104);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 41: //ranged agi
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
                //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Метательное оружие === ]|h|r", GOSSIP_SENDER_MAIN, 105);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 45: //weapon
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 50: //professions
            case 53:
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[ == Выберите 2 профессии == ]", GOSSIP_SENDER_MAIN, 99);
                if (!PlayerAlreadyHasNineProfessions(player))
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ALCHEMY, GOSSIP_SENDER_MAIN, 110);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BLACKSMITHING, GOSSIP_SENDER_MAIN, 111);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENCNANTING, GOSSIP_SENDER_MAIN, 112);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENGINEERING, GOSSIP_SENDER_MAIN, 113);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_HERBALISM, GOSSIP_SENDER_MAIN, 114);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_INSCRIPTION, GOSSIP_SENDER_MAIN, 115);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_JEWELCRAFTING, GOSSIP_SENDER_MAIN, 116);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_LEATHERWORKING, GOSSIP_SENDER_MAIN, 117);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_MINING, GOSSIP_SENDER_MAIN, 118);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_SKINNING, GOSSIP_SENDER_MAIN, 119);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_TAILORING, GOSSIP_SENDER_MAIN, 120);
                }
                else
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MSG_GOSSIP_READY, GOSSIP_SENDER_MAIN, 121);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 55:
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cFF006400[ Завершить перенос ]|h|r", GOSSIP_SENDER_MAIN, 122);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 60: //last
                player->CLOSE_GOSSIP_MENU();
                creature->Whisper("Вас уже перенесли, приятной игры!", LANG_UNIVERSAL, player, false);
                break;
            default:
                break;
            }
        }
        else
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Здравствуйте, уважаемый игрок! Рады приветствовать вас на нашем сервере - WOW.PG.UZ!\nУслуги данного НПЦ доступны только для персонажей, у которых активирована услуга Перенос.", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|cFF0000CD[ Информация о переносе ]|r", GOSSIP_SENDER_MAIN, 124);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[ Ладно, пока ]", GOSSIP_SENDER_MAIN, 126);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        }
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        std::string str = "";
        WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
        switch (action)
        {
        case 2:
            player->GiveLevel(80);
            player->AddItem(badge, 375); //give triumphs
            player->ModifyMoney(300000000);
            //player->AddItem(41600, 1); //give a bag
            player->StoreNewItemInBestSlots(41600, 1);
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,2);", player->GetGUIDLow());
            player->SaveToDB();
            if (player->getClass() == CLASS_DEATH_KNIGHT)
            {
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ДК]", GOSSIP_SENDER_MAIN, 130);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else
            {
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Выбор А6 сета]", GOSSIP_SENDER_MAIN, 125);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                //creature->Whisper("|cFF00FFFFЧТОБЫ ПРОДОЛЖИТЬ ПЕРЕНОС ОДЕНЬТЕ СУМКУ!|h|r", LANG_UNIVERSAL, player, false);
            }
            break;
        case 3:
            GiveSet(player, 0); //give arms set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,10);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 4:
            GiveSet(player, 2, true); //delete holy set
            GiveSet(player, 1); //give ret set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,5);", player->GetGUIDLow()); //state 5 pally dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ретри паладина]", GOSSIP_SENDER_MAIN, 4);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет холи паладина]", GOSSIP_SENDER_MAIN, 5);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 5:
            GiveSet(player, 1, true); //delete ret set
            GiveSet(player, 2); //give holy set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,5);", player->GetGUIDLow()); //state 5 pally dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ретри паладина]", GOSSIP_SENDER_MAIN, 4);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет холи паладина]", GOSSIP_SENDER_MAIN, 5);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 6: //next gossip
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,10);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 123);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 7:
            GiveSet(player, 4, true); //delete moonkin set
            GiveSet(player, 5, true); //delete restore set
            GiveSet(player, 3); //give feral set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,7);", player->GetGUIDLow()); //state 7 Druid dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ферал друида]", GOSSIP_SENDER_MAIN, 7);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет баланс друида]", GOSSIP_SENDER_MAIN, 8);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор друида]", GOSSIP_SENDER_MAIN, 9);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 8:
            GiveSet(player, 3, true); //delete feral set
            GiveSet(player, 5, true); //delete restore set
            GiveSet(player, 4); //give moonkin set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,7);", player->GetGUIDLow()); //state 7 Druid dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ферал друида]", GOSSIP_SENDER_MAIN, 7);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет баланс друида]", GOSSIP_SENDER_MAIN, 8);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор друида]", GOSSIP_SENDER_MAIN, 9);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 9:
            GiveSet(player, 3, true); //delete feral set
            GiveSet(player, 4, true); //delete moonkin set
            GiveSet(player, 5); //give restore set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,7);", player->GetGUIDLow()); //state 7 Druid dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ферал друида]", GOSSIP_SENDER_MAIN, 7);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет баланс друида]", GOSSIP_SENDER_MAIN, 8);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор друида]", GOSSIP_SENDER_MAIN, 9);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 10:
            GiveSet(player, 7, true); //delete shp set
            GiveSet(player, 6); //give heal priest set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,9);", player->GetGUIDLow()); //state 9 Priest dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет приста хила]", GOSSIP_SENDER_MAIN, 10);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ШП]", GOSSIP_SENDER_MAIN, 11);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 11:
            GiveSet(player, 6, true); //delete heal priest set
            GiveSet(player, 7); //give shp set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,9);", player->GetGUIDLow()); //state 9 Priest dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет приста хила]", GOSSIP_SENDER_MAIN, 10);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет ШП]", GOSSIP_SENDER_MAIN, 11);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 12:
            GiveSet(player, 9, true); //delete restore sham set
            GiveSet(player, 8); //give elem sham set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,11);", player->GetGUIDLow()); //state 11 Shaman dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет элем шамана]", GOSSIP_SENDER_MAIN, 12);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор шамана]", GOSSIP_SENDER_MAIN, 13);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 13:
            GiveSet(player, 8, true); //delete elem sham set
            GiveSet(player, 9); //give restore sham set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,11);", player->GetGUIDLow()); //state 11 Shaman dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет элем шамана]", GOSSIP_SENDER_MAIN, 12);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[А6 сет рестор шамана]", GOSSIP_SENDER_MAIN, 13);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 6);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 14:
            GiveSet(player, 11); //give hunter set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,10);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 15:
            GiveSet(player, 12); //give warlock set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,10);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 16:
            GiveSet(player, 13); //give mage set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,10);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 17:
            GiveSet(player, 14); //give rogue set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,10);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 18: //offsets
            GiveOffSet(player, 3, true); //delete spirit offset
            GiveOffSet(player, 4, true); //delete haste offset
            GiveOffSet(player, 2); //give crit offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,15);", player->GetGUIDLow()); //state 15 cloth offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с критом]", GOSSIP_SENDER_MAIN, 18);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с духом]", GOSSIP_SENDER_MAIN, 19);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет со скоростью]", GOSSIP_SENDER_MAIN, 20);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 19: //offsets
            GiveOffSet(player, 2, true); //delete crit offset
            GiveOffSet(player, 4, true); //delete haste offset
            GiveOffSet(player, 3); //give spirit offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,15);", player->GetGUIDLow()); //state 15 cloth offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с критом]", GOSSIP_SENDER_MAIN, 18);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с духом]", GOSSIP_SENDER_MAIN, 19);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет со скоростью]", GOSSIP_SENDER_MAIN, 20);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 20: //offsets
            GiveOffSet(player, 2, true); //delete crit offset
            GiveOffSet(player, 3, true); //delete spirit offset
            GiveOffSet(player, 4); //give haste offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,15);", player->GetGUIDLow()); //state 15 cloth offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с критом]", GOSSIP_SENDER_MAIN, 18);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с духом]", GOSSIP_SENDER_MAIN, 19);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет со скоростью]", GOSSIP_SENDER_MAIN, 20);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 21:
            GiveOffSet(player, 1, true); //delete hpally offset
            GiveOffSet(player, 0); //give rpally offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,16);", player->GetGUIDLow()); //state 16 plate offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ретрика]", GOSSIP_SENDER_MAIN, 21);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на холи пала]", GOSSIP_SENDER_MAIN, 22);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 22:
            GiveOffSet(player, 0, true); //delete rpally offset
            GiveOffSet(player, 1); //give hpally offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,16);", player->GetGUIDLow()); //state 16 plate offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ретрика]", GOSSIP_SENDER_MAIN, 21);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на холи пала]", GOSSIP_SENDER_MAIN, 22);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 23:
            GiveOffSet(player, 0); //give awar or DK offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,20);", player->GetGUIDLow()); //state 20
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 24:
            GiveOffSet(player, 6, true); //delete rsham offset
            GiveOffSet(player, 5); //give elem sham offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,17);", player->GetGUIDLow()); //state 17 mail offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на элема]", GOSSIP_SENDER_MAIN, 24);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор шамана]", GOSSIP_SENDER_MAIN, 26);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 26:
            GiveOffSet(player, 5, true); //delete elem sham offset
            GiveOffSet(player, 6); //give rsham offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,17);", player->GetGUIDLow()); //state 17 mail offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на элема]", GOSSIP_SENDER_MAIN, 24);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор шамана]", GOSSIP_SENDER_MAIN, 26);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 27:
            GiveOffSet(player, 9, true); //delete restore dru offset
            GiveOffSet(player, 10, true); //delete feral offset
            GiveOffSet(player, 8); //give moonkin offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,18);", player->GetGUIDLow()); //state 18 leather offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на баланс друида]", GOSSIP_SENDER_MAIN, 27);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор друида]", GOSSIP_SENDER_MAIN, 28);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ферал друида]", GOSSIP_SENDER_MAIN, 29);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 28:
            GiveOffSet(player, 8, true); //delete moonkin offset
            GiveOffSet(player, 10, true); //delete feral offset
            GiveOffSet(player, 9); //give restore dru offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,18);", player->GetGUIDLow()); //state 18 leather offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на баланс друида]", GOSSIP_SENDER_MAIN, 27);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор друида]", GOSSIP_SENDER_MAIN, 28);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ферал друида]", GOSSIP_SENDER_MAIN, 29);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 29:
            GiveOffSet(player, 8, true); //delete moonkin offset
            GiveOffSet(player, 9, true); //delete restore dru offset
            GiveOffSet(player, 10); //give feral offset
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,18);", player->GetGUIDLow()); //state 18 leather offset dilemma
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на баланс друида]", GOSSIP_SENDER_MAIN, 27);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор друида]", GOSSIP_SENDER_MAIN, 28);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ферал друида]", GOSSIP_SENDER_MAIN, 29);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Далее...(необратимо)]", GOSSIP_SENDER_MAIN, 25);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 30:
            GiveOffSet(player, 7); //give [Оффсет охотника]
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,20);", player->GetGUIDLow()); //state 20
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 31:
            GiveOffSet(player, 10); //give [Оффсет разбойника]
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,20);", player->GetGUIDLow()); //state 20
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 25: //next gossip (offset)
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,20);", player->GetGUIDLow()); //state 20
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 32: //backs start
            player->AddItem(backcaster[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,30);", player->GetGUIDLow()); //state 30
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 33:
            player->AddItem(backcaster[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,30);", player->GetGUIDLow()); //state 30
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 34:
            player->AddItem(backcaster[2], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,30);", player->GetGUIDLow()); //state 30
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 35:
            player->AddItem(backcaster[3], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,30);", player->GetGUIDLow()); //state 30
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 36:
            player->AddItem(backcaster[4], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,30);", player->GetGUIDLow()); //state 30
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 37:
            player->AddItem(backmelee[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,30);", player->GetGUIDLow()); //state 30
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 38:
            player->AddItem(backmelee[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,30);", player->GetGUIDLow()); //state 30
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ===== Подвески ===== ]|h|r", GOSSIP_SENDER_MAIN, 101);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 39: //necks start
            player->AddItem(neckcaster[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,35);", player->GetGUIDLow()); //state 35
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 102);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 40:
            player->AddItem(neckcaster[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,35);", player->GetGUIDLow()); //state 35
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 102);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 41:
            player->AddItem(neckcaster[2], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,35);", player->GetGUIDLow()); //state 35
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 102);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 42:
            player->AddItem(neckcaster[3], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,35);", player->GetGUIDLow()); //state 35
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 102);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 43:
            player->AddItem(neckcaster[4], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,35);", player->GetGUIDLow()); //state 35
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 102);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 44:
            player->AddItem(neckmelee[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,36);", player->GetGUIDLow()); //state 36 detect melees
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 103);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 45:
            player->AddItem(neckmelee[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,36);", player->GetGUIDLow()); //state 36 detect melees
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 103);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 46:
            player->AddItem(neckmelee[2], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,36);", player->GetGUIDLow()); //state 36 detect melees
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 103);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 47: //battlemaster
            player->AddItem(battlemaster[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,40);", player->GetGUIDLow()); //state 40
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Книги и щиты === ]|h|r", GOSSIP_SENDER_MAIN, 104);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 48: //battlemaster
            player->AddItem(battlemaster[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,40);", player->GetGUIDLow()); //state 40
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Книги и щиты === ]|h|r", GOSSIP_SENDER_MAIN, 104);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 49: //battlemaster
            player->AddItem(battlemaster[2], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,40);", player->GetGUIDLow()); //state 40
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Книги и щиты === ]|h|r", GOSSIP_SENDER_MAIN, 104);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 50: //battlemaster
            player->AddItem(battlemaster[3], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,40);", player->GetGUIDLow()); //state 40
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Книги и щиты === ]|h|r", GOSSIP_SENDER_MAIN, 104);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 51: //battlemaster
            player->AddItem(battlemaster[4], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,40);", player->GetGUIDLow()); //state 40
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Книги и щиты === ]|h|r", GOSSIP_SENDER_MAIN, 104);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 52: //battlemaster
            player->AddItem(battlemaster[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,41);", player->GetGUIDLow()); //state 41 melee
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Метательное оружие === ]|h|r", GOSSIP_SENDER_MAIN, 105);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 53: //battlemaster
            player->AddItem(battlemaster[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,41);", player->GetGUIDLow()); //state 41 melee
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Метательное оружие === ]|h|r", GOSSIP_SENDER_MAIN, 105);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 54: //battlemaster
            player->AddItem(battlemaster[2], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,41);", player->GetGUIDLow()); //state 41 melee
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Метательное оружие === ]|h|r", GOSSIP_SENDER_MAIN, 105);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 55: //battlemaster
            player->AddItem(battlemaster[4], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,41);", player->GetGUIDLow()); //state 41 melee
            player->PlayerTalkClass->ClearMenus();
            //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Метательное оружие === ]|h|r", GOSSIP_SENDER_MAIN, 105);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 56: //books
            player->AddItem(books[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,45);", player->GetGUIDLow()); //state 45 books
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 57: //books
            player->AddItem(books[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,45);", player->GetGUIDLow()); //state 45 books
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 58: //books
            player->AddItem(books[2], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,45);", player->GetGUIDLow()); //state 45 books
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 59: //books
            player->AddItem(books[3], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,45);", player->GetGUIDLow()); //state 45 books
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 60: //shield
            player->AddItem(shields[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,45);", player->GetGUIDLow()); //state 45 shield
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 61: //shield
            player->AddItem(shields[1], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,45);", player->GetGUIDLow()); //state 45 shield
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 62: //thrown
            if ((player->getClass() == CLASS_WARRIOR) || (player->getClass() == CLASS_ROGUE))
                player->AddItem(ranged[0], 1);
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,45);", player->GetGUIDLow()); //state 45 thrown
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cffff0000[ Оружие, маунты, и медальон ]|h|r", GOSSIP_SENDER_MAIN, 63);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 63: //weapon    
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                if ((player->getClass() == CLASS_SHAMAN) || (player->getClass() == CLASS_PRIEST) || (player->getClass() == CLASS_DRUID))
                {
                    Quest const* quest = sObjectMgr->GetQuestTemplate(24795);
                    if (player->CanAddQuest(quest, true))
                        player->AddQuestAndCheckCompletion(quest, NULL);
                }
                else
                {
                    if (player->getClass() != CLASS_ROGUE)
                    {
                        Quest const* quest = sObjectMgr->GetQuestTemplate(24796);
                        if (player->CanAddQuest(quest, true))
                            player->AddQuestAndCheckCompletion(quest, NULL);
                    }
                    else
                    {
                        player->AddItem(weapon[0], 1);
                        player->AddItem(weapon[1], 1);
                    }
                }
                GiveFactionSet(player, 0);
                if (player->getClass() == CLASS_HUNTER)
                    player->AddItem(weapon[2], 1);
            }
            else
            {
                if ((player->getClass() == CLASS_SHAMAN) || (player->getClass() == CLASS_PRIEST) || (player->getClass() == CLASS_DRUID))
                {
                    if (player->getRace() == RACE_BLOODELF)
                    {
                        Quest const* quest = sObjectMgr->GetQuestTemplate(24798);
                        if (player->CanAddQuest(quest, true))
                            player->AddQuestAndCheckCompletion(quest, NULL);
                    }
                    else
                    {
                        Quest const* quest = sObjectMgr->GetQuestTemplate(24799);
                        if (player->CanAddQuest(quest, true))
                            player->AddQuestAndCheckCompletion(quest, NULL);
                    }
                }
                else
                {
                    if (player->getClass() != CLASS_ROGUE)
                    {
                        if (player->getRace() == RACE_BLOODELF)
                        {
                            Quest const* quest = sObjectMgr->GetQuestTemplate(24800);
                            if (player->CanAddQuest(quest, true))
                                player->AddQuestAndCheckCompletion(quest, NULL);
                        }
                        else
                        {
                            Quest const* quest = sObjectMgr->GetQuestTemplate(24801);
                            if (player->CanAddQuest(quest, true))
                                player->AddQuestAndCheckCompletion(quest, NULL);
                        }
                    }
                    else if (player->getClass() == CLASS_ROGUE)
                    {
                        player->AddItem(weapon[0], 1);
                        player->AddItem(weapon[1], 1);
                    }
                }
                GiveFactionSet(player, 3);
                if (player->getClass() == CLASS_HUNTER)
                    player->AddItem(weapon[2], 1);
            }
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,50);", player->GetGUIDLow()); //state 50
            //player->CLOSE_GOSSIP_MENU();
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[ == Выберите 2 профессии == ]", GOSSIP_SENDER_MAIN, 99);
            if (!PlayerAlreadyHasNineProfessions(player))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ALCHEMY, GOSSIP_SENDER_MAIN, 110);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BLACKSMITHING, GOSSIP_SENDER_MAIN, 111);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENCNANTING, GOSSIP_SENDER_MAIN, 112);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_ENGINEERING, GOSSIP_SENDER_MAIN, 113);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_HERBALISM, GOSSIP_SENDER_MAIN, 114);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_INSCRIPTION, GOSSIP_SENDER_MAIN, 115);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_JEWELCRAFTING, GOSSIP_SENDER_MAIN, 116);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_LEATHERWORKING, GOSSIP_SENDER_MAIN, 117);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_MINING, GOSSIP_SENDER_MAIN, 118);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_SKINNING, GOSSIP_SENDER_MAIN, 119);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_TAILORING, GOSSIP_SENDER_MAIN, 120);
            }
            else
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MSG_GOSSIP_READY, GOSSIP_SENDER_MAIN, 121);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            creature->Whisper("|cFF00FFFFТЕПЕРЬ ВЫБЕРИТЕ 2 ПРОФЕССИИ!|h|r", LANG_UNIVERSAL, player, false);
            break;
        case 100: //backs
            player->PlayerTalkClass->ClearMenus();
            switch (player->getClass())
            {
            case CLASS_WARLOCK:
            case CLASS_MAGE:
            case CLASS_PRIEST:
            case CLASS_SHAMAN: //hit,spirit,crit,mp5,haste
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ==== Плащи для кастеров ==== ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь с меткостью ]", GOSSIP_SENDER_MAIN, 32);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь с духом ]", GOSSIP_SENDER_MAIN, 33);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь на крит ]", GOSSIP_SENDER_MAIN, 34);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь на МП реген ]", GOSSIP_SENDER_MAIN, 35);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь со скоростью ]", GOSSIP_SENDER_MAIN, 36);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_WARRIOR:
            case CLASS_DEATH_KNIGHT:
            case CLASS_HUNTER:
            case CLASS_ROGUE: //crit,hit
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Плащи для физ. дд === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь на крит ]", GOSSIP_SENDER_MAIN, 37);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь с меткостью ]", GOSSIP_SENDER_MAIN, 38);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_PALADIN:
            case CLASS_DRUID:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ==== Плащи для кастеров ==== ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь с меткостью ]", GOSSIP_SENDER_MAIN, 32);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь с духом ]", GOSSIP_SENDER_MAIN, 33);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь на крит ]", GOSSIP_SENDER_MAIN, 34);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь на МП реген ]", GOSSIP_SENDER_MAIN, 35);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь со скоростью ]", GOSSIP_SENDER_MAIN, 36);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Плащи для физ. дд === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь на крит ]", GOSSIP_SENDER_MAIN, 37);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Плащь с меткостью ]", GOSSIP_SENDER_MAIN, 38);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            default:
                break;
            }
            break;
        case 101: //necks
            player->PlayerTalkClass->ClearMenus();
            switch (player->getClass())
            {
            case CLASS_WARLOCK:
            case CLASS_MAGE:
            case CLASS_PRIEST:
            case CLASS_SHAMAN: //hit,spirit,crit,mp5,haste
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Подвески для кастеров === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с меткостью ]", GOSSIP_SENDER_MAIN, 39);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с духом ]", GOSSIP_SENDER_MAIN, 40);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска на крит ]", GOSSIP_SENDER_MAIN, 41);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска на МП реген ]", GOSSIP_SENDER_MAIN, 42);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска со скоростью ]", GOSSIP_SENDER_MAIN, 43);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_WARRIOR:
            case CLASS_DEATH_KNIGHT:
            case CLASS_HUNTER:
            case CLASS_ROGUE: //arp,crit,hit
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Подвески для физ. дд === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с пробиванием брони ]", GOSSIP_SENDER_MAIN, 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска на крит ]", GOSSIP_SENDER_MAIN, 45);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с меткостью ]", GOSSIP_SENDER_MAIN, 46);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_PALADIN:
            case CLASS_DRUID:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Подвески для кастеров === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с меткостью ]", GOSSIP_SENDER_MAIN, 39);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с духом ]", GOSSIP_SENDER_MAIN, 40);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска на крит ]", GOSSIP_SENDER_MAIN, 41);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска на МП реген ]", GOSSIP_SENDER_MAIN, 42);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска со скоростью ]", GOSSIP_SENDER_MAIN, 43);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Подвески для физ. дд === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с пробиванием брони ]", GOSSIP_SENDER_MAIN, 44);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска на крит ]", GOSSIP_SENDER_MAIN, 45);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Подвеска с меткостью ]", GOSSIP_SENDER_MAIN, 46);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            default:
                break;
            }
            break;
        case 102: //battlemasters caster
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 128 к криту ]", GOSSIP_SENDER_MAIN, 47);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 128 к меткости ]", GOSSIP_SENDER_MAIN, 48);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 256 к силе атаки ]", GOSSIP_SENDER_MAIN, 49);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 150 к силе заклинаний ]", GOSSIP_SENDER_MAIN, 50);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 128 к скорости ]", GOSSIP_SENDER_MAIN, 51);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 103: //battlemasters melee
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Тринкеты Военачальника === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 128 к криту ]", GOSSIP_SENDER_MAIN, 52);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 128 к меткости ]", GOSSIP_SENDER_MAIN, 53);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 256 к силе атаки ]", GOSSIP_SENDER_MAIN, 54);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ + 128 к скорости ]", GOSSIP_SENDER_MAIN, 55);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 104: //book, shield
            player->PlayerTalkClass->ClearMenus();
            switch (player->getClass())
            {
            case CLASS_PRIEST:
            case CLASS_WARLOCK:
            case CLASS_MAGE:
            case CLASS_DRUID:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ==== Книги ==== ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Скорость ]", GOSSIP_SENDER_MAIN, 56);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Крит ]", GOSSIP_SENDER_MAIN, 57);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Проник. способность заклинаний ]", GOSSIP_SENDER_MAIN, 58);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Дух ]", GOSSIP_SENDER_MAIN, 59);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_SHAMAN:
            case CLASS_PALADIN:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ ==== Щиты ==== ]|h|r", GOSSIP_SENDER_MAIN, 99);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Крит ]", GOSSIP_SENDER_MAIN, 60);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ МП реген ]", GOSSIP_SENDER_MAIN, 61);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            default:
                break;
            }
            break;
            /*case 105: thrown weapon case
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Метательное оружие === ]|h|r", GOSSIP_SENDER_MAIN, 99);
                if ((player->getClass() == CLASS_WARRIOR) || (player->getClass() == CLASS_ROGUE))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Багровая звезда ]", GOSSIP_SENDER_MAIN, 62);
                else
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[ Я такое не ношу, дальше...]", GOSSIP_SENDER_MAIN, 62);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;*/
        case 110:
            LearnProfession(player, creature, SKILL_ALCHEMY);
            break;
        case 111:
            LearnProfession(player, creature, SKILL_BLACKSMITHING);
            break;
        case 112:
            LearnProfession(player, creature, SKILL_ENCHANTING);
            break;
        case 113:
            LearnProfession(player, creature, SKILL_ENGINEERING);
            break;
        case 114:
            LearnProfession(player, creature, SKILL_HERBALISM);
            break;
        case 115:
            LearnProfession(player, creature, SKILL_INSCRIPTION);
            break;
        case 116:
            LearnProfession(player, creature, SKILL_JEWELCRAFTING);
            break;
        case 117:
            LearnProfession(player, creature, SKILL_LEATHERWORKING);
            break;
        case 118:
            LearnProfession(player, creature, SKILL_MINING);
            break;
        case 119:
            LearnProfession(player, creature, SKILL_SKINNING);
            break;
        case 120:
            LearnProfession(player, creature, SKILL_TAILORING);
            break;
        case 121:
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,55);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cFF006400[ Завершить перенос ]|h|r", GOSSIP_SENDER_MAIN, 122);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 122:
            creature->Whisper("|cFF00FFFFПеренос успешно завершен, приятной игры, пока!|h|r", LANG_UNIVERSAL, player, false);
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,60);", player->GetGUIDLow());
            player->CLOSE_GOSSIP_MENU();
            player->TeleportTo(571, 5804.15f, 624.771f, 647.767f, 1.64f);
            str = "|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Перенос завершен!";
            data << str;
            sWorld->SendGlobalGMMessage(&data);
            break;
        case 123:
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cffff0000[ === Оффсеты === ]|h|r", GOSSIP_SENDER_MAIN, 99);
            switch (player->getClass())
            {
            case CLASS_WARLOCK:
            case CLASS_MAGE:
            case CLASS_PRIEST:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с критом]", GOSSIP_SENDER_MAIN, 18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет с духом]", GOSSIP_SENDER_MAIN, 19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет со скоростью]", GOSSIP_SENDER_MAIN, 20);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_PALADIN:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ретрика]", GOSSIP_SENDER_MAIN, 21);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на холи пала]", GOSSIP_SENDER_MAIN, 22);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_WARRIOR:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на воина]", GOSSIP_SENDER_MAIN, 23);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_SHAMAN:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на элема]", GOSSIP_SENDER_MAIN, 24);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор шамана]", GOSSIP_SENDER_MAIN, 26);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_DRUID:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на баланс друида]", GOSSIP_SENDER_MAIN, 27);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на рестор друида]", GOSSIP_SENDER_MAIN, 28);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ферал друида]", GOSSIP_SENDER_MAIN, 29);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_HUNTER:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет охотника]", GOSSIP_SENDER_MAIN, 30);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_ROGUE:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет разбойника]", GOSSIP_SENDER_MAIN, 31);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case CLASS_DEATH_KNIGHT:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ДК]", GOSSIP_SENDER_MAIN, 23);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            default:
                break;
            }
            break;
        case 124:
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cFF0000CD[ Информация о переносе ]|h|r", GOSSIP_SENDER_MAIN, 99);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Перенос персонажа с другого сервера.\n|cFF8B0000При этом вы получите:\n - Персонажа 80 уровня,\n - А6 сет + 264 + 245 пвп оффсеты,\n - 30 000 голдов + 375 Эмблем Триумфа,\n - 2 профессии на выбор,[450-450]\n - 100% и 280% маунта,\n - Кельдалар (251 кинжалы для разбойников и 251 ПВЕ лук для охотников),\n - 270 ПВП оффхенды или щиты для кастеров.|h|r\nПеренос выполняется по предварительной заявке на форуме |cFF0000CD[forum.playground.uz]|h|rТам же вы получите ответы на интересующие вас вопросы касательно переноса персонажей с других серверов.", GOSSIP_SENDER_MAIN, 125);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 125:
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            break;
        case 126:
            player->CLOSE_GOSSIP_MENU();
            break;
        case 130:
            GiveSet(player, 15); //give dk set
            player->SaveToDB();
            CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,10);", player->GetGUIDLow());
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "[Оффсет на ДК]", GOSSIP_SENDER_MAIN, 23);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        default:
            break;
        }
        return true;
    }
};

void AddSC_npc_transfer()
{
    new npc_transfer();
}