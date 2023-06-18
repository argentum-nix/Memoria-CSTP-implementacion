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
    void setVehicleAppearTime(int t);

    int getVehicleID();
    int getVehicleType();
    int getVehicleCapacity();
    int getVehicleLocation();
    int getVehicleRound();
    int getVehicleAppearTime();
    float getVehiclePrepTime();
    float getVehicleTakeoffTime();
    float getVehicleLandingTime();
    float getVehicleOccupiedUntilTime();

    void resetOccuipedToFirstAvailability(int period_start);

    void printData();
    void printOccupiedVector();

    // metaheuristic-specific functions for trying new solutions
    void snapshotLastAssinment();
    void resetLastAssignment();
    void saveLastAssignment();
    void clearResetFlag();

private:
    int veh_id_m;
    int veh_capacity_q;
    int veh_type_e;
    int veh_appear_time;
    float veh_preptime_TDme;
    float veh_takeoff_TDSe;
    float veh_land_TATe;

    int was_already_reset = 0;
    int yet_to_snapshot = 1;
    int cursor_location;
    int cursor_occupied_until;
    std::vector<int> veh_total_rounds;
    std::vector<int> veh_prev_total_rounds;
    std::vector<int> veh_prev_curlocation;
    std::vector<int> veh_curlocation;
    std::vector<float> veh_prev_occupied_until;
    std::vector<float> veh_occupied_until;
};

#endif