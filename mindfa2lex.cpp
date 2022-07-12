#include "include/lex.hpp"
void mindfa::input_rule(string in)
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
            dfa_states.resize(dfa_state_num);
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
bool id::isid(string word) //判断是否是标识符
{
    char flag;
    int index = d.start_state; //将index初始化为初态
    for (size_t i = 0; i < word.size(); i++)
    {
        if (isdigit(word[i])) //是否是数字
        {
            flag = 'd'; //数字按d简化，以便简化正则表达式的处理
        }
        else if (isupper(word[i]) || islower(word[i])) //是否是字母
        {
            flag = 'l'; //字母按l简化，以便简化正则表达式的处理
        }
        else
        {
            return false; //都不是按false返回
        }
        int flag_edge = 0;                        //做没有边时的标志
        for (auto iter : dfa_states[index].edges) //变量当前状态的边
        {
            if (flag == iter.input) //判断当前字符是否有当前状态的边
            {
                index = iter.trans;
                flag_edge = 1;
            }
        }
        if (flag_edge == 0) //当前状态没有对应边返回false
        {
            return false;
        }
    }
    if (dfa_states[index].is_end == false) //最后状态不是终态返回false
    {
        return false;
    }
    return true; //表示是id返回true
}
bool num::isnum(string word) //判断是否是数字
{
    char flag;
    int index = d.start_state; //将index初始化为初态
    for (size_t i = 0; i < word.size(); i++)
    {
        if (word[i] == '-')
        {
            flag = 'n'; //-按+转化，以便正则表达式的处理
        }
        else if (word[i] == '+')
        {
            flag = 'p'; //+按p转化，以便正则表达式的处理
        }
        else if (isdigit(word[i])) //是否是数字
        {
            flag = 'd'; //数字按d简化，以便简化正则表达式的处理
        }
        else
        {
            return false; //都不是按false返回
        }

        int flag_edge = 0;                        //做没有边时的标志
        for (auto iter : dfa_states[index].edges) //变量当前状态的边
        {
            if (flag == iter.input) //判断当前字符是否有当前状态的边
            {
                index = iter.trans;
                flag_edge = 1;
            }
        }
        if (flag_edge == 0) //当前状态没有对应边返回false
        {
            return false;
        }
    }
    if (dfa_states[index].is_end == false) //最后状态不是终态返回false
    {
        return false;
    }
    return true; //表示是num返回true
}
string lexical::remove_comment(string in) //移除注释函数
{
    stringstream input(in);
    string temp;
    string result = "";
    while (getline(input, temp, '\n'))
    {
        int index = temp.find_first_of("#");
        result += temp.substr(0, index);
        result += "\n";
    }
    return result;
}
void lexical::input_code(string in) //处理输入代码
{
    string temp = remove_comment(in);
    stringstream buff(temp);
    string t;
    while (buff >> t) //按空格分割转换成向量
    {
        codes.push_back(t);
    }
}
void lexical::input_key(string in)
{ //处理输入关键字
    string temp = remove_comment(in);
    stringstream buff(temp);
    string t;
    while (buff >> t)
    {
        key.insert(t);
    }
}
void lexical::input_symbol(string in)
{ //处理输入符号
    string temp = remove_comment(in);
    stringstream buff(temp);
    string t;
    while (buff >> t)
    {
        symbol.insert(t);
    }
}
string lexical::judge(string word)
{ //判断
    if (key.count(word))
    {
        return "key"; //是关键词返回key
    }
    else if (symbol.count(word))
    {
        return "symbol"; //是符号返回symbol
    }
    else if (lid.isid(word))
    {
        return "id"; //是标识符返回id
    }
    else if (lnum.isnum(word))
    {
        return "num"; //是数字返回num
    }
    else
    {
        return "error"; //都不是返回error
    }
}
string lexical::output()
{ //输出，按token内容 token属性输出
    string result = "";
    for (auto iter : codes)
    {
        result += iter + " " + judge(iter) + "\n";
    }
    return result;
}