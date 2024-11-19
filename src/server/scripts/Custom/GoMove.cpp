#include <math.h>
#include "Cell.h"
#include "Chat.h"
#include "ChatTextBuilder.h"
#include "GameObject.h"
#include "GoMove.h"
#include "GridNotifiers.h"
#include "Map.h"
#include "MapManager.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ChatPackets.h"
#include <G3D/Quat.h>

GameObjectStore GOMove::Store;

void GOMove::SendAddonMessage(Player* player, char const* msg)
{
    if (!player || !msg)
        return;

    WorldPackets::Chat::Chat packet;
    packet.Initialize(CHAT_MSG_ADDON, LANG_ADDON, player, player, msg, 0, "", DEFAULT_LOCALE, "GOMOVE\t%s");
    player->SendDirectMessage(packet.Write());
}

GameObject * GOMove::GetGameObject(Player * player, ObjectGuid::LowType lowguid)
{
    if (GameObjectData const* data = sObjectMgr->GetGOData(lowguid))
        return ChatHandler(player->GetSession()).GetObjectGlobalyWithGuidOrNearWithDbGuid(lowguid, data->id);
    return nullptr;
}

void GOMove::SendAdd(Player * player, GameObject* go)
{
    if (!go)
        return;
    char msg[256];
    snprintf(msg, 256, "ADD|%u|%s|%u", go->GetGUIDLow(), go->GetName(), go->GetEntry());
    SendAddonMessage(player, msg);
}

void GOMove::SendRemove(Player * player, ObjectGuid::LowType lowguid)
{
    char msg[256];
    snprintf(msg, 256, "REMOVE|%llu||0", lowguid);

    SendAddonMessage(player, msg);
}

void GOMove::DeleteGameObject(GameObject * object)
{
    if (!object)
        return;

    // copy paste .gob del command
    ObjectGuid ownerGuid = object->GetOwnerGUID();
    if (ownerGuid)
    {
        Unit* owner = ObjectAccessor::GetUnit(*object, ownerGuid);
        if (owner && owner->GetTypeId() == TYPEID_PLAYER)
            owner->RemoveGameObject(object, false);
    }
    object->SetRespawnTime(0);
    object->Delete();
    object->DeleteFromDB();
}

GameObject * GOMove::SpawnGameObject(Player* player, float x, float y, float z, float o, uint32 p, uint32 entry)
{
    if (!player || !entry)
        return nullptr;

    if (!MapManager::IsValidMapCoord(player->GetMapId(), x, y, z))
        return nullptr;

    Position pos;
    pos.Relocate(x, y, z, o);

    // copy paste .gob add command
    GameObjectTemplate const* objectInfo = sObjectMgr->GetGameObjectTemplate(entry);
    if (!objectInfo)
        return nullptr;

    if (objectInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(objectInfo->displayId))
        return nullptr;

    Map* map = player->GetMap();

    GameObject* object = new GameObject();
    ObjectGuid::LowType guidLow = sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate();

    if (!object->Create(guidLow, objectInfo->entry, map, player->GetPhaseMgr().GetPhaseMaskForSpawn(), pos, G3D::Quat(), 255, GO_STATE_READY))
    {
        delete object;
        return nullptr;
    }

    // fill the gameobject data and save to the db
    object->SaveToDB(map->GetId(), (UI64LIT(1) << map->GetSpawnMode()), p);
    // guidLow = object->GetSpawnId();

    // delete the old object and do a clean load from DB with a fresh new GameObject instance.
    // this is required to avoid weird behavior and memory leaks
    delete object;

    object = new GameObject();
    // this will generate a new guid if the object is in an instance
    if (!object->LoadGameObjectFromDB(guidLow, map))
    {
        delete object;
        return nullptr;
    }

    sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));

    if (object)
        SendAdd(player, object);
    return object;
}

GameObject * GOMove::MoveGameObject(Player* player, float x, float y, float z, float o, uint32 p, ObjectGuid::LowType lowguid)
{
    if (!player)
        return nullptr;
    GameObject* object = nullptr;
    if (GameObjectData const* data = sObjectMgr->GetGOData(lowguid))
        object = ChatHandler(player->GetSession()).GetObjectGlobalyWithGuidOrNearWithDbGuid(lowguid, data->id);
    if (!object)
    {
        SendRemove(player, lowguid);
        return nullptr;
    }

    if (!MapManager::IsValidMapCoord(object->GetMapId(), x, y, z))
        return nullptr;

    // copy paste .gob move command
    // copy paste .gob turn command
    // object->Relocate(x, y, z, o);
    // object->SetWorldRotationAngles(o, 0, 0);
    // object->SaveToDB();
    SpawnGameObject(player, x, y, z, o, p, object->GetEntry()); 
    DeleteGameObject(object);
    
    SendRemove(player, lowguid);


    return object;
}

void GameObjectStore::SpawnQueAdd(ObjectGuid const& guid, uint32 entry)
{
    WriteGuard lock(_objectsToSpawnLock);
    objectsToSpawn[guid] = entry;
}

void GameObjectStore::SpawnQueRem(ObjectGuid const& guid)
{
    WriteGuard lock(_objectsToSpawnLock);
    objectsToSpawn.erase(guid);
}

uint32 GameObjectStore::SpawnQueGet(ObjectGuid const& guid)
{
    WriteGuard lock(_objectsToSpawnLock);
    auto it = objectsToSpawn.find(guid);
    if (it != objectsToSpawn.end())
        return it->second;
    return 0;
}

std::list<GameObject*> GOMove::GetNearbyGameObjects(Player* player, float range)
{
    float x, y, z;
    player->GetPosition(x, y, z);

    std::list<GameObject*> objects;
    Trinity::GameObjectInRangeCheck check(x, y, z, range);
    Trinity::GameObjectListSearcher<Trinity::GameObjectInRangeCheck> searcher(player, objects, check);
    player->GetMap()->VisitGrid(player->GetPositionX(), player->GetPositionY(), SIZE_OF_GRIDS, searcher);
    return objects;
};

class GOMove_commandscript : public CommandScript
{
public:
    GOMove_commandscript() : CommandScript("GOMove_commandscript")
    {
    }

    enum commandIDs
    {
        TEST,
        SELECTNEAR,
        DELET,
        X,
        Y,
        Z,
        O,
        GROUND,
        FLOOR,
        RESPAWN,
        GOTO,
        FACE,

        SPAWN,
        NORTH,
        EAST,
        SOUTH,
        WEST,
        NORTHEAST,
        NORTHWEST,
        SOUTHEAST,
        SOUTHWEST,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        PHASE,
        SELECTALLNEAR,
        SPAWNSPELL,
    };

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> GOMoveCommandTable=
        {
            { "gomove", SEC_ADMINISTRATOR, false, &GOMove_Command, ""},
            
        };
        return GOMoveCommandTable;
    }

    static bool GOMove_Command(ChatHandler* handler, const char* args)
    {
        if (!args)
            return false;

        char* ID_t = strtok(const_cast<char*>(args), " ");
        if (!ID_t)
            return false;
        uint32 ID = static_cast<uint32>(atoi(ID_t));

        ObjectGuid::LowType lowguid = 0;
        if (char* cLowguid = strtok(nullptr, " "))
            lowguid = static_cast<uint64>(atoi(cLowguid));

        char* ARG_t = strtok(nullptr, " ");
        uint32 ARG = 0;
        if (ARG_t)
            ARG = static_cast<uint32>(atoi(ARG_t));

        WorldSession* session = handler->GetSession();
        if (!session)
            return false;
        Player* player = session->GetPlayer();

        if (ID < SPAWN) // no args
        {
            if (ID >= DELET && ID <= GOTO) // has target (needs retrieve gameobject)
            {
                GameObject* target = GOMove::GetGameObject(player, lowguid);
                if (!target)
                    ChatHandler(player->GetSession()).PSendSysMessage("Object GUID: %u not found.", lowguid);
                else
                {
                    float x, y, z, o;
                    target->GetPosition(x, y, z, o);
                    uint32 p = target->GetPhaseMask();
                    switch (ID)
                    {
                    case DELET: GOMove::DeleteGameObject(target); GOMove::SendRemove(player, lowguid); break;
                    case X: GOMove::MoveGameObject(player, player->GetPositionX(), y, z, o, p, lowguid);       break;
                    case Y: GOMove::MoveGameObject(player, x, player->GetPositionY(), z, o, p, lowguid);       break;
                    case Z: GOMove::MoveGameObject(player, x, y, player->GetPositionZ(), o, p, lowguid);       break;
                    case O: GOMove::MoveGameObject(player, x, y, z, player->GetOrientation(), p, lowguid);     break;
                    case RESPAWN: GOMove::SpawnGameObject(player, x, y, z, o, p, target->GetEntry());                   break;
                    case GOTO:
                    {
                        // stop flight if need
                        if (player->isInFlight())
                        {
                            player->GetMotionMaster()->MovementExpired();
                            player->CleanupAfterTaxiFlight();
                        }
                        // save only in non-flight case
                        else
                            player->SaveRecallPosition();
                        player->TeleportTo(target->GetMapId(), x, y, z, o);
                    } break;
                    case GROUND:
                    {
                        float ground = target->GetMap()->GetHeight(target->GetPhases(), x, y, MAX_HEIGHT);
                        if (ground != INVALID_HEIGHT)
                            GOMove::MoveGameObject(player, x, y, ground, o, p, lowguid);
                    } break;
                    case FLOOR:
                    {
                        float floor = target->GetMap()->GetHeight(target->GetPhases(), x, y, z);
                        if (floor != INVALID_HEIGHT)
                            GOMove::MoveGameObject(player, x, y, floor, o, p, lowguid);
                    } break;
                    }
                }
            }
            else
            {
                switch (ID)
                {
                case TEST:
                    session->SendNotification("%s", player->GetName());
                    break;
                case FACE:
                {
                    float const piper2 = float(M_PI) / 2.0f;
                    float const multi = player->GetOrientation() / piper2;
                    float const multi_int = floor(multi);
                    float const new_ori = (multi - multi_int > 0.5f) ? (multi_int + 1)*piper2 : multi_int*piper2;
                    player->SetFacingTo(new_ori);
                } break;
                case SELECTNEAR:
                {
                    GameObject* object = handler->GetNearbyGameObject();
                    if (!object)
                        ChatHandler(player->GetSession()).PSendSysMessage("No objects found");
                    else
                    {
                        GOMove::SendAdd(player, object);
                        session->SendNotification("Selected %s", object->GetName());
                    }
                } break;
                }
            }
        }
        else if (ARG && ID >= SPAWN)
        {
            if (ID >= NORTH && ID <= PHASE)
            {
                GameObject* target = GOMove::GetGameObject(player, lowguid);
                if (!target)
                    ChatHandler(player->GetSession()).PSendSysMessage("Object GUID: %u not found", lowguid);
                else
                {
                    float x, y, z, o;
                    target->GetPosition(x, y, z, o);
                    uint32 p = target->GetPhaseMask();
                    switch (ID)
                    {
                    case NORTH: GOMove::MoveGameObject(player, x + ((float)ARG / 100), y, z, o, p, lowguid);                            break;
                    case EAST: GOMove::MoveGameObject(player, x, y - ((float)ARG / 100), z, o, p, lowguid);                             break;
                    case SOUTH: GOMove::MoveGameObject(player, x - ((float)ARG / 100), y, z, o, p, lowguid);                            break;
                    case WEST: GOMove::MoveGameObject(player, x, y + ((float)ARG / 100), z, o, p, lowguid);                             break;
                    case NORTHEAST: GOMove::MoveGameObject(player, x + ((float)ARG / 100), y - ((float)ARG / 100), z, o, p, lowguid);   break;
                    case SOUTHEAST: GOMove::MoveGameObject(player, x - ((float)ARG / 100), y - ((float)ARG / 100), z, o, p, lowguid);   break;
                    case SOUTHWEST: GOMove::MoveGameObject(player, x - ((float)ARG / 100), y + ((float)ARG / 100), z, o, p, lowguid);   break;
                    case NORTHWEST: GOMove::MoveGameObject(player, x + ((float)ARG / 100), y + ((float)ARG / 100), z, o, p, lowguid);   break;
                    case UP: GOMove::MoveGameObject(player, x, y, z + ((float)ARG / 100), o, p, lowguid);                               break;
                    case DOWN: GOMove::MoveGameObject(player, x, y, z - ((float)ARG / 100), o, p, lowguid);                             break;
                    case RIGHT: GOMove::MoveGameObject(player, x, y, z, o - ((float)ARG / 100), p, lowguid);                            break;
                    case LEFT: GOMove::MoveGameObject(player, x, y, z, o + ((float)ARG / 100), p, lowguid);                             break;
                    case PHASE: GOMove::MoveGameObject(player, x, y, z, o, ARG, lowguid);                                               break;
                    }
                }
            }
            else
            {
                switch (ID)
                {
                case SPAWN:
                {
                    if (GOMove::SpawnGameObject(player, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), player->GetPhaseMgr().GetPhaseMaskForSpawn(), ARG))
                        GOMove::Store.SpawnQueAdd(player->GetGUID(), ARG);
                } break;
                case SPAWNSPELL:
                {
                    GOMove::Store.SpawnQueAdd(player->GetGUID(), ARG);
                } break;
                case SELECTALLNEAR:
                {
                    for (GameObject * go : GOMove::GetNearbyGameObjects(player, static_cast<float>(ARG)))
                        GOMove::SendAdd(player, go);
                } break;
                }
            }
        }
        else
            return false;
        return true;
    }
};

// possible spells:
// 27651, 897

class GOMove_spell_place : public SpellScriptLoader
{
public:
    GOMove_spell_place() : SpellScriptLoader("GOMove_spell_place") { }

    class GOMove_spellscript : public SpellScript
    {
        PrepareSpellScript(GOMove_spellscript);

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;
            const WorldLocation* summonPos = GetExplTargetDest();
            if (!summonPos)
                return;
            if (uint32 entry = GOMove::Store.SpawnQueGet(player->GetGUID()))
                GOMove::SpawnGameObject(player, summonPos->GetPositionX(), summonPos->GetPositionY(), summonPos->GetPositionZ(), player->GetOrientation(), player->GetPhaseMgr().GetPhaseMaskForSpawn(), entry);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(GOMove_spellscript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new GOMove_spellscript();
    }
};

class GOMove_player_track : public PlayerScript
{
public:
    GOMove_player_track() : PlayerScript("GOMove_player_track") { }

    void OnLogout(Player* player) override
    {
        GOMove::Store.SpawnQueRem(player->GetGUID());
    }
};

void AddSC_GOMove_commandscript()
{
    new GOMove_commandscript();
    new GOMove_spell_place();
}