#include "ScriptPCH.h"
#include "Chat.h"
#include "ArenaTeamMgr.h"
#include "BattlegroundMgr.h"
#include "WorldSession.h"
#include "Player.h"
#include "ArenaTeam.h"
#include "Battleground.h"
#include "CreatureTextMgr.h"
#include "../../../scripts/Custom/Transmog/Transmogrification.h"

#define MSG_NOT_ENOUGH_MONEY "У вас недостаточно средств или рейтинга для приобретения данного предмета!"
#define MSG_THANKS "О, отличный выбор! Спасибо за покупку!"
#define MSG_BAG_ERROR "У вас забиты сумки, освободите место и попробуйте снова!"
#define MSG_ADVA "Налетай, торопись, покупай живопись! ОЙ...то есть, редкие диковины!"
#define MSG_ADVB "Редкие маунты! Подходим не стесняемся!"
#define MSG_ADVC "Туц-Туц-Туц-Туц-Туц-Туц ... I'm sexy and I know it ... "

enum NpcVendorActions 
{
    ACTION_BACK             = 1,
	ACTION_BUY_A			= 10,
    ACTION_CONFIRM_A		= 15,
    ACTION_BUY_B            = 20,
	ACTION_CONFIRM_B		= 25,
    ACTION_BUY_C            = 30,
    ACTION_CONFIRM_C		= 35,
    ACTION_BUY_D            = 40,
    ACTION_CONFIRM_D		= 45,
    ACTION_BUY_E            = 50,
    ACTION_CONFIRM_E		= 55,
    ACTION_BUY_F            = 60,
    ACTION_CONFIRM_F        = 65,
    ACTION_BUY_G            = 70,
    ACTION_CONFIRM_G        = 75,
};
// First item
const uint32 itemA = 49286;
const uint32 price = 400;
const uint32 token = 49426;
const uint32 prating = 1;
const int32 AP = 3000;

// second item
const uint32 itemB = 43599;
//const uint32 price = 300;
//onst uint32 token = 49426;
//const uint32 prating = 1;
//const int32 AP = 3000;

// third item
const uint32 itemC = 37719;
//const uint32 price = 300;
//const uint32 token = 49426;
//const uint32 prating = 1;
//const int32 AP = 3000;

// fourth item
const uint32 itemD = 45802;
//const uint32 price = 300;
//const uint32 token = 49426;
//const uint32 prating = 1;
//const int32 AP = 3000;

// fifth item
const uint32 itemE = 54860;
//const uint32 price = 300;
//const uint32 token = 49426;
//const uint32 prating = 1;
//const int32 AP = 3000;

// sixth item
const uint32 itemF = 21176;
//const uint32 price = 300;
//const uint32 token = 49426;
//const uint32 prating = 1;
//const int32 AP = 3000;

// seventh item
const uint32 itemG = 54218;
//const uint32 price = 300;
//const uint32 token = 49426;
//const uint32 prating = 1;
//const int32 AP = 3000;

class npc_vendor : public CreatureScript
{
public:
    npc_vendor() : CreatureScript("npc_vendor") { }
        struct npc_vendorAI : public ScriptedAI
        {
            npc_vendorAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 tickTimer;
            
            void Reset()
            {
                tickTimer = 60000;
            }

            void UpdateAI(uint32 diff)
            {
                /*if (timer <= diff)
                {
                    if (Player* player = me->SelectNearestPlayer(15.0f))
                    {
                        std::string str;
                        str = (player->GetName() + MSG_ADV);
                        const char* c = str.c_str();
                        me->Whisper(c, player);
                        timer = urand(30000, 40000);
                    }
                }
                else timer -= diff;*/
                if (tickTimer <= diff)
                {
                    if (Player* player = me->SelectNearestPlayer(30.0f))
                    {
                        me->HandleEmoteCommand(ANIM_STAND);
                        me->Say(MSG_ADVA, LANG_UNIVERSAL);
                        Reset();
                    }
                    else
                    {
                        if (urand(1, 2) == 1)
                        {
                            me->Yell(MSG_ADVB, LANG_UNIVERSAL);
                        }
                        else
                        {
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DANCE);
                            me->Yell(MSG_ADVC, LANG_UNIVERSAL);
                        }
                        Reset();
                    }
                }
                else
                    tickTimer -= diff;
            }
        };
    
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_vendorAI(creature);
        }

        bool HasFreeSpace(Player* player, uint32 itemID)
        {
            ItemPosCountVec dest;
            uint32 noSpaceForCount = 0;
            uint32 count = 1;
            InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemID, count, &noSpaceForCount);
            if (msg != EQUIP_ERR_OK)
                count -= noSpaceForCount;
            if (count == 0 || dest.empty())
                return false;
            else
                return true;
        }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
			WorldSession* session = pPlayer->GetSession();
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemA, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemA, session), GOSSIP_SENDER_MAIN, ACTION_BUY_A);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemB, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemB, session), GOSSIP_SENDER_MAIN, ACTION_BUY_B);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemC, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemC, session), GOSSIP_SENDER_MAIN, ACTION_BUY_C);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemD, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemD, session), GOSSIP_SENDER_MAIN, ACTION_BUY_D);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemE, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemE, session), GOSSIP_SENDER_MAIN, ACTION_BUY_E);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemF, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemF, session), GOSSIP_SENDER_MAIN, ACTION_BUY_F);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemG, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemG, session), GOSSIP_SENDER_MAIN, ACTION_BUY_G);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }

		bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
		{
			uint32 teamARate = 0;
			uint32 teamBRate = 0;
			if (player->GetArenaTeamId(0) > 0)
				teamARate = player->GetArenaPersonalRating(0);
			if (player->GetArenaTeamId(1) > 0)
				teamBRate = player->GetArenaPersonalRating(1);
			player->PlayerTalkClass->ClearMenus();
			WorldSession* session = player->GetSession();
			std::ostringstream ssA;
			ssA << std::endl;
            ssA << "        за " << price << " x " << "|TInterface/ICONS/INV_Misc_FrostEmblem_01:30|t" << " " << sTransmogrification->GetItemLink(token, session) << std::endl;
            std::ostringstream ss;
            ss << std::endl << price << " x " << "|TInterface/ICONS/INV_Misc_FrostEmblem_01:30|t" << " " << sTransmogrification->GetItemLink(token, session) << std::endl;
            if (!(AP == 0))
            {
                ssA << "и " << AP << " x " << " " << "|TInterface/PVPFrame/PVP-ArenaPoints-Icon:30|t" << " [Очки Арены]" << " ?";
                ss << "и " << AP << " x " << " " << "|TInterface/PVPFrame/PVP-ArenaPoints-Icon:30|t" << " [Очки Арены]";
            }
            else ssA << " ?";
            if (!(prating == 1))
			{
				ss << std::endl << "|cffff0000Требование:|h|r";
				ss << std::endl << "Персональный рейтинг 2х2/3х3 арены не менее - " << prating;
			}
			switch (action)
			{
			case ACTION_BUY_A: // itemA
                if (player->HasItemCount(token, price) && (prating == 1 || (teamARate >= prating) || (teamBRate >= prating)) && (player->GetArenaPoints() >= AP))
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemA, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemA, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str(), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_A, "Вы действительно хотите купить\n" + sTransmogrification->GetItemIcon(itemA, 40, 40, -15, -10) + sTransmogrification->GetItemLink(itemA, session) + ssA.str(), 0, false);
                else
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemA, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemA, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str() + "\n|cFF8B0000У вас недостаточно средств или рейтинга для приобретения данного предмета!|r", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад...", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case ACTION_CONFIRM_A:
                if (HasFreeSpace(player, itemA))
                {
                    if (player->HasItemCount(token, price) && (player->GetArenaPoints() >= AP))
                    {
                        player->ModifyArenaPoints(-AP);
                        player->DestroyItemCount(token, price, true);
                        player->AddItem(itemA, 1);
                        creature->Whisper(MSG_THANKS, LANG_UNIVERSAL, player);
                        creature->HandleEmoteCommand(ANIM_EMOTE_KISS);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        creature->Whisper(MSG_NOT_ENOUGH_MONEY, LANG_UNIVERSAL, player);
                        player->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    creature->Whisper(MSG_BAG_ERROR, LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
            case ACTION_BUY_B: // itemB
                if (player->HasItemCount(token, price) && (prating == 1 || (teamARate >= prating) || (teamBRate >= prating)) && (player->GetArenaPoints() >= AP))
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemB, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemB, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str(), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_B, "Вы действительно хотите купить\n" + sTransmogrification->GetItemIcon(itemB, 40, 40, -15, -10) + sTransmogrification->GetItemLink(itemB, session) + ssA.str(), 0, false);
                else
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemB, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemB, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str() + "\n|cFF8B0000У вас недостаточно средств или рейтинга для приобретения данного предмета!|r", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад...", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case ACTION_CONFIRM_B:
                if (HasFreeSpace(player, itemB))
                {
                    if (player->HasItemCount(token, price) && (player->GetArenaPoints() >= AP))
                    {
                        player->ModifyArenaPoints(-AP);
                        player->DestroyItemCount(token, price, true);
                        player->AddItem(itemB, 1);
                        creature->Whisper(MSG_THANKS, LANG_UNIVERSAL, player);
                        creature->HandleEmoteCommand(ANIM_EMOTE_KISS);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        creature->Whisper(MSG_NOT_ENOUGH_MONEY, LANG_UNIVERSAL, player);
                        player->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    creature->Whisper(MSG_BAG_ERROR, LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
            case ACTION_BUY_C: // itemC
                if (player->HasItemCount(token, price) && (prating == 1 || (teamARate >= prating) || (teamBRate >= prating)) && (player->GetArenaPoints() >= AP))
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemC, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemC, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str(), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_C, "Вы действительно хотите купить\n" + sTransmogrification->GetItemIcon(itemC, 40, 40, -15, -10) + sTransmogrification->GetItemLink(itemC, session) + ssA.str(), 0, false);
                else
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemC, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemC, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str() + "\n|cFF8B0000У вас недостаточно средств или рейтинга для приобретения данного предмета!|r", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад...", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case ACTION_CONFIRM_C:
                if (HasFreeSpace(player, itemC))
                {
                    if (player->HasItemCount(token, price) && (player->GetArenaPoints() >= AP))
                    {
                        player->ModifyArenaPoints(-AP);
                        player->DestroyItemCount(token, price, true);
                        player->AddItem(itemC, 1);
                        creature->Whisper(MSG_THANKS, LANG_UNIVERSAL, player);
                        creature->HandleEmoteCommand(ANIM_EMOTE_KISS);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        creature->Whisper(MSG_NOT_ENOUGH_MONEY, LANG_UNIVERSAL, player);
                        player->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    creature->Whisper(MSG_BAG_ERROR, LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
            case ACTION_BUY_D: // itemD
                if (player->HasItemCount(token, price) && (prating == 1 || (teamARate >= prating) || (teamBRate >= prating)) && (player->GetArenaPoints() >= AP))
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemD, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemD, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str(), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_D, "Вы действительно хотите купить\n" + sTransmogrification->GetItemIcon(itemD, 40, 40, -15, -10) + sTransmogrification->GetItemLink(itemD, session) + ssA.str(), 0, false);
                else
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemD, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemD, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str() + "\n|cFF8B0000У вас недостаточно средств или рейтинга для приобретения данного предмета!|r", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад...", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case ACTION_CONFIRM_D:
                if (HasFreeSpace(player, itemD))
                {
                    if (player->HasItemCount(token, price) && (player->GetArenaPoints() >= AP))
                    {
                        player->ModifyArenaPoints(-AP);
                        player->DestroyItemCount(token, price, true);
                        player->AddItem(itemD, 1);
                        creature->Whisper(MSG_THANKS, LANG_UNIVERSAL, player);
                        creature->HandleEmoteCommand(ANIM_EMOTE_KISS);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        creature->Whisper(MSG_NOT_ENOUGH_MONEY, LANG_UNIVERSAL, player);
                        player->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    creature->Whisper(MSG_BAG_ERROR, LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
            case ACTION_BUY_E: // itemE
                if (player->HasItemCount(token, price) && (prating == 1 || (teamARate >= prating) || (teamBRate >= prating)) && (player->GetArenaPoints() >= AP))
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemE, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemE, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str(), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_E, "Вы действительно хотите купить\n" + sTransmogrification->GetItemIcon(itemE, 40, 40, -15, -10) + sTransmogrification->GetItemLink(itemE, session) + ssA.str(), 0, false);
                else
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemE, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemE, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str() + "\n|cFF8B0000У вас недостаточно средств или рейтинга для приобретения данного предмета!|r", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад...", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case ACTION_CONFIRM_E:
                if (HasFreeSpace(player, itemE))
                {
                    if (player->HasItemCount(token, price) && (player->GetArenaPoints() >= AP))
                    {
                        player->ModifyArenaPoints(-AP);
                        player->DestroyItemCount(token, price, true);
                        player->AddItem(itemE, 1);
                        creature->Whisper(MSG_THANKS, LANG_UNIVERSAL, player);
                        creature->HandleEmoteCommand(ANIM_EMOTE_KISS);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        creature->Whisper(MSG_NOT_ENOUGH_MONEY, LANG_UNIVERSAL, player);
                        player->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    creature->Whisper(MSG_BAG_ERROR, LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
            case ACTION_BUY_F: // itemF
                if (player->HasItemCount(token, price) && (prating == 1 || (teamARate >= prating) || (teamBRate >= prating)) && (player->GetArenaPoints() >= AP))
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemF, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemF, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str(), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_F, "Вы действительно хотите купить\n" + sTransmogrification->GetItemIcon(itemF, 40, 40, -15, -10) + sTransmogrification->GetItemLink(itemF, session) + ssA.str(), 0, false);
                else
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemF, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemF, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str() + "\n|cFF8B0000У вас недостаточно средств или рейтинга для приобретения данного предмета!|r", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад...", GOSSIP_SENDER_MAIN, ACTION_BACK);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case ACTION_CONFIRM_F:
                if (HasFreeSpace(player, itemF))
                {
                    if (player->HasItemCount(token, price) && (player->GetArenaPoints() >= AP))
                    {
                        player->ModifyArenaPoints(-AP);
                        player->DestroyItemCount(token, price, true);
                        player->AddItem(itemF, 1);
                        creature->Whisper(MSG_THANKS, LANG_UNIVERSAL, player);
                        creature->HandleEmoteCommand(ANIM_EMOTE_KISS);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        creature->Whisper(MSG_NOT_ENOUGH_MONEY, LANG_UNIVERSAL, player);
                        player->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    creature->Whisper(MSG_BAG_ERROR, LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
            case ACTION_BUY_G: // itemG
                if (player->HasItemCount(token, price) && (prating == 1 || (teamARate >= prating) || (teamBRate >= prating)) && (player->GetArenaPoints() >= AP))
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemG, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemG, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str(), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_G, "Вы действительно хотите купить\n" + sTransmogrification->GetItemIcon(itemG, 40, 40, -15, -10) + sTransmogrification->GetItemLink(itemG, session) + ssA.str(), 0, false);
                else
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sTransmogrification->GetItemIcon(itemG, 30, 30, -18, 0) + sTransmogrification->GetItemLink(itemG, session) + "\n|cffff0000     Цена:|h|r\n" + ss.str() + "\n|cFF8B0000У вас недостаточно средств или рейтинга для приобретения данного предмета!|r", GOSSIP_SENDER_MAIN, ACTION_BACK);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tНазад...", GOSSIP_SENDER_MAIN, ACTION_BACK);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case ACTION_CONFIRM_G:
                if (HasFreeSpace(player, itemG))
                {
                    if (player->HasItemCount(token, price) && (player->GetArenaPoints() >= AP))
                    {
                        player->ModifyArenaPoints(-AP);
                        player->DestroyItemCount(token, price, true);
                        player->AddItem(itemG, 1);
                        creature->Whisper(MSG_THANKS, LANG_UNIVERSAL, player);
                        creature->HandleEmoteCommand(ANIM_EMOTE_KISS);
                        player->CLOSE_GOSSIP_MENU();
                    }
                    else
                    {
                        creature->Whisper(MSG_NOT_ENOUGH_MONEY, LANG_UNIVERSAL, player);
                        player->CLOSE_GOSSIP_MENU();
                    }
                }
                else
                {
                    creature->Whisper(MSG_BAG_ERROR, LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
			case ACTION_BACK:
				OnGossipHello(player, creature);
				break;
			default:
				player->CLOSE_GOSSIP_MENU();
			}
			return true;
		}
};

void AddSC_npc_vendor_script()
{
    new npc_vendor();
}