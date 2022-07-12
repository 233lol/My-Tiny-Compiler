#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;
struct state
{
    string sta_name;
    bool operator<(const state right) const //重载<
    {
        return sta_name < right.sta_name;
    }
};
struct edge
{
    state edg_start;
    state edg_end;
    char edg_symbol;
};
class nfa
{
public:
    class nfa_unit
    { //记录NFA数据
    public:
        set<state> K;             //状态集合
        set<string> symbol_table; //字母表
        vector<edge> f;           //状态映射
        state S;                  //开始状态
        state Z;                  //终止状态
        nfa_unit()
        { //初始化构造函数
            f.clear();
            K.clear();
            symbol_table.clear();
            Z = state();
            S = state();
        }
        nfa_unit(const nfa_unit &other)
        { //拷贝函数
            K = other.K;
            symbol_table = other.symbol_table;
            f = other.f;
            S = other.S;
            Z = other.Z;
        }
        nfa_unit &operator=(const nfa_unit &other)
        { //重载=
            if (this != &other)
            {
                K = other.K;
                symbol_table = other.symbol_table;
                f = other.f;
                Z = other.Z;
                S = other.S;
            }
            return *this;
        }
        nfa_unit &operator+=(const nfa_unit &other)
        {
            K.insert(other.K.begin(), other.K.end());
            symbol_table.insert(other.symbol_table.begin(), other.symbol_table.end());
            f.insert(f.end(), other.f.begin(), other.f.end());
            return *this;
        }
    };

private:
    nfa_unit Data;
    string expr;
    int sta_num;

public:
    nfa();                                    //构造函数
    nfa(const nfa &other);                    //拷贝构造
    nfa &operator=(const nfa &other);         //重载=
    nfa_unit get_nfa() const { return Data; } //得到数据
    string get_expr() const { return expr; }  //得到表达式

    void input(string input); //输入
    void to_nfa();            //转成NFA
    string output() const;        //返回NFA
    string run(string regex);               //运行
private:
    void change();                      //处理便于表达式转换
    void postexp();                     //中缀转后缀
    int in_priority(const char) const;  //栈内优先级
    int out_priority(const char) const; //栈外优先级
    nfa_unit Or(nfa_unit, nfa_unit);    //整合a|b
    nfa_unit mul(nfa_unit, nfa_unit);   //整合ab
    nfa_unit star(nfa_unit);            //整合a*
    nfa_unit unit(const char);          //整合单元
};