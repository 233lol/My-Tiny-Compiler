#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "include/nfa.hpp"
#include "include/fio.hpp"
nfa::nfa()
{
    sta_num = 0;
}
void nfa::input(string input) //导入正则表达式
{
    expr = input;
}
void nfa::change() //转换正则表达式例如abc*->a+b+c*
{
    string t;
    char s, e;
    for (size_t i = 0; i < expr.size(); i++)
    {
        s = expr[i];
        e = expr[i + 1];
        t += s;
        if (s != '(' && s != '|' && (islower(e) | isupper(e) | isdigit(e)))
        {
            t += '+';
        }
        else if (e == '(' && s != '|' && s != '(')
        {
            t += '+';
        }
    }
    t+=e;
    expr = t;
}
int nfa::in_priority(const char c) const //栈内优先级
{
    switch (c)
    {
    case '#':
        return 0;
    case '(':
        return 1;
    case '*':
        return 7;
    case '|':
        return 5;
    case '+':
        return 3;
    case ')':
        return 8;
    }
    return -1;
}
int nfa::out_priority(const char c) const //栈外优先级
{
    switch (c)
    {
    case '#':
        return 0;
    case '(':
        return 8;
    case '*':
        return 6;
    case '|':
        return 4;
    case '+':
        return 2;
    case ')':
        return 1;
    }
    return -1;
}
void nfa::postexp()
{
    expr += '#'; //末尾加#当终止符
    string t = "";
    stack<char> s;
    char ch1 = '#', ch2, op;
    s.push(ch1);
    int r_l = 0;
    ch1 = expr[r_l];
    r_l++;
    while (!s.empty()) //判断是否是空栈
    {
        if (islower(ch1) || isupper(ch1) || isdigit(ch1)) //判断是否是操作数
        {
            t += ch1;
            ch1 = expr[r_l];
            r_l++;
        }
        else
        {
            ch2 = s.top();
            if (in_priority(ch2) < out_priority(ch1)) //判断栈内优先级是否比栈外高
            {
                s.push(ch1); //栈外高，入栈
                ch1 = expr[r_l];
                r_l++;
            }
            else if (in_priority(ch2) > out_priority(ch1)) //判断栈内优先级是否比栈外高
            {
                op = s.top(); //栈内高出栈
                s.pop();
                t += op;
            }
            else //优先级一样
            {
                op = s.top();
                s.pop();
                if (op == '(') //判断是()还是#
                {
                    ch1 = expr[r_l];
                    r_l++;
                }
            }
        }
    }
    t.erase(t.end() - 1); //删除终止符#
    expr = t;
}
void nfa::to_nfa() //后缀式运算
{
    char item;
    nfa_unit left, right;
    stack<nfa_unit> s;
    for (size_t i = 0; i < expr.size(); i++)
    {
        item = expr[i];
        switch (item)
        {
        case '|': //选择运算
            right = s.top();
            s.pop();
            left = s.top();
            s.pop();
            Data = Or(left, right);
            s.push(Data);
            break;
        case '*': //闭包运算
            left = s.top();
            s.pop();
            Data = star(left);
            s.push(Data);
            break;
        case '+': //链接运算
            right = s.top();
            s.pop();
            left = s.top();
            s.pop();
            Data = mul(left, right);
            s.push(Data);
            break;
        default:
            Data = unit(item);
            s.push(Data);
            break;
        }
    }
    Data = s.top();
    s.pop();
}
nfa::nfa_unit nfa::Or(nfa_unit left, nfa_unit right)
{ //选择运算
    nfa_unit unit;
    edge e1, e2, e3, e4;

    state start {  std::to_string(sta_num++)  };
    state end{  std::to_string(sta_num++)  };

    e1.edg_start = start;//开始的两条ε边
    e1.edg_end = left.S;
    e1.edg_symbol = '#';

    e2.edg_start = start;
    e2.edg_end = right.S;
    e2.edg_symbol = '#';

    e3.edg_start = left.Z;//结束的两条ε边
    e3.edg_end = end;
    e3.edg_symbol = '#';

    e4.edg_start = right.Z;
    e4.edg_end = end;
    e4.edg_symbol = '#';

    unit = left;
    unit += right;
    unit.f.push_back(e1);
    unit.f.push_back(e2);
    unit.f.push_back(e3);
    unit.f.push_back(e4);

    unit.S = start;
    unit.Z = end;

    return unit;
}
nfa::nfa_unit nfa::mul(nfa_unit left, nfa_unit right) //连接运算
{
    for (auto &item : right.f)
    {
        if (item.edg_start.sta_name == right.S.sta_name)
        {
            item.edg_start = left.Z;
            sta_num--;
        }
        else if (item.edg_end.sta_name == right.S.sta_name)
        {
            item.edg_end = left.Z;
            sta_num--;
        }
    }
    right.S = left.Z;
    left += right;
    left.Z = right.Z;
    return left;
}
nfa::nfa_unit nfa::unit(const char ch)
{ //单个字符处理如a 变成 1--a-->2
    nfa_unit u;
    edge e;
    state start{std::to_string(sta_num++)};
    state end{std::to_string(sta_num++)};

    e.edg_start = start;
    e.edg_end = end;
    e.edg_symbol = ch;

    u.f.push_back(e);
    u.S = start;
    u.Z = end;
    return u;
}
nfa::nfa_unit nfa::star(nfa_unit u)
{ //闭包运算
    nfa_unit temp;
    edge e1, e2, e3, e4;
    state start{std::to_string(sta_num++)};
    state end{std::to_string(sta_num++)};
    e1.edg_start = start; //各条ε边按闭包运算的连接方式进行连接
    e1.edg_end = end;
    e1.edg_symbol = '#';

    e2.edg_start = u.Z;
    e2.edg_end = u.S;
    e2.edg_symbol = '#';

    e3.edg_start = start;
    e3.edg_end = u.Z;
    e3.edg_symbol = '#';

    e4.edg_start = u.Z;
    e4.edg_end = start;
    e4.edg_symbol = '#';

    temp = u;
    temp.f.push_back(e1);
    temp.f.push_back(e2);
    temp.f.push_back(e3);
    temp.f.push_back(e4);

    temp.S = start;
    temp.Z = end;

    return temp;
}

string nfa::output()const{//输出字符串
    string output=Data.S.sta_name+","+Data.Z.sta_name+'\n';
    for (auto item:Data.f){
        output+=item.edg_start.sta_name+","+item.edg_end.sta_name+",";
        if (item.edg_symbol=='#')
        {
            output+="%\n";//为了方便之后dfa处理，所以才有%替代ε
        }else{
            output+=item.edg_symbol;
            output+='\n';
        }
        
    }
    return output;
}
string nfa::run(string regex){
    input(regex);
    change();
    postexp();
    to_nfa();
    return output();
}