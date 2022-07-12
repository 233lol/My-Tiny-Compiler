#include"include/medto4.hpp"
string medto4(string in){
    stringstream sin(in);
    stringstream out;
    string temp1;
    while (getline(sin, temp1, '\n')){
        stringstream temp2(temp1);
        string temp;
        vector<string> pmed;
        while (temp2>>temp)
        {
            pmed.push_back(temp);
        }
        if(pmed[0]=="EQU"){
            out<<"AS,"<<pmed[2]<<","<<pmed[1]<<",_"<<endl;
        }else if (pmed[0]=="MUL")
        {
            out<<"MUL,"<<pmed[1]<<","<<pmed[2]<<","<<pmed[3]<<endl;
        }else if (pmed[0]=="DIV")
        {
            out<<"DIV,"<<pmed[1]<<","<<pmed[2]<<","<<pmed[3]<<endl;
        }else if (pmed[0]=="ADD")
        {
            out<<"ADD,"<<pmed[1]<<","<<pmed[2]<<","<<pmed[3]<<endl;
        }else if (pmed[0]=="SUB")
        {
            out<<"SUB,"<<pmed[1]<<","<<pmed[2]<<","<<pmed[3]<<endl;
        }else if (pmed[0]=="CMP<")
        {
            out<<"CMPL,"<<pmed[1]<<","<<pmed[2]<<","<<pmed[3]<<endl;
        }else if (pmed[0]=="CMP=")
        {
            out<<"CMPE,"<<pmed[1]<<","<<pmed[2]<<","<<pmed[3]<<endl;
        }else if (pmed[0]=="IF_F")
        {
            out<<"IFF,"<<pmed[1]<<","<<pmed[3]<<",_"<<endl;
        }else if (pmed[0]=="IN")
        {
            out<<"IN,"<<pmed[1]<<",_,_"<<endl;
        }else if (pmed[0]=="OUT")
        {
            out<<"OUT,"<<pmed[1]<<",_,_"<<endl;
        }else if (pmed[0]=="goto")
        {
            out<<"JMP,"<<pmed[1]<<",_,_"<<endl;
        }else if (pmed[0]=="Label")
        {
            out<<"LAB,"<<pmed[1]<<",_,_"<<endl;
        }else{
            cout<<"ERROR"<<endl;
        }
        
    }
    return out.str();
}