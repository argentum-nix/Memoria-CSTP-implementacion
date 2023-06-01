#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <vector>

#define TYPE_HELICOPTER 1
#define TYPE_AMBULANCE 0

class Vehicle
{
public:
    Vehicle();
    ~Vehicle();
    void setVehicleID(int id);
    void setVehicleType(int type);
    void setVehicleCapacity(int q);
    void setVehicleLocation(int node_id);
    void setVehiclePrepTime(float td);
    void setVehicleTakeoffTime(float tds);
    void setVehicleLandingTime(float tat);
    void setVehicleOccupiedUntilTime(float t);
    void setVehicleRound(int round);

    int getVehicleID();
    int getVehicleType();
    int getVehicleCapacity();
    int getVehicleLocation();
    int getVehicleRound();
    float getVehiclePrepTime();
    float getVehicleTakeoffTime();
    float getVehicleLandingTime();
    float getVehicleOccupiedUntilTime();

    void resetOccuipedToFirstAvailability(int period_start);

    void printData();

private:
    int veh_id_m;
    int veh_capacity_q;
    int veh_type_e;
    float veh_preptime_TDme;
    float veh_takeoff_TDSe;
    float veh_land_TATe;

    int total_rounds;
    int veh_curlocation;
    std::vector<float> occupied_until;
};

#endif