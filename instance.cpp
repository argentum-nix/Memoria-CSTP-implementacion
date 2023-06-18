#include "instance.h"
#include <fstream>

using namespace std;

// Instance class constructor
Instance::Instance(std::string instance_directory, std::string instance_name, std::string network_name)
{
    instance_txt_name = instance_name;
    load_directory = instance_directory;
    network_txt_name = network_name;

    DEBUG(instance_txt_name);
    DEBUG(load_directory);
    DEBUG(network_txt_name);
}

void Instance::setDeteriorationTime(float time, int g)
{
    deterioration_time_pi[g][0] = time;
}

void Instance::setStabilizationTime(float st1, float st2, float st3, int g)
{
    stabilization_time_tp[g][0] = st1;
    stabilization_time_tp[g][1] = st2;
    stabilization_time_tp[g][2] = st3;
}
void Instance::setDetFunctParams(float kappa, float phi, float w, float pg, int g)
{
    deterioration_funct_params[g][0] = kappa;
    deterioration_funct_params[g][1] = phi;
    deterioration_funct_params[g][2] = w;
    deterioration_funct_params[g][3] = pg;
}

void Instance::addHopsital(Hospital h)
{
    hospitals.push_back(h);
}

void Instance::addCasualty(Casualty c)
{
    casualties.push_back(c);
}

void Instance::addVehicle(Vehicle v)
{
    if (v.getVehicleType() == TYPE_AMBULANCE)
    {
        ambulance_fleet.push_back(v);
    }
    else if (v.getVehicleType() == TYPE_HELICOPTER)
    {
        helicopter_fleet.push_back(v);
    }
}

int Instance::getDeteriorationTimeValue(int g)
{
    return deterioration_time_pi[g - 1][0];
}

// Casualty-wrappers GET type
int Instance::getCasualtyAssignedVehicle(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyAssignedVehicle();
}
int Instance::getCasualtyAssignedVehicleType(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyAssignedVehicleType();
}
int Instance::getCasualtyGravity(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyGravity();
}

float Instance::getCasualtyPriority(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyPriority();
}

int Instance::getCasualtyAge(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyAge();
}

int Instance::getCasualtyLocation(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyLocation();
}

int Instance::getCasualtyAssignedHospital(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyAssignedHospital();
}

void Instance::clearCasualtyResetFlag(int casualty_id)
{
    casualties[casualty_id - 1].clearResetFlag();
}

float Instance::getCasualtyTimeToHeliport(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyTimeToHeliport();
}
float Instance::getCasualtyWaitingTime(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyWaitTime();
}

float Instance::getCasualtyAppearTime(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyAppearTime();
}

float Instance::getCasualtyVehArrivedTime(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyVehArrivedTime();
}
float Instance::getCasualtyStabilizedTime(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyStabilizedTime();
}
float Instance::getCasualtyAdmittedAtHopsitalTime(int casualty_id)
{
    return casualties[casualty_id - 1].getCasualtyAdmittedAtHospitalTime();
}

float Instance::getDetFunctParam(int param, int g)
{
    // kappa
    return deterioration_funct_params[g - 1][param];
}

float Instance::getCasualtyStabilizationTime(int casualty_id)
{
    int g = casualties[casualty_id - 1].getCasualtyGravity();
    int a = casualties[casualty_id - 1].getCasualtyAge();
    return stabilization_time_tp[g - 1][a - 1];
}

float Instance::getTimeBetweenNodes(int origin, int destination, int veh_type)
{
    int internal_origin_id = node_id_correlation_map[origin];
    int internal_dest_id = node_id_correlation_map[destination];
    // amb
    if (veh_type == 0)
    {
        return travel_time_ambulance[internal_origin_id][internal_dest_id];
    }
    // heli
    else if (veh_type == 1)
    {
        return travel_time_helicopter[internal_origin_id][internal_dest_id];
    }
    return -1;
}

int Instance::getPeriod(int period_id)
{
    return period_timestamps[period_id - 1];
}

float Instance::getSpeedDecrease(int period_id)
{
    return speed_decrease[period_id - 1];
}

// Vehicle-wrappers GET type
int Instance::getVehicleLocation(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        return ambulance_fleet[veh_id - 1].getVehicleLocation();
    }
    else if (veh_type == 1)
    {
        return helicopter_fleet[veh_id - 1].getVehicleLocation();
    }
    return -1;
}

int Instance::getVehicleAppearTime(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        return ambulance_fleet[veh_id - 1].getVehicleAppearTime();
    }
    else if (veh_type == 1)
    {
        return helicopter_fleet[veh_id - 1].getVehicleAppearTime();
    }
    return -1;
}

float Instance::getVehiclePrepTime(int veh_id, int veh_type)

{
    if (veh_type == 0)
    {
        return ambulance_fleet[veh_id - 1].getVehiclePrepTime();
    }
    else if (veh_type == 1)
    {
        return helicopter_fleet[veh_id - 1].getVehiclePrepTime();
    }
    return -1;
}
float Instance::getVehicleLandingTime(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        return ambulance_fleet[veh_id - 1].getVehicleLandingTime();
    }
    else if (veh_type == 1)
    {
        return helicopter_fleet[veh_id - 1].getVehicleLandingTime();
    }
    return -1;
}

int Instance::getVehicleRound(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        return ambulance_fleet[veh_id - 1].getVehicleRound();
    }
    else if (veh_type == 1)
    {
        return helicopter_fleet[veh_id - 1].getVehicleRound();
    }
    return -1;
}

float Instance::getVehicleTakeoffTime(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        return ambulance_fleet[veh_id - 1].getVehicleTakeoffTime();
    }
    else if (veh_type == 1)
    {
        return helicopter_fleet[veh_id - 1].getVehicleTakeoffTime();
    }
    return -1;
}

float Instance::getVehicleOccupiedUntilTime(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        return ambulance_fleet[veh_id - 1].getVehicleOccupiedUntilTime();
    }
    else if (veh_type == 1)
    {
        return helicopter_fleet[veh_id - 1].getVehicleOccupiedUntilTime();
    }
    return -1;
}

void Instance::updateVehicleOccupiedUntilTime(int veh_id, int veh_type, float t)
{
    if (veh_type == 0)
    {
        ambulance_fleet[veh_id - 1].setVehicleOccupiedUntilTime(t);
    }
    else if (veh_type == 1)
    {
        helicopter_fleet[veh_id - 1].setVehicleOccupiedUntilTime(t);
    }
}

void Instance::addVehicleRound(int veh_id, int veh_type)
{
    int prev_round;
    if (veh_type == 0)
    {
        prev_round = ambulance_fleet[veh_id - 1].getVehicleRound();
        if (prev_round == -1)
        {
            prev_round += 2;
        }
        else
        {
            prev_round += 1;
        }
        ambulance_fleet[veh_id - 1].setVehicleRound(prev_round);
    }
    else if (veh_type == 1)
    {
        prev_round = helicopter_fleet[veh_id - 1].getVehicleRound();
        if (prev_round == -1)
        {
            prev_round += 2;
        }
        else
        {
            prev_round += 1;
        }
        helicopter_fleet[veh_id - 1].setVehicleRound(prev_round);
    }
}

void Instance::resetVehicleOccuipedUntil(int veh_id, int veh_type, float period_start)
{
    if (veh_type == 0)
    {
        ambulance_fleet[veh_id - 1].resetOccuipedToFirstAvailability(period_start);
    }
    else if (veh_type == 1)
    {
        helicopter_fleet[veh_id - 1].resetOccuipedToFirstAvailability(period_start);
    }
}

// Casualty-wrappers UPDATE/SET type

void Instance::updateCasualtyHospital(int casualty_id, int hospital_id)
{
    casualties[casualty_id - 1].setCasualtyAssignedHospital(hospital_id);
}

void Instance::updateCasualtyWaitingTime(int casualty_id, float t)
{
    casualties[casualty_id - 1].setCasualtyWaitTime(t);
}

void Instance::updateCasualtyGravity(int casualty_id, int g, float t, int inroute_flag)
{
    casualties[casualty_id - 1].setCasualtyGravity(g);
    casualties[casualty_id - 1].addGravityChangeTimestamp(t, inroute_flag);
}

void Instance::resetCasualtyGravity(int casualty_id, float current_time)
{
    if (casualties[casualty_id - 1].getLastGravityChange() >= current_time)
    {
        casualties[casualty_id - 1].resetGravityChange();
    }
}
void Instance::resetCasualtyLastAssignment(int casualty_id)
{
    casualties[casualty_id - 1].resetLastAssignment();
}

void Instance::updateCasualtyRound(int casualty_id, int r)
{
    casualties[casualty_id].setCasualtyRound(r);
}

int Instance::getCasualtyRound(int casualty_id)
{
    return casualties[casualty_id].getCasualtyRound();
}

void Instance::saveCasualtyLastAssignment(int casualty_id)
{
    casualties[casualty_id - 1].saveLastAssignment();
}
void Instance::snapshotHospitalLastAssignment(int hospital_id, int g)
{
    hospitals[hospital_id - 1].snapshotSolution(g);
}

void Instance::updateVehicleLocation(int veh_id, int veh_type, int hospital_id)
{
    int node_id = hospitals[hospital_id - 1].getHospitalLocation();
    if (veh_type == 0)
    {
        ambulance_fleet[veh_id - 1].setVehicleLocation(node_id);
    }
    else if (veh_type == 1)
    {
        helicopter_fleet[veh_id - 1].setVehicleLocation(node_id);
    }
}

void Instance::snapshotVehicleLastAssignment(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        ambulance_fleet[veh_id - 1].snapshotLastAssinment();
    }
    else if (veh_type == 1)
    {
        helicopter_fleet[veh_id - 1].snapshotLastAssinment();
    }
}

void Instance::resetVehicleLastAssignment(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        ambulance_fleet[veh_id - 1].resetLastAssignment();
    }
    else if (veh_type == 1)
    {
        helicopter_fleet[veh_id - 1].resetLastAssignment();
    }
}

void Instance::saveVehicleLastAssignment(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        ambulance_fleet[veh_id - 1].saveLastAssignment();
    }
    else if (veh_type == 1)
    {
        helicopter_fleet[veh_id - 1].saveLastAssignment();
    }
}

void Instance::clearVehicleResetFlag(int veh_id, int veh_type)
{
    if (veh_type == 0)
    {
        ambulance_fleet[veh_id - 1].clearResetFlag();
    }
    else if (veh_type == 1)
    {
        helicopter_fleet[veh_id - 1].clearResetFlag();
    }
}

void Instance::updateCasualtyAppearTime(int casualty_id, float t)
{
    casualties[casualty_id - 1].setCasualtyAppearTime(t);
}

void Instance::updateCasualtyRouteTimes(int casualty_id, float assign_t, float arrive_t, float st_t, float h_t)
{
    casualties[casualty_id - 1].setCasualtyWaitTime(assign_t);
    casualties[casualty_id - 1].setCasualtyVehArrivedTime(arrive_t);
    casualties[casualty_id - 1].setCasualtyStabilizedTime(st_t);
    casualties[casualty_id - 1].setCasualtyAdmittedAtHospitalTime(h_t);
}

void Instance::updateCasualtyPriority(int casualty_id, float lambda)
{
    casualties[casualty_id - 1].setCasualtyPriority(lambda);
}

void Instance::updateCasualtyAssignedVehicle(int casualty_id, int veh_id, int veh_type)
{
    casualties[casualty_id - 1].setCasualtyAssignedVehicle(veh_id);
    casualties[casualty_id - 1].setCasualtyAssignedVehicleType(veh_type);
}

// Hospital-wrappers GET type
int Instance::getHospitalCurCapacity(int hospital_id, int g)
{
    return hospitals[hospital_id - 1].getHospitalCurCapacity(g);
}

int Instance::getHospitalLocation(int hospital_id)
{
    return hospitals[hospital_id - 1].getHospitalLocation();
}

int Instance::getHospitalAppearTime(int hospital_id)
{
    return hospitals[hospital_id - 1].getHospitalAppearTime();
}

// Hospital-wrappers UPDATE/SET type

void Instance::resetHospitalLastAssignment(int hospital_id)
{
    hospitals[hospital_id - 1].resetLastAssignment();
}

void Instance::updateHospitalBedCapacity(int hospital_id, int g, int beds)
{
    hospitals[hospital_id - 1].setHospitalCurCapacity(g, beds);
}

void Instance::printStabilizationTimeMatrix()
{
    for (int i = 0; i < 3; i++)
    {
        cout << "Stabilization Time for g=:" << i + 1;
        cout << ": [" << stabilization_time_tp[i][0] << ", ";
        cout << stabilization_time_tp[i][1] << ", ";
        cout << stabilization_time_tp[i][2] << "]" << endl;
    }
}

void Instance::printDeteriorationParamMatrix()
{
    for (int i = 0; i < 3; i++)
    {
        cout << "Deterioration Funct Params for g=:" << i + 1;
        cout << ": [" << deterioration_funct_params[i][0] << ", ";
        cout << deterioration_funct_params[i][1] << ", ";
        cout << deterioration_funct_params[i][2] << "]" << endl;
    }
}
void Instance::printDeteriorationTimeMatrix()
{
    cout << "Deterioration Time for (g1-2,g2-3): [" << deterioration_time_pi[0][0];
    cout << ", " << deterioration_time_pi[1][0] << "]" << endl;
}

void Instance::printHospital(int id)
{
    hospitals[id - 1].printData();
}

void Instance::printHospitals()
{
    for (unsigned int i = 1; i <= hospitals.size(); i++)
    {
        printHospital(i);
    }
}

void Instance::printCasualty(int id)
{
    casualties[id - 1].printData();
}

void Instance::printCasualties()
{
    for (unsigned int i = 1; i <= casualties.size(); i++)
    {
        printCasualty(i);
    }
}

void Instance::printAmbulances()
{
    for (unsigned int i = 1; i <= ambulance_fleet.size(); i++)
    {
        ambulance_fleet[i - 1].printData();
    }
}
void Instance::printHelicopters()
{
    for (unsigned int i = 1; i <= helicopter_fleet.size(); i++)
    {
        helicopter_fleet[i - 1].printData();
    }
}

void Instance::printPeriods()
{
    cout << "Periods: [";
    for (unsigned int i = 0; i < period_timestamps.size(); i++)
    {
        cout << period_timestamps[i] << ", ";
    }
    cout << "]" << endl;
}

// Instance class destructor
Instance::~Instance() {}

// int Instance::loadNetwork
//======================================================================
// Lector de datos de archivo con el grafo de la red
//======================================================================
// Returns: int, LOADING_FAILED si hubo error en lectura de datos, LOADING_OK etoc
int Instance::loadNetwork()
{

    ifstream in_file(this->load_directory + this->network_txt_name + ".txt");
    if (in_file.is_open())
    {
        streambuf *cinbuf = cin.rdbuf();
        cin.rdbuf(in_file.rdbuf());

        int count;
        string res = " ";
        // Read header, until we find a * character that marks the end of the instance header
        while (res[0] != '*')
        {
            getline(cin, res);
        }
        cin >> count;
        // resize the vectors to number of nodes in the network
        travel_time_ambulance.resize(count, vector<float>(count, 0.0));
        travel_time_helicopter.resize(count, vector<float>(count, 0.0));
        for (int i = 0; i < count; i++)
        {
            travel_time_ambulance[i].resize(count);
            travel_time_helicopter[i].resize(count);
        }
        // read the content of the file (network pairs)
        int internal_id = 0, cur_row = 0, cur_column = 0;
        int prev_row_node;

        int node_orig, node_dest;
        float heli_time, amb_time;
        for (int line = 0; line < count * count; line++)
        {
            cin >> node_orig >> node_dest >> heli_time >> amb_time;

            // add internal id, but only for first row to avoid repetition
            // dictionary, ex {1000001:0, 100002:1....} where key= instance node id, and value is 2d vector row/column
            if (line == 0)
            {
                prev_row_node = node_orig;
            }
            if (internal_id < count)
            {
                node_id_correlation_map.insert({node_dest, internal_id});
                internal_id++;
            }

            // reading next node row, should add values to next vectores
            if (node_orig != prev_row_node)
            {
                prev_row_node = node_orig;
                cur_row++;
            }
            travel_time_ambulance[cur_row][cur_column] = amb_time;
            travel_time_helicopter[cur_row][cur_column] = heli_time;
            // move cursor to next column, and if it gets out of bounds of number of nodes, reset it
            cur_column++;
            if (cur_column == count)
            {
                cur_column = 0;
            }
        }
        cin.rdbuf(cinbuf);
        in_file.close();
        return LOADING_OK;
    }
    return LOADING_FAILED;
}

// int Instance::loadData
//======================================================================
// Lector de datos de archivos de las instancias.
//======================================================================
// Returns: int, LOADING_FAILED si hubo error en lectura de datos, LOADING_OK etoc
int Instance::loadInstance()
{
    ifstream in_file(this->load_directory + this->instance_txt_name + ".txt");

    // Auxiliary variables for inputs
    float a, b, c, d, e, f, g, h;
    if (in_file.is_open())
    {
        streambuf *cinbuf = cin.rdbuf();
        cin.rdbuf(in_file.rdbuf());
        int count;

        string res = " ";
        // Read header, until we find a * character that marks the end of the instance header
        while (res[0] != '*')
        {
            getline(cin, res);
        }
        for (int txt_section = 0; txt_section < 7; txt_section++)
        {
            cin >> count;
            // Section 1: Time to Stabilize TP
            if (txt_section == 0)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c;
                    setStabilizationTime(a, b, c, line);
                }
            }
            // Section 2: Deterioration function parameters
            else if (txt_section == 1)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d;
                    setDetFunctParams(a, b, c, d, line);
                }
            }
            // Section 3: Deterioration time PI
            else if (txt_section == 2)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a;
                    setDeteriorationTime(a, line);
                }
            }
            // Section 4: Hospitals
            else if (txt_section == 3)
            {
                qty_hospitals = count;
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f;
                    Hospital hospital;
                    hospital.setHospitalID(a);
                    hospital.setHospitalLocation(b);
                    hospital.setHospitalAppearTime(c);
                    hospital.setHospitalMaxCapacity(1, d);
                    hospital.setHospitalMaxCapacity(2, e);
                    hospital.setHospitalMaxCapacity(3, f);
                    hospital.setHospitalCurCapacity(1, d);
                    hospital.setHospitalCurCapacity(2, e);
                    hospital.setHospitalCurCapacity(3, f);
                    addHopsital(hospital);
                }
            }
            // Section 5: Vehicles
            else if (txt_section == 4)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f >> g >> h;
                    Vehicle vehicle;
                    vehicle.setVehicleType(a);
                    if (a == 0)
                    {
                        qty_ambulances++;
                    }
                    else if (a == 1)
                    {
                        qty_helicopters++;
                    }
                    vehicle.setVehicleID(b);
                    vehicle.setVehicleLocation(c);
                    vehicle.setVehicleAppearTime(d);
                    vehicle.setVehicleCapacity(e);
                    vehicle.setVehiclePrepTime(f);
                    vehicle.setVehicleLandingTime(g);
                    vehicle.setVehicleTakeoffTime(h);
                    vehicle.setVehicleOccupiedUntilTime(d);
                    vehicle.setVehicleRound(0);
                    addVehicle(vehicle);
                }
            }
            // Section 6: Period timestamps
            else if (txt_section == 5)
            {
                for (int line = 0; line < count; line++)
                {
                    qty_periods = count;
                    cin >> a >> b;
                    period_timestamps.push_back(a);
                    speed_decrease.push_back(b);
                }
            }
            // Section 7: Casualties
            else if (txt_section == 6)
            {
                qty_casualties = count;
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f;
                    Casualty casualty;
                    casualty.setCasualtyLocation(a);
                    casualty.setCasualtyID(b);
                    casualty.setCasualtyAge(c);
                    casualty.setCasualtyGravity(d);
                    casualty.setCasualtyWaitTime(e);
                    casualty.setCasualtyAppearTime(e);
                    casualty.setCasualtyTimeToHeliport(f);
                    casualty.addGravityChangeTimestamp(e, 0);
                    casualty.setCasualtyAssignedVehicle(-1);
                    casualty.setCasualtyAssignedVehicleType(-1);
                    casualty.setCasualtyAssignedHospital(-1);
                    casualty.setCasualtyVehArrivedTime(-1);
                    casualty.setCasualtyStabilizedTime(-1);
                    casualty.setCasualtyAdmittedAtHospitalTime(-1);
                    casualty.setCasualtyPriority(-1);
                    addCasualty(casualty);
                }
            }
        }
        // Prints the input data
        // printStabilizationTimeMatrix();
        // printDeteriorationParamMatrix();
        printDeteriorationTimeMatrix();
        // printAmbulances();
        // printHelicopters();
        // printHospitals();
        // printPeriods();
        // printCasualties();

        cin.rdbuf(cinbuf);
        in_file.close();
        return LOADING_OK;
    }

    return LOADING_FAILED;
}