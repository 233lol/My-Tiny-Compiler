#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <queue>
#include <cstring>
#include "include/nfa.hpp"
#include "include/fio.hpp"
#include "include/dfa.hpp"
#define maxsize 256
void nfa2dfa::input(string in) //通过字符串输入nfa并储存
{
	stringstream sin(in);
	string temp1;
	int flag = 0;
	while (getline(sin, temp1, '\n'))
	{
		if (flag == 0) //开头读取初态和终态
		{
			string temp2;
			stringstream tsin(temp1);
			getline(tsin, temp2, ','); //以，分割
			nfa_start = atoi(temp2.c_str());
			getline(tsin, temp2, ',');
			nfa_end = atoi(temp2.c_str());
			flag = 1;
			nfa_states.resize(nfa_end + 1);
			nfa_states.resize(nfa_end + 1);
			nfa_states[nfa_end].index = nfa_end;
		}
		else
		{ //读取边和状态点
			stringstream tsin(temp1);
			string a, b, c;
			getline(tsin, a, ',');
			getline(tsin, b, ',');
			getline(tsin, c, ',');
			if (c[0] != '%') //ε边的读取和储存
			{
				if (int(nfa_states.size()) < atoi(a.c_str())) //扩充状态点向量保证不溢出
				{
					nfa_states.resize(atoi(a.c_str()) + 1);
				}
				nfa_states[atoi(a.c_str())].index = atoi(a.c_str());
				nfa_states[atoi(a.c_str())].input = c[0];
				nfa_states[atoi(a.c_str())].ch_trans = atoi(b.c_str());
			}
			else //非ε边的读取和储存
			{
				if (int(nfa_states.size()) < atoi(a.c_str()))
				{
					nfa_states.resize(atoi(a.c_str()) + 1);
				}
				nfa_states[atoi(a.c_str())].index = atoi(a.c_str());
				nfa_states[atoi(a.c_str())].ep_trans.insert(atoi(b.c_str()));
			}
		}
	}
}

void nfa2dfa::init()
{ //初始化
	for (int i = 0; i < maxsize; i++)
	{

		dfa_states[i].index = i;
		dfa_states[i].is_end = false;

		for (int j = 0; j < 10; j++)
		{

			dfa_states[i].edges[j].input = '#';
			dfa_states[i].edges[j].trans = -1;
		}
	}
}
//求一个状态集的ε闭包
set<int> nfa2dfa::epcloure(set<int> s)
{

	stack<int> ep_stack;
	for (set<int>::iterator it = s.begin(); it != s.end(); it++)
	{
		ep_stack.push(*it); //将该状态集中的每一个元素都压入栈中
	}

	while (!ep_stack.empty())
	{

		int temp = ep_stack.top(); //从栈中弹出一个元素
		ep_stack.pop();
		for (set<int>::iterator iter = nfa_states[temp].ep_trans.begin(); iter != nfa_states[temp].ep_trans.end(); iter++)
		{
			if (!s.count(*iter))	  //遍历它通过ε能转换到的状态集
			{						  //如果当前元素没有在集合中出现
				s.insert(*iter);	  //则把它加入集合中
				ep_stack.push(*iter); //同时压入栈中
			}
		}
	}

	return s; //最后的s即为ε闭包
}

//求一个状态集s的ε-move
set<int> nfa2dfa::move_ep(set<int> s, char ch)
{

	set<int> temp;
	for (set<int>::iterator it = s.begin(); it != s.end(); it++) //遍历当前集合s中的每个元素
	{
		if (nfa_states[*it].input == ch) //如果对应转换边上的值为ch，ch_trans放入集合temp中
		{
			temp.insert(nfa_states[*it].ch_trans);
		}
	}

	temp = epcloure(temp); //最后求temp的ε闭包
	return temp;
}
//判断一个状态是否为终态
bool nfa2dfa::is_end(set<int> s)
{
	for (set<int>::iterator it = s.begin(); it != s.end(); it++) //遍历该状态所包含的nfa状态集
	{
		if (*it == nfa_states[nfa_end].index) //如果包含nfa的终态，则该状态为终态，返回true
		{
			return true;
		}
	}

	return false; //如果不包含，则不是终态，返回false
}
// nfa转dfa函数
nfa2dfa::dfa nfa2dfa::nfa_to_dfa()
{
	set<set<int>> states; //定义一个存储整数集合的集合，用于判断是否出现新状态

	for (size_t i = 0; i < nfa_states.size(); i++) //遍历字母表
	{
		if (isupper(nfa_states[i].input) || isdigit(nfa_states[i].input) || islower(nfa_states[i].input))
		//如果遇到非ε(#)，则把它加入到dfa的终结符集中
		{
			d.terminator.insert(nfa_states[i].input);
		}
	}

	d.start_state = 0; // dfa的初态为0

	set<int> tempSet;
	tempSet.insert(nfa_states[nfa_start].index); //将nfa的初态加入到集合中

	dfa_states[0].closure = epcloure(tempSet);			  //求dfa的初态
	dfa_states[0].is_end = is_end(dfa_states[0].closure); //判断初态是否为终态

	dfa_state_num++;

	queue<int> q;
	q.push(d.start_state); //把dfa的初态存入队列中

	while (!q.empty()) //只要队列不为空，就一直循环
	{

		int num = q.front(); //出去队首元素
		q.pop();
		for (set<char>::iterator it = d.terminator.begin(); it != d.terminator.end(); it++) //遍历终结符集
		{

			set<int> temp = move_ep(dfa_states[num].closure, *it); //计算每个终结符的ε-闭包(move(ch))
			if (!states.count(temp) && !temp.empty())			   //如果求出来的状态集不为空且与之前求出来的状态集不同，则新建一个DFA状态
			{

				states.insert(temp); //将新求出来的状态集加入到状态集合中

				dfa_states[dfa_state_num].closure = temp;

				dfa_states[num].edges[dfa_states[num].edgeNum].input = *it;			  //该状态边的输入即为当前终结符
				dfa_states[num].edges[dfa_states[num].edgeNum].trans = dfa_state_num; //边转移到的状态为最后一个DFA状态
				dfa_states[num].edgeNum++;

				dfa_states[dfa_state_num].is_end = is_end(dfa_states[dfa_state_num].closure); //判断是否为终态

				q.push(dfa_state_num); //将新的状态号加入队列中

				dfa_state_num++;
			}
			else //求出来的状态集在之前求出的某个状态集相同
			{
				for (int i = 0; i < dfa_state_num; i++) //遍历之前求出来的状态集合
				{
					if (temp == dfa_states[i].closure) //找到与该集合相同的DFA状态
					{

						dfa_states[num].edges[dfa_states[num].edgeNum].input = *it; //该状态边的输入即为当前终结符
						dfa_states[num].edges[dfa_states[num].edgeNum].trans = i;
						dfa_states[num].edgeNum++;

						break;
					}
				}
			}
		}
	}

	//计算dfa的终态集
	for (int i = 0; i < dfa_state_num; i++) //遍历dfa的所有状态
	{
		if (dfa_states[i].is_end == true) //如果该状态是终态
		{
			d.end_states.insert(i); //则将该状态号加入到DFA的终态集中
		}
	}

	return d;
}

//输出DFA转换函数
string nfa2dfa::output()
{
	string strtemp = "";
	strtemp += to_string(dfa_state_num) + ",";	// DFA状态数量
	strtemp += to_string(d.start_state) + "\n"; //初态
	for (set<char>::iterator it = d.terminator.begin(); it != d.terminator.end(); it++)
	{
		strtemp.push_back(*it);
		strtemp += " "; //遍历字母表
	}
	strtemp += "\n";
	for (set<int>::iterator iter = d.end_states.begin(); iter != d.end_states.end(); iter++)
	{
		strtemp += to_string(*iter) + " "; //遍历终态
	}
	strtemp += "\n";
	for (int i = 0; i < dfa_state_num; i++) // DFA图
	{
		for (int j = 0; j < dfa_states[i].edgeNum; j++)
		{

			strtemp += to_string(dfa_states[i].index) + ",";
			strtemp += to_string(dfa_states[i].edges[j].trans) + ",";
			strtemp.push_back(dfa_states[i].edges[j].input);
			strtemp += "\n";
		}
	}
	return strtemp;
}
string nfa2dfa::run(string in) //运行
{
	input(in);
	init();
	nfa_to_dfa();
	return output();
}