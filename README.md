# Tiny 编译器

#### 介绍
一个简单的编译器。
#### 提醒
不建议大作业直接照搬我的项目，这样对你自己的能力没有任何提高。而且被发现有挂科风险。
#### 操作
命令：
./main.exe
然后在命令行输入D或者R选择是否进行代码优化。
本程序输入yf-rule.txt为语法语义规则，按空格分割符号，按行分产生式；cf-rule.txt为词法规则，按空格分割符号；code.txt为处理的语句，按空格分割。
本程序输出为med.txt 输出经过S属性语义之后的中间代码,4.txt为四元三地址码。
#### 四元三地址码操作符含义解释
    AS,x,t,_：赋值操作符，t=x
    IN,x,_,_：输入操作符，输入x的值
    ADD,x,y,t ：加法操作符，t=x+y
    SUB,x,y,t ：减法操作符，t=x-y
    MUL,x,y,t ：乘法操作符，t=x*y
    DIV,x,y,t ：除法操作符，t=x/y
    CMPL,x,y,t：比较是否小于操作符，t=x<y的是否为真
    CMPE,x,y,t：比较是否等于操作符，t=x==y的是否为真
    IFF,x,L1,_：条件操作符，如果x为假（0）就跳转到L1处
    LAB,L1,_,_：标签操作符，设置标签用于跳转
    JMP,L1,_,_：无条件跳转操作符，无条件跳转到L1
    OUT,x,_,_：输出操作符，输出x的值
#### 源程序代码
源程序例子：

    read x ;
    read y ;
    x = 12 * y + 2 - ( 2 + 4 ) / 2 ;
    x = 3 + 4 ;
    write x ;
    x = 5 * 2 ;
    read x ;
    if ( x < 3 ) then
        x = 5 ;
        y = 2 ;
    else 
        x = 3 ;
    endif ;
    while ( x < 10 )
        x = x + 1 ;
        y = y + 2 ;
    endw ;
    repeat
        x = x + 2 ;
    until ( x < 12 ) ;
    write x ;

关键词：
if else while endif endw read write repeat until then
