#ifndef VICTIM_H
#define VICTIM_H

#include <iostream>

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

    void printData();

private:
    int cas_id_k;
    int cas_curgravity_g;
    int cas_age_a;
    int cas_curlocation;
    float cas_appear_time;
    float cas_time_to_heliport;

    float cas_wait_time;
    float cas_priority_lambda;
    int cas_assigned_hosp;
    int cas_assigned_veh;
    int cas_assigned_veh_type;
};

#endif