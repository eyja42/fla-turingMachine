#ifndef __TM_STATE_H__
#define __TM_STATE_H__

#include <iostream>
using namespace std;

class State {
public:
    State() {stateName = "";}

    State(string state) { stateName = state; }

    State(const State &s) { stateName = s.stateName; }

    const string &name() const { return stateName; }

    bool operator==(const State &other) const {
        return stateName == other.stateName;
    }

    bool operator<(const State &other) const {
        return stateName < other.stateName;
    }

private:
    string stateName;
};

#endif