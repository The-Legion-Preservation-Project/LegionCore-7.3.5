#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "World.h"
#include "LFGMgr.h"
#include "Chat.h"
#include "Opcodes.h"

class lfg_solo : public PlayerScript
{
public:
    lfg_solo() : PlayerScript("lfg_solo") { }

    void OnLogin(Player* player)
    {
        if (sConfigMgr->GetIntDefault("LFG.SoloMode", true))
        {
            if (!sLFGMgr->IsSoloLFG())
            {
                sLFGMgr->ToggleSoloLFG();
            }
        }
    }
};

void AddLfgSoloScripts()
{
    new lfg_solo();
}
