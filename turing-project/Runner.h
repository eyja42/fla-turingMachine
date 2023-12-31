#ifndef __RUN_H__
#define __RUN_H__

#include "Parser.h"
#include "Tape.h"
#include "TuringMachine.h"
#include <string>
using namespace std;

class Runner {
public:
    Runner(bool verbose = false)
            : verbose(verbose), turingMachine(nullptr), parser(nullptr), step(0) {}

    ~Runner() {
        if (parser)
            delete parser;
        parser = nullptr;
    }
    //加载tm程序。传入一个tm文件路径
    void loadProgram(const string &filepath);
    //传入图灵机的0号纸带
    void loadTape(const string &input);
    //启动图灵机，如果是verbose模式还会打印每步详细信息。最后输出结果到std::out
    void run();

private:
    bool verbose;
    int step;
    //verbose格式相关
    const string verboseLine = "---------------------------------------------";
    const string verboseErr = "==================== ERR ====================";
    const string verboseEnd = "==================== END ====================";
    const string verboseRun = "==================== RUN ====================";

    Parser *parser;
    TuringMachine *turingMachine;
    TuringMachine::tmState currTmState;

    //让图灵机执行一步
    void oneStep();
    //verbose模式下，打印当前tm状态
    void printTmState();
    //辅助函数，分别打印纸带索引和纸带内容。
    void printIndex(const Tape &tp, const tapePtr &tptr);
    //返回值为当前指针头的打印偏移量
    int printTape(const Tape &tp, const tapePtr &tptr);

    //程序退出
    void end(int code);
};

#endif