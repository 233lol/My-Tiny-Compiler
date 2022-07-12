#include"include/mindfa.hpp"
void dfa2mindfa::init()
	{ //初始化
		for (int i = 0; i < maxsize; i++)
		{

			minDFA_states[i].index = i;
		}
	}
void dfa2mindfa::input(string in)
{
    stringstream sin(in);
    string temp1;
    int flag = 0;
    while (getline(sin, temp1, '\n'))
    {
        if (flag == 0) //开头读取DFA状态总数和初态
        {
            string temp2;
            stringstream tsin(temp1);
            getline(tsin, temp2, ','); //以，分割
            dfa_state_num = atoi(temp2.c_str());
            getline(tsin, temp2, ',');
            d.start_state = atoi(temp2.c_str());
            flag = 1;
        }
        else if (flag == 1) //读取第二行的字符集
        {
            string temp2;
            stringstream tsin(temp1);
            while (getline(tsin, temp2, ' ')) //以空格分割
            {
                d.terminator.insert(temp2[0]);
            }
            flag = 2;
        }
        else if (flag == 2) //读取第三行终态集
        {
            string temp2;
            stringstream tsin(temp1);
            while (getline(tsin, temp2, ' ')) //以空格分割
            {
                int inttemp = atoi(temp2.c_str());
                d.end_states.insert(inttemp);
                dfa_states[inttemp].is_end = true;
            }
            flag = 3;
        }
        else //读取剩余边集
        {

            string temp2;
            stringstream tsin(temp1);
            getline(tsin, temp2, ','); //以，分割
            int start = atoi(temp2.c_str());
            getline(tsin, temp2, ',');
            int end = atoi(temp2.c_str());
            getline(tsin, temp2, ',');
            char symbol = temp2[0];
            dfa_states[start].index = start;
            dfa_states[start].edges[dfa_states[start].edge_num].trans = end;
            dfa_states[start].edges[dfa_states[start].edge_num].input = symbol;
            dfa_states[start].edge_num++;
        }
    }
}

int dfa2mindfa::findset_num(int count, int n)
{ //当前划分总数为count，返回状态n所属的状态集标号i

    for (int i = 0; i < count; i++)
    {
        if (s[i].count(n))
        {
            return i;
        }
    }
    return -1;
}
void dfa2mindfa::minDFA()
{
    minDfa.terminator = d.terminator; //把dfa的字符集赋给minDfa

    //做第一次划分，即将终态与非终态分开
    bool end_flag = true; //判断dfa的所有状态是否全为终态的标志
    for (int i = 0; i < dfa_state_num; i++)
    {
        if (dfa_states[i].is_end == false)
        {

            end_flag = false;
            minDFA_state_num = 2; //第一次划分应该有两个集合

            s[1].insert(dfa_states[i].index); //把该状态的状态号加入s[1]集合中
        }
        else //如果该dfa状态是终态
        {
            s[0].insert(dfa_states[i].index); //把该状态的状态号加入s[0]集合中
        }
    }

    if (end_flag) //如果标志为真，则所有dfa状态都是终态
    {
        minDFA_state_num = 1; //第一次划分结束应只有一个集合
    }

    bool cut_flag = true; //上一次是否产生新的划分的标志
    while (cut_flag)      //只要上一次产生新的划分就继续循环，划分
    {

        int cut_count = 0;                         //需要产生新的划分的数量
        for (int i = 0; i < minDFA_state_num; i++) //遍历每个划分集合
        {

            for (set<char>::iterator it = d.terminator.begin(); it != d.terminator.end(); it++) //遍历dfa的字符集
            {

                int set_num = 0;
                state_set temp[20];
                for (set<int>::iterator iter = s[i].begin(); iter != s[i].end(); iter++)
                {

                    bool ep_flag = true; //判断该集合中是否存在没有该字符对应的转换弧的状态
                    for (int j = 0; j < dfa_states[*iter].edge_num; j++)
                    {

                        if (dfa_states[*iter].edges[j].input == *it) //如果该边的输入为该字符
                        {

                            ep_flag = false; //则标志为false

                            //计算该状态转换到的状态集的标号
                            int trans_num = findset_num(minDFA_state_num, dfa_states[*iter].edges[j].trans);

                            int curset_num = 0; //遍历缓冲区，寻找是否存在到达这个标号的状态集
                            while ((temp[curset_num].index != trans_num) && (curset_num < set_num))
                            {
                                curset_num++;
                            }

                            if (curset_num == set_num) //缓冲区中不存在到达这个标号的状态集
                            {

                                //在缓冲区中新建一个状态集
                                temp[set_num].index = trans_num;
                                temp[set_num].s.insert(*iter);

                                set_num++;
                            }
                            else //缓冲区中存在到达这个标号的状态集
                            {
                                temp[curset_num].s.insert(*iter);
                            }
                        }
                    }

                    if (ep_flag) //如果该状态不存在与该字符对应的转换弧
                    {

                        //寻找缓冲区中是否存在转换到标号为-1的状态集（不存在转换弧）
                        int curset_num = 0;
                        while ((temp[curset_num].index != -1) && (curset_num < set_num))
                        {
                            curset_num++;
                        }

                        if (curset_num == set_num) //如果不存在这样的状态集
                        {

                            //在缓冲区中新建一个状态集
                            temp[set_num].index = -1;
                            temp[set_num].s.insert(*iter);

                            set_num++;
                        }
                        else //缓冲区中存在到达这个标号的状态集
                        {
                            temp[curset_num].s.insert(*iter);
                        }
                    }
                }

                if (set_num > 1) //如果缓冲区中的状态集个数大于1，表示同一个状态集中的元素能转换到不同的状态集，则需要划分
                {

                    cut_count++;

                    //为每组划分创建新的dfa状态
                    for (int j = 1; j < set_num; j++) //遍历缓冲区，这里从1开始是将第0组划分留在原集合中
                    {

                        for (set<int>::iterator t = temp[j].s.begin(); t != temp[j].s.end(); t++)
                        {

                            s[i].erase(*t);
                            s[minDFA_state_num].insert(*t);
                        }

                        minDFA_state_num++;
                    }
                }
            }
        }

        if (cut_count == 0) //如果需要划分的次数为0，表示本次不需要进行划分
        {
            cut_flag = false;
        }
    }

    //遍历每个划分好的状态集，化简
    for (int i = 0; i < minDFA_state_num; i++)
    {

        for (set<int>::iterator y = s[i].begin(); y != s[i].end(); y++) //遍历集合中的每个元素
        {

            if (*y == d.start_state) //如果当前状态为dfa的初态，则该最小化DFA状态也为初态
            {
                minDfa.start_state = i;
            }

            if (d.end_states.count(*y)) //如果当前状态是终态，则该最小化DFA状态也为终态
            {

                minDFA_states[i].is_end = true;
                minDfa.end_states.insert(i);
            }

            for (int j = 0; j < dfa_states[*y].edge_num; j++) //遍历该DFA状态的每条弧，为最小化DFA创建弧
            {

                //遍历划分好的状态集合，找出该弧转移到的状态现在属于哪个集合
                for (int t = 0; t < minDFA_state_num; t++)
                {
                    if (s[t].count(dfa_states[*y].edges[j].trans))
                    {

                        bool haveEdge = false;                              //判断该弧是否已经创建的标志
                        for (int l = 0; l < minDFA_states[i].edge_num; l++) //遍历已创建的弧
                        {
                            if ((minDFA_states[i].edges[l].input == dfa_states[*y].edges[j].input) && (minDFA_states[i].edges[l].trans == t))
                            {
                                haveEdge = true; //如果该弧已经存在,标志为真
                            }
                        }

                        if (!haveEdge) //如果该弧不存在，则创建一条新的弧
                        {

                            minDFA_states[i].edges[minDFA_states[i].edge_num].input = dfa_states[*y].edges[j].input;
                            minDFA_states[i].edges[minDFA_states[i].edge_num].trans = t;
                            minDFA_states[i].edge_num++;
                        }

                        break;
                    }
                }
            }
        }
    }
}

string dfa2mindfa::output()
{
    string strtemp = "";
    strtemp += to_string(minDFA_state_num) + ",";    // minDFA状态数量
    strtemp += to_string(minDfa.start_state) + "\n"; //初态
    for (set<char>::iterator it = minDfa.terminator.begin(); it != minDfa.terminator.end(); it++)
    {
        strtemp.push_back(*it);
        strtemp += " "; //遍历字母表
    }
    strtemp += "\n";
    for (set<int>::iterator iter = minDfa.end_states.begin(); iter != minDfa.end_states.end(); iter++)
    {
        strtemp += to_string(*iter) + " "; //遍历终态
    }
    strtemp += "\n";
    for (int i = 0; i < minDFA_state_num; i++) // minDFA图
    {
        for (int j = 0; j < minDFA_states[i].edge_num; j++)
        {

            strtemp += to_string(minDFA_states[i].index) + ",";
            strtemp += to_string(minDFA_states[i].edges[j].trans) + ",";
            strtemp.push_back(minDFA_states[i].edges[j].input);
            strtemp += "\n";
        }
    }
    return strtemp;
}

string dfa2mindfa::run(string in)
{
    init();
    input(in);
    minDFA();
    return output();
}