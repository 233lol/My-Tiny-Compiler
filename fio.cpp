#include <fstream>
#include <string>
#include <sstream>
#include "include/fio.hpp"
using namespace std;
string read(string name)
{                    //读函数
    ifstream infile; // ifstream对象，来读取文件
    infile.open(name);
    ostringstream buf; // ostringstream对象，方便字符串的拼接处理
    char ch;
    while (buf && infile.get(ch)) //通过get方式一个字符一个字符读取并放入ostringstream buf
        buf.put(ch);
    infile.close();
    return buf.str(); //转换成字符串的形式并返回
}

void write(string name, string data)
{                     //写函数
    ofstream outfile; // ofstream对象，来写入文件
    outfile.open(name);
    outfile << data; //将data中的数据写入文件中
    outfile.close();
}