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

    // setters
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

    // getters
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

    // period-change specific methods
    void resetOccuipedToFirstAvailability(int period_start);

    void printData();
    void printOccupiedVector();

    // metaheuristic-specific methods
    void snapshotLastAssinment();
    void resetLastAssignment();
    void saveLastAssignment();
    void clearResetFlag();

private:
    // Static values
    int veh_id_m;
    int veh_capacity_q;
    int veh_type_e;
    int veh_appear_time;
    float veh_preptime_TDme;
    float veh_takeoff_TDSe;
    float veh_land_TATe;

    // Changing values (assignment-specific)
    std::vector<int> veh_total_rounds;
    std::vector<int> veh_prev_total_rounds;

    std::vector<int> veh_curlocation;
    std::vector<int> veh_prev_curlocation;

    std::vector<float> veh_occupied_until;
    std::vector<float> veh_prev_occupied_until;

    int cursor_location;
    int cursor_occupied_until;

    // methauristic-specific
    int was_already_reset = 0;
    int yet_to_snapshot = 1;
};

#endif