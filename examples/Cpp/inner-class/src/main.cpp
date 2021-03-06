#include <stdio.h>
#include <type_traits>
#include <iostream>
#include <fstream>
#include "AutogenConfig.h"
#include "Utility/StringUtil.h"
#include "Utility/CSVReader.h"

#ifndef ASSERT
#define ASSERT assert
#endif


using namespace std;
using namespace config;

static std::string readfile(const char* filepath)
{
    std::string filename = stringPrintf("../res/%s", filepath);
    std::ifstream ifs(filename.c_str());
    std::string content((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));
    //cout << (void*)content.data() << endl;
    //cout << content << endl;
    return std::move(content);
}

static void LoadConfig(vector<config::BoxProbabilityDefine>& data) 
{
    string content = readfile("box_probability_define.csv");
    CSVReader reader(config::TAB_CSV_SEP, config::TAB_CSV_QUOTE);
    reader.Parse(content);
    auto rows = reader.GetRows();
    ASSERT(!rows.empty());
    for (size_t i = 0; i < rows.size(); i++)
    {
        auto row = rows[i];
        if (!row.empty())
        {
            config::BoxProbabilityDefine item;
            config::BoxProbabilityDefine::ParseFromRow(row, &item);
            data.push_back(item);
        }
    }
}

int main(int argc, char* argv[])
{
    vector<config::BoxProbabilityDefine> data;
    LoadConfig(data);
    cout << stringPrintf("%d box config loaded", (int)data.size());
    for (const BoxProbabilityDefine& item : data)
    {
        cout << stringPrintf("%s, %d", item.ID.c_str(), item.Total) << endl;
		for (size_t i = 0; i < item.ProbabilityGoods.size(); i++)
		{
			const BoxProbabilityDefine::ProbabilityGoodsDefine& goods = item.ProbabilityGoods[i];
			cout << stringPrintf("\t%s: %d, %d", goods.GoodsID.c_str(), goods.Num, goods.Probability) << endl;
		}
    }
    return 0;
}
