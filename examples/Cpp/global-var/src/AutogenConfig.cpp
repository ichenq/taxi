// This file is auto-generated by Tabular v1.4.0, DO NOT EDIT!

#include "AutogenConfig.h"
#include <stddef.h>
#include <assert.h>
#include <memory>
#include <fstream>
#include "Utility/Conv.h"
#include "Utility/StringUtil.h"

using namespace std;

#ifndef ASSERT
#define ASSERT assert
#endif


namespace config
{

// parse data object from csv rows
int GlobalPropertyDefine::ParseFromRows(const vector<vector<StringPiece>>& rows, GlobalPropertyDefine* ptr)
{
    ASSERT(rows.size() >= 12 && rows[0].size() >= 2);
    ASSERT(ptr != nullptr);
    ptr->GoldExchangeTimeFactor1 = ParseTextAs<double>(rows[0][2]);
    ptr->GoldExchangeTimeFactor2 = ParseTextAs<double>(rows[1][2]);
    ptr->GoldExchangeTimeFactor3 = ParseTextAs<double>(rows[2][2]);
    ptr->GoldExchangeResource1Price = ParseTextAs<uint16_t>(rows[3][2]);
    ptr->GoldExchangeResource2Price = ParseTextAs<uint16_t>(rows[4][2]);
    ptr->GoldExchangeResource3Price = ParseTextAs<uint16_t>(rows[5][2]);
    ptr->GoldExchangeResource4Price = ParseTextAs<uint16_t>(rows[6][2]);
    ptr->FreeCompleteSeconds = ParseTextAs<uint16_t>(rows[7][2]);
    ptr->CancelBuildReturnPercent = ParseTextAs<uint16_t>(rows[8][2]);
    ptr->EnableSearch = ParseTextAs<bool>(rows[9][2]);
    {
        const auto& array = Split(rows[10][2], TAB_ARRAY_DELIM, true);
        for (size_t i = 0; i < array.size(); i++)
        {
            ptr->SpawnLevelLimit.push_back(ParseTextAs<int>(array[i]));
        }
    }
    {
        const auto& dict = Split(rows[11][2], TAB_MAP_DELIM1, true);
        for (size_t i = 0; i < dict.size(); i++)
        {
            const auto& kv = Split(dict[i], TAB_MAP_DELIM2, true);
            ASSERT(kv.size() == 2);
            if(kv.size() == 2)
            {
                const auto& key = ParseTextAs<std::string>(kv[0]);
                ASSERT(ptr->FirstRechargeReward.count(key) == 0);
                ptr->FirstRechargeReward[key] = ParseTextAs<int>(kv[1]);
            }
        }
    }
    return 0;
}


} // namespace config 
