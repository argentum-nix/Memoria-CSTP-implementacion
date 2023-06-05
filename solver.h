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
    void updateCasualtyState(int casualty_id, float te);
    void greedyAssignment(char fleet_mode);

    std::pair<int, float> findClosestHospitalWithBeds(int casualty_id, int id_closest_veh, int veh_type);
    std::pair<int, float> findClosestAvailableVehicle(int casualty_id, int veh_type);
    std::pair<int, float> findFirstAvailableVehicle(int veh_type);

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