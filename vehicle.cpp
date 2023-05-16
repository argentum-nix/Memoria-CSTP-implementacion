#include "vehicle.h"

// Vehicle class constructor
Vehicle::Vehicle() {}

// Vehicle class destructor
Vehicle::~Vehicle() {}

void Vehicle::setVehicleID(int id)
{
    veh_id_m = id;
}

void Vehicle::setVehicleType(int type)
{
    veh_type_e = type;
}

void Vehicle::setVehicleCapacity(int q)
{
    veh_capacity_q = q;
}

void Vehicle::setVehicleLocation(int node_id)
{
    veh_curlocation = node_id;
}

void Vehicle::setVehiclePrepTime(float td)
{
    veh_preptime_TDme = td;
}

void Vehicle::setVehicleTakeoffTime(float tds)
{
    veh_takeoff_TDSe = tds;
}

void Vehicle::setVehicleLandingTime(float tat)
{
    veh_land_TATe = tat;
}

int Vehicle::getVehicleID()
{
    return veh_id_m;
}

int Vehicle::getVehicleCapacity()
{
    return veh_capacity_q;
}

int Vehicle::getVehicleType()
{
    return veh_type_e;
}
int Vehicle::getVehicleLocation()
{
    return veh_curlocation;
}
float Vehicle::getVehiclePrepTime()
{
    return veh_preptime_TDme;
}
float Vehicle::getVehicleTakeoffTime()
{
    return veh_takeoff_TDSe;
}
float Vehicle::getVehicleLandingTime()
{
    return veh_land_TATe;
}

void Vehicle::printData()
{
    if (veh_type_e == TYPE_AMBULANCE)
    {
        std::cout << "Vehicle ID: " << veh_id_m << " Type: Ambulance ";
    }
    else if (veh_type_e == TYPE_HELICOPTER)
    {
        std::cout << "Vehicle ID: " << veh_id_m << " Type: Helicopter ";
    }
    std::cout << "Capacity: " << veh_capacity_q;
    std::cout << " Location: " << veh_curlocation;
    std::cout << " Prep Time: " << veh_preptime_TDme;
    std::cout << " Takeoff Time: " << veh_takeoff_TDSe;
    std::cout << " Landing Time: " << veh_land_TATe << std::endl;
}