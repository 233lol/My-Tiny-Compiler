#pragma once
#include<iostream>
#include"nfa.hpp"
#include"fio.hpp"
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <queue>
#include <cstring>
#include "nfa.hpp"
#include "fio.hpp"
#define maxsize 256
class nfa2dfa
{
public:
	// NFA数据结构
	struct nfa_state //定义NFA状态
	{

		int index; // NFA状态的状态号

		char input = '#'; // NFA状态边上的值,默认为#
		int ch_trans;	  // NFA状态边转移到的状态号

		set<int> ep_trans; //当前状态通过ε转移到的状态号集合
	};
	vector<nfa_state> nfa_states;
	int nfa_start, nfa_end;
	// DFA数据结构
	struct edge //定义DFA的转换边
	{

		char input; //边上的值
		int trans;	//边所指向的状态号
	};

	struct dfa_state //定义DFA状态
	{

		bool is_end; //是否为终态

		int index;		  // DFA状态的状态号
		set<int> closure; // NFA的ε闭包

		int edgeNum;	// DFA状态上的射出边数
		edge edges[10]; // DFA状态上的射出边
	};

	dfa_state dfa_states[maxsize]; // DFA状态数组
	int dfa_state_num = 0;		   // DFA状态总数

	struct dfa //定义DFA结构
	{

		int start_state; // DFA的初态

		set<int> end_states;  // DFA的终态集
		set<char> terminator; // DFA的终结符集
	};
	dfa d;				  // DFA
    void input(string in);
    void init();
    set<int> epcloure(set<int> s);
    set<int> move_ep(set<int> s, char ch);
    bool is_end(set<int> s);
    dfa nfa_to_dfa();
    string output();
    string run(string in);
};