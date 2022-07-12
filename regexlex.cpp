#include"include/regexlex.hpp"
string regexlex::regextomindfa(string temp)
{
    nfa t;
    string temp2 = t.run(temp); //先regex生成nfa
    nfa2dfa t2;
    string temp3 = t2.run(temp2);
    dfa2mindfa t3;
    return t3.run(temp3);
}
string regexlex::run(string srule,string code)
{
    stringstream rule(srule);
    string temp;
    lexical lex;
    while (getline(rule, temp, '\n')) //按行分割然判断当前行是哪个规则然后导入。
    {
        if (temp.find("num:") != string::npos)
        {
            string snum = regextomindfa(temp.substr(4));
            lex.lnum.input_rule(snum);
        }
        else if (temp.find("id:") != string::npos)
        {
            string sid = regextomindfa(temp.substr(3));
            lex.lid.input_rule(sid);
        }
        else if (temp.find("key:") != string::npos)
        {
            lex.input_key(temp.substr(4));
        }
        else if (temp.find("symbol:") != string::npos)
        {
            lex.input_symbol(temp.substr(7));
        }
    }
    lex.input_code(code);
    return lex.output();
}
bool regexlex::check_error(string token){
    stringstream stoken(token);
    string temp;
    int flag=0;
    int cnt=0;
    while (getline(stoken, temp, '\n')){
        cnt++;
        stringstream t(temp);
        string a,b;
        t>>a>>b;
        if(b=="error"){
            flag=1;
            cout<<"error in "<<cnt<<" is \""<<a<<"\""<<endl;
        }
    }
    if(flag==1){
        return true;
    }else{
        return false;
    }
}