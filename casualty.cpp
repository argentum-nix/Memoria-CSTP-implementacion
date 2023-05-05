#include "casualty.h"

// Casualty class constructor
Casualty::Casualty() {}

// Casualty class destructor
Casualty::~Casualty() {}

void Casualty::setCasualtyID(int id)
{
    cas_id_k = id;
}
void Casualty::setCasualtyGravity(int g)
{
    cas_curgravity_g = g;
}
void Casualty::setCasualtyAge(int a)
{
    cas_age_a = a;
}
void Casualty::setCasualtyLocation(int node_id)
{
    cas_curlocation = node_id;
}
void Casualty::setCasualtyPriority(float lambda)
{
    cas_priority_lambda = lambda;
}
void Casualty::setCasualtyAppearTime(int time)
{
    cas_appear_time = time;
}
void Casualty::setCasualtyAssignedHospital(int h)
{
    cas_assigned_hosp = h;
}
void Casualty::setCasualtyAssignedVehicle(int m)
{
    cas_assigned_veh = m;
}

int Casualty::getCasualtyID()
{
    return cas_id_k;
}
int Casualty::getCasualtyGravity()
{
    return cas_curgravity_g;
}
int Casualty::getCasualtyAge()
{
    return cas_age_a;
}
int Casualty::getCasualtyLocation()
{
    return cas_curlocation;
}
float Casualty::getCasualtyPriority()
{
    return cas_priority_lambda;
}
int Casualty::getCasualtyAssignedHospital()
{
    return cas_assigned_hosp;
}
int Casualty::getCasualtyAssignedVehicle()
{
    return cas_assigned_veh;
}

int Casualty::getCasualtyAppearTime()
{
    return cas_appear_time;
}

void Casualty::printData()
{
    std::cout << "Casualty ID: " << cas_id_k << " Age: " << cas_age_a << " Gravity: " << cas_curgravity_g;
    std::cout << " Location: " << cas_curlocation << " Priority: " << cas_priority_lambda << " Appear Time: " << cas_appear_time;
    std::cout << " Vehicle: " << cas_assigned_veh << " Hospital: " << cas_assigned_hosp << std::endl;
}