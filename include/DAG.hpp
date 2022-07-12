#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
using namespace std;
class DAG
{
public:
    vector<vector<vector<string>>> blocks(string in);
    class status
    {
    public:
        int left = -1;
        int right = -1;
        int order = -1;
        vector<string> uvar;
        vector<string> muvar;
        bool must = false;
        int dep_num = 0;
        string rv;
        string op;
        set<string> val;
    };
    int findnum(vector<status> ts, string num);
    int findtemp(vector<status> ts, string temp);
    int findid(vector<status> ts, string id);
    int aishave(vector<status> s, status t);
    int ishave(vector<status> s, status t);
    bool havevar(status t);
    vector<vector<string>> toDAG(vector<vector<vector<string>>> blocks);
    string vtos(vector<vector<string>> med);
};
