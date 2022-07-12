#include "include/LALR1.hpp"
using namespace std;
int LALR1::ishave2(status in_status, size_t number)
{
    for (size_t i = 0; i < statuses.size(); i++)
    {
        if (i == number)
        {
            continue;
        }
        if (statuses[i].isdrop)
        {
            continue;
        }
        if (statuses[i].left.size() != in_status.left.size())
        {
            continue;
        }
        size_t count = 0;
        for (size_t j = 0; j < in_status.left.size(); j++)
        {
            for (size_t k = 0; k < statuses[i].left.size(); k++)
            {
                if (statuses[i].left[k] == in_status.left[j] && statuses[i].rightA[k] == in_status.rightA[j] && statuses[i].rightB[k] == in_status.rightB[j])
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
void LALR1::replace(size_t in1, size_t in2)
{
    for (size_t i = 0; i < statuses.size(); i++)
    {
        for (auto &iter : statuses[i].path)
        {
            if (iter.second == int(in1))
            {
                iter.second = in2;
            }
        }
    }
}
void LALR1::reduce()
{
    for (size_t i = 0; i < statuses.size(); i++)
    {
        if (statuses[i].isdrop)
        {
            continue;
        }
        int place = ishave2(statuses[i], i);
        if (place == -1)
        {
            continue;
        }
        status tstatus = statuses[place];
        for (size_t j = 0; j < tstatus.lookahead.size(); j++)
        {
            for (size_t k = 0; k < statuses[i].left.size(); k++)
            {
                if (statuses[i].left[k] == tstatus.left[j] && statuses[i].rightA[k] == tstatus.rightA[j] && statuses[i].rightB[k] == tstatus.rightB[j])
                {
                    tstatus.lookahead[j].insert(statuses[i].lookahead[k].begin(), statuses[i].lookahead[k].end());
                    break;
                }
            }
        }
        set<string> tset;
        size_t num = 0;
        for (size_t j = 0; j < tstatus.lookahead.size(); j++)
        {
            if (tstatus.rightB[j][0] == "^")
            {
                num += tstatus.lookahead[j].size();
                tset.insert(tstatus.lookahead[j].begin(), tstatus.lookahead[j].end());
            }
        }
        if (num > tset.size())
        {
            cout << "can't LALR1" << endl;
            return;
        }
        statuses[i].isdrop = true;
        statuses[place] = tstatus;
        replace(i, place);
    }
}