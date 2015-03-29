#include "ScriptPCH.h"
#include "Chat.h"
#include "ArenaTeamMgr.h"
#include "BattlegroundMgr.h"
#include "WorldSession.h"
#include "Player.h"
#include "ArenaTeam.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "CreatureTextMgr.h"
#include "Config.h"
#include "Language.h"

class arena_spectator_commands : public CommandScript
{
    public:
        arena_spectator_commands() : CommandScript("arena_spectator_commands") { }

        static bool HandleSpectateCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            ObjectGuid target_guid;
            std::string target_name;
            if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
                return false;

            Player* player = handler->GetSession()->GetPlayer();
            if (target == player || target_guid == player->GetGUID())
            {
                handler->PSendSysMessage("Нельзя просто так взять и...наблюдать за собой :)");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->IsInCombat())
            {
                handler->PSendSysMessage("Нельзя сделать это находясь в бою");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->isDead())
            {
                handler->PSendSysMessage("Нельзя сделать это будучи мертвым");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!target)
            {
                handler->PSendSysMessage("Игрок не найден");
                handler->SetSentErrorMessage(true);
                return false;
            }
			
			if (player->IsMounted())
            {
                handler->PSendSysMessage("Невозможно сделать это находясь на средстве передвижения");
                handler->SetSentErrorMessage(true);
                return false;
            }			

			/*if (target && (target->HasAura(32728) || target->HasAura(32727))) // Check Arena Preparation thx XXsupr
            {
                handler->PSendSysMessage("Cant do that. Arena didn`t started.");
                handler->SetSentErrorMessage(true);
                return false;
            }
            */
            if (player->GetPet())
            {
                handler->PSendSysMessage("Вы должны убрать питомца");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetMap()->IsBattlegroundOrArena() && !player->IsSpectator())
            {
                handler->PSendSysMessage("Невозможно сделать это находясь на арене или поле боя");
                handler->SetSentErrorMessage(true);
                return false;
            }

            Map* cMap = target->GetMap();
            if (!cMap->IsBattleArena())
            {
                handler->PSendSysMessage("Игрок не на арене");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetMap()->IsBattleground())
            {
                handler->PSendSysMessage("Невозможно сделать это находясь на поле боя");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // all's well, set bg id
            // when porting out from the bg, it will be reset to 0
            player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
            // remember current position as entry point for return at bg end teleportation
            if (!player->GetMap()->IsBattlegroundOrArena())
                player->SetBattlegroundEntryPoint();

            if (target->IsSpectator())
            {
                handler->PSendSysMessage("Невозможно, т.к. выбранный вами игрок - зритель");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // stop flight if need
            if (player->IsInFlight())
            {
               player->GetMotionMaster()->MovementExpired();
               player->CleanupAfterTaxiFlight();
            }
            // save only in non-flight case
            else
                player->SaveRecallPosition();

            // search for two teams
            Battleground *bGround = target->GetBattleground();
            if (bGround->isRated())
            {
                uint32 slot = bGround->GetArenaType() - 2;
                if (bGround->GetArenaType() > 3)
                    slot = 2;
                uint32 firstTeamID = target->GetArenaTeamId(slot);
                uint32 secondTeamID = 0;
                Player *firstTeamMember  = target;
                Player *secondTeamMember = NULL;
                for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
                    if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                    {
                        if (tmpPlayer->IsSpectator())
                            continue;

                        uint32 tmpID = tmpPlayer->GetArenaTeamId(slot);
                        if (tmpID != firstTeamID && tmpID > 0)
                        {
                            secondTeamID = tmpID;
                            secondTeamMember = tmpPlayer;
                            break;
                        }
                    }

                if (firstTeamID > 0 && secondTeamID > 0 && secondTeamMember)
                {
					ArenaTeam *firstTeam = sArenaTeamMgr->GetArenaTeamById(firstTeamID);
                    ArenaTeam *secondTeam = sArenaTeamMgr->GetArenaTeamById(secondTeamID);
                    if (firstTeam && secondTeam)
                    {
                        handler->PSendSysMessage("Рейтинговый бой");
                        handler->PSendSysMessage("Команды:");
                        handler->PSendSysMessage("%s - %s", firstTeam->GetName().c_str(), secondTeam->GetName().c_str());
                        handler->PSendSysMessage("%u(%u) - %u(%u)", firstTeam->GetRating(), firstTeam->GetAverageMMR(firstTeamMember->GetGroup()),
                        secondTeam->GetRating(), secondTeam->GetAverageMMR(secondTeamMember->GetGroup()));
                    }
                }
            }

            // to point to see at target with same orientation
            float x, y, z;
            target->GetContactPoint(player, x, y, z);

            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target), TELE_TO_GM_MODE);
            player->SetPhaseMask(target->GetPhaseMask(), true);
            player->SetSpectate(true);
            target->GetBattleground()->AddSpectator(player->GetGUID());

            return true;
        }
       static bool HandleSpectateCancelCommand(ChatHandler* handler, const char* /*args*/)
        {
            Player* player =  handler->GetSession()->GetPlayer();

            if (!player->IsSpectator())
            {
                handler->PSendSysMessage("Вы не зритель");
                handler->SetSentErrorMessage(true);
                return false;
            }

            player->GetBattleground()->RemoveSpectator(player->GetGUID());
            player->CancelSpectate();
            player->TeleportToBGEntryPoint();

            return true;
        }

        static bool HandleSpectateFromCommand(ChatHandler* handler, const char *args)
        {
            Player* target;
            ObjectGuid target_guid;
            std::string target_name;
            if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
                return false;

            Player* player = handler->GetSession()->GetPlayer();

            if (!target)
            {
                handler->PSendSysMessage("Игрок не найден");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->IsSpectator())
            {
                handler->PSendSysMessage("Вы не зритель, для начала войдите в режим зрителя");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->IsSpectator() && target != player)
            {
                handler->PSendSysMessage("Невозможно, т.к. выбранный вами игрок - зритель");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetMap() != target->GetMap())
            {
                handler->PSendSysMessage("Cant do that. Different arenas?");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // check for arena preperation
            // if exists than battle didn`t begin
            /*if (target->HasAura(32728) || target->HasAura(32727))
            {
                handler->PSendSysMessage("Cant do that. Arena didn`t started.");
                handler->SetSentErrorMessage(true);
                return false;
            }*/

            (target == player && player->getSpectateFrom()) ? player->SetViewpoint(player->getSpectateFrom(), false) :
                                                                player->SetViewpoint(target, true);
            return true;
        }

        static bool HandleSpectateResetCommand(ChatHandler* handler, const char *args)
        {
            Player* player = handler->GetSession()->GetPlayer();

            if (!player)
            {
                handler->PSendSysMessage("Игрок не найден");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->IsSpectator())
            {
                handler->PSendSysMessage("Вы не зритель, для начала войдите в режим зрителя");
                handler->SetSentErrorMessage(true);
                return false;
            }

            Battleground *bGround = player->GetBattleground();
            if (!bGround)
                return false;

            if (bGround->GetStatus() != STATUS_IN_PROGRESS)
                return true;

            return true;
        }

        ChatCommand* GetCommands() const
        {
            static ChatCommand spectateCommandTable[] =
            {
                { "player",         SEC_PLAYER,      true,  &HandleSpectateCommand,        "", NULL },
                { "view",           SEC_PLAYER,      true,  &HandleSpectateFromCommand,    "", NULL },
                { "reset",          SEC_PLAYER,      true,  &HandleSpectateResetCommand,   "", NULL },
                { "leave",          SEC_PLAYER,      true,  &HandleSpectateCancelCommand,  "", NULL },
                { NULL,             0,               false, NULL,                          "", NULL }
            };

            static ChatCommand commandTable[] =
            {
                { "spectate",       SEC_PLAYER, false,  NULL, "", spectateCommandTable },
                { NULL,             0,          false,  NULL, "", NULL }
            };
            return commandTable;
        }
};


enum NpcSpectatorActions {
    // will be used for scrolling
    NPC_SPECTATOR_ACTION_LIST_GAMES         = 1000,
    NPC_SPECTATOR_ACTION_LIST_TOP_GAMES     = 2000,

    // NPC_SPECTATOR_ACTION_SELECTED_PLAYER + player.Guid()
    NPC_SPECTATOR_ACTION_SELECTED_PLAYER    = 3000
};

const uint16 TopGamesRating = 1800;
const uint8  GamesOnPage    = 20;

class npc_arena_spectator : public CreatureScript
{
    public:
        npc_arena_spectator() : CreatureScript("npc_arena_spectator") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_PVP_A_13:30:30:-18:0|tСмотреть бои 1800+", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_TOP_GAMES);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_PVP_G_06:30:30:-18:0|tСмотреть бои до 1800", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES);
            //pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Spectate Specific Player", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER);
			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }
};


void AddSC_arena_spectator_script()
{
    new arena_spectator_commands();
    new npc_arena_spectator();
}