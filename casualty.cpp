#include "casualty.h"

Casualty::Casualty() {}

Casualty::~Casualty() {}

void Casualty::setCasualtyID(int id)
{
    cas_id_k = id;
}

void Casualty::setCasualtyAge(int a)
{
    cas_age_a = a;
}

void Casualty::setCasualtyRound(int r)
{
    // std::cout << "SET ROUND FOR V" << cas_id_k << " EQUIAL TO " << r << std::endl;
    round = r;
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
    cas_priority_lambda = lambda;
}
void Casualty::setCasualtyGravity(int g)
{
    cas_prev_g = cas_curgravity_g;
    cas_curgravity_g.push_back(g);
}
void Casualty::setCasualtyAssignedHospital(int h)
{
    cas_prev_prev_hosp = cas_prev_hosp;
    cas_prev_hosp = cas_assigned_hosp;
    cas_assigned_hosp = h;
}
void Casualty::setCasualtyAssignedVehicle(int m)
{
    cas_prev_prev_veh = cas_prev_veh;
    cas_prev_veh = cas_assigned_veh;
    cas_assigned_veh = m;
}
void Casualty::setCasualtyAssignedVehicleType(int e)
{
    cas_prev_prev_veh_type = cas_prev_veh_type;
    cas_prev_veh_type = cas_assigned_veh_type;
    cas_assigned_veh_type = e;
}
void Casualty::setCasualtyWaitTime(float t)
{
    cas_prev_prev_wait_time = cas_prev_wait_time;
    cas_prev_wait_time = cas_wait_time;
    cas_wait_time = t;
}
void Casualty::addGravityChangeTimestamp(float t, int inroute_flag)
{
    /*if (inroute_flag == 1)
    {
        std::cout << "CAPTURED INROUTE GRAVITY CHANGE FOR V" << cas_id_k << std::endl;
    }*/

    if (inroute_flag != g_inroute_flag)
    {
        prev_prev_g_inroute_flag = prev_g_inroute_flag;
        prev_g_inroute_flag = g_inroute_flag;
        g_inroute_flag = inroute_flag;
    }
    cas_prev_g_change_timestamp = cas_g_change_timestamp;
    cas_g_change_timestamp.push_back(t);
}
void Casualty::setCasualtyVehArrivedTime(float t)
{
    cas_prev_arrival_time = cas_arrival_time;
    cas_arrival_time = t;
}
void Casualty::setCasualtyStabilizedTime(float t)
{
    cas_prev_st_time = cas_st_time;
    cas_st_time = t;
}
void Casualty::setCasualtyAdmittedAtHospitalTime(float t)
{
    cas_prev_h_time = cas_h_time;
    cas_h_time = t;
}

int Casualty::getCasualtyRound()
{
    return round;
}

float Casualty::getCasualtyVehArrivedTime()
{
    return cas_arrival_time;
}
float Casualty::getCasualtyStabilizedTime()
{
    return cas_st_time;
}

float Casualty::getCasualtyAdmittedAtHospitalTime()
{
    return cas_h_time;
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
int Casualty::getCasualtyAssignedVehicleType()
{
    return cas_assigned_veh_type;
}
float Casualty::getCasualtyWaitTime()
{
    return cas_wait_time;
}
float Casualty::getLastGravityChange()
{
    return cas_g_change_timestamp.back();
}

void Casualty::resetGravityChange(int on_period_reset)
{
    /*std::cout << "V" << cas_id_k << " BEFORE INROUTE FLAG = " << g_inroute_flag << std::endl;
    std::cout << "PREV GRAVITY FOR V" << cas_id_k << std::endl;
    for (unsigned int i = 0; i < cas_curgravity_g.size(); i++)
    {
        std::cout << cas_curgravity_g[i] << " ";
    }
    std::cout << std::endl;*/

    // RESET only if: its new period and reset is needed, or there was change in rerouting in the same period
    if (on_period_reset || (g_inroute_flag != prev_g_inroute_flag))
    {
        /*if (on_period_reset)
        {
            std::cout << "RESETTING FOR NEW PERIOD BECAUSE LAST CHANGE WAS AFTER NEW PERIOD START" << std::endl;
        }
        else if ((g_inroute_flag != prev_g_inroute_flag))
        {
            std::cout << "RESETTING BECAUSE FLAG != PREV ROUTE FLAG" << std::endl;
        }*/

        cas_curgravity_g = cas_prev_g;
        cas_g_change_timestamp = cas_prev_g_change_timestamp;
        prev_prev_g_inroute_flag = prev_g_inroute_flag;
        prev_g_inroute_flag = g_inroute_flag;
        g_inroute_flag = prev_prev_g_inroute_flag;
        /*std::cout << "INROUTE FLAG RESET TO " << g_inroute_flag << std::endl;
        std::cout << "CUR GRAVITY FOR V" << cas_id_k << std::endl;
        for (unsigned int i = 0; i < cas_curgravity_g.size(); i++)
        {
            std::cout << cas_curgravity_g[i] << " ";
        }*/
    }
    /*
    else
    {
        std::cout << "NO GRAVITY RESET NEEDED" << std::endl;
    }
    std::cout << std::endl;*/
}

void Casualty::resetLastAssignment()
{
    // reset gravity only if an in-route change ocurred in this iteration, otherwise maintain current history of changes
    if (g_inroute_flag == 1)
    {
        cas_curgravity_g = cas_prev_g;
        cas_g_change_timestamp = cas_prev_g_change_timestamp;
        g_inroute_flag = prev_g_inroute_flag;
        prev_g_inroute_flag = prev_prev_g_inroute_flag;
    }
    // id of assigned vehicle
    cas_assigned_veh = cas_prev_veh;
    cas_prev_veh = cas_prev_prev_veh;

    // type of vehicle assigned
    cas_assigned_veh_type = cas_prev_veh_type;
    cas_prev_veh_type = cas_prev_prev_veh_type;

    // assigned hospital
    cas_assigned_hosp = cas_prev_hosp;
    cas_prev_hosp = cas_prev_prev_hosp;

    // victim has to wait until X to vehicle to be assigned to it
    cas_wait_time = cas_prev_wait_time;
    cas_prev_wait_time = cas_prev_prev_wait_time;

    // the vehicle arrives at victims location at X
    cas_arrival_time = cas_prev_arrival_time;
    cas_prev_arrival_time = cas_prev_prev_arrival_time = -1;

    // victim is stabilized at X
    cas_st_time = cas_prev_st_time;
    cas_prev_st_time = cas_prev_prev_st_time;

    // victim is admitted to hospital at X
    cas_h_time = cas_prev_h_time;
    cas_prev_h_time = cas_prev_prev_h_time;
}

void Casualty::saveLastAssignment()
{
    // g_inroute_flag = 0;
    //  cas_prev_g = cas_prev_prev_g;
    prev_g_inroute_flag = prev_prev_g_inroute_flag;
    cas_prev_veh = cas_prev_prev_veh;
    cas_prev_veh_type = cas_prev_prev_veh_type;
    cas_prev_hosp = cas_prev_prev_hosp;
    cas_prev_wait_time = cas_prev_prev_wait_time;
    cas_prev_arrival_time = cas_prev_prev_arrival_time;
    cas_prev_st_time = cas_prev_prev_st_time;
    cas_prev_h_time = cas_prev_prev_h_time;
    // cas_prev_g_change_timestamp = cas_prev_prev_g_change_timestamp;
}

void Casualty::printData()
{
    std::cout << "Casualty ID: " << cas_id_k;
    std::cout << " Age: " << cas_age_a;
    std::cout << " Gravity: " << cas_curgravity_g.back();
    std::cout << " Location: " << cas_curlocation;
    std::cout << " Wait Time: " << cas_wait_time;
    std::cout << " Appear Time: " << cas_appear_time;
    std::cout << " Distance to Heliport: " << cas_time_to_heliport;
    std::cout << " Vehicle: " << cas_assigned_veh;
    std::cout << " Hospital: " << cas_assigned_hosp;
    std::cout << " Priority: " << cas_priority_lambda << std::endl;
}