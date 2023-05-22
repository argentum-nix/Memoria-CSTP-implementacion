#include "solver.h"
#include <math.h>
using namespace std;

Solver::Solver(Instance *in)
{
    instance = in;

    float lambda;
    for (int i = 1; i <= instance->qty_casualties; i++)
    {
        // Poblate the priority list with incoming victims
        // TODO: check if id "i" already was instanced, if so, update its lambda or skip it entirely
        lambda = calculatePriority(instance->getCasualtyWaitingTime(i), instance->getCasualtyGravity(i));
        instance->updateCasualtyPriority(i, lambda);
        // add new victim to priority_list
        priority_list.push_back(make_pair(lambda, i));
    }
    // order the list from greater to smaller
    sort(priority_list.rbegin(), priority_list.rend());
    printPriorityList();
}

void Solver::printPriorityList()
{
    cout << "PRIORITY LIST: ";
    for (int i = 0; i < instance->qty_casualties; i++)
    {
        cout << "(" << priority_list[i].first << " " << priority_list[i].second << "), ";
    }
    cout << endl;
}

// Solver class destructor
Solver::~Solver() {}

// update TE
void Solver::updateWaitingTime(int casualty_id)
{
    // TODO: 5 es el valor de inicio de periodo TT, instancia todavia no maneja estos valores.
    // Deberian venir dentro de la misma instancia.
    int TT = 5;
    int TA = instance->getCasualtyAppearTime(casualty_id);
    instance->updateCasualtyWaitingTime(casualty_id, TT - TA);
}

void Solver::updateStatesOfCasualties()
{
    // TODO: tiempo de espera maxima para pasar a siguiente LSI debe venir dentro de la instancia...
    int TEmax1 = 1140;
    int TEmax2 = 360;
    for (int i = 1; i <= instance->qty_casualties; i++)
    {
        // Case 1: Any victim of LSI 1 already waited enough to be considered LSI 2
        int g = instance->getCasualtyGravity(i);
        int te = instance->getCasualtyWaitingTime(i);
        if (g == 1 && te > TEmax1)
        {
            instance->updateCasualtyGravity(i, 2);
            instance->updateCasualtyAppearTime(i, instance->getCasualtyAppearTime(i) + instance->getDeteriorationTimeValue(g));
        }
        // Case 1: Any victim of LSI 2 already waited enough to be considered LSI 3
        else if (g == 2 && te > TEmax2)
        {
            instance->updateCasualtyGravity(i, 3);
            instance->updateCasualtyWaitingTime(i, instance->getCasualtyWaitingTime(i) - TEmax2);
            instance->updateCasualtyAppearTime(i, instance->getCasualtyAppearTime(i) + instance->getDeteriorationTimeValue(g));
        }
    }
}

void Solver::updatePriorityOfCasualties()
{
    // TODO: tiempo de espera maxima para pasar a siguiente LSI debe venir dentro de la instancia...
    int TEmax1 = 1140;
    int TEmax2 = 360;
    for (int i = 1; i <= instance->qty_casualties; i++)
    {
        int g = instance->getCasualtyGravity(i);
        int te = instance->getCasualtyWaitingTime(i);
        if (g == 1 && te <= TEmax1)
        {
            instance->updateCasualtyPriority(i, calculatePriority(instance->getCasualtyWaitingTime(i), 1));
        }
        else if (g == 2 && te <= TEmax2)
        {
            instance->updateCasualtyPriority(i, calculatePriority(instance->getCasualtyWaitingTime(i), 2));
        }
        else if (g == 3)
        {
            instance->updateCasualtyPriority(i, calculatePriority(instance->getCasualtyWaitingTime(i), 3));
        }
    }
}

float Solver::calculatePriority(float te, int g)
{
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

void Solver::greedyAssignment()
{
}