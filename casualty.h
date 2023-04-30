#ifndef VICTIM_H
#define VICTIM_H

class Casualty
{
public:
    Casualty();
    ~Casualty();

    void setCasualtyID();
    void setCasualtyGravity();
    void setCasualtyAge();
    void setCasualtyLocation();
    void setCasualtyPriority();
    void setCasualtyAssignedHospital();
    void setCasualtyAssignedVehicle();

    int getCasualtyID();
    int getCasualtyGravity();
    int getCasualtyAge();
    int getCasualtyLocation();
    int getCasualtyPriority();
    int getCasualtyAssignedHospital();
    int getCasualtyAssignedVehicle();

private:
    int cas_id_k;
    int cas_curgravity_g;
    int cas_age_a;
    int cas_curlocation;
    int cas_priority_lambda;
    int cas_assigned_hosp;
    int cas_assigned_veh;
};

#endif