#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include "fio.hpp"
#include "nfa.hpp"
#include "dfa.hpp"
#include "mindfa.hpp"
#include "lex.hpp"
class regexlex
{
public:
    string regextomindfa(string temp);
    string run(string srule,string code);
    bool check_error(string token);
};
