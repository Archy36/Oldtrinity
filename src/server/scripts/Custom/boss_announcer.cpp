#include "ScriptPCH.h"
#include "InstanceScript.h"
#include "Group.h"

class boss_announcer : public PlayerScript
{
public:
    boss_announcer() : PlayerScript("boss_announcer") {}

    void OnCreatureKill(Player* player, Creature* boss)
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (instance->instance->GetId() == 631 || instance->instance->GetId() == 724 || instance->instance->GetId() == 603)
            {
                if (player->GetGroup() && boss->isWorldBoss())
                {
                    uint32 count = instance->instance->GetPlayersCountExceptGMs();
                    uint32 maxcount = 0;
                    uint32 diff = instance->instance->GetDifficulty();
                    std::string rdiff = "";
                    std::string nameLink = ChatHandler(player->GetSession()).playerLink(player->GetGroup()->GetLeaderName());
                    std::string bname;
                    switch (diff)
                    {
                        case 0:
                        {
                            rdiff = "10об";
                            maxcount = 10;
                            break;
                        }
                        case 1:
                        {
                            rdiff = "25об";
                            maxcount = 25;
                            break;
                        }
                        case 2:
                        {
                            rdiff = "10г";
                            maxcount = 10;
                            break;
                        }
                        case 3:
                        {
                            rdiff = "25г";
                            maxcount = 25;
                            break;
                        }
                        default:
                            break;
                    }
                    switch (boss->GetEntry())
                    {
                        case 37972:
                        case 37973:
                        case 37970:
                            return;
                            break;
                        case 38008:
                            bname = "Совет Принцев Крови";
                            break;
                        default:
                            bname = boss->GetNameForLocaleIdx(LOCALE_ruRU).c_str();
                            break;
                    }
                    char msg[250];
                    snprintf(msg, 250, "|cFFFF0000[Сервер]:|r Рейд [%u/%u] во главе с %s одержал победу над |CFF18BE00[%s] [%s]|r", count, maxcount, nameLink.c_str(), bname.c_str(), rdiff.c_str());
                    sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
                }
            }
        }
    }
};

void AddSC_boss_announcer()
{
    new boss_announcer();
}