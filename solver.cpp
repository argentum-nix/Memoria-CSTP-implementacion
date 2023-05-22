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
        // update waiting time
        updateWaitingTime(i);
    }
    // order the list from greater to smaller

    sort(priority_list.rbegin(), priority_list.rend(), [](const auto &l, const auto &r)
         { return (l.first == r.first) ? l.second > r.second : l.first < r.first; });
    // sort(priority_list.rbegin(), priority_list.rend());
    printPriorityList();
    cout << endl;
    cout << "=GREEDY=" << endl;
    greedyAssignment();
}

void Solver::printPriorityList()
{
    cout << "PRIORITY LIST: ";
    for (int i = 0; i < instance->qty_casualties; i++)
    {
        cout << "(" << priority_list[i].first << ", " << priority_list[i].second << "), ";
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
    // TODO: Revisar los vehiculos y si ya se desocupan
    int first_id, g, h_capacity;
    int closest_h_amb = -1;
    int closest_h_heli = -1;
    float min_dh_amb = 999999;
    float min_dh_heli = 999999;
    float min_dv_amb = 999999;
    float min_dv_heli = 999999;
    int closest_amb = -1;
    int closest_heli = -1;
    for (int i = 0; i < int(priority_list.size()); i++)
    {
        // pick the first victim on top of the list
        first_id = priority_list[i].second;
        g = instance->getCasualtyGravity(first_id);
        // Find the closest hospital that has de capacity to attend the casualty
        min_dh_amb = 999999;
        min_dh_heli = 999999;
        for (int h = 1; h <= instance->qty_hospitals; h++)
        {
            h_capacity = instance->getHospitalCurCapacity(h, g);
            // if this hospital can attend such gravity then
            if (h_capacity > 0)
            {
                // find closest hospital for AMBULANCE
                cout << "CASUALTY+AMBULANCE -> HOSPTAL /// DISTANCE BETWEEN " << instance->getCasualtyLocation(first_id) << " AND " << instance->getHospitalLocation(h) << " IS: ";
                cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 0) << endl;
                if (instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 0) < min_dh_amb)
                {
                    min_dh_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 0);
                    closest_h_amb = h;
                }
                // find closest hospital for HELICOPTER
                cout << "CASUALTY+HELICOPTER -> HOSPTAL /// DISTANCE BETWEEN " << instance->getCasualtyLocation(first_id) << " AND " << instance->getHospitalLocation(h) << " IS: ";
                cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 1) << endl;
                if (instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 1) < min_dh_heli)
                {
                    min_dh_heli = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 1);
                    closest_h_heli = h;
                }
            }
        }
        // Find an available vehicle, that takes the smallest time to get to the victim
        // Case: ambulance
        min_dv_amb = 999999;
        for (int a = 1; a <= instance->qty_ambulances; a++)
        {
            cout << "AMBULANCE -> CASUALTY /// DISTANCE BETWEEN " << instance->getCasualtyLocation(first_id) << " AND " << instance->getVehicleLocation(a, 0) << " IS: ";
            cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(a, 0), 0) << endl;
            if (instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(a, 0), 0) < min_dv_amb)
            {
                min_dv_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(a, 0), 0);
                DEBUG(min_dv_amb);
                closest_amb = a;
            }
        }
        min_dv_heli = 999999;
        // Case: helicopter
        for (int h = 1; h <= instance->qty_helicopters; h++)
        {
            cout << "HELICOPTER -> CASUALTY /// DISTANCE BETWEEN " << instance->getCasualtyLocation(first_id) << " AND " << instance->getVehicleLocation(h, 1) << " IS: ";
            cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(h, 1), 1) << endl;
            if (instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(h, 1), 1) < min_dv_heli)
            {
                min_dv_heli = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(h, 1), 1);
                closest_heli = h;
            }
        }

        // falta sumar tiempo de inicio de operaciones de vehiculos y takeoff/arrival de helis
        cout << "V" << first_id << endl;
        cout << "AMBULANCE CASE: AMB ASSIGNED= " << closest_amb << " AMB ARRIVAL IN= " << min_dv_amb << " STABILIZATION IN= " << min_dv_amb + instance->getCasualtyStabilizationTime(first_id) << " HOSP CHECKIN= " << min_dv_amb + instance->getCasualtyStabilizationTime(first_id) + min_dh_amb << " HOSP ASSIGNED= " << closest_h_amb << endl;
        cout << "HELICOPTER CASE: HELI ASSIGNED= " << closest_heli << " HELI ARRIVAL IN= " << min_dv_heli << " STABILIZATION IN= " << min_dv_heli + instance->getCasualtyStabilizationTime(first_id) << " HOSP CHECKIN= " << min_dv_heli + instance->getCasualtyStabilizationTime(first_id) + min_dh_heli << " HOSP ASSIGNED= " << closest_h_heli << endl;
        cout << "================================" << endl;
    }
}