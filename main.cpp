#include<iostream>
#include"include/fio.hpp"
#include"include/LALR1.hpp"
#include"include/LR1.hpp"
#include"include/regexlex.hpp"
#include"include/gramma.hpp"
#include"include/medto4.hpp"
#include"include/DAG.hpp"
using namespace std;
int main(){
    cout<<"Debug mode or Release mode ? [D/R]"<<endl;
    string mode;
    cin>> mode;
    bool debugmode;
    if(mode=="D"||mode=="Debug"){
        debugmode=true;
    }else if (mode=="R"||mode=="Release")
    {
        debugmode=false;
    }else{
        cout<<"ERROR MODE INPUT!"<<endl;
        return -1;
    }
    
    string cf_rule=read("cf-rule.txt");
    string code=read("code.txt");
    regexlex cflex;
    string stoken=cflex.run(cf_rule,code);
    if(cflex.check_error(stoken)){
        cout<<"error in lexical"<<endl;
    }
    string yf_rule=read("yf-rule.txt");
    gramma gram;
    string med;
    cout<<gram.run(yf_rule,stoken,med)<<endl;
    if(debugmode==false){
        DAG tDAG;
        vector<vector<vector<string>>> block = tDAG.blocks(med);
        med=tDAG.vtos(tDAG.toDAG(block));
    }
    write("med.txt",med);
    write("4.txt",medto4(med));
}
