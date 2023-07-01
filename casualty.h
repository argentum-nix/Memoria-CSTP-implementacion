#ifndef VICTIM_H
#define VICTIM_H

#include <iostream>
#include <vector>

class Casualty
{
public:
    Casualty();
    ~Casualty();
    void snapshotLastAssinment();
    void setCasualtyID(int id);
    void setCasualtyGravity(int g);
    void setCasualtyAge(int a);
    void setCasualtyLocation(int node_id);
    void setCasualtyPriority(float lambda);
    void setCasualtyAppearTime(float time);
    void setCasualtyAssignedHospital(int h);
    void setCasualtyAssignedVehicle(int m);
    void setCasualtyAssignedVehicleType(int e);
    void setCasualtyWaitTime(float t);
    void setCasualtyTimeToHeliport(float d);
    void setCasualtyVehArrivedTime(float t);
    void setCasualtyStabilizedTime(float t);
    void setCasualtyAdmittedAtHospitalTime(float t);
    void setCasualtyRound(int r);
    void addGravityChangeTimestamp(float t, int inroute_flag);

    int getCasualtyRound();
    int getCasualtyID();
    int getCasualtyGravity();
    int getCasualtyAge();
    int getCasualtyLocation();
    float getCasualtyPriority();
    int getCasualtyAssignedHospital();
    int getCasualtyAssignedVehicle();
    int getCasualtyAssignedVehicleType();
    float getCasualtyAppearTime();
    float getCasualtyWaitTime();
    float getCasualtyTimeToHeliport();
    float getLastGravityChange();
    float getCasualtyVehArrivedTime();
    float getCasualtyStabilizedTime();
    float getCasualtyAdmittedAtHospitalTime();

    void resetGravityChange(int on_period_reset);
    void printData();

    // metaheuristic-specific functions for trying new solutions
    void resetLastAssignment();
    void saveLastAssignment();
    int getCurrentInrouteFlag();

private:
    // Static values
    int yet_to_snapshot = 1;
    int cas_id_k;
    int cas_age_a;
    int cas_curlocation;
    float cas_appear_time;
    float cas_time_to_heliport;

    // Changing values (assignment-specific)
    int round = 0;
    float cas_priority_lambda;

    int prev_prev_g_inroute_flag = 0;
    int prev_g_inroute_flag = 0;
    int g_inroute_flag = 0;

    // id of assigned vehicle
    int cas_assigned_veh = -1;
    int cas_prev_veh = -1;
    int cas_prev_prev_veh = -1;

    // type of vehicle assigned
    int cas_assigned_veh_type = -1;
    int cas_prev_veh_type = -1;
    int cas_prev_prev_veh_type = -1;

    // assigned hospital
    int cas_assigned_hosp = -1;
    int cas_prev_hosp = -1;
    int cas_prev_prev_hosp = -1;

    // victim has to wait until X to vehicle to be assigned to it
    float cas_wait_time = -1;
    float cas_prev_wait_time = -1;
    float cas_prev_prev_wait_time = -1;

    // the vehicle arrives at victims location at X
    float cas_arrival_time = -1;
    float cas_prev_arrival_time = -1;
    float cas_prev_prev_arrival_time = -1;

    // victim is stabilized at X
    float cas_st_time = -1;
    float cas_prev_st_time = -1;
    float cas_prev_prev_st_time = -1;

    // victim is admitted to hospital at X
    float cas_h_time = -1;
    float cas_prev_h_time = -1;
    float cas_prev_prev_h_time = -1;

    // current gravity
    std::vector<int> cas_curgravity_g;
    std::vector<int> cas_prev_g;
    // timestamp of last gravity change
    std::vector<float> cas_g_change_timestamp;
    std::vector<float> cas_prev_g_change_timestamp;
};

#endif