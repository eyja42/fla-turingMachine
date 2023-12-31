#ifndef __TM_TM_H__
#define __TM_TM_H__

#include "State.h"
#include "Tape.h"
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

class Tape;

struct transition {
    State q;                  //当前状态
    State p;                  //下一状态
    string curSymbol;         //当前纸带符号
    string newSymbol;          //新纸带符号
    string dir;          //移动方向
};

class TuringMachine {
public:
    struct tmError {
        string input;
        int loc;
    };
    //图灵机的当前状态。包括纸带状态，指针位置和图灵机状态
    struct tmState {
        const vector<Tape> *tape;
        const vector<tapePtr> *tapeptr;
        const State *curr_state;
    };

    //执行一步
    void oneStep();

    TuringMachine(set<State> &tmpStates, set<State> &tmpFinishStates, set<char> &tmpInputSymbol,
                  set<char> &tmpTapeSymbol, map<State, vector<transition>> &tmpTrans, const State &startState,
                  char tmpSpaceSymbol, int tmpTapeCnt) {
        for (const State &s: tmpStates) {
            states.emplace(s);
        }
        for (const State &s: tmpFinishStates) {
            finalStates.emplace(s);
        }
        for (const char &c: tmpInputSymbol) {
            inputSymbol.emplace(c);
        }
        for (const char &c: tmpTapeSymbol) {
            tapeSymbol.emplace(c);
        }
        for (const auto &tr: tmpTrans) {
            for (const auto &i: tr.second) {
                transitions[tr.first].push_back(i);
            }
        }

        this->startState = startState;
        this->currState = startState;
        this->spaceSymbol = tmpSpaceSymbol;
        this->tapeCnt = tmpTapeCnt;
        this->tape.resize(tapeCnt);
        for (auto &i: tape) {
            i.setSpaceSymbol(spaceSymbol);
        }
        this->tapePtrs.resize(tapeCnt);

        this->accept = finalStates.count(currState) != 0;
    }

    //返回图灵机是否停机。
    bool isHalt();

    //返回图灵机是否接受输入。
    bool Accept() const {
        return accept;
    }

    //设置图灵机0号纸带上的内容，同时进行合法性检查。
    void loadTape(const string &input);

    //打印第一条纸带上的内容。用于运行结束后打印结果
    void printTape();

    //获取当前图灵机状态
    tmState getState() const;

private:
    const static State dead;
    bool accept;

    //查看指定纸带指针处的符号
    char &peekTape(int tapeId);

    set<State> states;
    set<State> finalStates;
    set<char> inputSymbol;
    set<char> tapeSymbol;
    State startState;
    State currState = dead;
    char spaceSymbol = '_';
    int tapeCnt;
    vector<Tape> tape;
    map<State, vector<transition>> transitions;
    vector<tapePtr> tapePtrs;
};

#endif