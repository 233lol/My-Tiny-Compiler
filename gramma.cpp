#include "include/gramma.hpp"
vector<string> gramma::convert(string token)
{
    vector<string> tokens;
    stringstream stoken(token);
    string temp;
    while (getline(stoken, temp, '\n'))
    {
        stringstream t(temp);
        string a, b;
        t >> a >> b;
        if (b == "key")
        {
            tokens.push_back(a);
        }
        else if (b == "id")
        {
            id_table.push_back(a);
            tokens.push_back(b);
        }
        else if (b == "num")
        {
            num_table.push_back(atoi(a.c_str()));
            tokens.push_back(b);
        }
        else if (b == "symbol")
        {
            tokens.push_back(a);
        }
        else
        {
            cout << "error" << endl;
        }
    }
    return tokens;
}
string gramma::run(string srule, string token,string &med)
{
    string ts = srule;
    string flag;

    LALR1 t1;
    t1.input(ts);
    t1.DFA();
    t1.reduce();
    t1.make_table();
    if (t1.analyse(convert(token),med,num_table,id_table))
    {
        flag = "right";
    }
    else
    {
        flag = "grammer,error";
    }
    return flag;
}