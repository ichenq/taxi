// This file is auto-generated by Tabular v1.4.0, DO NOT EDIT!

#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "Utility/Range.h"


namespace config
{


static const char TAB_CSV_SEP = ',';       // CSV field separator
static const char TAB_CSV_QUOTE = '"';     // CSV field quote
static const char* TAB_ARRAY_DELIM = ",";  // array item delimiter
static const char* TAB_MAP_DELIM1 = ";";   // map item delimiter
static const char* TAB_MAP_DELIM2 = "=";   // map key-value delimiter

// 新手引导配置
struct NewbieGuideDefine 
{
    std::string                      Name;                  // ID
    std::string                      Type;                  // 任务类型
    std::string                      Target;                // 目标
    std::vector<int16_t>             Accomplishment;        // 完成步骤
    std::map<std::string, uint32_t>  Goods;                 // 物品
    std::string                      Description;           // 描述

    static int ParseFromRow(const std::vector<StringPiece>& row, NewbieGuideDefine* ptr);
};

} // namespace config
