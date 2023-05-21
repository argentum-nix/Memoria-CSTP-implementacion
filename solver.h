#ifndef SOLVER_H
#define SOLVER_H

#include "instance.h"

class Solver
{
public:
    Solver(Instance *in);
    ~Solver();
    float calculatePriority(float te, int g);
    void updateWaitingTime(int casualty_id);
    void updateStatesOfCasualties();
    void updatePriorityOfCasualties();
    void greedyAssignment();

private:
    Instance *instance;
};

#endif