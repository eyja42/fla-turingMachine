#ifndef __TM_TAPE_H__
#define __TM_TAPE_H__

#include <vector>

using namespace std;

/**
 * 图灵机的纸带。使用vector实现。
 * 为了处理图灵机纸带序号可以为负的问题，使用了两个vector分别存储正索引(从1开始)的纸带(tapePositive)和负索引(从-1开始)的纸带(tapeNegative)。
 * 序号0的字符存储再zero_c中。
 */
class Tape {
public:
    Tape(int size = 4096, char B = '_') {
        this->size = size;
        tapePositive.resize(size, B);
        tapeNegative.resize(size, B);
        zero_c = 0;
    }

    //以数组方式访问(传入索引)
    char &operator[](int index);
    const char &operator[](int index) const;
    //重设纸带大小
    void resize();
    //设置空格符号，同时清空纸带
    void setSpaceSymbol(char B) {
        zero_c = B;
        tapePositive = vector<char>(size, B);
        tapeNegative = vector<char>(size, B);
        this->spaceSymbol = B;
    }

    int max_index() const { return size; }

    int min_index() const { return -size; }

private:
    char zero_c;
    vector<char> tapePositive;
    vector<char> tapeNegative;
    char spaceSymbol;
    int size;
};

struct tapePtr {
    int index = 0;
    int leftmost = 0;
    int rightmost = 0;

    void set_leftmost(int l) { leftmost = l; }

    void set_rightmost(int r) { rightmost = r; }

    tapePtr operator++() { return tapePtr{++index, leftmost, rightmost}; }

    tapePtr operator++(int) {
        tapePtr tmp = *this;
        ++index;
        return tmp;
    }

    tapePtr operator--() { return tapePtr{--index, leftmost, rightmost}; }

    tapePtr operator--(int) {
        tapePtr tmp = *this;
        --index;
        return tmp;
    }
};

#endif