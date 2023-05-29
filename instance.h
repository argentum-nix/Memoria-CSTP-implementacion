#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <vector>
#include <map>

#include "vehicle.h"
#include "hospital.h"
#include "casualty.h"

#define LOADING_FAILED -1
#define LOADING_OK 0

#define DEBUG(x) cout << #x << " = " << x << endl

class Instance
{
public:
    Instance(std::string instance_directory, std::string instance_name, std::string network_name);
    ~Instance();
    int loadInstance();
    int loadNetwork();

    int qty_casualties = 0;
    int qty_ambulances = 0;
    int qty_helicopters = 0;
    int qty_hospitals = 0;
    int qty_periods = 0;

    // Set Instance-Specific values
    void setDeteriorationTime(float time, int g);
    void setDetFunctParams(float kappa, float phi, float w, float pg, int g);
    void setStabilizationTime(float st1, float st2, float st3, int g);

    // Poblate Instance vectors with data
    void addHopsital(Hospital h);
    void addVehicle(Vehicle v);
    void addCasualty(Casualty c);

    // Get Instance-Specific values
    // int getStabilizationTimeValue(int g, int a);
    int getDeteriorationTimeValue(int g);
    float getTimeBetweenNodes(int origin, int destination, int veh_type);
    int getPeriod(int period_id);
    float getDetFunctParam(int param, int g);

    // Casualty-class wrappers
    int getCasualtyLocation(int casualty_id);
    int getCasualtyGravity(int casualty_id);
    float getCasualtyWaitingTime(int casualty_id);
    int getCasualtyAge(int casualty_id);
    float getCasualtyAppearTime(int casualty_id);
    float getCasualtyTimeToHeliport(int casualty_id);
    float getCasualtyStabilizationTime(int casualty_id);
    void updateCasualtyGravity(int casualty_id, int g);
    void updateCasualtyWaitingTime(int casualty_id, float t);
    void updateCasualtyAppearTime(int casualty_id, float t);
    void updateCasualtyPriority(int casualty_id, int lambda);

    // Vehicle-class wrappers
    int getVehicleLocation(int veh_id, int veh_type);
    float getVehicleLandingTime(int veh_id, int veh_type);
    float getVehicleTakeoffTime(int veh_id, int veh_type);
    float getVehiclePrepTime(int veh_id, int veh_type);
    int getVehicleRound(int veh_id, int veh_type);
    float getVehicleOccupiedUntilTime(int veh_id, int veh_type);
    void updateVehicleOccupiedUntilTime(int veh_id, int veh_type, float t);
    void addVehicleRound(int veh_id, int veh_type);

    // Hospital-class wrappers
    int getHospitalLocation(int hospital_id);
    int getHospitalCurCapacity(int hospital_id, int g);
    void updateHospitalBedCapacity(int hospital_id, int g, int beds);

    // Print Instance-Specific values
    void printStabilizationTimeMatrix();
    void printDeteriorationParamMatrix();
    void printDeteriorationTimeMatrix();
    void printCasualty(int id);
    void printCasualtyVector();
    void printHospital(int id);
    void printHospitalVector();
    void printVehicle(int id, int type);
    void printAmbulances();
    void printHelicopters();
    void printPeriods();

private:
    std::string instance_txt_name;
    std::string network_txt_name;
    std::string load_directory;

    float stabilization_time_tp[3][3];
    float deterioration_funct_params[3][4];
    float deterioration_time_pi[2][1];

    std::vector<int> period_timestamps;
    std::map<int, int> node_id_correlation_map;
    std::vector<std::vector<float>> travel_time_ambulance;
    std::vector<std::vector<float>> travel_time_helicopter;
    std::vector<Vehicle> ambulance_fleet;
    std::vector<Vehicle> helicopter_fleet;
    std::vector<Hospital> hospitals;
    std::vector<Casualty> casualties;
};

#endif