#include "solver.h"
#include <math.h>
using namespace std;

Solver::Solver(Instance *in)
{
    instance = in;
    // TODO: delete, temp.
    cout << calculatePriority(5, 1) << endl;
}

// Solver class destructor
Solver::~Solver() {}

void Solver::updateWaitingTime(int casualty_id)
{
    // TODO: 5 es el valor de inicio de periodo TT, instancia todavia no maneja estos valores.
    // Deberian venir dentro de la misma instancia.
    int TT = 5;
    int TA = instance->getCasualtyAppearTime(casualty_id);
    instance->updateCasualtyWaitingTime(casualty_id, TT - TA);
}

float Solver::calculatePriority(float te, int g)
{
    // en la condicion tienen que tiempo de espera de victima debe ser < te_max
    // TODO: tiene sentido agregarlo???
    // diferencia de resultados de prioridad es debido a que en modelo usaron euler 2.718281
    if (g == 1)
    {
        return 0.1 + 0.1 * exp(1 + 0.048 * te / 60);
    }
    else if (g == 2)
    {
        return 0.4 + 0.1 * exp(1.6 + 0.374 * te / 60);
    }
    else if (g == 3)
    {
        return 4.5 + 0.1 * exp(1.8 + 0.4 * te / 60);
    }
    return 0;
}
