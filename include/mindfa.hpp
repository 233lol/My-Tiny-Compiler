#pragma once
#include <iostream>
#include "fio.hpp"
#include <vector>
#include <set>
#include <string>
#include <cstring>
#define maxsize 256
class dfa2mindfa
{
public:
    struct edge //定义DFA的转换边
    {

        char input = '#'; //边上的值
        int trans = -1;   //边所指向的状态号
    };

    struct dfa_state //定义DFA状态
    {

        bool is_end = false; //是否为终态

        int index; // DFA状态的状态号

        int edge_num = 0; // DFA状态上的射出边数
        edge edges[10];   // DFA状态上的射出边
    };

    dfa_state dfa_states[maxsize]; // DFA状态数组
    int dfa_state_num = 0;         // DFA状态总数

    struct dfa //定义DFA结构
    {

        int start_state; // DFA的初态

        set<int> end_states;  // DFA的终态集
        set<char> terminator; // DFA的字符集
    };
    dfa d;                            // DFA
    set<int> s[maxsize];              //划分出来的集合数组
    dfa_state minDFA_states[maxsize]; // minDFA状态数组

    int minDFA_state_num = 0; // minDFA的状态总数，同时也是划分出的集合数
    struct state_set //划分状态集
    {

        int index;  //该状态集所能转换到的状态集标号
        set<int> s; //该状态集中的DFA状态号
    };
    void input(string in);
    int findset_num(int count, int n);
    dfa minDfa;
    void minDFA();
    string output();
    string run(string in);
    void init();
};