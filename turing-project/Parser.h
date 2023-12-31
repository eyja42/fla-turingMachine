#ifndef __TM_PARSE_H__
#define __TM_PARSE_H__

#include "State.h"
#include "TuringMachine.h"
#include <cctype>
#include <functional>
#include <iostream>
#include <map>
#include <set>
using namespace std;

class Parser {
public:
    struct parserError {
        const char *getReason() { return reason.c_str(); }
        //错误原因
        string reason;
        //详细报错信息
        string relate_segment;
    };

    Parser();

    ~Parser() { delete resultTm; }
    //返回解析好的图灵机
    TuringMachine *res() {
        if(resultTm== nullptr)
            throw parserError{"tm not init",""};
        return resultTm;
    }

    //核心函数,解析传入的tm程序，进行合法性检查，构造图灵机对象
    void parse(const string &inputText);

private:
    //文件缓存和当前指针
    string text;
    int ptr;

    //分别表示输入符号集S和纸带符号集G中!非法的符号集。
    const static set<char> gchar;

    //验证构建的图灵机是否合法，不合法时抛出parserError。
    //如果合法则设置resultTm
    void validate();
    //读取一个符号名
    char oneSymbol();
    //读取一个状态名
    State oneState(bool star = false);
    //读取一组转移函数中的状态名称
    string oneStream() {
        if(text[ptr]==',')
            throw parserError{"unexpected char",""};
        int curPtr = ptr;
        while (isgraph(text[ptr]) && !gchar.count(text[ptr]) || text[ptr] == '*') {
            ptr++;
        }
        return text.substr(curPtr,ptr-curPtr);
    }

    void skipSpace();
    //判断是否读到文件末尾
    bool end() const;

    TuringMachine *resultTm;
    set<State> states;        // 状态集Q
    set<State> finalStates; // 终结状态集F
    set<char> inputSymbol;      // 输入符号集S
    set<char> tapeSymbol;       // 纸带符号集G
    State startState;             // 初始状态q0
    char spaceSymbol;
    int tapeCnt; // 纸带数;
    bool Q, B, F, S, G, q0, N, tr;
    map<State, vector<transition>> transitions;
};

#endif