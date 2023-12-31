#include "TuringMachine.h"
#include "State.h"
#include "Parser.h"
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

//const set<char> Parser::schar = {' ', ',', ';', '{', '}', '*', '_'};
const set<char> Parser::gchar = {' ', ',', ';', '{', '}', '*'};

Parser::Parser() {
    ptr = 0;
    resultTm = nullptr;
}

bool Parser::end() const { return ptr >= text.size(); }

void Parser::skipSpace() {
    while (isspace(text[ptr])) {
        ptr++;
    }
}

State Parser::oneState(bool star) {
    int tmpPtr = ptr;
    while (isalnum(text[ptr]) || text[ptr]=='_' || (star && text[ptr] == '*')) {
        ptr++;
    }

    return State(text.substr(tmpPtr,ptr-tmpPtr));
}

char Parser::oneSymbol() {
    if(text[ptr]==',')
        throw parserError{"unexpected char",""};
    char c = text[ptr];
    ptr++;
    return c;
}

//用于读取集合。
//用func获取集合元素，再添加到table中
#define parseHelper(func, table)                                      \
  do {                                                                         \
    while (text[ptr] != '{') {                                                 \
      ptr++;                                                               \
    }                                                                          \
    skipSpace();                                                         \
    ptr++;                                                                 \
    if (text[ptr] != '}') {                                                    \
      auto name = func();                                                      \
      table.insert(name);                                                      \
      skipSpace();                                                       \
      while (text[ptr] == ',') {                                               \
        ptr++;                                                             \
        skipSpace();                                                     \
        name = func();                                                         \
        table.insert(name);                                                    \
        skipSpace();                                                     \
      }                                                                        \
      if(text[ptr]!='}'){                                                      \
        throw parserError{"unexpected char",string(1,text[ptr])};   \
      }                                                                         \
      ptr++;                                                               \
    } else {                                                                   \
      ptr++;                                                               \
    }                                                                          \
  } while (0)

void Parser::parse(const string &inputText) {
    this->text = inputText;
    ptr = 0;

    while (!end()) {
        if (text[ptr] == ';') {
            while (text[ptr] != '\n') {
                ptr++;
            }
            ptr++;
        } else if (text[ptr] == '#') {
            ptr++;
            switch (text[ptr]) {
                case 'Q': {
                    Q = true;
                    parseHelper(oneState, states);
                }
                    break;
                case 'F': {
                    F = true;
                    parseHelper(oneState, finalStates);
                }
                    break;
                case 'G': {
                    G = true;
                    parseHelper(oneSymbol, tapeSymbol);
                }
                    break;
                case 'S': {
                    S = true;
                    parseHelper(oneSymbol, inputSymbol);
                }
                    break;
                case 'B':{
                    B = true;
                    ptr++;
                    while (text[ptr] == ' ' || text[ptr] == '=')
                        ptr++;
                    spaceSymbol = text[ptr];
                    ptr++;
                }
                    break;
                case 'N': {
                    N = true;
                    ptr++;
                    while (text[ptr] == ' ' || text[ptr] == '=')
                        ptr++;
                    int tmpPtr = ptr;
                    while (isdigit(text[ptr])) {
                        ptr++;
                    }
                    string n = text.substr(tmpPtr,ptr-tmpPtr);
                    tapeCnt = stoi(n);

                    if(tapeCnt <= 0){
                        throw parserError{"too few tapes", to_string(tapeCnt)};
                    }

                }
                    break;
                case 'q': {
                    ptr++;
                    if(text[ptr]!='0'){
                        throw parserError{"unexpected char--q0", string(1, text[ptr])};
                    }

                    ptr++;
                    q0 = true;
                    while (text[ptr] == ' ' || text[ptr] == '=')
                        ptr++;
                    auto sstate = oneState();
                    startState = sstate;
                }
                    break;
                default: {
                    throw parserError{"invalid option", string(1, text[ptr])};
                }
            }
        } else if (isspace(text[ptr])) {
            skipSpace();
        } else {
            tr = true;

            State tmpQ;
            State tmpP;
            string tmpCurSymbol;
            string tmpNewSymbol;
            string tmpDir;

            tmpQ = oneState(1);
            skipSpace();
            auto tmp = oneStream();
            for (const auto &i: tmp) {
                tmpCurSymbol.push_back(i);
            }
            skipSpace();
            tmp = oneStream();
            for (const auto &i: tmp) {
                tmpNewSymbol.push_back(i);
            }
            skipSpace();
            tmp = oneStream();
            for (const auto &i: tmp) {
                tmpDir.push_back(i);
            }
            skipSpace();
            tmpP = oneState(1);
            transition tmpTrans{tmpQ,tmpP,tmpCurSymbol,tmpNewSymbol,tmpDir};

            transitions[tmpTrans.q].push_back(tmpTrans);
        }
    }

    //进行合法性检查
    validate();
}

#undef parseHelper

void Parser::validate() {
    if(!(Q && B && F && S && G && q0 && N && tr)){
        throw parserError{"all parts must be initialized", ""};
    }
    if(states.count(startState) == 0){
        throw parserError{"start state not in Q", ""};
    }
    if(!includes(states.begin(), states.end(), finalStates.begin(), finalStates.end())){
        throw parserError{"finish states not subset of states", ""};
    }
    if(tapeSymbol.count(spaceSymbol) == 0){
        throw parserError{"space symbol not in G", string(1, spaceSymbol)};
    }

    //检查转移函数
    for (const auto &u: transitions) {
        for (const auto &i: u.second) {
            auto q = i.q;
            auto p = i.p;
            auto tmpcurSymbol = i.curSymbol;
            auto tmpNewSymbol = i.newSymbol;
            auto direc = i.dir;
            if(states.count(q)==0){
                throw parserError{"transition q not in Q", q.name()};
            }
            if(states.count(p)==0){
                throw parserError{"transition p not in Q", p.name()};
            }
            if(tmpcurSymbol.size() != tapeCnt){
                throw parserError{"curSymbol width not match", to_string(tapeCnt)};
            }
            if(tmpNewSymbol.size() != tapeCnt){
                throw parserError{"newSymbol width not match", to_string(tapeCnt)};
            }
            if(direc.size() != tapeCnt){
                throw parserError{"dir width not match", to_string(tapeCnt)};
            }

            for (int j = 0; j < tapeCnt; ++j) {
                if(tapeSymbol.count(tmpcurSymbol[j]) == 0 && tmpcurSymbol[j] != '*'){
                    throw parserError{"transition symbol not in G", string(1, tmpcurSymbol[j])};
                }
                if(tapeSymbol.count(tmpNewSymbol[j]) == 0 && tmpNewSymbol[j] != '*'){
                    throw parserError{"transition symbol not in G", string(1, tmpNewSymbol[j])};
                }
                if(direc[j]!='r' && direc[j]!='l' && direc[j]!='*'){
                    throw parserError{"invalid direction", string(1, direc[j])};
                }
            }
        }
    }

    resultTm = new TuringMachine(states, finalStates, inputSymbol, tapeSymbol, transitions, startState, spaceSymbol, tapeCnt);
}

#undef parseHelper
