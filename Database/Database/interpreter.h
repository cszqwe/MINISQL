#ifndef Interpreter_h
#define Interpreter_h

#include <iostream>
#include <string>

using namespace std;

/*
 Interpreter模块直接与用户交互，主要实现以下功能：
 1.	程序流程控制，即“启动并初始化 → ‘接收命令、处理命令、显示命令结果’循环 → 退出”流程。
 2.	接收并解释用户输入的命令，生成命令的内部数据结构表示，同时检查命令的语法正确性和语义正确性，对正确的命令调用API层提供的函数执行并显示执行结果，对不正确的命令显示错误信息。
 */
class apicaller{
private:
    int commtype;
    string comm;
public:
    apicaller(){
        comm="";
        commtype=-1;
    }
    void wronginput();
    void queryin();
    void judgecommtype();
    void callselect();
    void callinsert();
    void calldelete();
    void callcreatetable();
    void callcreateindex();
    void calldroptable();
    void calldropindex();
};

class OpType{
public:
    int ope;//±Ì æ≈–∂œ∑˚ 1.< 2.> 3.<= 4.>= 5.== 6.!=
    int attrType;//∏√ Ù–‘µƒ¿‡–Õ 1.int 2.float 3.char*
    string attrName;
    int attrPos;//∏√ Ù–‘ «’‚∏ˆtableµƒµ⁄º∏∏ˆ Ù–‘
    string value;
    OpType(){}
    OpType(int ope, string attrName, int attrPos,string value,int attrType){
        this->ope = ope;
        this->attrName = attrName;
        this->value = value;
        this->attrPos = attrPos;
        this->attrType = attrType;
    }
};


#endif /* Interpreter_h */
