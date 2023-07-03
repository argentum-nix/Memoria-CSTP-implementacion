#ifndef SOLVER_H
#define SOLVER_H

#include "instance.h"
#include <algorithm>
#include <random>
#include <vector>
#include <set>

class Solver
{
public:
    Solver(Instance *in, int heuristic_flag, int grasp_flag, int grasp_window, int s, float closeness_param, float availability_param, char f);
    ~Solver();
    float calculatePriority(float te, int g);
    void updateCasualtyState(int casualty_id, float te, float change_timestamp);
    void greedyAssignment(char fleet_mode, int cursor, int flag_save, int GRASP_flag, int GRASP_window_size, int HOrder_flag);
    int checkIfHighST(int casualty_id);
    void printTimestamp(float t);
    float calculateSolutionQuality(char f, int total_flag);
    void printCasualtyRouteRow(int casualty_id);
    void printPriorityList();
    void printVictimsWithoutAssignment();
    void printAllHospitalsStates();
    void heuristicProcedure(float closeness_factor, float availability_factor);
    void saveSolution(int casualty_id, float lambda, int veh_id, int veh_type, int r, int hospital_id, float wait_t, float arrive_t, float stabilize_t, float hospital_t);
    void printSolutions();
    void greedyRouteCreator(int cas_id, char fleet_mode, int flag_save, float lambda);

    std::vector<std::pair<float, int>> priority_list;
    std::vector<std::pair<float, int>> assignment_list;
    std::pair<int, float> findClosestHospitalWithBeds(int casualty_id, int id_closest_veh, int veh_type);
    std::pair<int, float> findClosestAvailableVehicle(int casualty_id, int veh_type);
    std::pair<int, float> findFirstAvailableVehicle(int veh_type);

    int start_time;
    int current_time;

private:
    Instance *instance;
    int useGrasp;
    int useHeuristic;
    int graspWindowSize;
    float closeness_factor;
    float availability_factor;
    char functionMode;
    unsigned int seed;
    // key: victim, vector: gravity, age, priority, stabilization time, veh_type, veh_id, mcc_id, appear, wait_till, arrive, stabilize, admit_at_hos;
    std::map<int, std::vector<float>> solutions;
};

#endif