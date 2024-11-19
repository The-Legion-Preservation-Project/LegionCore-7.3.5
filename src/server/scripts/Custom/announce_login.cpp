#include "ScriptMgr.h"

class announce_login : public PlayerScript
{
public:
    announce_login() : PlayerScript("announce_login") { }

    void OnLogin(Player* player, bool /*loginFirst*/)
    {
		//if (sConfigMgr->GetBoolDefault("Login.Announcer.Enable", true))
		//{
        
            if (player->GetTeam() == ALLIANCE)
            {
                std::ostringstream ss;
                ss << "|cff3DAEFF[ Anuncio de conexion ]|cffFFD800 : jugador|cff4CFF00 " << player->GetName() << " |cffFFD800 esta Online. Este jugador es |cff0026FF ALIANZA";
                sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
            }
            else
            {
                std::ostringstream ss;
                ss << "|cff3DAEFF[ Anuncio de Conexion ]|cffFFD800 : jugador |cff4CFF00 " << player->GetName() << " |cffFFD800 esta Online. Este jugador es |cffFF0000 HORDA";
                sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
            }
        
		//}
    }
};

void AddSC_announce_login()
{
    new announce_login;
}