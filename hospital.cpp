#include "hospital.h"

// Hospital class constructor
Hospital::Hospital() {}

// Hospital class destructor
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

int Hospital::getHospitalID()
{
    return hosp_id_h;
}
int Hospital::getHospitalMaxCapacity(int g)
{
    return hosp_maxcapacity[g - 1];
}
int Hospital::getHospitalCurCapacity(int g)
{
    return hosp_curcapacity[g - 1];
}

void Hospital::printData()
{
    std::cout << "Hospital ID: " << hosp_id_h;
    std::cout << " Location: " << hosp_location;
    std::cout << " Max Capacity: [" << hosp_maxcapacity[0] << ", ";
    std::cout << hosp_maxcapacity[1] << ", ";
    std::cout << hosp_maxcapacity[2] << "]";
    std::cout << " Cur Capacity: [" << hosp_curcapacity[0] << ", ";
    std::cout << hosp_curcapacity[1] << ", ";
    std::cout << hosp_curcapacity[2] << "]" << std::endl;
}