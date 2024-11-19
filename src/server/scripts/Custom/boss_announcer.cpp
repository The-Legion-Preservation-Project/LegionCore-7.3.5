#include "ScriptMgr.h"
#include "Config.h"
#include <Player.h>
#include "World.h"

class boss_announcer : public PlayerScript
{
public:
	boss_announcer() : PlayerScript("boss_announcer") {}

	void OnCreatureKill(Player* player, Creature* boss)
	{
		if (boss->isWorldBoss())
			{
			std::string plr = player->GetName();
			std::string boss_n = boss->GetName();
			bool ingroup = player->GetGroup();
			std::string tag_colour = "7bbef7";
			std::string plr_colour = "7bbef7";
			std::string boss_colour = "ff0000";
			std::ostringstream stream;
			stream << "|CFF" << tag_colour <<
				"|r|cff" << plr_colour << " " << plr <<
				/*"|r 's group killed |CFF" << boss_colour << "[" << boss_n << "]|r " "boss" << "!";
			sWorld->SendServerMessage(SERVER_MSG_STRING, stream.str().c_str());*/
			"|rEl grupo de|r""|r|cff" << plr_colour << " " << plr << "|r mató a"" |CFF" << boss_colour << "[" << boss_n << "]|r " "que es un jefe" << "! Enhorabuena";
			sWorld->SendServerMessage(SERVER_MSG_STRING, stream.str().c_str());
			}
		
	};
};

void AddSC_boss_announcer()
{
	new boss_announcer();
}