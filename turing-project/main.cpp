#include "Runner.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

void help() {
    cerr << "usage: turing [-v|--verbose] [-h|--help] <turingMachine> <input>" << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    string tmPath;
    string input;
    bool verbose = 0;
    int argCnt = 0;
    if (argc < 2)
        help();
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
            verbose = 1;
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            help();
        } else {
            if (argCnt < 2) {
                (argCnt ? input : tmPath) = argv[i];
                ++argCnt;
            } else {
                help();
            }
        }
    }
    if (tmPath.empty())
        help();
    Runner runner(verbose);
    //调用Parser解析tm程序文件
    runner.loadProgram(tmPath);
    //调用tm检查输入纸带合法性，如果合法则顺便设置图灵机的初始纸带
    runner.loadTape(input);
    runner.run();
    return 0;
}
