
#include "ScriptPCH.h"

enum ActNpcRename
{
    ACT_RENAME                 = 1001,
    ACT_CUSTOMIZE              = 1002,
    ACT_CHANGE_FACTION         = 1003,
    ACT_CONFIRM_RENAME         = 1004,
    ACT_CONFIRM_CUSTOMIZE      = 1005,
    ACT_CONFIRM_CHANGE_FACTION = 1006,
    ACT_CHANGE_RACE            = 1007,
    ACT_CONFIRM_CHANGE_RACE = 1008
};

#define PRISE_RENAME_CHAR_AP 250000000   
#define PRISE_CUSTOMIZE_CHAR_AP 500000000
#define PRISE_CHANGE_RACE_AP 750000000
#define PRISE_CHANGE_FACTION_AP 1000000000

#define GOSSIP_RENAME_HELLO "Изменить свой ник!"
#define GOSSIP_CUSTOMIZE_HELLO "Поменять внешность и пол!"
#define GOSSIP_CHANGE_RACE_HELLO "Изменить рассу!"
#define GOSSIP_CHANGE_FACTION_HELLO "Сменить Фракцию (Альянс/Орда)!"

#define GOSSIP_RENAME_CONFIRM "Cтоимость смены ника 25.000 Золотых. Вас устраивает? (Требуется перезайти в игру, чтобы изменения вступили в силу)."
#define GOSSIP_CUSTOMIZE_CONFIRM "Стоимость смены внешнего вида и пола 50.000. Вас устраивает? (Требуется перезайти в игру, чтобы изменения вступили в силу)."
#define GOSSIP_CHANGE_RACE_CONFIRM "Стоимость смены рассы 75.000 золотых. Вас устраивает? (Требуется перезайти в игру, чтобы изменения вступили в силу)."
#define GOSSIP_CHANGE_FACTION_CONFIRM "Стоимость смены фракции 100.000 золотых. Вас устраивает? (Требуется перезайти в игру, чтобы изменения вступили в силу, возможны необратимые последствия – потеря магических навыков, и одежды, обмену не подлежит)."

#define MSG_DRUID_CHANGE_RACE "Вы друид, и не можете сменить расу не меняя фракции!"
#define MSG_CHANGE_RACE_ERROR "Вы не можете сменить расу не меняя фракции!" 
#define MSG_NOT_MONEY_FOR_RENAME "Вы банкрот! Проваливайте..."
#define MSG_COMPLETE_RENAME "Готово, теперь выйди из игры и зайди снова. Тебе будет предложено ввести новое имя!"
#define MSG_COMPLETE_CUSTOMIZE "Готово! Теперь выйди из игры и зайди снова. Тебе будет предложено изменить свой внешний вид и пол!"
#define MSG_COMPLETE_CHANGE_RACE "Готово! Теперь выйди из игры и зайди снова. Тебе будет предложено сменить рассу!"
#define MSG_COMPLETE_CHANGE_FACTION "Готово, теперь выйди из игры и зайди снова. Тебе будет предложено сменить фракцию (Альянс/Орда)!"

class npc_customize : public CreatureScript
{
public:
    npc_customize() : CreatureScript("npc_customize") { }
    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RENAME_HELLO, GOSSIP_SENDER_MAIN, ACT_RENAME);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CUSTOMIZE_HELLO, GOSSIP_SENDER_MAIN, ACT_CUSTOMIZE);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHANGE_RACE_HELLO, GOSSIP_SENDER_MAIN, ACT_CHANGE_RACE);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHANGE_FACTION_HELLO, GOSSIP_SENDER_MAIN, ACT_CHANGE_FACTION);		
        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
		return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        if (uiSender != GOSSIP_SENDER_MAIN)
            return false;

        pPlayer->PlayerTalkClass->ClearMenus();

        switch (uiAction)
        {
        case ACT_RENAME:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RENAME_CONFIRM, GOSSIP_SENDER_MAIN, ACT_CONFIRM_RENAME);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            break;
        case ACT_CUSTOMIZE:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CUSTOMIZE_CONFIRM, GOSSIP_SENDER_MAIN, ACT_CONFIRM_CUSTOMIZE);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            break;
        case ACT_CHANGE_RACE:
			if (pPlayer->getClassMask() == 1024)
			{
				pCreature->Whisper(MSG_DRUID_CHANGE_RACE, LANG_UNIVERSAL, pPlayer);
				break;
			}
			if (pPlayer->getClassMask() == 2 && pPlayer->getRaceMask() == 512)
			{
                pCreature->Whisper(MSG_CHANGE_RACE_ERROR, LANG_UNIVERSAL, pPlayer);
				break;
			}
			if (pPlayer->getClassMask() == 64 && pPlayer->getRaceMask() == 1024)
			{
                pCreature->Whisper(MSG_CHANGE_RACE_ERROR, LANG_UNIVERSAL, pPlayer);
				break;
			}
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHANGE_RACE_CONFIRM, GOSSIP_SENDER_MAIN, ACT_CONFIRM_CHANGE_RACE);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            break;
        case ACT_CHANGE_FACTION:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHANGE_FACTION_CONFIRM, GOSSIP_SENDER_MAIN, ACT_CONFIRM_CHANGE_FACTION);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            break;			
        case ACT_CONFIRM_RENAME:
            if (pPlayer->GetMoney() < PRISE_RENAME_CHAR_AP)
            {
                pCreature->Whisper(MSG_NOT_MONEY_FOR_RENAME, LANG_UNIVERSAL, pPlayer);
            }else{
                pCreature->Whisper(MSG_COMPLETE_RENAME, LANG_UNIVERSAL, pPlayer);
                pPlayer->ModifyMoney(-PRISE_RENAME_CHAR_AP);
                pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
            }
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case ACT_CONFIRM_CUSTOMIZE:
            if (pPlayer->GetMoney() < PRISE_CUSTOMIZE_CHAR_AP)
            {
                pCreature->Whisper(MSG_NOT_MONEY_FOR_RENAME, LANG_UNIVERSAL, pPlayer);
            }else{
                pCreature->Whisper(MSG_COMPLETE_CUSTOMIZE, LANG_UNIVERSAL, pPlayer);
                pPlayer->ModifyMoney(-PRISE_CUSTOMIZE_CHAR_AP);
                pPlayer->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
            }
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case ACT_CONFIRM_CHANGE_RACE:
            if (pPlayer->GetMoney() < PRISE_CHANGE_RACE_AP)
            {  
                pCreature->Whisper(MSG_NOT_MONEY_FOR_RENAME, LANG_UNIVERSAL, pPlayer);
            }
			else
			{
                pCreature->Whisper(MSG_COMPLETE_CHANGE_RACE, LANG_UNIVERSAL, pPlayer);
                pPlayer->ModifyMoney(-PRISE_CHANGE_RACE_AP);
                pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
            }
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case ACT_CONFIRM_CHANGE_FACTION:
            if (pPlayer->GetMoney() < PRISE_CHANGE_FACTION_AP)
            {
                pCreature->Whisper(MSG_NOT_MONEY_FOR_RENAME, LANG_UNIVERSAL, pPlayer);
            }else{
                pCreature->Whisper(MSG_COMPLETE_CHANGE_FACTION, LANG_UNIVERSAL, pPlayer);
                pPlayer->ModifyMoney(-PRISE_CHANGE_FACTION_AP);
                pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            }
            pPlayer->CLOSE_GOSSIP_MENU();
            break;			
        default:
            pPlayer->CLOSE_GOSSIP_MENU();
        }
        return true;
    }
};

void AddSC_npc_customize()
{
    new npc_customize;
}
