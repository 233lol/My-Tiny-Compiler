#pragma once
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "fio.hpp"
using namespace std;
class mindfa
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
    vector<dfa_state> dfa_states;
    int dfa_state_num = 0; // DFA状态总数
    struct dfa             //定义DFA结构
    {

        int start_state; // DFA的初态

        set<int> end_states;  // DFA的终态集
        set<char> terminator; // DFA的字符集
    };
    dfa d; // minDFA
    void input_rule(string in);
};
class id : public mindfa//标识符类
{
public:
    bool isid(string word);//判断是否是标识符
};
class num : public mindfa//数字类
{
    public:
    bool isnum(string word);//判断是否是数字
};
class lexical
{
public:
    vector<string> codes; //储存输入后按空格分割成的词
    id lid;
    num lnum;
    set<string> key;
    set<string> symbol;
    string remove_comment(string in);//移除注释函数
    void input_code(string in);//处理输入代码
    void input_key(string in);//处理输入关键字
    void input_symbol(string in);//处理输入符号
    string judge(string word);//判断
    string output();//输出，按token内容 token属性输出
};