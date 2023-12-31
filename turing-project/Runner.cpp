#include "Runner.h"
#include "Parser.h"
#include "Tape.h"
#include "TuringMachine.h"
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <ostream>
#include <string>
#include <sys/types.h>

using namespace std;

void Runner::oneStep() {
    turingMachine->oneStep();
    ++step;
    currTmState = turingMachine->getState();
}

void Runner::printTmState() {
    //冒号左侧需要留空的字符数
    auto offset = to_string(currTmState.tape->size()).length() + 6;
    cout << setw(offset) << "Step" << ": " << step << endl;
    cout << setw(offset) << "State" << ": " << currTmState.curr_state->name() << endl;
    for (int i = 0; i < currTmState.tape->size(); ++i) {
        string tmp;
        tmp = "Index" + to_string(i);
        cout << setw(offset) << tmp << ": ";
        printIndex((*(currTmState.tape))[i], (*(currTmState.tapeptr))[i]);

        tmp = "Tape" + to_string(i);
        cout << setw(offset) << tmp << ": ";
        size_t headPos = printTape((*(currTmState.tape))[i], (*(currTmState.tapeptr))[i]);

        tmp = "Head" + to_string(i);
        cout << setw(offset) << tmp << ": ";
        cout << string(headPos, ' ') << '^' << endl;
    }
}

int Runner::printTape(const Tape &tp, const tapePtr &tptr) {
    int l = min(tptr.leftmost, tptr.index);
    int r = max(tptr.rightmost, tptr.index);
    int width = 0;
    bool stop = false;
    for (int i = l; i <= r; ++i) {
        cout << left << setw(to_string(abs(i)).size()) << tp[i] << ' ';
        stop = stop || (tptr.index == i);
        if (!stop)
            width += to_string(abs(i)).length() + 1;
    }
    cout << endl;
    return width;
}

void Runner::printIndex(const Tape &tp, const tapePtr &tptr) {
    size_t width = 0;
    bool stop = false;
    int l = min(tptr.leftmost, tptr.index);
    int r = max(tptr.rightmost, tptr.index);
    for (int i = l; i <= r; ++i) {
        if (i == tptr.index)
            stop = true;
        if (!stop) {
            width += to_string(abs(i)).length() + 1;
        }

        cout << abs(i) << ' ';
    }
    cout << endl;
}

void Runner::loadProgram(const string &filepath) {
    ifstream p(filepath);
    if (p.fail()) {
        cerr << "open tmFile failed: " << filepath << endl;
        end(EXIT_FAILURE);
    }
    string sb;
    string tmp;
    while (getline(p, tmp)) {
        sb += tmp;
        sb += '\n';
    }
    this->parser = new Parser();
    try {
        this->parser->parse(sb);
    } catch (Parser::parserError parserErr) {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << parserErr.getReason() << endl;
        end(EXIT_FAILURE);
    } catch (TuringMachine::tmError tmErr) {
        cerr << "syntax error" << endl;
        end(EXIT_FAILURE);
    }
    turingMachine = this->parser->res();
    currTmState = turingMachine->getState();
}

void Runner::loadTape(const string &input) {
    try {
        turingMachine->loadTape(input);
    } catch (TuringMachine::tmError tmErr) {
        if (verbose) {
            cerr << "Input: " << input << endl;
            cerr << verboseErr << endl;
            cerr << "error: Symbol \"" << tmErr.input[tmErr.loc]
                 << "\" in input is not defined in the set of input symbols" << endl;
            cerr << "Input: " << tmErr.input << endl;
            cerr << string(7 + tmErr.loc, ' ') << '^' << endl;
            cerr << verboseEnd << endl;
        } else {
            cerr << "illegal input string" << endl;
        }
        end(EXIT_FAILURE);
    }
    if (verbose) {
        cout << "Input: " << input << endl;
    }
    assert(!turingMachine->isHalt());
}


void Runner::end(int code) {
    exit(code);
}

void Runner::run() {
    if (verbose)
        cout << verboseRun << endl;
    while (!turingMachine->isHalt()) {
        if (verbose) {
            printTmState();
            cout << verboseLine << endl;
        }
        oneStep();
    }
    if (verbose) {
        //todo:打印图灵机是否接收了输入
        if (turingMachine->Accept()) {
            cout << "ACCEPTED" << endl;
        } else {
            cout << "UNACCEPTED" << endl;
        }
        printf("Result: ");
        turingMachine->printTape();
        cout << verboseEnd << endl;
    } else {
        //todo:普通模式：输出图灵机是否接收
        if (turingMachine->Accept()) {
            cout << "(ACCEPTED) ";
        } else {
            cout << "(UNACCEPTED) ";
        }
        turingMachine->printTape();
    }

    end(0);
}
