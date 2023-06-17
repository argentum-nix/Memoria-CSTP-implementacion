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

void Vehicle::setVehicleAppearTime(int t)
{
    veh_appear_time = t;
}
void Vehicle::setVehicleOccupiedUntilTime(float t)
{
    veh_occupied_until.push_back(t);
}

void Vehicle::setVehicleRound(int round)
{
    veh_total_rounds.push_back(round);
}

void Vehicle::setVehicleLocation(int node_id)
{
    veh_curlocation.push_back(node_id);
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
int Vehicle::getVehicleAppearTime()
{
    return veh_appear_time;
}

float Vehicle::getVehicleOccupiedUntilTime()
{
    return veh_occupied_until.back();
}
int Vehicle::getVehicleLocation()
{
    return veh_curlocation.back();
}
int Vehicle::getVehicleRound()
{
    return veh_total_rounds.back();
}

void Vehicle::resetOccuipedToFirstAvailability(int period_start)
{
    int index = 0;
    for (unsigned int i = 0; i < veh_occupied_until.size(); i++)
    {
        if (veh_occupied_until[i] >= period_start)
        {
            index = i;
            break;
        }
    }
    // delete all elements after the first availability that occurs on or after the period start time
    veh_total_rounds.erase(veh_total_rounds.begin() + index + 1, veh_total_rounds.end());
    veh_occupied_until.erase(veh_occupied_until.begin() + index + 1, veh_occupied_until.end());
}

void Vehicle::printOccupiedVector()
{
    for (unsigned int i = 0; i < veh_occupied_until.size(); i++)
    {
        std::cout << veh_occupied_until[i] << " ";
    }
    std::cout << std::endl;
}

// es la funcion que se usa antes de probar nuevas rutas
void Vehicle::snapshotLastAssinment()
{
    // save previous states as copies
    veh_prev_total_rounds = veh_total_rounds;
    veh_prev_curlocation = veh_curlocation;
    veh_prev_occupied_until = veh_occupied_until;
    // slice the vector - we only want the slice of solution between start and cursor
    veh_curlocation.pop_back();
    veh_occupied_until.pop_back();
    veh_prev_total_rounds.pop_back();
}

// esta funcion se usa para deshacer nuevas rutas de metaheuristica y volver a la original con que se hizo un snapshpt
void Vehicle::resetLastAssignment()
{
    // copye prev solution to current, empty the prevs, reset the cursors
    veh_curlocation = veh_prev_curlocation;
    veh_occupied_until = veh_prev_occupied_until;
    veh_total_rounds = veh_prev_total_rounds;
    veh_prev_curlocation.clear();
    veh_prev_occupied_until.clear();
    veh_prev_total_rounds.clear();
}

void Vehicle::saveLastAssignment()
{
    // empty the prev-state containers
    // cur-state is the one we do not modify ("save")
    veh_prev_curlocation.clear();
    veh_prev_occupied_until.clear();
    veh_prev_total_rounds.clear();
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
    std::cout << " Location: " << veh_curlocation.back();
    std::cout << " Appear Time: " << veh_appear_time;
    std::cout << " Prep Time: " << veh_preptime_TDme;
    std::cout << " Takeoff Time: " << veh_takeoff_TDSe;
    std::cout << " Landing Time: " << veh_land_TATe << std::endl;
}