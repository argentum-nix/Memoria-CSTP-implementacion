#include "instance.h"
#include <iostream>
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

void Instance::printStabilizationTimeMatrix()
{
    DEBUG(this->stabilization_time_tp[0][0]);
    DEBUG(this->stabilization_time_tp[0][1]);
    DEBUG(this->stabilization_time_tp[0][2]);
    DEBUG(this->stabilization_time_tp[1][0]);
    DEBUG(this->stabilization_time_tp[1][1]);
    DEBUG(this->stabilization_time_tp[1][2]);
    DEBUG(this->stabilization_time_tp[2][0]);
    DEBUG(this->stabilization_time_tp[2][1]);
    DEBUG(this->stabilization_time_tp[2][2]);
}

void Instance::printDeteriorationParamMatrix()
{
    DEBUG(this->deterioration_funct_params[0][0]);
    DEBUG(this->deterioration_funct_params[0][1]);
    DEBUG(this->deterioration_funct_params[0][2]);
    DEBUG(this->deterioration_funct_params[1][0]);
    DEBUG(this->deterioration_funct_params[1][1]);
    DEBUG(this->deterioration_funct_params[1][2]);
    DEBUG(this->deterioration_funct_params[2][0]);
    DEBUG(this->deterioration_funct_params[2][1]);
    DEBUG(this->deterioration_funct_params[2][2]);
}
void Instance::printDeteriorationTimeMatrix()
{
    DEBUG(this->deterioration_time_pi[0][0]);
    DEBUG(this->deterioration_time_pi[1][0]);
    DEBUG(this->deterioration_time_pi[2][0]);
}
void Instance::printFactorSeverityMatrix()
{
    DEBUG(this->factor_severity_PG[0][0]);
    DEBUG(this->factor_severity_PG[1][0]);
    DEBUG(this->factor_severity_PG[2][0]);
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
            cout << res << endl;
        }
        cin >> count;
        DEBUG(count);
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
        for (int txt_section = 0; txt_section < 7; txt_section++)
        {
            cin >> count;
            // Section 1: Time to Stabilize TP
            if (txt_section == 0)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c;
                    this->stabilization_time_tp[line][0] = a;
                    this->stabilization_time_tp[line][1] = b;
                    this->stabilization_time_tp[line][2] = c;
                }
                printStabilizationTimeMatrix();
            }
            // Section 2: Factor of Severity PG
            else if (txt_section == 1)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a;
                    this->factor_severity_PG[line][0] = a;
                }
                printFactorSeverityMatrix();
            }
            // Section 3: Deterioration function parameters
            else if (txt_section == 2)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c;
                    this->deterioration_funct_params[line][0] = a;
                    this->deterioration_funct_params[line][1] = b;
                    this->deterioration_funct_params[line][2] = c;
                }
                printDeteriorationParamMatrix();
            }
            // Section 4: Deterioration time PI
            else if (txt_section == 3)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a;
                    this->deterioration_time_pi[line][0] = a;
                }
                printDeteriorationTimeMatrix();
            }
            // Section 5: Hospitals
            else if (txt_section == 4)
            {
                cout << "Hospitales:\n";
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d;
                }
            }
            // Section 5: Vehicles
            else if (txt_section == 5)
            {
                cout << "Vehiculos:\n";
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f >> g;
                }
            }
            // Section 7: Casualties
            else if (txt_section == 6)
            {
                cout << "Victimas:\n";
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f;
                }
            }
        }
        cin.rdbuf(cinbuf);
        in_file.close();
        return LOADING_OK;
    }
    return LOADING_FAILED;
}