#ifndef SOLVER_H
#define SOLVER_H

#include "instance.h"
#include <algorithm>

class Solver
{
public:
    Solver(Instance *in);
    ~Solver();
    float calculatePriority(float te, int g);
    void updateWaitingTime(int casualty_id);
    void updateCasualtyState(int casualty_id, int te);
    void greedyAssignment();

    void printPriorityList();

    // PAIR: victim priority lambda,victim id
    std::vector<std::pair<float, int>> priority_list;
    // 7:41, first period start
    int start_time;
    int current_time;

private:
    Instance *instance;
};

#endif