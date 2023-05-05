#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>

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

    int getVehicleID();
    int getVehicleType();
    int getVehicleCapacity();
    int getVehicleLocation();
    float getVehiclePrepTime();
    float getVehicleTakeoffTime();
    float getVehicleLandingTime();

    void printData();

private:
    int veh_id_m;
    int veh_capacity_q;
    int veh_type_e;
    int veh_curlocation;
    float veh_preptime_TDme;
    float veh_takeoff_TDSe;
    float veh_land_TATe;
};

#endif