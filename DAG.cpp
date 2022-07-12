#include "include/DAG.hpp"
vector<vector<vector<string>>> DAG::blocks(string in)
{
    vector<vector<vector<string>>> out;
    stringstream sin(in);
    string temp1;
    vector<vector<string>> med;
    while (getline(sin, temp1, '\n'))
    {
        stringstream temp2(temp1);
        string temp;
        vector<string> pmed;
        while (temp2 >> temp)
        {
            pmed.push_back(temp);
        }
        med.push_back(pmed);
    }
    vector<vector<string>> temp;
    for (size_t i = 0; i < med.size(); i++)
    {
        if (med[i][0] == "IF_F")
        {
            temp.push_back(med[i]);
            out.push_back(temp);
            temp.clear();
        }
        else if (med[i][0] == "Label")
        {
            out.push_back(temp);
            temp.clear();
            temp.push_back(med[i]);
        }
        else if (med[i][0] == "goto")
        {
            temp.push_back(med[i]);
            out.push_back(temp);
            temp.clear();
        }
        else
        {
            temp.push_back(med[i]);
        }
    }
    if (temp.size() > 0)
    {
        out.push_back(temp);
    }
    vector<vector<vector<string>>> out2;
    for (size_t i = 0; i < out.size(); i++)
    {
        if (out[i].size() > 0)
        {
            out2.push_back(out[i]);
        }
    }

    return out2;
}
int DAG::findnum(vector<status> ts, string num)
{
    for (size_t i = 0; i < ts.size(); i++)
    {
        if (ts[i].op == "num" && ts[i].val.count(num))
        {
            return i;
        }
    }
    return -1;
}
int DAG::findtemp(vector<status> ts, string temp)
{
    for (size_t i = 0; i < ts.size(); i++)
    {
        if (ts[i].val.count(temp))
        {
            return i;
        }
    }
    return -1;
}
int DAG::findid(vector<status> ts, string id)
{
    for (size_t i = 0; i < ts.size(); i++)
    {
        if (ts[i].val.count(id))
        {
            return i;
        }
    }
    return -1;
}
int DAG::aishave(vector<status> s, status t)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if (((t.left == s[i].left && t.right == s[i].right) || (t.right == s[i].left && t.left == s[i].right)) && t.op == s[i].op)
        {
            return i;
        }
    }
    return -1;
}
int DAG::ishave(vector<status> s, status t)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if (t.left == s[i].left && t.right == s[i].right && t.op == s[i].op)
        {
            return i;
        }
    }
    return -1;
}
bool DAG::havevar(status t)
{
    for (auto iter : t.val)
    {
        if (iter.find("temp") == string::npos)
        {
            if (!isdigit(iter[0]))
            {
                return true;
            }
        }
    }
    return false;
}
vector<vector<string>> DAG::toDAG(vector<vector<vector<string>>> blocks)
{
    vector<vector<string>> med;
    int temp_num = 0;
    for (size_t i = 0; i < blocks.size(); i++)
    {
        vector<status> ts;
        int cnt = 0;
        for (size_t j = 0; j < blocks[i].size(); j++)
        {
            if (blocks[i][j][0] == "Label")
            {
                vector<string> tout;
                tout.push_back(blocks[i][j][0]);
                tout.push_back(blocks[i][j][1]);
                med.push_back(tout);
                continue;
            }
            if (blocks[i][j][0] == "EQU")
            {
                status t;
                if (blocks[i][j][2].find("temp") == string::npos)
                {
                    if (isdigit(blocks[i][j][2][0]))
                    {
                        int place = findnum(ts, blocks[i][j][2]);
                        if (place == -1)
                        {
                            t.op = "num";
                            t.val.insert(blocks[i][j][2]);
                            t.val.insert(blocks[i][j][1]);
                            t.order = cnt;
                            cnt++;
                            ts.push_back(t);
                        }
                        else
                        {
                            ts[place].val.insert(blocks[i][j][2]);
                            ts[place].val.insert(blocks[i][j][1]);
                        }
                    }
                    else
                    {
                        int place = findid(ts, blocks[i][j][2]);
                        if (place == -1)
                        {
                            t.op = "id";
                            t.val.insert(blocks[i][j][2]);
                            t.val.insert(blocks[i][j][1]);
                            t.order = cnt;
                            cnt++;
                            ts.push_back(t);
                        }
                        else
                        {
                            ts[place].val.insert(blocks[i][j][2]);
                            ts[place].val.insert(blocks[i][j][1]);
                        }
                    }
                }
                else
                {
                    if (blocks[i][j][1].find("temp") == string::npos)
                    {
                        int place = findtemp(ts, blocks[i][j][2]);
                        for (size_t k = 0; k < ts.size(); k++)
                        {
                            if (ts[k].val.count(blocks[i][j][1]) && ts[k].op != "id")
                            {

                                ts[k].val.erase(blocks[i][j][1]);
                            }
                        }
                        ts[place].val.insert(blocks[i][j][2]);
                        ts[place].val.insert(blocks[i][j][1]);
                    }
                    else
                    {
                        int place = findtemp(ts, blocks[i][j][2]);
                        ts[place].val.insert(blocks[i][j][2]);
                        ts[place].val.insert(blocks[i][j][1]);
                    }
                }
            }
            else if (blocks[i][j][0] == "ADD" || blocks[i][j][0] == "MUL" || blocks[i][j][0] == "CMP=")
            {
                status t;
                int place = findtemp(ts, blocks[i][j][1]);
                if (place == -1)
                {
                    cout << "ERROR" << endl;
                }
                else
                {
                    t.left = place;
                    ts[place].dep_num++;
                }
                place = findtemp(ts, blocks[i][j][2]);
                if (place == -1)
                {
                    cout << "ERROR" << endl;
                }
                else
                {
                    t.right = place;
                    ts[place].dep_num++;
                }
                t.op = blocks[i][j][0];
                t.val.insert(blocks[i][j][3]);
                place = aishave(ts, t);
                if (place == -1)
                {
                    t.order = cnt;
                    ts.push_back(t);
                    cnt++;
                }
                else
                {
                    ts[place].val.insert(blocks[i][j][3]);
                }
            }
            else if (blocks[i][j][0] == "SUB" || blocks[i][j][0] == "DIV" || blocks[i][j][0] == "CMP<")
            {
                status t;
                int place = findtemp(ts, blocks[i][j][1]);
                if (place == -1)
                {
                    cout << "ERROR" << endl;
                }
                else
                {
                    t.left = place;
                    ts[place].dep_num++;
                }
                place = findtemp(ts, blocks[i][j][2]);
                if (place == -1)
                {
                    cout << "ERROR" << endl;
                }
                else
                {
                    t.right = place;
                    ts[place].dep_num++;
                }
                t.op = blocks[i][j][0];
                t.val.insert(blocks[i][j][3]);
                place = ishave(ts, t);
                if (place == -1)
                {
                    t.order = cnt;
                    ts.push_back(t);
                    cnt++;
                }
                else
                {
                    ts[place].val.insert(blocks[i][j][3]);
                }
            }
            else if (blocks[i][j][0] == "IF_F")
            {
                status t;
                int place = findtemp(ts, blocks[i][j][1]);
                t.left = place;
                ts[place].dep_num++;
                t.op = blocks[i][j][0];
                t.val.insert(blocks[i][j][3]);
                t.order = cnt;
                cnt++;
                ts.push_back(t);
            }
            else if (blocks[i][j][0] == "goto")
            {
                status t;
                t.left = -2;
                t.op = blocks[i][j][0];
                t.uvar.push_back(blocks[i][j][1]);
                t.order = cnt;
                cnt++;
                ts.push_back(t);
            }
            else if (blocks[i][j][0] == "OUT")
            {
                status t;
                int place = findtemp(ts, blocks[i][j][1]);
                if (place != -1)
                {
                    ts[place].muvar.push_back(blocks[i][j][1]);
                    ts[place].must = true;
                    t.right = place;
                    ts[place].dep_num++;
                }
                t.left = -2;
                t.op = blocks[i][j][0];
                t.uvar.push_back(blocks[i][j][1]);
                t.order = cnt;
                cnt++;
                ts.push_back(t);
            }
            else if (blocks[i][j][0] == "IN")
            {
                status t;
                t.left = -2;
                t.op = blocks[i][j][0];
                for (size_t k = 0; k < ts.size(); k++)
                {
                    if (ts[k].val.count(blocks[i][j][1]))
                    {

                        ts[k].val.erase(blocks[i][j][1]);
                    }
                }
                t.uvar.push_back(blocks[i][j][1]);
                t.order = cnt;
                cnt++;
                ts.push_back(t);
            }
        }
        for (ssize_t j = ts.size() - 1; j >= 0; j--)
        {
            if (ts[j].op == "IF_F" || ts[j].op == "OUT" || ts[j].op == "goto" || havevar(ts[j]))
            {
                if (havevar(ts[j]))
                {
                    if (ts[j].dep_num == 0)
                    {
                        ts[j].dep_num++;
                    }
                }

                continue;
            }
            else
            {
                if (ts[j].dep_num == 0)
                {
                    if (ts[j].left >= 0)
                    {
                        ts[ts[j].left].dep_num--;
                    }
                    if (ts[j].right >= 0)
                    {
                        ts[ts[j].right].dep_num--;
                    }
                }
            }
        }

        for (size_t j = 0; j < ts.size(); j++)
        {
            for (auto iter : ts[j].val)
            {
                if (iter.find("temp") == string::npos)
                {
                    if (isdigit(iter[0]))
                    {
                        ts[j].rv = iter;
                    }
                    else
                    {
                        if (ts[j].rv.empty())
                        {
                            ts[j].rv = iter;
                            ts[j].uvar.push_back(iter);
                        }
                        else
                        {
                            ts[j].uvar.push_back(iter);
                        }
                    }
                }
            }
            if (ts[j].rv.empty() && ts[j].left != -2)
            {
                ts[j].rv = "temp" + to_string(temp_num);
                temp_num++;
            }
        }

        for (size_t j = 0; j < ts.size(); j++)
        {
            if (ts[j].left != -1 || ts[j].right != -1)
            {
                if (ts[j].op == "IF_F")
                {
                    vector<string> tout;
                    tout.push_back(ts[j].op);
                    tout.push_back(ts[ts[j].left].rv);
                    tout.push_back("goto");
                    tout.push_back(*ts[j].val.begin());
                    med.push_back(tout);
                }
                else if (ts[j].op == "goto" || ts[j].op == "OUT" || ts[j].op == "IN")
                {
                    vector<string> tout;
                    tout.push_back(ts[j].op);
                    tout.push_back(*ts[j].uvar.begin());
                    med.push_back(tout);
                }
                else
                {
                    if (ts[j].dep_num == 0)
                    {
                        continue;
                    }
                    vector<string> tout;
                    tout.push_back(ts[j].op);
                    tout.push_back(ts[ts[j].left].rv);
                    tout.push_back(ts[ts[j].right].rv);
                    tout.push_back(ts[j].rv);
                    med.push_back(tout);
                }
                if (!ts[j].muvar.empty())
                {
                    vector<string> tout;
                    tout.push_back("EQU");
                    tout.push_back(*ts[j].muvar.begin());
                    tout.push_back(ts[j].rv);
                    med.push_back(tout);
                }
            }
            else
            {
                if (!ts[j].muvar.empty())
                {
                    vector<string> tout;
                    tout.push_back("EQU");
                    tout.push_back(*ts[j].muvar.begin());
                    tout.push_back(ts[j].rv);
                    med.push_back(tout);
                }
                else
                {
                    if (!ts[j].uvar.empty() && ts[j].uvar[0] != ts[j].rv)
                    {
                        vector<string> tout;
                        tout.push_back("EQU");
                        tout.push_back(*ts[j].uvar.begin());
                        tout.push_back(ts[j].rv);
                        med.push_back(tout);
                    }
                }
            }
        }
    }
    size_t ntemp_num = 0;
    set<string> temp_set;
    for (auto iter : med)
    {
        for (auto it1 : iter)
        {
            if (it1.find("temp") != string::npos)
            {
                temp_set.insert(it1);
            }
        }
    }
    for (auto iter : temp_set)
    {
        for (size_t i = 0; i < med.size(); i++)
        {
            for (size_t j = 0; j < med[i].size(); j++)
            {
                if (med[i][j] == iter)
                {
                    med[i][j] = "temp" + to_string(ntemp_num);
                }
            }
        }
        ntemp_num++;
    }

    return med;
}
string DAG::vtos(vector<vector<string>> med)
{
    stringstream out;
    for (auto iter : med)
    {
        for (size_t i = 0; i < iter.size(); i++)
        {
            if (i == iter.size() - 1)
            {
                out << iter[i];
            }
            else
            {
                out << iter[i] << " ";
            }
        }
        out << endl;
    }
    return out.str();
}