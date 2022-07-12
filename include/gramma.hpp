#pragma once
#include <iostream>
#include "fio.hpp"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include "LR1.hpp"
#include "LALR1.hpp"
using namespace std;
class gramma
{
public:
    vector<int> num_table;
    vector<string> id_table;
    string run(string srule,string token,string &med);
    vector<string> convert(string token);
};
