#include "TuringMachine.h"
#include "State.h"
#include <iostream>
#include <iterator>
#include <ostream>
#include <cassert>


const State TuringMachine::dead{""};

TuringMachine::tmState TuringMachine::getState() const {
    return {&tape, &tapePtrs, &currState};
}

void TuringMachine::oneStep() {
    auto curTransitions = transitions[currState];
    for (const auto &trans: curTransitions) {
        assert(trans.q == currState);

        bool match = true;
        string symbols = trans.curSymbol;
        //将转移函数中的符号和当前纸带符号进行对比。所有纸带符号匹配上则进行转移。
        //匹配条件：如果转移函数中对应符号为'*'则匹配除了空格外的所有符号；否则两个符号相等则匹配
        for (int i = 0; i < symbols.size(); ++i) {
            if (!(symbols[i] == peekTape(i) || (symbols[i] == '*' && peekTape(i) != spaceSymbol))) {
                match = false;
                break;
            }
        }

        if (match) {
            for (int j = 0; j < tapeCnt; ++j) {
                if(trans.newSymbol[j]!='*')
                    peekTape(j) = trans.newSymbol[j];
                if(trans.dir[j]=='l')
                    --(tapePtrs[j]);
                else if(trans.dir[j]=='r')
                    ++(tapePtrs[j]);

                tapePtrs[j].set_leftmost(tape[j].max_index());
                tapePtrs[j].set_rightmost(tape[j].min_index());
                for(int k = tape[j].min_index();k<=tape[j].max_index();++k){
                    if(tape[j][k]!=spaceSymbol){
                        tapePtrs[j].set_leftmost(k);
                        break;
                    }
                }
                for(int k = tape[j].max_index();k>=tape[j].min_index();--k){
                    if(tape[j][k]!=spaceSymbol){
                        tapePtrs[j].set_rightmost(k);
                        break;
                    }
                }
                currState = trans.p;
                if(finalStates.count(currState) > 0){
                    accept = true;
                }
            }
            return;
        }
    }
    currState = dead;
}


void TuringMachine::loadTape(const string &input) {
    for (int i = 0; i < input.size(); ++i) {
        if (!inputSymbol.count(input[i])) {
            currState = dead;
            accept = false;
            throw tmError{input, i};
        }
    }
    for (int i = 0; i < input.size(); ++i) {
        tape[0][i] = input[i];
    }
    tapePtrs[0].index = 0;
    tapePtrs[0].set_rightmost((int) input.size() - 1);
}


char &TuringMachine::peekTape(int tapeId) {
    return tape[tapeId][tapePtrs[tapeId].index];
}

bool TuringMachine::isHalt() { return currState == dead; }

void TuringMachine::printTape() {
    for (int i = tapePtrs[0].leftmost; i <= tapePtrs[0].rightmost; ++i) {
        cout << tape[0][i];
    }
    cout << endl;
}
