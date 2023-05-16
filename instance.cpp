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

Casualty Instance::getCasualty(int id)
{
    return casualties[id - 1];
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

void Instance::printHospitalVector()
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

void Instance::printCasualtyVector()
{
    for (unsigned int i = 1; i <= casualties.size(); i++)
    {
        printCasualty(i);
    }
}

void Instance::printVehicle(int id, int type)
{
    if (type == TYPE_AMBULANCE)
    {
        ambulance_fleet[id - 1].printData();
    }
    else if (type == TYPE_HELICOPTER)
    {
        helicopter_fleet[id - 1].printData();
    }
}
void Instance::printAmbulances()
{
    for (unsigned int i = 1; i <= ambulance_fleet.size(); i++)
    {
        printVehicle(i, 0);
    }
}
void Instance::printHelicopters()
{
    for (unsigned int i = 1; i <= helicopter_fleet.size(); i++)
    {
        printVehicle(i, 1);
    }
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
        for (int line; line < count * count; line++)
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
    float a, b, c, d, e, f, g;
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
        for (int txt_section = 0; txt_section < 6; txt_section++)
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
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e;
                    Hospital hospital;
                    hospital.setHospitalID(a);
                    hospital.setHospitalLocation(b);
                    hospital.setHospitalMaxCapacity(1, c);
                    hospital.setHospitalMaxCapacity(2, d);
                    hospital.setHospitalMaxCapacity(3, e);
                    hospital.setHospitalCurCapacity(1, c);
                    hospital.setHospitalCurCapacity(2, d);
                    hospital.setHospitalCurCapacity(3, e);
                    addHopsital(hospital);
                }
            }
            // Section 5: Vehicles
            else if (txt_section == 4)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f >> g;
                    Vehicle vehicle;
                    vehicle.setVehicleType(a);
                    vehicle.setVehicleID(b);
                    vehicle.setVehicleLocation(c);
                    vehicle.setVehicleCapacity(d);
                    vehicle.setVehiclePrepTime(e);
                    vehicle.setVehicleLandingTime(f);
                    vehicle.setVehicleTakeoffTime(g);
                    addVehicle(vehicle);
                }
            }
            // Section 6: Casualties
            else if (txt_section == 5)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f >> g;
                    Casualty casualty;
                    casualty.setCasualtyLocation(a);
                    casualty.setCasualtyID(b);
                    casualty.setCasualtyAge(c);
                    casualty.setCasualtyGravity(d);
                    casualty.setCasualtyWaitTime(e);
                    casualty.setCasualtyAppearTime(f);
                    casualty.setCasualtyTimeToHeliport(g);
                    casualty.setCasualtyAssignedVehicle(-1);
                    casualty.setCasualtyAssignedHospital(-1);
                    casualty.setCasualtyPriority(0);
                    addCasualty(casualty);
                }
            }
        }
        printStabilizationTimeMatrix();
        printDeteriorationParamMatrix();
        printDeteriorationTimeMatrix();
        printAmbulances();
        printHelicopters();
        printHospitalVector();
        printCasualtyVector();

        cin.rdbuf(cinbuf);
        in_file.close();
        return LOADING_OK;
    }

    return LOADING_FAILED;
}