#pragma once
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <list>
#include <algorithm>
#include "fio.hpp"
class LR1
{
public:
    typedef struct item
    {
        vector<vector<string>> prods; //产生式集合
        set<string> first;            // first集合
        set<string> follow;           // follow集合
        bool rfirst = false;
        bool rfollow = false;
        size_t fsize = 0;
        vector<vector<string>> op;
    } item;
    map<string, item> Orign_Nterminal; //原非终结符集合
    set<string> Orign_Nterminal_set;   //原非终结符符号集合
    string start = "Start";
    set<string> Nterminal_set;
    void getNterminal(string in);
    set<string> terminal; //终结符集合
    void input(string in);
    string outProds(map<string, item> &N);
    typedef struct active
    {
        bool is_shift;
        bool is_acc = false;
        int status_num;
        vector<string> prods;

    } active;

    typedef struct status
    {
        vector<string> left;
        vector<vector<string>> rightA, rightB;
        vector<set<string>> lookahead;
        map<string, int> path;
        map<string, active> act;
        map<string, int> go;
        bool isdrop = false;
    } status;
    vector<status> statuses;
    void closure(status &in_status);
    int ishave(status in_status);
    void DFA();
    string outDFA();
    set<string> findFirst(string left);
    set<string> findFirstList(vector<string>::iterator its, vector<string>::iterator ite, string left);
    void first();
    vector<string> order;
    /**
     * @brief 保证开始符在第一个位置
     *
     */
    void myorder();
    /**
     * @brief 得到follow集合
     *
     */
    void follow();
    void drop_status();
    void make_table();
    string out_table();
    bool analyse(vector<string> tokens,string & med,vector<int> &num_table,vector<string> &id_table);
};
