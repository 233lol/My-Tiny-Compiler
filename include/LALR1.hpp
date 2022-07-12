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
using namespace std;
class LALR1 : public LR1{
    public:
    int ishave2(status in_status, size_t number);
    void replace(size_t in1, size_t in2);
    void reduce();

};