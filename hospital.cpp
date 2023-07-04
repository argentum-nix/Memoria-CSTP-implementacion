#include "hospital.h"

Hospital::Hospital() {}
Hospital::~Hospital() {}

void Hospital::setHospitalID(int id)
{
    hosp_id_h = id;
}

void Hospital::setHospitalLocation(int node_id)
{
    hosp_location = node_id;
}
void Hospital::setHospitalMaxCapacity(int g, int k)
{
    hosp_maxcapacity[g - 1] = k;
}
void Hospital::setHospitalCurCapacity(int g, int k)
{
    hosp_curcapacity[g - 1] = k;
}

void Hospital::setHospitalAppearTime(int t)
{
    hosp_appear_time = t;
}

int Hospital::getHospitalID()
{
    return hosp_id_h;
}

int Hospital::getHospitalLocation()
{
    return hosp_location;
}
int Hospital::getHospitalMaxCapacity(int g)
{
    return hosp_maxcapacity[g - 1];
}
int Hospital::getHospitalCurCapacity(int g)
{
    return hosp_curcapacity[g - 1];
}
int Hospital::getHospitalAppearTime()
{
    return hosp_appear_time;
}

void Hospital::snapshotSolution()
{
    if (yet_to_snapshot)
    {
        hosp_prev_capacity = hosp_curcapacity;
    }
    yet_to_snapshot = 0;
}

void Hospital::clearResetFlag()
{
    yet_to_snapshot = 1;
    was_already_reset = 0;
}

void Hospital::resetLastAssignment()
{
    if (was_already_reset != 1)
    {
        std::cout << "RESET LAST ASSIGNMENT"
                  << "MCC" << hosp_id_h << "hosp_curcapacity";
        for (int i = 0; i < hosp_curcapacity.size(); i++)
        {
            std::cout << hosp_curcapacity[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "RESET LAST ASSIGNMENT prev_capacity";
        for (int i = 0; i < hosp_prev_capacity.size(); i++)
        {
            std::cout << hosp_prev_capacity[i] << " ";
        }
        std::cout << std::endl;
        hosp_curcapacity = hosp_prev_capacity;
        hosp_prev_capacity.clear();
    }
    was_already_reset = 1;
    yet_to_snapshot = 1;
}

void Hospital::saveLastAssignment()
{
    hosp_prev_capacity.clear();
    was_already_reset = 0;
    yet_to_snapshot = 1;
}

void Hospital::printData()
{
    std::cout << "Hospital ID: " << hosp_id_h;
    std::cout << " Location: " << hosp_location;
    std::cout << " Appear Time: " << hosp_appear_time << " (" << hosp_appear_time / 3600 << ":" << (hosp_appear_time / 60) % 60 << ":" << hosp_appear_time % 60 << ") ";
    std::cout << " Max Capacity: [" << hosp_maxcapacity[0] << ", ";
    std::cout << hosp_maxcapacity[1] << ", ";
    std::cout << hosp_maxcapacity[2] << "]";
    std::cout << " Cur Capacity: [" << hosp_curcapacity[0] << ", ";
    std::cout << hosp_curcapacity[1] << ", ";
    std::cout << hosp_curcapacity[2] << "]" << std::endl;
}