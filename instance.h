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

    void setDeteriorationTime(float time, int g);
    void setFactorSeverity(float factor, int g);
    void setDetFunctParams(float kappa, float phi, float w, int g);
    void setStabilizationTime(float st1, float st2, float st3, int g);

    void addHopsital(Hospital h);
    void addVehicle(Vehicle v);
    void addCasualty(Casualty c);

    void printStabilizationTimeMatrix();
    void printDeteriorationParamMatrix();
    void printDeteriorationTimeMatrix();
    void printFactorSeverityMatrix();
    void printCasualty(int id);
    void printCasualtyVector();
    void printHospital(int id);
    void printHospitalVector();
    void printVehicle(int id, int type);
    void printAmbulances();
    void printHelicopters();

private:
    std::string instance_txt_name;
    std::string network_txt_name;
    std::string load_directory;

    float stabilization_time_tp[3][3];
    float deterioration_funct_params[3][3];
    float deterioration_time_pi[3][1];
    float factor_severity_PG[3][1];

    std::map<int, int> node_id_correlation_map;

    std::vector<std::vector<float>> travel_time_ambulance;
    std::vector<std::vector<float>> travel_time_helicopter;
    std::vector<Vehicle> ambulance_fleet;
    std::vector<Vehicle> helicopter_fleet;
    std::vector<Hospital> hospitals;
    std::vector<Casualty> casualties;
};

#endif