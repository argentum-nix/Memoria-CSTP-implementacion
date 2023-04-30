#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <vector>

#include "vehicle.h"
#include "hospital.h"
#include "casualty.h"

#define LOADING_INSTANCE_FAILED -1
#define LOADING_INSTANCE_OK 0

#define DEBUG(x) cout << #x << " = " << x << endl

class Instance
{
public:
    Instance(std::string instance_name, std::string instance_directory);
    ~Instance();
    int loadData();

private:
    std::string name;
    std::string dir;

    float stabilization_time_tp[3][3];
    float deterioration_funct_params[3][3];
    float deterioration_time_pi[3][1];
    float factor_severity_PG[3][1];

    std::vector<Vehicle> ambulance_fleet;
    std::vector<Vehicle> helicopter_fleet;
    std::vector<Hospital> hospitals;
    std::vector<Casualty> casualties;
};

#endif