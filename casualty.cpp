#include "casualty.h"

Casualty::Casualty() {}

Casualty::~Casualty() {}

void Casualty::setCasualtyID(int id)
{
    cas_id_k = id;
}
void Casualty::setCasualtyGravity(int g)
{
    cas_curgravity_g.push_back(g);
}
void Casualty::setCasualtyAge(int a)
{
    cas_age_a = a;
}
void Casualty::setCasualtyLocation(int node_id)
{
    cas_curlocation = node_id;
}
void Casualty::setCasualtyAppearTime(float time)
{
    cas_appear_time = time;
}
void Casualty::setCasualtyTimeToHeliport(float d)
{
    cas_time_to_heliport = d;
}
void Casualty::setCasualtyPriority(float lambda)
{
    cas_priority_lambda.push_back(lambda);
}
void Casualty::setCasualtyAssignedHospital(int h)
{
    cas_assigned_hosp.push_back(h);
}
void Casualty::setCasualtyAssignedVehicle(int m)
{
    cas_assigned_veh.push_back(m);
}
void Casualty::setCasualtyAssignedVehicleType(int e)
{
    cas_assigned_veh_type.push_back(e);
}
void Casualty::setCasualtyWaitTime(float t)
{
    cas_wait_time.push_back(t);
}
void Casualty::addGravityChangeTimestamp(float t)
{
    g_change_timestamps.push_back(t);
}
void Casualty::setCasualtyVehArrivedTime(float t)
{
    cas_arrival_time.push_back(t);
}
void Casualty::setCasualtyStabilizedTime(float t)
{
    cas_st_time.push_back(t);
}
void Casualty::setCasualtyAdmittedAtHospitalTime(float t)
{
    cas_h_time.push_back(t);
}

float Casualty::getCasualtyVehArrivedTime()
{
    return cas_arrival_time.back();
}
float Casualty::getCasualtyStabilizedTime()
{
    return cas_st_time.back();
}

float Casualty::getCasualtyAdmittedAtHospitalTime()
{
    return cas_h_time.back();
}

int Casualty::getCasualtyID()
{
    return cas_id_k;
}
int Casualty::getCasualtyAge()
{
    return cas_age_a;
}
int Casualty::getCasualtyLocation()
{
    return cas_curlocation;
}
float Casualty::getCasualtyAppearTime()
{
    return cas_appear_time;
}
float Casualty::getCasualtyTimeToHeliport()
{
    return cas_time_to_heliport;
}
int Casualty::getCasualtyGravity()
{
    return cas_curgravity_g.back();
}
float Casualty::getCasualtyPriority()
{
    return cas_priority_lambda.back();
}
int Casualty::getCasualtyAssignedHospital()
{
    return cas_assigned_hosp.back();
}
int Casualty::getCasualtyAssignedVehicle()
{
    return cas_assigned_veh.back();
}
int Casualty::getCasualtyAssignedVehicleType()
{
    return cas_assigned_veh_type.back();
}
float Casualty::getCasualtyWaitTime()
{
    return cas_wait_time.back();
}
float Casualty::getLastGravityChange()
{
    return g_change_timestamps.back();
}
void Casualty::resetGravityChange()
{
    g_change_timestamps.pop_back();
    cas_curgravity_g.pop_back();
}

void Casualty::temporaryDeassign()
{
    int cursor = 0;
    if (cas_curgravity_g.size() > 1)
    {
        cursor = cas_curgravity_g.size() - 1;
        cas_curgravity_g.push_back(cas_curgravity_g[cursor - 1]);
    }
    if (cas_priority_lambda.size() > 1)
    {
        cursor = cas_priority_lambda.size() - 1;
        cas_priority_lambda.push_back(cas_priority_lambda[cursor - 1]);
    }
    if (cas_assigned_veh.size() > 1)
    {
        cursor = cas_assigned_veh.size() - 1;
        cas_assigned_veh.push_back(cas_assigned_veh[cursor - 1]);
    }
    if (cas_assigned_veh_type.size() > 1)
    {
        cursor = cas_assigned_veh_type.size() - 1;
        cas_assigned_veh_type.push_back(cas_assigned_veh_type[cursor - 1]);
    }
    if (cas_assigned_hosp.size() > 1)
    {
        cursor = cas_assigned_hosp.size() - 1;
        cas_assigned_hosp.push_back(cas_assigned_hosp[cursor - 1]);
    }
    if (cas_wait_time.size() > 1)
    {
        cursor = cas_wait_time.size() - 1;
        cas_wait_time.push_back(cas_wait_time[cursor - 1]);
    }
    if (cas_arrival_time.size() > 1)
    {
        cursor = cas_arrival_time.size() - 1;
        cas_arrival_time.push_back(cas_arrival_time[cursor - 1]);
    }
    if (cas_st_time.size() > 1)
    {
        cursor = cas_st_time.size() - 1;
        cas_st_time.push_back(cas_st_time[cursor - 1]);
    }
    if (cas_h_time.size() > 1)
    {
        cursor = cas_h_time.size() - 1;
        cas_h_time.push_back(cas_h_time[cursor - 1]);
    }
}

void Casualty::printData()
{
    std::cout << "Casualty ID: " << cas_id_k;
    std::cout << " Age: " << cas_age_a;
    std::cout << " Gravity: " << cas_curgravity_g.back();
    std::cout << " Location: " << cas_curlocation;
    std::cout << " Wait Time: " << cas_wait_time.back();
    std::cout << " Appear Time: " << cas_appear_time;
    std::cout << " Distance to Heliport: " << cas_time_to_heliport;
    std::cout << " Vehicle: " << cas_assigned_veh.back();
    std::cout << " Hospital: " << cas_assigned_hosp.back();
    std::cout << " Priority: " << cas_priority_lambda.back() << std::endl;
}