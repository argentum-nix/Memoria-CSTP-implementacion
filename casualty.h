#ifndef VICTIM_H
#define VICTIM_H

#include <iostream>
#include <vector>
#include <vector>

class Casualty
{
public:
    Casualty();
    ~Casualty();

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
    void addGravityChangeTimestamp(float t);

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

    void resetGravityChange();
    void temporaryDeassign();
    void printData();

private:
    // Static values
    int cas_id_k;
    int cas_age_a;
    int cas_curlocation;
    float cas_appear_time;
    float cas_time_to_heliport;

    // Changing values (assignment-specific)
    std::vector<int> cas_curgravity_g;
    std::vector<float> cas_priority_lambda;
    std::vector<int> cas_assigned_veh;
    std::vector<int> cas_assigned_veh_type;
    std::vector<int> cas_assigned_hosp;
    // victim has to wait until X to vehicle to be assigned to it
    std::vector<float> cas_wait_time;
    // the vehicle arrives at victims location at X
    std::vector<float> cas_arrival_time;
    // victim is stabilized at X
    std::vector<float> cas_st_time;
    // victim is admitted to hospital at X
    std::vector<float> cas_h_time;

    std::vector<float> g_change_timestamps;
};

#endif