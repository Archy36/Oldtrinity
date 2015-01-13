#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "Player.h"
#include "BattlegroundMgr.h"
#include "WorldSession.h"
#include "Chat.h"

class monthly_kills : public PlayerScript
{
public:
    monthly_kills() : PlayerScript("monthly_kills") {}
    
    void OnMapChanged(Player* player)
    {
        if (player->HasAura(70923))
            player->RemoveAurasDueToSpell(70923);
    }

    void OnLogin(Player* player, bool /*firstLogin*/)
    {
        if (player->HasAura(70923))
            player->RemoveAurasDueToSpell(70923);
    }

};

class npc_monthly_kills : public CreatureScript
{
public:
    npc_monthly_kills() : CreatureScript("npc_monthly_kills") { }

    bool OnGossipHello(Player * player, Creature * creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Показать Топ-30 ПВП убийц за месяц", GOSSIP_SENDER_MAIN, 1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "А сколько у меня убийств за месяц?", GOSSIP_SENDER_MAIN, 2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[Пока...]", GOSSIP_SENDER_MAIN, 3);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player * player, Creature * creature, uint32 sender, uint32 action)
    {
        if (action == 1)
        {
            player->PlayerTalkClass->ClearMenus();
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cFF000000==== [ Топ-30 ПВП убийц за месяц ] ====|r", GOSSIP_SENDER_MAIN, 10);
            QueryResult result = CharacterDatabase.Query("SELECT `name`, `monthly`, `class`, `race`, `guid` FROM `char_monthly_kills` ORDER BY `monthly` DESC LIMIT 30;");
            if (!result)
                return false;
            uint8 rows = result->GetRowCount();
            Field * fields = NULL;
            uint32 number = 0;
            uint8 team = 0; //ally
            do
            {
                number++;
                fields = result->Fetch();
                std::string name = fields[0].GetString();
                uint32 monthly = fields[1].GetUInt32();
                uint32 clas = fields[2].GetUInt32();
                uint32 race = fields[3].GetUInt32();
                uint32 guid = fields[4].GetUInt32();
                std::string classstr;
                classstr.clear();
                switch (clas)
                {
                case 1:
                    classstr = "воин";
                    break;
                case 2:
                    classstr = "паладин";
                    break;
                case 3:
                    classstr = "охотник";
                    break;
                case 4:
                    classstr = "разбойник";
                    break;
                case 5:
                    classstr = "жрец";
                    break;
                case 6:
                    classstr = "рыцарь смерти";
                    break;
                case 7:
                    classstr = "шаман";
                    break;
                case 8:
                    classstr = "маг";
                    break;
                case 9:
                    classstr = "чернокнижник";
                    break;
                case 11:
                    classstr = "друид";
                    break;
                default:
                    break;
                }
                switch (race)
                {
                case 1:
                case 3:
                case 4:
                case 7:
                case 11:
                    team = 0;
                    break;
                default:
                    team = 1; //horde
                    break;
                }
                std::ostringstream ss;
                ss.clear();
                if (team == 0)
                {
                    /*if (number == 1)
                    {
                        if (player->GetGUIDLow() == guid)
                            ss << "|cFF00FFFF#" << number << " [Вы]|r |cff0000ff[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                        else
                            ss << "|cFF00FFFF#" << number << "|r |cff0000ff[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                    }*/
                    if (number < 4) //NUM: ally top 3; self springgreen |cFF00FF7F; NAME: others blue;
                    {
                        if (player->GetGUIDLow() == guid)
                            ss << "|cFF00FF7F#" << number << " [Вы]|r |cff0000ff[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                        else
                            ss << "|cFF7FFF00#" << number << "|r |cff0000ff[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                    }
                    else if (number >= 4)//NUM: 4+ self springgreen |cFF00FF7F; others CHARTREUSE  |cFF7FFF00
                    {
                        if (player->GetGUIDLow() == guid)
                            ss << "|cFF00FF7F#" << number << " [Вы]|r |cff0000ff[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                        else
                            ss << "#" << number << "|cff0000ff [" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                    }
                }
                else if (team == 1)
                {
                    /*if (number == 1)
                    {
                        if (player->GetGUIDLow() == guid)
                            ss << "|cFF00FFFF#" << number << " [Вы]|r |cffff0000[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                        else
                            ss << "|cFF00FFFF#" << number << "|r |cffff0000[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                    }*/
                    if (number < 4) //NUM: horde top5 self springgreen |cFF00FF7F; others tomato |cFFFF6347; NAME: others red;
                    {
                        if (player->GetGUIDLow() == guid)
                            ss << "|cFF00FF7F#" << number << " [Вы]|r |cffff0000[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                        else
                            ss << "|cFF7FFF00#" << number << "|r |cffff0000[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                    }
                    else if (number >= 4)//NUM: 4+ self springgreen |cFF00FF7F; others CHARTREUSE  |cFF7FFF00
                    {
                        if (player->GetGUIDLow() == guid)
                            ss << "|cFF00FF7F#" << number << " [Вы]|r |cffff0000[" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                        else
                            ss << "#" << number << "|cffff0000 [" << name << "]|r [" << classstr << "] " << "убийств: |cFF8B0000" << monthly << "|r\n";
                    }

                }
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, ss.str(), GOSSIP_SENDER_MAIN, 10);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            } while (result->NextRow());
            return true;
        }
        if (action == 2)
        {
            QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM `char_monthly_kills` WHERE guid=%u;", player->GetGUIDLow());
            if (!resultDB)
            {
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "В этом месяце за вами не засчитано ни одного ПВП убийства.", GOSSIP_SENDER_MAIN, 3);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else
            {
                Field *fieldsDB = resultDB->Fetch();
                std::string monthly = fieldsDB[5].GetString();
                char msgg[250];
                snprintf(msgg, 250, "%s, у вас |cff00ff00%s|r ПВП убийств(а) за текущий месяц.", player->GetName().c_str(), monthly.c_str());
                player->PlayerTalkClass->ClearMenus();
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, msgg, GOSSIP_SENDER_MAIN, 2);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "[Назад...]", GOSSIP_SENDER_MAIN, 4);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            return true;
        }
        if (action == 3)
        {
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
        if (action == 4)
        {
            player->PlayerTalkClass->ClearMenus();
            OnGossipHello(player, creature);
            return true;
        }
        return true;
    }
};

class monthlykills_commands : public CommandScript
{
public:
    monthlykills_commands() : CommandScript("monthlykills_commands") { }
    static bool HandleMonthlyListCommand(ChatHandler* handler, char const* args)
    {
        QueryResult result = CharacterDatabase.Query("SELECT `name`, `guid`, `monthly` FROM `char_monthly_kills` ORDER BY `monthly` DESC LIMIT 10");
        if (!result)
        {
            handler->PSendSysMessage("Ошибка: таблица char_monthly_kills пуста!");
            return true;
        }
        else
        {
            uint32 num = 0;
            Field * fields = NULL;
            do
            {
                num++;
                fields = result->Fetch();
                std::string name = fields[0].GetString();
                uint32 guid = fields[1].GetUInt32();
                uint32 monthly = fields[2].GetUInt32();
                char msg[250];
                snprintf(msg, 250, "%u. %s (guid: %u) -- киллы: %u", num, name.c_str(), guid, monthly);
                handler->PSendSysMessage(msg);
            } while (result->NextRow());
            return true;
        }
    }
    static bool HandleMonthlyResetCommand(ChatHandler* handler, char const* args)
    {
        QueryResult result = CharacterDatabase.Query("SELECT `name`, `guid`, `monthly` FROM `char_monthly_kills` ORDER BY `monthly` DESC LIMIT 10;");
        if (!result)
        {
            handler->PSendSysMessage("Ошибка: таблица char_monthly_kills уже пуста!");
            return true;
        }
        else
        {
            CharacterDatabase.PExecute("TRUNCATE TABLE `char_monthly_kills`;");
            handler->PSendSysMessage("Таблица char_monthly_kills очищена!");
            return true;
        }
    }
    static bool HandleMonthlyHelpCommand(ChatHandler* handler, char const* args)
    {
        handler->PSendSysMessage("Управление подсчетом ПВП киллов за месяц.");
        return true;
    }

    ChatCommand* GetCommands() const override
    {
        static ChatCommand monthlyCommandTable[] =
        {
            { "list", SEC_MODERATOR, false, &HandleMonthlyListCommand, "", NULL },
            { "reset", SEC_MODERATOR, false, &HandleMonthlyResetCommand, "", NULL },
            { "help", SEC_MODERATOR, false, &HandleMonthlyHelpCommand, "", NULL },
            { NULL, 0, false, NULL, "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "monthly", SEC_MODERATOR, false, NULL, "", monthlyCommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        return commandTable;
    }
};

void AddSC_monthly_kills()
{
    new npc_monthly_kills();
    new monthly_kills();
    new monthlykills_commands();
}