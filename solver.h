#ifndef SOLVER_H
#define SOLVER_H

#include "instance.h"

class Solver
{
public:
    Solver(Instance *in);
    ~Solver();

private:
    Instance *instance;
}

#endif