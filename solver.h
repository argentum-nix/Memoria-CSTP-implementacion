#ifndef SOLVER_H
#define SOLVER_H

#include "instance.h"

class Solver
{
public:
    Solver(Instance *in);
    ~Solver();
    float calculatePriority(float te, int a);

private:
    Instance *instance;
};

#endif