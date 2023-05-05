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
    void setCasualtyAppearTime(int time);
    void setCasualtyAssignedHospital(int h);
    void setCasualtyAssignedVehicle(int m);

    int getCasualtyID();
    int getCasualtyGravity();
    int getCasualtyAge();
    int getCasualtyLocation();
    float getCasualtyPriority();
    int getCasualtyAssignedHospital();
    int getCasualtyAssignedVehicle();
    int getCasualtyAppearTime();

    void printData();

private:
    int cas_id_k;
    int cas_curgravity_g;
    int cas_age_a;
    int cas_curlocation;
    float cas_priority_lambda;
    int cas_assigned_hosp;
    int cas_assigned_veh;
    int cas_appear_time;
};

#endif