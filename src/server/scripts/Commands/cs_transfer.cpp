#include "ScriptPCH.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "WorldSession.h"
#include "Chat.h"
#include "CreatureTextMgr.h"
#include "Language.h"
#include "ObjectMgr.h"

class transfer_commands : public CommandScript
{
public:
    transfer_commands() : CommandScript("transfer_commands") { }

    static bool HandleSetTransferCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->getBoolConfig(CONFIG_TRANSFER_ENABLED))
        {
            handler->PSendSysMessage("Перенос персонажей сейчас выключен.");
            handler->SetSentErrorMessage(true);
            return false;
        }
        Player* target;
        ObjectGuid target_guid;
        std::string target_name;
        if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
            return false;

        Player* player = handler->GetSession()->GetPlayer();
        if (!target && !target_guid)
        {
            handler->PSendSysMessage("Персонаж не найден.");
            handler->SetSentErrorMessage(true);
            return false;
        }
        QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM `char_transfers` WHERE guid=%u;", target_guid);
        if (!resultDB)
        {
            if (!target)
            {
                CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,1);", target_guid);
                handler->PSendSysMessage("Персонаж %s (offline) добавлен в базу переносов. ", target_name.c_str());
                handler->SetSentErrorMessage(true);
            }
            else
            {
                if (target->getClass() == CLASS_DEATH_KNIGHT)
                {
                    if (!target->IsQuestRewarded(13188) && !target->IsQuestRewarded(13189))
                    {
                        handler->PSendSysMessage("Перенос невозможен: выбранный вами ДК не выполнил свой последний классовый квест.");
                        handler->SetSentErrorMessage(true);
                        return false;
                    }
                    else
                    {
                        CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,1);", target_guid);
                        handler->PSendSysMessage("Персонаж %s добавлен в базу переносов.", target_name.c_str());
                        handler->SetSentErrorMessage(true);
                        ChatHandler(target->GetSession()).PSendSysMessage("|cff00ff00А'дал:|h|r %s, тебе назначили перенос, подходи, я тебе помогу!", target_name.c_str());
                    }
                }
                else
                {
                    CharacterDatabase.PExecute("REPLACE INTO `char_transfers` (guid,state) VALUES (%u,1);", target_guid);
                    handler->PSendSysMessage("Персонаж %s добавлен в базу переносов.", target_name.c_str());
                    ChatHandler(target->GetSession()).PSendSysMessage("|cff00ff00А'дал:|h|r %s, тебе назначили перенос, подходи, я тебе помогу!", target_name.c_str());
                }
            }
        }
        else
        {
            Field *fieldsDB = resultDB->Fetch();
            uint32 state = fieldsDB[1].GetUInt32();
            if (state == 60)
                handler->PSendSysMessage("Персонаж %s уже перенесен.", target_name.c_str());
            else
                handler->PSendSysMessage("Персонаж %s уже переносится.", target_name.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }
        return true;
    }
    static bool HandleTransferInfoCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        Player* player = handler->GetSession()->GetPlayer();
        ObjectGuid target_guid;
        std::string target_name;
        if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
            return false;

        if (!target && !target_guid)
        {
            handler->PSendSysMessage("Персонаж не найден.");
            handler->SetSentErrorMessage(true);
            return false;
        }
        QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM `char_transfers` WHERE guid=%u;", target_guid);
        if (!resultDB)
        {
            handler->PSendSysMessage("Инфо: персонаж %s не найден в базе переносов.", target_name.c_str());
            handler->SetSentErrorMessage(true);
            return true;
        }
        else
        {
            Field *fieldsDB = resultDB->Fetch();
            uint32 pguid = fieldsDB[0].GetUInt32();
            uint32 state = fieldsDB[1].GetUInt32();
            if (state == 60)
                handler->PSendSysMessage("Инфо: персонаж %s в базе переносов, стадия: %u (перенос завершен)", target_name.c_str(), state);
            else
                handler->PSendSysMessage("Инфо: персонаж %s в базе переносов, стадия: %u", target_name.c_str(), state);
            handler->SetSentErrorMessage(true);
            return true;
        }
        return true;
    }
    static bool HandleTransferCancelCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->getBoolConfig(CONFIG_TRANSFER_ENABLED))
        {
            handler->PSendSysMessage("Перенос персонажей сейчас выключен.");
            handler->SetSentErrorMessage(true);
            return false;
        }
        Player* target;
        ObjectGuid target_guid;
        std::string target_name;
        Player* player = handler->GetSession()->GetPlayer();
        if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
            return false;
        if (!target && !target_guid)
        {
            handler->PSendSysMessage("Персонаж не найден.");
            handler->SetSentErrorMessage(true);
            return false;
        }
        QueryResult resultDB = CharacterDatabase.PQuery("SELECT state FROM `char_transfers` WHERE guid=%u;", target_guid);
        if (resultDB)
        {
            CharacterDatabase.PExecute("DELETE FROM `char_transfers` WHERE guid = %u", target_guid);
            handler->PSendSysMessage("%s: перенос отменен.", target_name.c_str());
            handler->SetSentErrorMessage(true);
            return true;
        }
        else
        {
            handler->PSendSysMessage("Ошибка: персонаж %s не найден в базе переносов.", target_name.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }
        return true;
    }
    static bool HandleTransferHelpCommand(ChatHandler* handler, char const* /*args*/)
    {
        handler->PSendSysMessage("Help: .transfer player player_name(target)");
        handler->SetSentErrorMessage(true);
        return true;
    }

    ChatCommand* GetCommands() const override
    {
        static ChatCommand transferCommandTable[] =
        {
            { "player", SEC_MODERATOR, false, &HandleSetTransferCommand, "", NULL },
            { "info", SEC_MODERATOR, false, &HandleTransferInfoCommand, "", NULL },
            { "cancel", SEC_MODERATOR, false, &HandleTransferCancelCommand, "", NULL },
            { "help", SEC_MODERATOR, false, &HandleTransferHelpCommand, "", NULL },
            { NULL, 0, false, NULL, "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "transfer", SEC_MODERATOR, false, NULL, "", transferCommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        return commandTable;
    }
};

void AddSC_transfer_commands()
{
    new transfer_commands();
}