/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "DatabaseEnv.h"
#include "Chat.h"
#include "BracketMgr.h"
#include "CollectionMgr.h"
#include "PetBattle.h"
#include "QuestData.h"

class CustomRewardScript : public PlayerScript
{
public:
    CustomRewardScript() : PlayerScript("CustomRewardScript") { }

    void OnLogin(Player* player) override
    {
        if (sWorld->getBoolConfig(CONFIG_DISABLE_DONATE))
            return;

        if(!player)
            return;
        ObjectGuid::LowType owner_guid = player->GetGUID().GetCounter();
        ChatHandler chH = ChatHandler(player);
        uint32 accountId = player->GetSession()->GetAccountId();

        //type:
        // 1 achievement
        // 2 title
        // 3 item
        // 4 quest
        // 5 give 1 level
        // 6 level up to
        // 7 spell
        // 8 Remove pvp items from sliver char
        // 9 Remove items from char from loot
        // 10 Create garrison
        // 11 Remove title from char
        // 12 Clean pve items
        // 13 Add ban "Decency" points for char
        
        bool rewarded = false;
        QueryResult result = CharacterDatabase.PQuery("SELECT guid, type, id, count FROM `character_reward` WHERE owner_guid = '%u'", owner_guid);
        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                ObjectGuid::LowType guid = fields[0].GetUInt32();
                uint32 type = fields[1].GetUInt32();
                uint32 id = fields[2].GetUInt32();
                uint32 count = fields[3].GetUInt32();
                rewarded = false;
                switch (type)
                {
                    case 1: // Add achievement to char
                    {
                        if (AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(id))
                            player->CompletedAchievement(achievementEntry);
                        rewarded = true;
                    }
                    break;
                    case 2: // Add title to char
                    {
                        if(CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id))
                        {
                            char const* targetName = player->GetName();
                            char titleNameStr[80];
                            snprintf(titleNameStr, 80, titleInfo->Name->Str[sObjectMgr->GetDBCLocaleIndex()], targetName);
                            player->SetTitle(titleInfo);
                            chH.PSendSysMessage(LANG_TITLE_ADD_RES, id, titleNameStr, targetName);
                            rewarded = true;
                        }
                    }
                    break;
                    case 3: // Add items to char
                    {
                        if(ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(id))
                        {
                            //Adding items
                            uint32 noSpaceForCount = 0;

                            // check space and find places
                            ItemPosCountVec dest;
                            InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count, &noSpaceForCount);
                            if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
                                count -= noSpaceForCount;

                            if (count == 0 || dest.empty())                         // can't add any
                            {
                                chH.PSendSysMessage(LANG_ITEM_CANNOT_CREATE, id, noSpaceForCount);
                                break;
                            }

                            Item* item = player->StoreNewItem(dest, id, true, Item::GenerateItemRandomPropertyId(id));

                            if (count > 0 && item)
                                player->SendNewItem(item, count, false, true);

                            if (noSpaceForCount > 0)
                            {
                                chH.PSendSysMessage(LANG_ITEM_CANNOT_CREATE, id, noSpaceForCount);
                                CharacterDatabase.PExecute("UPDATE character_reward SET count = count - %u WHERE guid = %u", count, guid);
                            }
                            else
                                rewarded = true;
                        }
                    }
                    break;
                    case 4: // Add quest to char
                    {
                        //not implemented
                    }
                    break;
                    case 5: // Add 1 level to char
                    {
                        //not implemented
                    }
                    break;
                    case 6: // Level up to
                    {
                        //not implemented
                    }
                    break;
                    case 7: // Learn spell
                    {
                        //not implemented
                    }
                    break;
                    case 8:
                    {
                        CleanItems(player);
                        //player->SaveToDB();
                        rewarded = true;
                    }
                    break;
                    case 9:
                    {
                        FindItem(player, id, count);
                        //player->SaveToDB();
                        rewarded = true;
                    }
                    break;
                    case 10:
                    {
                        player->AddDelayedEvent(15000, [player, id]() -> void
                        {
                            player->CreateGarrison(id, true);
                            uint32 quest_id = id == 2 ? 34586 : 34378;
                            Quest const* quest = sQuestDataStore->GetQuestTemplate(quest_id);
                            if (player->CanAddQuest(quest, true))
                            {
                                player->AddQuest(quest, NULL);

                                player->RewardQuest(quest, 0, player);
                            }
                        });
                        rewarded = true;
                    }
                    break;
                    case 11: // Remove title from char
                    {
                        if(CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id))
                        {
                            char const* targetName = player->GetName();
                            char titleNameStr[80];
                            snprintf(titleNameStr, 80, titleInfo->Name1->Str[sObjectMgr->GetDBCLocaleIndex()], targetName);
                            player->SetTitle(titleInfo, true);
                            chH.PSendSysMessage(LANG_TITLE_REMOVE_RES, id, titleNameStr, targetName);
                            rewarded = true;
                        }
                    }
                    break;
                    case 12:
                    {
                        CleanPvEItems(player, count);
                        rewarded = true;
                    }
                    break;
                    case 13: // Mod "Decency" points for bans
                    {
                        if (auto bonus = player->GetCurrency(1160))
                        {
                            int32 count = bonus * 10 / 100;
                            player->ModifyCurrency(1160, -count, false);
                        }
                        player->ModifyCurrency(1161, 1, false);
                        rewarded = true;
                    }
                    break;
                    default:
                        break;
                }
                if(rewarded)
                    CharacterDatabase.PExecute("DELETE FROM character_reward WHERE guid = %u", guid);
            }while (result->NextRow());
        }

        if (AchievementEntry const *achiev = sAchievementStore.LookupEntry(11446))
            if (player->GetAchievementMgr()->IsCompletedAchievement(achiev, player))
                player->CompletedAchievement(achiev);

        if (!sWorld->getBoolConfig(CONFIG_ARGUSWOW_ENABLE))
        {
            if(QueryResult share_result = CharacterDatabase.PQuery("SELECT * FROM `account_share` WHERE account = '%u'", accountId))
            {
                uint32 totaltime = player->GetTotalAccountTime();
                bool update = false;
                Field* fields = share_result->Fetch();
                uint32 bonus1 = fields[1].GetBool();
                uint32 bonus2 = fields[2].GetBool();
                uint32 bonus3 = fields[3].GetBool();
                uint32 bonus4 = fields[4].GetBool();
                uint32 bonus5 = fields[5].GetBool();
                uint32 bonus6 = fields[6].GetBool();
                uint32 bonus7 = fields[7].GetBool();
                uint32 bonus8 = fields[8].GetBool();
                uint32 bonus9 = fields[9].GetBool();
                uint32 bonus10 = fields[10].GetBool();
                uint32 bonus11 = fields[11].GetBool();

                if(!bonus1 && totaltime >= (100 * HOUR))
                {
                    player->AddBattlePet(210675, BATTLEPET_FLAG_GIFT); // item 136903
                    update = true;
                    bonus1 = true;
                }
                if(!bonus2 && totaltime >= (250 * HOUR))
                {
                    player->AddBattlePet(155748, BATTLEPET_FLAG_GIFT); // item 109014
                    update = true;
                    bonus2 = true;
                }
                if(!bonus3 && totaltime >= (500 * HOUR))
                {
                    player->AddBattlePet(177234, BATTLEPET_FLAG_GIFT); // item 118517
                    update = true;
                    bonus3 = true;
                }
                if(!bonus4 && totaltime >= (1000 * HOUR))
                {
                    if (sDB2Manager.GetMount(107203))
                        player->GetCollectionMgr()->AddMount(107203, MOUNT_FLAG_GIFT, false, !player->IsInWorld()); // Item 76755

                    update = true;
                    bonus4 = true;
                }
                if(!bonus5 && totaltime >= (2500 * HOUR))
                {
                    if (sDB2Manager.GetMount(142073))
                        player->GetCollectionMgr()->AddMount(142073, MOUNT_FLAG_GIFT, false, !player->IsInWorld()); // Item 98618

                    update = true;
                    bonus5 = true;
                }
                if(!bonus6 && totaltime >= (5000 * HOUR))
                {
                    if (sDB2Manager.GetMount(189998))
                        player->GetCollectionMgr()->AddMount(189998, MOUNT_FLAG_GIFT, false, !player->IsInWorld()); // Item 128425

                    update = true;
                    bonus6 = true;
                }
                if(!bonus7 && totaltime >= (9000 * HOUR))
                {
                    if (sDB2Manager.GetMount(180545))
                        player->GetCollectionMgr()->AddMount(180545, MOUNT_FLAG_GIFT, false, !player->IsInWorld()); // Item 122469

                    update = true;
                    bonus7 = true;
                }
                if(update)
                    CharacterDatabase.PQuery("UPDATE `account_share` SET `bonus1` = '%u', `bonus2` = '%u', `bonus3` = '%u', `bonus4` = '%u', `bonus5` = '%u', `bonus6` = '%u', `bonus7` = '%u', `bonus8` = '%u', `bonus9` = '%u', `bonus10` = '%u', `bonus11` = '%u' WHERE account = '%u'", bonus1, bonus2, bonus3, bonus4, bonus5, bonus6, bonus7, bonus8, bonus9, bonus10, bonus11, accountId);
            }
            else
                CharacterDatabase.PQuery("INSERT INTO `account_share` (`account`, `bonus1`, `bonus2`, `bonus3`, `bonus4`, `bonus5`, `bonus6`, `bonus7`, `bonus8`, `bonus9`, `bonus10`, `bonus11`) values('%u','0','0','0','0','0','0','0','0','0','0','0')", accountId);
        }

        if (sWorld->getBoolConfig(CONFIG_SHARE_ENABLE))
        {
            if(QueryResult share_result = CharacterDatabase.PQuery("SELECT * FROM `character_share` WHERE guid = '%u'", owner_guid))
            {
                uint32 totaltime = player->GetTotalAccountTime();
                bool update = false;
                Field* fields = share_result->Fetch();
                uint32 bonus1 = fields[1].GetBool();
                uint32 bonus2 = fields[2].GetBool();
                uint32 bonus3 = fields[3].GetBool();
                uint32 bonus4 = fields[4].GetBool();
                uint32 bonus5 = fields[5].GetBool();
                uint32 bonus6 = fields[6].GetBool();
                uint32 bonus7 = fields[7].GetBool();
                uint32 bonus8 = fields[8].GetBool();
                uint32 bonus9 = fields[9].GetBool();
                uint32 bonus10 = fields[10].GetBool();
                uint32 bonus11 = fields[11].GetBool();

                if(!bonus1 && totaltime >= (5 * HOUR))
                {
                    player->ModifyMoney(2000000);
                    update = true;
                    bonus1 = true;
                }
                if(!bonus2 && totaltime >= (10 * HOUR))
                {
                    player->ModifyMoney(4000000);
                    update = true;
                    bonus2 = true;
                }
                if(!bonus3 && totaltime >= (20 * HOUR))
                {
                    player->ModifyMoney(8000000);
                    update = true;
                    bonus3 = true;
                }
                if(!bonus4 && totaltime >= (50 * HOUR))
                {
                    CharacterDatabase.PQuery("INSERT INTO `character_reward` (`owner_guid`, `type`, `id`, `count`) VALUES('%u','3','%u','1')", owner_guid, 72068);
                    update = true;
                    bonus4 = true;
                }
                if(update)
                    CharacterDatabase.PQuery("UPDATE `character_share` SET `bonus1` = '%u', `bonus2` = '%u', `bonus3` = '%u', `bonus4` = '%u', `bonus5` = '%u', `bonus6` = '%u', `bonus7` = '%u', `bonus8` = '%u', `bonus9` = '%u', `bonus10` = '%u', `bonus11` = '%u' WHERE guid = '%u'", bonus1, bonus2, bonus3, bonus4, bonus5, bonus6, bonus7, bonus8, bonus9, bonus10, bonus11, owner_guid);
            }
            else
                CharacterDatabase.PQuery("INSERT INTO `character_share` (`guid`, `bonus1`, `bonus2`, `bonus3`, `bonus4`, `bonus5`, `bonus6`, `bonus7`, `bonus8`, `bonus9`, `bonus10`, `bonus11`) values('%u','0','0','0','0','0','0','0','0','0','0','0')", owner_guid);
        }
    }

    void FindItem(Player* player, uint32 entry, uint32 count)
    {
        // in inventory
        for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                if(pItem->GetEntry() == entry)
                {
                    count = ItemDel(pItem, player, count);
                    if(!count)
                        return;
                }

        for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
            if (Bag* pBag = player->GetBagByPos(i))
                for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                    if (Item* pItem = pBag->GetItemByPos(j))
                        if(pItem->GetEntry() == entry)
                        {
                            count = ItemDel(pItem, player, count);
                            if(!count)
                                return;
                        }

        for (int i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                count = ItemDel(pItem, player, count);
                if(!count)
                    return;
            }

        for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                if(pItem->GetEntry() == entry)
                {
                    count = ItemDel(pItem, player, count);
                    if(!count)
                        return;
                }

        for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
            if (Bag* pBag = player->GetBagByPos(i))
                for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                    if (Item* pItem = pBag->GetItemByPos(j))
                        if(pItem->GetEntry() == entry)
                        {
                            count = ItemDel(pItem, player, count);
                            if(!count)
                                return;
                        }
    }

    uint32 ItemDel(Item* _item, Player* player, uint32 count)
    {
        uint32 tempcount = count;
        QueryResult result = CharacterDatabase.PQuery("SELECT itemEntry FROM character_donate WHERE itemguid = '%u'", _item->GetGUID().GetCounter());
        if(!result)
        {
            ChatHandler chH = ChatHandler(player);
            if (_item->GetCount() >= count)
                count = 0;
            else
                count -=_item->GetCount();

            player->DestroyItemCount(_item, tempcount, true);
            //TC_LOG_ERROR("ItemDel item delete %u, count %u, tempcount %u", _item->GetEntry(), count, tempcount);
            chH.PSendSysMessage(20021, sObjectMgr->GetItemTemplate(_item->GetEntry())->GetName()->Str[player->GetSession()->GetSessionDbLocaleIndex()]);
        }
        return count;
    }

    void CleanItems(Player* player)
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        player->m_killPoints = 0.0f;
        if (HonorInfo* honor = player->GetHonorInfo())
        {
            honor->CurrentHonorAtLevel = 0;
            honor->NextHonorLevel = 0;
            honor->PrestigeLevel = 0;
            honor->HonorLevel = 0;
        }
        player->RemoveAllPvPTalent();
        CharacterDatabase.PQuery("DELETE FROM character_brackets_info WHERE guid = '%u'", player->GetGUIDLow());

        sBracketMgr->DeleteBracketInfo(player->GetGUID());

        if (sWorld->getBoolConfig(CONFIG_FUN_OPTION_ENABLED))
        {
            CharacterDatabase.PQuery("DELETE FROM account_achievement WHERE account = '%u' AND achievement IN (247,516,231,246,245,229,512,389,396,388,604,610,611,612,614,727,907,2016,11178,230,8052,11008,11010,10991,10992,10988,10993,10995,11468,11469,11470,11471,11472,11165,1167,11169,11685,11686,11687,11688,11689,11690,11691,11692,204,201,872,199,168,166,167,200,206,713,1259,203,202,207,1172,585,711,73,165,161,154,155,158,1153,157,156,159,162,583,1169,587,216,212,208,209,783,784,214,213,1258,233,211,1171,218,219,221,222,1151,225,226,223,220,582,707,709,1166,1167,3376,3777,3845,3846,3847,3848,3849,3950,3852,3853,3854,3856,3855,3857,1310,1762,1308,1309,1765,1757,1761,2193,1763,2189,1764,2190,1766,2191,2194,5254,5255,5252,5257,5249,5256,5247,5245,5246,5248,5250,5251,5253,5262,5258,5229,5208,5231,5210,5209,5211,5213,5215,5216,5226,5219,5221,5230,5223,7100,6739,6883,7039,7049,7057,7062,7099,7102,7103,7106,6980,6950,6947,6971,6740,6882,6970,6973,6972,6981,8331,8332,8333,8350,8351,8354,8355,8359,8358,8360,1717,1755,2199,1737,1723,1727,1751,2080,1722,1721,3136,3137,1752,4586,4585,3837,3836,6108,6045,5416,5412,5718,5415,5488,5487,5486,5489,9214,9104,9256,9228,9105,9216,9218,9408,9222,9714,9106,9225,9102,8716,8716,5331,5330,5322,5327,5328,5332,5333,5334,5335,5336,5337,5359,5339,5340,5341,5357,5343,6942,699,408,397,398,399,402,406,1162,409,2090,2093,2092,2091,11697,11698,11474,11477,11476,11478,11475,11579,418,6316,6317,6124,11004,11003,11014,406,407,404,1161,10095,10094,10097,6939,8654,11030,10114,8658,12173,8652,10102,11043,8243,11018,9230,12037,11056,6940,8657,11029,10115,8659,12172,8653,10103,11042,8244,11019,9231,12041,11057,11697,420,8791,3436,8214,12090,419,11578,11698,4599,3336,6002)", player->GetSession()->GetAccountId());
            CharacterDatabase.PQuery("DELETE FROM character_achievement WHERE guid = '%u' AND achievement IN (247,516,231,246,245,229,512,389,396,388,604,610,611,612,614,727,907,2016,11178,230,8052,11008,11010,10991,10992,10988,10993,10995,11468,11469,11470,11471,11472,11165,1167,11169,11685,11686,11687,11688,11689,11690,11691,11692,204,201,872,199,168,166,167,200,206,713,1259,203,202,207,1172,585,711,73,165,161,154,155,158,1153,157,156,159,162,583,1169,587,216,212,208,209,783,784,214,213,1258,233,211,1171,218,219,221,222,1151,225,226,223,220,582,707,709,1166,1167,3376,3777,3845,3846,3847,3848,3849,3950,3852,3853,3854,3856,3855,3857,1310,1762,1308,1309,1765,1757,1761,2193,1763,2189,1764,2190,1766,2191,2194,5254,5255,5252,5257,5249,5256,5247,5245,5246,5248,5250,5251,5253,5262,5258,5229,5208,5231,5210,5209,5211,5213,5215,5216,5226,5219,5221,5230,5223,7100,6739,6883,7039,7049,7057,7062,7099,7102,7103,7106,6980,6950,6947,6971,6740,6882,6970,6973,6972,6981,8331,8332,8333,8350,8351,8354,8355,8359,8358,8360,1717,1755,2199,1737,1723,1727,1751,2080,1722,1721,3136,3137,1752,4586,4585,3837,3836,6108,6045,5416,5412,5718,5415,5488,5487,5486,5489,9214,9104,9256,9228,9105,9216,9218,9408,9222,9714,9106,9225,9102,8716,8716,5331,5330,5322,5327,5328,5332,5333,5334,5335,5336,5337,5359,5339,5340,5341,5357,5343,6942,699,408,397,398,399,402,406,1162,409,2090,2093,2092,2091,11697,11698,11474,11477,11476,11478,11475,11579,418,6316,6317,6124,11004,11003,11014,406,407,404,1161,10095,10094,10097,6939,8654,11030,10114,8658,12173,8652,10102,11043,8243,11018,9230,12037,11056,6940,8657,11029,10115,8659,12172,8653,10103,11042,8244,11019,9231,12041,11057,11697,420,8791,3436,8214,12090,419,11578,11698,4599,3336,6002)", player->GetGUIDLow());
        }

        // in inventory
        for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                ItemIfExistDel(pItem, player, -1, trans);

        for (uint8 i = REAGENT_SLOT_START; i < REAGENT_SLOT_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                ItemIfExistDel(pItem, player, -1, trans);

        for (uint8 i = CHILD_EQUIPMENT_SLOT_START; i < CHILD_EQUIPMENT_SLOT_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                ItemIfExistDel(pItem, player, -1, trans);

        for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
            if (Bag* pBag = player->GetBagByPos(i))
                for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                    if (Item* pItem = pBag->GetItemByPos(j))
                        ItemIfExistDel(pItem, player, -1, trans);

        for (int i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                ItemIfExistDel(pItem, player, -1, trans);

        for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                ItemIfExistDel(pItem, player, -1, trans);

        for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
            if (Bag* pBag = player->GetBagByPos(i))
                for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                    if (Item* pItem = pBag->GetItemByPos(j))
                        ItemIfExistDel(pItem, player, -1, trans);

        for (uint32 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
            if (VoidStorageItem* voidItem = player->GetVoidStorageItem(i))
                if (Item* pItem = voidItem->item)
                    ItemIfExistDel(pItem, player, i, trans);

        // if (!player->m_mailsLoaded)
            // player->_LoadMail();

        // for (std::unordered_map<ObjectGuid::LowType, Item*>::iterator iter = player->mMitems.begin(); iter != player->mMitems.end(); ++iter)
            // if (Item* pItem = iter->second)
                // ItemIfExistDel(pItem, player, -2);

        CharacterDatabase.CommitTransaction(trans);
    }

    void CleanPvEItems(Player* player, uint32 fromTime)
    {
        uint32 timeForDelete = fromTime - 2 * WEEK;
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        player->m_killPoints = 0.0f;

        CharacterDatabase.PQuery("DELETE FROM account_achievement WHERE account = '%u' AND achievement IN (10575,10678,10696,10697,10699,10704,10742,10754,10772,10817,10818,10819,10820,10821,10822,10823,10824,10825,10826,10827,10829,10837,10838,10839,10840,10842,10843,10844,10845,10846,10847,10848,10849,10850,10851,10866,10868,10911,10912,10913,10914,10915,10916,10917,10919,10920,10921,10922,10923,10924,10925,10926,10927,10928,10929,10930,10931,10932,10933,10934,10935,10936,10937,10938,10939,10940,10941,10942,10943,10944,10945,10946,10947,10948,10949,10950,10951,10952,10953,10954,10955,10956,10957,10959,10960,10961,10962,10963,10964,10965,10966,10967,10968,10969,10970,10971,10972,10973,10974,10975,10976,10977,10978,10979,10980,11191,11192,11194,11195,11238,11239,11337,11377,11387,11394,11396,11397,11398,11403,11404,11405,11407,11408,11409,11410,11411,11412,11413,11414,11415,11416,11417,11418,11426,11580,11581,11674,11675,11676,11683,11696,11699,11724,11767,11770,11773,11774,11775,11776,11777,11778,11779,11780,11781,11782,11784,11787,11788,11789,11790,11874,11875,11877,11878,11879,11880,11881,11882,11883,11884,11885,11886,11887,11888,11889,11890,11891,11892,11893,11894,11895,11896,11897,11898,11899,11900,11901,11902,11903,11904,11905,11906,11907,11908,11909,11910,11911,11912,11915,11928,11930,11948,11949,11954,11955,11956,11957,11958,11959,11960,11961,11962,11963,11964,11965,11966,11967,11968,11969,11970,11971,11972,11973,11974,11975,11976,11977,11978,11979,11980,11981,11982,11983,11984,11985,11986,11988,11989,11990,11991,11992,11993,11994,11995,11996,11997,11998,11999,12000,12001,12002,12030,12046,12065,12067,12110,12111,12112,12113,12117,12118,12119,12120,12121,12122,12123,12124,12125,12126,12127,12129,12257,12258)", player->GetSession()->GetAccountId());
        CharacterDatabase.PQuery("DELETE FROM character_achievement WHERE guid = '%u' AND achievement IN (10575,10678,10696,10697,10699,10704,10742,10754,10772,10817,10818,10819,10820,10821,10822,10823,10824,10825,10826,10827,10829,10837,10838,10839,10840,10842,10843,10844,10845,10846,10847,10848,10849,10850,10851,10866,10868,10911,10912,10913,10914,10915,10916,10917,10919,10920,10921,10922,10923,10924,10925,10926,10927,10928,10929,10930,10931,10932,10933,10934,10935,10936,10937,10938,10939,10940,10941,10942,10943,10944,10945,10946,10947,10948,10949,10950,10951,10952,10953,10954,10955,10956,10957,10959,10960,10961,10962,10963,10964,10965,10966,10967,10968,10969,10970,10971,10972,10973,10974,10975,10976,10977,10978,10979,10980,11191,11192,11194,11195,11238,11239,11337,11377,11387,11394,11396,11397,11398,11403,11404,11405,11407,11408,11409,11410,11411,11412,11413,11414,11415,11416,11417,11418,11426,11580,11581,11674,11675,11676,11683,11696,11699,11724,11767,11770,11773,11774,11775,11776,11777,11778,11779,11780,11781,11782,11784,11787,11788,11789,11790,11874,11875,11877,11878,11879,11880,11881,11882,11883,11884,11885,11886,11887,11888,11889,11890,11891,11892,11893,11894,11895,11896,11897,11898,11899,11900,11901,11902,11903,11904,11905,11906,11907,11908,11909,11910,11911,11912,11915,11928,11930,11948,11949,11954,11955,11956,11957,11958,11959,11960,11961,11962,11963,11964,11965,11966,11967,11968,11969,11970,11971,11972,11973,11974,11975,11976,11977,11978,11979,11980,11981,11982,11983,11984,11985,11986,11988,11989,11990,11991,11992,11993,11994,11995,11996,11997,11998,11999,12000,12001,12002,12030,12046,12065,12067,12110,12111,12112,12113,12117,12118,12119,12120,12121,12122,12123,12124,12125,12126,12127,12129,12257,12258)", player->GetGUIDLow());
        CharacterDatabase.PQuery("DELETE FROM `challenge_member` WHERE `member` = '%u';", player->GetGUIDLow());

        // in inventory
        for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                PvEItemDel(pItem, player, -1, trans, timeForDelete);

        for (uint8 i = REAGENT_SLOT_START; i < REAGENT_SLOT_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                PvEItemDel(pItem, player, -1, trans, timeForDelete);

        for (uint8 i = CHILD_EQUIPMENT_SLOT_START; i < CHILD_EQUIPMENT_SLOT_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                PvEItemDel(pItem, player, -1, trans, timeForDelete);

        for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
            if (Bag* pBag = player->GetBagByPos(i))
                for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                    if (Item* pItem = pBag->GetItemByPos(j))
                        PvEItemDel(pItem, player, -1, trans, timeForDelete);

        for (int i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_BAG_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                PvEItemDel(pItem, player, -1, trans, timeForDelete);

        for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                PvEItemDel(pItem, player, -1, trans, timeForDelete);

        for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
            if (Bag* pBag = player->GetBagByPos(i))
                for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                    if (Item* pItem = pBag->GetItemByPos(j))
                        PvEItemDel(pItem, player, -1, trans, timeForDelete);

        for (uint32 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
            if (VoidStorageItem* voidItem = player->GetVoidStorageItem(i))
                if (Item* pItem = voidItem->item)
                    PvEItemDel(pItem, player, i, trans, timeForDelete);

        CharacterDatabase.CommitTransaction(trans);
    }

    void PvEItemDel(Item* _item, Player* player, int16 slot, SQLTransaction& trans, uint32 timeForDelete)
    {
        if (_item->createdTime <= timeForDelete) // Deleted only before 2 week from ban
            return;

        if (!_item->dungeonEncounterID) // Only item from instance or WorldBoss
            return;

        ItemTemplate const* proto = _item->GetTemplate();
        if (!proto)
            return;

        // if (proto->GetArtifactID())
        // {
            // std::string sqlQuery("DELETE FROM item_instance_artifact_powers WHERE itemGuid = '" + std::to_string(_item->GetGUIDLow()) + "'");
            // trans->Append(sqlQuery.c_str());
            // sqlQuery = "DELETE FROM item_instance_artifact WHERE itemGuid = '" + std::to_string(_item->GetGUIDLow()) + "'";
            // trans->Append(sqlQuery.c_str());
            // _item->SetUInt64Value(ITEM_FIELD_ARTIFACT_XP, 0);
            // _item->ClearDynamicValue(ITEM_DYNAMIC_FIELD_ARTIFACT_POWERS);
            // _item->SetModifier(ITEM_MODIFIER_ARTIFACT_TIER, 0);
            // _item->InitArtifactPowers(_item->GetTemplate()->GetArtifactID());
            // _item->SetState(ITEM_CHANGED, player);
            // _item->SaveToDB(trans);
        // }

        ChatHandler chH = ChatHandler(player);
        chH.PSendSysMessage(20022, proto->GetName()->Str[player->GetSession()->GetSessionDbLocaleIndex()]);

        if (slot == -1) // Inventory
            player->DestroyItem(_item->GetBagSlot(), _item->GetSlot(), true);
        else if (slot == -2) // Mail
        {
            SQLTransaction temp = SQLTransaction(NULL);
            player->RemoveMItem(_item->GetGUIDLow());
            _item->FSetState(ITEM_REMOVED);
            _item->SaveToDB(temp);
        }
        else // VoidStorage
            player->DeleteVoidStorageItem(slot);
    }

    void ItemIfExistDel(Item* _item, Player* player, int16 slot, SQLTransaction& trans)
    {
        bool needDelete = false;
        if (ItemTemplate const* proto = _item->GetTemplate())
        {
            if (sWorld->getBoolConfig(CONFIG_FUN_OPTION_ENABLED))
            {
                if (proto->GetArtifactID())
                {
                    std::string sqlQuery("DELETE FROM item_instance_artifact_powers WHERE itemGuid = '" + std::to_string(_item->GetGUIDLow()) + "'");
                    trans->Append(sqlQuery.c_str());
                    sqlQuery = "DELETE FROM item_instance_artifact WHERE itemGuid = '" + std::to_string(_item->GetGUIDLow()) + "'";
                    trans->Append(sqlQuery.c_str());
                    _item->SetUInt64Value(ITEM_FIELD_ARTIFACT_XP, 0);
                    _item->ClearDynamicValue(ITEM_DYNAMIC_FIELD_ARTIFACT_POWERS);
                    _item->SetModifier(ITEM_MODIFIER_ARTIFACT_TIER, 0);
                    _item->InitArtifactPowers(_item->GetTemplate()->GetArtifactID());
                    _item->SetState(ITEM_CHANGED, player);
                    _item->SaveToDB(trans);
                }
            }

            if (_item->dungeonEncounterID == 1 && proto->IsLegionLegendary())
                needDelete = true;

            switch (proto->GetItemNameDescriptionID())
            {
                case 13295: // Legion Season 3
                case 13296: // Legion Season 3 Elite
                case 13297: // Legion Season 4
                case 13298: // Legion Season 4 Elite
                case 13299: // Legion Season 5
                case 13300: // Legion Season 5 Elite
                case 13311: // Legion Season 6
                case 13313: // Legion Season 6 Elite
                case 13312: // Legion Season 7
                case 13314: // Legion Season 7 Elite
                case 13219: // CA
                    needDelete = true;
                    break;
            }
        }

        if (needDelete)
        {
            QueryResult result = CharacterDatabase.PQuery("SELECT itemEntry FROM character_donate WHERE itemguid = '%u'", _item->GetGUID().GetCounter());
            if(!result && !_item->GetDonateItem())
            {
                ChatHandler chH = ChatHandler(player);
                chH.PSendSysMessage(20020, sObjectMgr->GetItemTemplate(_item->GetEntry())->GetName()->Str[player->GetSession()->GetSessionDbLocaleIndex()]);

                if (slot == -1) // Inventory
                    player->DestroyItem(_item->GetBagSlot(), _item->GetSlot(), true);
                else if (slot == -2) // Mail
                {
                    SQLTransaction temp = SQLTransaction(NULL);
                    player->RemoveMItem(_item->GetGUIDLow());
                    _item->FSetState(ITEM_REMOVED);
                    _item->SaveToDB(temp);
                }
                else // VoidStorage
                    player->DeleteVoidStorageItem(slot);
            }
        }
    }
};

class AccountScript : public SessionScript
{
public:
    AccountScript() : SessionScript("AccountScript") { }

    void OnLogin(WorldSession* session) override
    {
        if(!session)
            return;

        uint32 accountId = session->GetAccountId();

        //type:
        // 1 ???

        if (QueryResult result = CharacterDatabase.PQuery("SELECT guid, type, id, count FROM `account_reward` WHERE account = '%u'", accountId))
        {
            bool rewarded = false;
            do
            {
                Field* fields = result->Fetch();
                uint32 guid = fields[0].GetUInt32();
                uint32 type = fields[1].GetUInt32();
                uint32 id = fields[2].GetUInt32();
                uint32 count = fields[3].GetUInt32();
                rewarded = false;
                switch (type)
                {
                    case 1:
                    {
                        rewarded = true;
                    }
                    break;
                    default:
                        break;
                }
                if(rewarded)
                    CharacterDatabase.PExecute("DELETE FROM account_reward WHERE guid = %u", guid);
            }while (result->NextRow());
        }

    }
};

void AddSC_custom_reward()
{
    new CustomRewardScript();
    new AccountScript();
}
