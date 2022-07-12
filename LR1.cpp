#include "include/LR1.hpp"
using namespace std;

void LR1::getNterminal(string in)
{
    stringstream sin(in);
    string temp1;
    while (getline(sin, temp1, '\n'))
    {
        stringstream stemp1(temp1);
        string item;
        stemp1 >> item;
        Orign_Nterminal_set.insert(item);
    }
}
void LR1::input(string in)
{
    getNterminal(in);
    stringstream sin(in);
    string temp1;
    int mystartflag = 0;
    while (getline(sin, temp1, '\n'))
    {
        string action;
        int index = temp1.find_first_of("#");
        if (index != -1)
        {
            action = temp1.substr(index + 1, temp1.size());
            temp1 = temp1.substr(0, index);
        }
        if (!temp1.compare("")) //相等返回0
            continue;
        int flag = 0; //未获取产生式左部状态
        string left;
        item right;
        vector<string> tprod; //临时产生式
        stringstream stemp1(temp1);
        string titem;
        while (stemp1 >> titem)
        {
            if (flag == 0)
            {
                if (mystartflag == 0)
                {
                    mystartflag = 1;
                    item mytitem;
                    vector<string> mytprod;
                    mytprod.push_back(titem);
                    mytitem.prods.push_back(mytprod);
                    Orign_Nterminal_set.insert(start);
                    Orign_Nterminal.insert(make_pair(start, mytitem));
                    left = titem;
                    flag = 1;
                    continue;
                }
                left = titem;
                flag = 1;
                continue;
            }
            else if (flag == 1)
            {
                if (!titem.compare("->"))
                {
                    flag = 2;
                    continue;
                }
                else
                {
                    cout << "ERROR:is not ->" << endl;
                    exit(1);
                }
            }
            else if (flag == 2)
            {
                if (titem == "|")
                {
                    //将产生式压入众产生式集合
                    right.prods.push_back(tprod);
                    tprod.clear();
                }
                else
                {
                    if (!Orign_Nterminal_set.count(titem))
                    {
                        if (titem != "%")
                        {
                            terminal.insert(titem);
                        }
                    }
                    tprod.push_back(titem);
                }
            }
        }
        if (flag == 2)
        {
            if (!tprod.empty())
            {
                right.prods.push_back(tprod);
            }
            vector<string> tempop;
            if (action.size() == 0)
            {
                tempop.push_back("^");
            }
            else
            {
                stringstream tact(action);
                string top_t;
                while (tact >> top_t)
                {
                    tempop.push_back(top_t);
                }
            }
            if (Orign_Nterminal.count(left))
            {
                Orign_Nterminal[left].prods.insert(Orign_Nterminal[left].prods.end(), right.prods.begin(), right.prods.end());
                Orign_Nterminal[left].op.push_back(tempop);
            }
            else
            {
                Orign_Nterminal.insert(pair<string, item>(left, right));
                Orign_Nterminal[left].op.push_back(tempop);
            }
        }
        else
        {
            cout << "ERROR: flag is not 2" << endl;
            exit(1);
        }
    }
}

string LR1::outProds(map<string, item> &N)
{
    stringstream temp;
    temp << start << " -> " << N[start].prods[0][0] << endl;
    for (auto iter : N)
    {
        if (iter.first == start)
        {
            continue;
        }
        for (auto it1 : iter.second.prods)
        {
            temp << iter.first << " -> ";
            for (auto it2 : it1)
            {
                temp << it2 << " ";
            }
            temp << endl;
        }
    }
    return temp.str();
}

void LR1::closure(status &in_status)
{
    int flag = 0;
    while (flag == 0)
    {
        flag = 1;
        for (size_t i = 0; i < in_status.left.size(); i++)
        {
            if (Orign_Nterminal_set.count(in_status.rightB[i][0]))
            {
                for (auto iter : Orign_Nterminal[in_status.rightB[i][0]].prods)
                {
                    int tflag = 0;
                    for (size_t j = 0; j < in_status.left.size(); j++)
                    {
                        if (in_status.left[j] == in_status.rightB[i][0] && in_status.rightA[j][0] == "^" && in_status.rightB[j] == iter)
                        {
                            set<string> tset;
                            if (in_status.rightB[i].size() == 1)
                            {
                                tset.insert(in_status.lookahead[i].begin(), in_status.lookahead[i].end());
                            }
                            else
                            {
                                if (Orign_Nterminal_set.count(in_status.rightB[i][1]))
                                {
                                    tset.insert(Orign_Nterminal[in_status.rightB[i][1]].first.begin(), Orign_Nterminal[in_status.rightB[i][1]].first.end());
                                }
                                else
                                {
                                    tset.insert(in_status.rightB[i][1]);
                                }
                            }
                            size_t tsize = in_status.lookahead[j].size();
                            in_status.lookahead[j].insert(tset.begin(), tset.end());
                            if (tsize != in_status.lookahead[j].size())
                            {
                                flag = 0;
                            }
                            tflag = 1;
                            break;
                        }
                    }
                    if (tflag == 0)
                    {
                        flag = 0;
                        in_status.left.push_back(in_status.rightB[i][0]);
                        in_status.rightA.push_back(vector<string>({"^"}));
                        in_status.rightB.push_back(iter);
                        set<string> tset;
                        if (in_status.rightB[i].size() == 1)
                        {
                            tset.insert(in_status.lookahead[i].begin(), in_status.lookahead[i].end());
                        }
                        else
                        {
                            if (Orign_Nterminal_set.count(in_status.rightB[i][1]))
                            {
                                tset.insert(Orign_Nterminal[in_status.rightB[i][1]].first.begin(), Orign_Nterminal[in_status.rightB[i][1]].first.end());
                            }
                            else
                            {
                                tset.insert(in_status.rightB[i][1]);
                            }
                        }

                        in_status.lookahead.push_back(tset);
                    }
                }
            }
        }
    }
}
int LR1::ishave(status in_status)
{
    for (size_t i = 0; i < statuses.size(); i++)
    {
        if (statuses[i].left.size() != in_status.left.size())
        {
            continue;
        }
        size_t count = 0;
        for (size_t j = 0; j < in_status.left.size(); j++)
        {
            for (size_t k = 0; k < statuses[i].left.size(); k++)
            {
                if (statuses[i].left[k] == in_status.left[j] && statuses[i].rightA[k] == in_status.rightA[j] && statuses[i].rightB[k] == in_status.rightB[j] && statuses[i].lookahead[k] == in_status.lookahead[j])
                {
                    count++;
                    break;
                }
            }
        }
        if (count == in_status.left.size())
        {
            return i;
        }
    }
    return -1;
}
void LR1::DFA()
{
    status t;
    t.left.push_back(start);
    t.rightA.push_back(vector<string>({"^"}));
    t.rightB.push_back(Orign_Nterminal[start].prods[0]);
    t.lookahead.push_back(set<string>({"$"}));
    first();
    closure(t);
    statuses.push_back(t);
    for (size_t i = 0; i < statuses.size(); i++)
    {
        for (size_t j = 0; j < statuses[i].left.size(); j++)
        {
            if (statuses[i].rightB[j].size() == 1 && statuses[i].rightB[j][0] == "^")
            {
                continue;
            }
            string tleft = statuses[i].left[j];
            vector<string> trightA, trightB;
            string tsymbol = statuses[i].rightB[j][0];
            set<string> tset = statuses[i].lookahead[j];
            if (statuses[i].rightA[j].size() == 1 && statuses[i].rightA[j][0] == "^")
            {
                trightA.push_back(tsymbol);
                if (statuses[i].rightB[j].size() == 1)
                {
                    trightB.push_back("^");
                }
                else
                {
                    for (size_t k = 1; k < statuses[i].rightB[j].size(); k++)
                    {
                        trightB.push_back(statuses[i].rightB[j][k]);
                    }
                }
            }
            else
            {
                for (size_t k = 0; k < statuses[i].rightA[j].size(); k++)
                {
                    trightA.push_back(statuses[i].rightA[j][k]);
                }
                trightA.push_back(tsymbol);
                if (statuses[i].rightB[j].size() == 1)
                {
                    trightB.push_back("^");
                }
                else
                {
                    for (size_t k = 1; k < statuses[i].rightB[j].size(); k++)
                    {
                        trightB.push_back(statuses[i].rightB[j][k]);
                    }
                }
            }
            if (statuses[i].path.count(tsymbol))
            {
                status tstatus = statuses[statuses[i].path[tsymbol]];
                tstatus.left.push_back(tleft);
                tstatus.rightA.push_back(trightA);
                tstatus.rightB.push_back(trightB);
                tstatus.lookahead.push_back(tset);
                closure(tstatus);
                int place = ishave(tstatus);

                if (place == -1)
                {
                    statuses[statuses[i].path[tsymbol]] = tstatus;
                }
                else
                {
                    statuses[i].path[tsymbol] = place;
                }
            }
            else
            {
                status tstatus;
                tstatus.left.push_back(tleft);
                tstatus.rightA.push_back(trightA);
                tstatus.rightB.push_back(trightB);
                tstatus.lookahead.push_back(tset);
                closure(tstatus);
                int place = ishave(tstatus);

                if (place == -1)
                {
                    statuses[i].path.insert(make_pair(tsymbol, statuses.size()));
                    statuses.push_back(tstatus);
                }
                else
                {
                    statuses[i].path.insert(make_pair(tsymbol, place));
                }
            }
        }
    }
}
string LR1::outDFA()
{
    stringstream temp;
    for (size_t i = 0; i < statuses.size(); i++)
    {
        temp << i << endl;
        for (size_t j = 0; j < statuses[i].left.size(); j++)
        {
            temp << statuses[i].left[j] << " -> ";
            for (auto iter : statuses[i].rightA[j])
            {
                if (iter == "^")
                {
                    continue;
                }
                temp << iter << " ";
            }
            temp << ".";
            for (auto iter : statuses[i].rightB[j])
            {
                if (iter == "^")
                {
                    continue;
                }
                temp << iter << " ";
            }
            temp << endl;
        }
        for (auto iter : statuses[i].path)
        {
            temp << iter.first << " " << iter.second << endl;
        }
        temp << endl
             << endl;
    }

    return temp.str();
}
set<string> LR1::findFirst(string left)
{
    set<string> tempset;

    //终结符或者空
    if (terminal.count(left) || !left.compare("%"))
    {
        tempset.insert(left);
    }
    else
    {
        if (Orign_Nterminal[left].rfirst)
        { //快速返回first集
            return Orign_Nterminal[left].first;
        }
        item right(Orign_Nterminal[left]);
        for (auto it1 : right.prods)
        {
            //调用函数：返回串的first集合
            set<string> get = findFirstList(it1.begin(), it1.end(), left);
            tempset.insert(get.begin(), get.end());
        }
    }
    return tempset;
}
/**
 * @brief 返回串的first集合
 *
 * @param its
 * @param ite
 * @return set<string>
 */

set<string> LR1::findFirstList(vector<string>::iterator its, vector<string>::iterator ite, string left)
{
    set<string> myset;

    for (vector<string>::iterator it = its; it != ite; it++)
    {
        if (*it == left)
        {
            break;
        }
        set<string> get = findFirst(*it);
        if (!terminal.count(*it) && (*it).compare("%"))
        {
            Orign_Nterminal[*it].first.insert(get.begin(), get.end());
            Orign_Nterminal[*it].rfirst = true;
        }
        myset.insert(get.begin(), get.end());

        if (!(get.count("%") && it != ite - 1))
            break;
        myset.erase("%");
    }

    return myset;
}
/**
 * @brief 生成first集合
 *
 */

void LR1::first()
{
    myorder();
    for (auto iter : order)
    {
        set<string> get = findFirst(iter);
        Orign_Nterminal[iter].first.insert(get.begin(), get.end());
        Orign_Nterminal[iter].rfirst = true;
    }
}

/**
 * @brief 保证开始符在第一个位置
 *
 */
void LR1::myorder()
{
    Orign_Nterminal[start].follow.insert("$");
    order.push_back(start);
    Orign_Nterminal[start].fsize = 1;
    Nterminal_set.insert(start);
    for (auto iter : Orign_Nterminal)
    {
        if (iter.first == start)
        {
            continue;
        }
        order.push_back(iter.first);
        Nterminal_set.insert(iter.first);
    }
}
/**
 * @brief 得到follow集合
 *
 */
void LR1::follow()
{
    int flag = 1;
    while (flag != 0)
    {
        flag = 0;
        for (auto iter : order)
        {
            for (auto &it1 : Orign_Nterminal[iter].prods)
            {
                for (auto it2 = it1.begin(); it2 != it1.end(); it2++)
                {
                    if (!Nterminal_set.count(*it2))
                    {
                        continue;
                    }
                    if (it2 == it1.end() - 1)
                    {
                        Orign_Nterminal[*it2].follow.insert(
                            Orign_Nterminal[iter].follow.begin(),
                            Orign_Nterminal[iter].follow.end());
                    }
                    else
                    {
                        if (!Nterminal_set.count(*(it2 + 1)))
                        {
                            Orign_Nterminal[*it2].follow.insert(*(it2 + 1));
                        }
                        else
                        {
                            Orign_Nterminal[*it2].follow.insert(
                                Orign_Nterminal[*(it2 + 1)].first.begin(),
                                Orign_Nterminal[*(it2 + 1)].first.end());
                            if (Orign_Nterminal[*it2].follow.count("%"))
                            {
                                Orign_Nterminal[*it2].follow.insert(
                                    Orign_Nterminal[*(it2 + 1)].follow.begin(),
                                    Orign_Nterminal[*(it2 + 1)].follow.end());
                                Orign_Nterminal[*it2].follow.erase("%");
                            }
                        }
                    }
                }
            }
            if (Orign_Nterminal[iter].follow.size() != Orign_Nterminal[iter].fsize)
            {
                flag = 1;
                Orign_Nterminal[iter].fsize = Orign_Nterminal[iter].follow.size();
            }
        }
    }
}
void LR1::drop_status()
{
    set<int> not_drop_set;
    for (size_t i = 0; i < statuses.size(); i++)
    {
        for (auto iter : statuses[i].path)
        {
            not_drop_set.insert(iter.second);
        }
    }
    for (size_t i = 1; i < statuses.size(); i++)
    {
        if (!not_drop_set.count(i))
        {
            statuses[i].isdrop = true;
        }
    }
}

void LR1::make_table()
{
    drop_status();
    for (size_t i = 0; i < statuses.size(); i++)
    {
        if (statuses[i].isdrop)
        {
            continue;
        }
        for (size_t j = 0; j < statuses[i].left.size(); j++)
        {
            if (statuses[i].rightB[j][0] == "^")
            {
                active tactive;
                if (statuses[i].left[j] == start)
                {
                    tactive.is_shift = false;
                    tactive.is_acc = true;
                    statuses[i].act.insert(make_pair("$", tactive));
                    continue;
                }
                tactive.is_shift = false;
                vector<string> tprod;
                tprod.push_back(statuses[i].left[j]);
                tprod.insert(tprod.end(), statuses[i].rightA[j].begin(), statuses[i].rightA[j].end());
                tactive.prods = tprod;
                for (auto iter : statuses[i].lookahead[j])
                {
                    statuses[i].act.insert(make_pair(iter, tactive));
                }
            }
            else
            {
                for (auto iter : statuses[i].path)
                {
                    if (Orign_Nterminal_set.count(iter.first))
                    {
                        statuses[i].go.insert(iter);
                    }
                    else
                    {
                        active tact;
                        tact.is_shift = true;
                        tact.status_num = iter.second;
                        statuses[i].act.insert(make_pair(iter.first, tact));
                    }
                }
            }
        }
    }
}
string LR1::out_table()
{
    stringstream temp;
    for (size_t i = 0; i < statuses.size(); i++)
    {
        temp << i << endl;
        temp << "action" << endl;
        for (auto iter : statuses[i].act)
        {
            if (iter.second.is_shift)
            {
                temp << iter.first << " s " << iter.second.status_num << endl;
            }
            else if (iter.second.is_acc)
            {
                temp << iter.first << " acc" << endl;
            }
            else
            {
                temp << iter.first << " r ";
                int flag = 0;
                for (auto it2 : iter.second.prods)
                {
                    if (flag == 0)
                    {
                        temp << it2 << " -> ";
                        flag = 1;
                        continue;
                    }
                    temp << it2 << " ";
                }
                temp << endl;
            }
        }
        temp << "go" << endl;
        for (auto iter : statuses[i].go)
        {
            temp << iter.first << " " << iter.second << endl;
        }
    }
    return temp.str();
}
bool LR1::analyse(vector<string> tokens, string &med, vector<int> &num_table, vector<string> &id_table)
{
    int temp_id = 0;
    int Label_id = 0;
    class sitem
    {
    public:
        string token;
        int num_id = -1;
        int id_id = -1;
        string pmed;
        int temp_id = -1;
        int status;
    };
    class stokenitem
    {
    public:
        string token;
        int num_id = -1;
        int id_id = -1;
    };
    vector<sitem> st;
    tokens.push_back("$");
    sitem tsitem;
    tsitem.token = "$";
    tsitem.status = 0;
    st.push_back(tsitem);
    vector<stokenitem> sinput;
    int cnt_num = num_table.size()-1;
    int cnt_id = id_table.size()-1;
    for (auto iter = tokens.rbegin(); iter != tokens.rend(); iter++)
    {
        stokenitem t;
        t.token = *iter;

        if (*iter == "num")
        {
            t.num_id = cnt_num;
            cnt_num--;
        }
        else if (*iter == "id")
        {
            t.id_id = cnt_id;
            cnt_id--;
        }
        sinput.push_back(t);
    }

    while (!st.empty())
    {
        int tstatus = st.back().status;
        if (!statuses[tstatus].act.count(sinput.back().token))
        {
            if (!statuses[tstatus].act.count("&"))
            {
                cout<<"GRAMMA ERROR! MAYBE AT:"<<endl;
                for (size_t i = 0; i < sinput.size(); i++)
                {
                    if(i>10){
                        break;
                    }
                    if(sinput[sinput.size()-1-i].token=="id"){
                        cout<<id_table[sinput[sinput.size()-1-i].id_id]<<" ";
                    }else if (sinput[sinput.size()-1-i].token=="num")
                    {
                        cout<<num_table[sinput[sinput.size()-1-i].num_id]<<" ";
                    }else{
                        cout<<sinput[sinput.size()-1-i].token<<" ";
                    }
                }
                cout<<endl;
                return false;
            }
            else
            {
                stokenitem t;
                t.token = "&";
                sinput.push_back(t);
            }
        }
        active tactive = statuses[tstatus].act[sinput.back().token];
        if (tactive.is_shift)
        {
            sitem ttsitem;
            ttsitem.status = tactive.status_num;
            ttsitem.token = sinput.back().token;
            ttsitem.num_id = sinput.back().num_id;
            ttsitem.id_id = sinput.back().id_id;
            st.push_back(ttsitem);
            sinput.pop_back();
        }
        else if (tactive.is_acc)
        {
            return true;
        }
        else
        {
            string tN = tactive.prods[0];
            vector<string> tp;
            for (size_t i = 1; i < tactive.prods.size(); i++)
            {
                tp.push_back(tactive.prods[i]);
            }

            sitem ttsitem;
            size_t tindex = 0;
            for (size_t i = 0; i < Orign_Nterminal[tactive.prods[0]].prods.size(); i++)
            {
                if (Orign_Nterminal[tactive.prods[0]].prods[i] == tp)
                {
                    tindex = i;
                    break;
                }
            }
            vector<sitem> pttsitem;
            for (size_t i = 0; i < tactive.prods.size() - 1; i++)
            {
                pttsitem.push_back(st.back());
                st.pop_back();
            }
            int ttstatus = st.back().status;
            if (!statuses[ttstatus].go.count(tN))
            {
                cout<<"GRAMMA ERROR! MAYBE AT:"<<endl;
                for (size_t i = 0; i < sinput.size(); i++)
                {
                    if(i>10){
                        break;
                    }
                    if(sinput[sinput.size()-1-i].token=="id"){
                        cout<<id_table[sinput[sinput.size()-1-i].id_id]<<" ";
                    }else if (sinput[sinput.size()-1-i].token=="num")
                    {
                        cout<<num_table[sinput[sinput.size()-1-i].num_id]<<" ";
                    }else{
                        cout<<sinput[sinput.size()-1-i].token<<" ";
                    }
                }
                cout<<endl;
                return false;
            }
            else
            {
                ttsitem.status = statuses[ttstatus].go[tN];
                ttsitem.token = tN;
                if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "mul")
                {
                    ttsitem.pmed = "*";
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "div")
                {
                    ttsitem.pmed = "/";
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "add")
                {
                    ttsitem.pmed = "+";
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "sub")
                {
                    ttsitem.pmed = "-";
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "cmp<")
                {
                    ttsitem.pmed = "<";
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "cmp=")
                {
                    ttsitem.pmed = "==";
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "rd")
                {
                    stringstream tmed;
                    tmed << "IN " << id_table.at(pttsitem[0].id_id) << endl;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "wr")
                {
                    stringstream tmed;
                    tmed << "OUT " << id_table.at(pttsitem[0].id_id) << endl;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "eqnum")
                {
                    stringstream tmed;
                    tmed << "EQU "
                         << "temp" << temp_id << " " << num_table.at(pttsitem[0].num_id) << endl;
                    ttsitem.temp_id = temp_id;
                    temp_id++;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "eqid")
                {
                    stringstream tmed;
                    tmed << "EQU "
                         << "temp" << temp_id << " " << id_table.at(pttsitem[0].id_id) << endl;
                    ttsitem.temp_id = temp_id;
                    temp_id++;
                    ttsitem.pmed = tmed.str();
                }else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "eqexp")
                {
                    stringstream tmed;
                    tmed <<pttsitem[1].pmed<< "EQU "
                         << "temp" << temp_id << " temp" << pttsitem[1].temp_id << endl;
                    ttsitem.temp_id = temp_id;
                    temp_id++;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "eq")
                {
                    ttsitem.temp_id = pttsitem[0].temp_id;
                    ttsitem.pmed = pttsitem[0].pmed;
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "mul2")
                {
                    stringstream tmed;
                    if (pttsitem[1].pmed == "*")
                    {
                        tmed << pttsitem[2].pmed << pttsitem[0].pmed << "MUL temp" << pttsitem[2].temp_id << " temp" << pttsitem[0].temp_id << " temp" << temp_id << endl;
                    }
                    else if (pttsitem[1].pmed == "/")
                    {
                        tmed << pttsitem[2].pmed << pttsitem[0].pmed << "DIV temp" << pttsitem[2].temp_id << " temp" << pttsitem[0].temp_id << " temp" << temp_id << endl;
                    }
                    else
                    {
                        cout << "ERROR" << endl;
                    }
                    ttsitem.temp_id = temp_id;
                    temp_id++;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "add2")
                {
                    stringstream tmed;
                    if (pttsitem[1].pmed == "+")
                    {
                        tmed << pttsitem[2].pmed << pttsitem[0].pmed << "ADD temp" << pttsitem[2].temp_id << " temp" << pttsitem[0].temp_id << " temp" << temp_id << endl;
                    }
                    else if (pttsitem[1].pmed == "-")
                    {
                        tmed << pttsitem[2].pmed << pttsitem[0].pmed << "SUB temp" << pttsitem[2].temp_id << " temp" << pttsitem[0].temp_id << " temp" << temp_id << endl;
                    }
                    else
                    {
                        cout << "ERROR" << endl;
                    }
                    ttsitem.temp_id = temp_id;
                    temp_id++;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "cmp")
                {
                    stringstream tmed;
                    if (pttsitem[1].pmed == "<")
                    {
                        tmed << pttsitem[2].pmed << pttsitem[0].pmed << "CMP< temp" << pttsitem[2].temp_id << " temp" << pttsitem[0].temp_id << " temp" << temp_id << endl;
                    }
                    else if (pttsitem[1].pmed == "==")
                    {
                        tmed << pttsitem[2].pmed << pttsitem[0].pmed << "CMP= temp" << pttsitem[2].temp_id << " temp" << pttsitem[0].temp_id << " temp" << temp_id << endl;
                    }
                    else
                    {
                        cout << "ERROR" << endl;
                    }
                    ttsitem.temp_id = temp_id;
                    temp_id++;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "as")
                {
                    stringstream tmed;
                    tmed << pttsitem[0].pmed << "EQU " << id_table[pttsitem[2].id_id] << " temp" << pttsitem[0].temp_id << endl;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "seq")
                {
                    stringstream tmed;
                    tmed << pttsitem[2].pmed << pttsitem[0].pmed;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "ed")
                {
                    med = pttsitem[0].pmed;
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "if")
                {
                    stringstream tmed;
                    tmed << pttsitem[4].pmed << "IF_F temp" << pttsitem[4].temp_id << " goto L" << Label_id << endl;
                    tmed << pttsitem[1].pmed << "Label L" << Label_id << endl;
                    Label_id++;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "if-e")
                {
                    stringstream tmed;
                    tmed << pttsitem[6].pmed << "IF_F temp" << pttsitem[6].temp_id << " goto L" << Label_id << endl;
                    int tlabel = Label_id;
                    Label_id++;
                    tmed << pttsitem[3].pmed << "goto L" << Label_id << endl;
                    tmed << "Label L" << tlabel << endl
                         << pttsitem[1].pmed << "Label L" << Label_id << endl;
                    Label_id++;
                    ttsitem.pmed = tmed.str();
                }
                else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "while")
                {
                    stringstream tmed;
                    tmed << "Label L" << Label_id << endl;
                    int tlabel = Label_id;
                    Label_id++;
                    tmed << pttsitem[3].pmed << "IF_F temp" << pttsitem[3].temp_id << " goto L" << Label_id << endl;
                    tmed << pttsitem[1].pmed << "goto L" << tlabel << endl;
                    tmed << "Label L" << Label_id << endl;
                    Label_id++;
                    ttsitem.pmed = tmed.str();
                }else if (Orign_Nterminal[tactive.prods[0]].op[tindex][0] == "repeat")
                {
                    stringstream tmed;
                    tmed << "Label L" << Label_id << endl;
                    tmed << pttsitem[4].pmed<<pttsitem[1].pmed << "IF_F temp" << pttsitem[1].temp_id << " goto L" << Label_id << endl;
                    Label_id++;
                    ttsitem.pmed = tmed.str();
                }
            }
            st.push_back(ttsitem);
        }
    }

    return false;
}