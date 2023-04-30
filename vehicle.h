#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle
{
public:
    Vehicle();
    ~Vehicle();
    void setVehicleID();
    void setVehicleType();
    void setVehicleLocation();
    void setVehiclePrepTime();
    void setVehicleTakeoffTime();
    void setVehicleLandingTime();

    int getVehicleID();
    int getVehicleType();
    int getVehicleLocation();
    int getVehiclePrepTime();
    int getVehicleTakeoffTime();
    int getVehicleLandingTime();

private:
    int veh_id_m;
    char veh_type_e;
    int veh_curlocation;
    int veh_preptime_TDme;
    int veh_takeoff_TDSe;
    int veh_land_TATe;
};

#endif