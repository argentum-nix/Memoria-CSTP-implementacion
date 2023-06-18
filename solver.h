#ifndef SOLVER_H
#define SOLVER_H

#include "instance.h"
#include <algorithm>
#include <vector>

class Solver
{
public:
    Solver(Instance *in);
    ~Solver();
    float calculatePriority(float te, int g);
    void updateCasualtyState(int casualty_id, float te, float change_timestamp);
    void greedyAssignment(char fleet_mode, int cursor);
    int checkIfHighST(int casualty_id);
    void printTimestamp(float t);

    float calculateSolutionQuality();
    void printCasualtyRouteRow(int casualty_id);

    std::pair<int, float> findClosestHospitalWithBeds(int casualty_id, int id_closest_veh, int veh_type);
    std::pair<int, float> findClosestAvailableVehicle(int casualty_id, int veh_type);
    std::pair<int, float> findFirstAvailableVehicle(int veh_type);

    void printPriorityList();

    void saveSolution(int casualty_id, float lambda, int veh_id, int veh_type, int r, int hospital_id, float wait_t, float arrive_t, float stabilize_t, float hospital_t);
    void printSolutions();

    // PAIR: victim priority lambda,victim id
    std::vector<std::pair<float, int>> priority_list;
    // 7:41, first period start
    int start_time;
    int current_time;

private:
    Instance *instance;
    // key: victim, vector: gravity, age, priority, stabilization time, veh_type, veh_id, mcc_id, appear, wait_till, arrive, stabilize, admit_at_hos;
    std::map<int, std::vector<float>> solutions;
};

#endif