// This file is auto-generated by taxi v1.2.0, DO NOT EDIT!

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


// parse value from text
template <typename T>
inline T ParseValue(StringPiece text)
{
    text = trimWhitespace(text);
    if (text.empty())
    {
        return T();
    }
    return to<T>(text);
}


namespace autogen
{

std::function<std::string(const char*)> AutogenConfigManager::reader = AutogenConfigManager::ReadFileContent;

namespace 
{
    static std::vector<SoldierPropertyDefine>* _instance_soldierpropertydefine = nullptr;
}

void AutogenConfigManager::LoadAll()
{
    ASSERT(reader);
    SoldierPropertyDefine::Load("soldier_property_define.csv");
}

void AutogenConfigManager::ClearAll()
{
    delete _instance_soldierpropertydefine;
    _instance_soldierpropertydefine = nullptr;
}

//Load content of an asset file
std::string AutogenConfigManager::ReadFileContent(const char* filepath)
{
    ASSERT(filepath != nullptr);
    std::ifstream ifs(filepath);
    ASSERT(!ifs.fail());
    std::string content;
    content.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    return std::move(content);
}


const std::vector<SoldierPropertyDefine>* SoldierPropertyDefine::GetData()
{
    ASSERT(_instance_soldierpropertydefine != nullptr);
    return _instance_soldierpropertydefine;
}

const SoldierPropertyDefine* SoldierPropertyDefine::Get(const std::string& Name, int Level)
{
    const vector<SoldierPropertyDefine>* dataptr = GetData();
    ASSERT(dataptr != nullptr && dataptr->size() > 0);
    for (size_t i = 0; i < dataptr->size(); i++)
    {
        if (dataptr->at(i).Name == Name && dataptr->at(i).Level == Level)
        {
            return &dataptr->at(i);
        }
    }
    return nullptr;
}

std::vector<const SoldierPropertyDefine*> SoldierPropertyDefine::GetRange(const std::string& Name)
{
    const vector<SoldierPropertyDefine>* dataptr = GetData();
    std::vector<const SoldierPropertyDefine*> range;
    ASSERT(dataptr != nullptr && dataptr->size() > 0);
    for (size_t i = 0; i < dataptr->size(); i++)
    {
        if (dataptr->at(i).Name == Name)
        {
            range.push_back(&dataptr->at(i));
        }
    }
    return range;
}

// load data from csv file
int SoldierPropertyDefine::Load(const char* filepath)
{
    vector<SoldierPropertyDefine>* dataptr = new vector<SoldierPropertyDefine>;
    string content = AutogenConfigManager::reader(filepath);
    MutableStringPiece sp((char*)content.data(), content.size());
    sp.replaceAll("\r\n", " \n");
    auto lines = Split(sp, "\n");
    ASSERT(!lines.empty());
    for (size_t i = 0; i < lines.size(); i++)
    {
        auto line = trimWhitespace(lines[i]);
        if (!line.empty())
        {
            const auto& row = Split(lines[i], ",");
            if (!row.empty())
            {
                SoldierPropertyDefine item;
                SoldierPropertyDefine::ParseFromRow(row, &item);
                dataptr->push_back(item);
            }
        }
    }
    delete _instance_soldierpropertydefine;
    _instance_soldierpropertydefine = dataptr;
    return 0;
}

// parse data object from an csv row
int SoldierPropertyDefine::ParseFromRow(const vector<StringPiece>& row, SoldierPropertyDefine* ptr)
{
    ASSERT(row.size() >= 24);
    ASSERT(ptr != nullptr);
    ptr->Name = ParseValue<std::string>(row[0]);
    ptr->Level = ParseValue<int>(row[1]);
    ptr->NameID = ParseValue<std::string>(row[2]);
    ptr->Description = ParseValue<std::string>(row[3]);
    ptr->BuildingName = ParseValue<std::string>(row[4]);
    ptr->BuildingLevel = ParseValue<uint32_t>(row[5]);
    ptr->RequireSpace = ParseValue<uint32_t>(row[6]);
    ptr->Volume = ParseValue<uint32_t>(row[7]);
    ptr->UpgradeTime = ParseValue<uint32_t>(row[8]);
    ptr->UpgradeMaterialID = ParseValue<std::string>(row[9]);
    ptr->UpgradeMaterialNum = ParseValue<int>(row[10]);
    ptr->ConsumeMaterial = ParseValue<std::string>(row[11]);
    ptr->ConsumeMaterialNum = ParseValue<int>(row[12]);
    ptr->ConsumeTime = ParseValue<int>(row[13]);
    ptr->Act = ParseValue<int>(row[14]);
    ptr->Hp = ParseValue<int>(row[15]);
    ptr->BombLoad = ParseValue<int>(row[16]);
    ptr->Hurt = ParseValue<uint32_t>(row[17]);
    ptr->Duration = ParseValue<float>(row[18]);
    ptr->TriggerInterval = ParseValue<float>(row[19]);
    ptr->SearchScope = ParseValue<float>(row[20]);
    ptr->AtkFrequency = ParseValue<float>(row[21]);
    ptr->AtkRange = ParseValue<float>(row[22]);
    ptr->MovingSpeed = ParseValue<float>(row[23]);
    return 0;
}


} // namespace autogen 