#include "instance.h"
#include <iostream>
#include <fstream>

using namespace std;

// Instance class constructor
Instance::Instance(std::string instance_name, std::string instance_directory)
{
    dir = instance_directory;
    name = instance_name;
}

// Instance class destructor
Instance::~Instance() {}

// int Instance::loadData
//======================================================================
// Lector de datos de archivos de las instancias.
//======================================================================
// Returns: int, -1 si hubo error en lectura de datos, 0 etoc
int Instance::loadData()
{
    ifstream in_file(this->dir + this->name + ".txt");

    // Auxiliary variables for inputs
    float a, b, c, d, e, f, g, h;
    char i;
    if (in_file.is_open())
    {
        streambuf *cinbuf = cin.rdbuf();
        cin.rdbuf(in_file.rdbuf());
        int count;
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
            // Section 2: Factor of Severity PG
            else if (txt_section == 1)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a;
                    this->factor_severity_PG[line][0] = a;
                }
                DEBUG(this->factor_severity_PG[0][0]);
                DEBUG(this->factor_severity_PG[1][0]);
                DEBUG(this->factor_severity_PG[2][0]);
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
            // Section 4: Deterioration time PI
            else if (txt_section == 3)
            {
                for (int line = 0; line < count; line++)
                {
                    cin >> a;
                    this->deterioration_time_pi[line][0] = a;
                }
                DEBUG(this->deterioration_time_pi[0][0]);
                DEBUG(this->deterioration_time_pi[1][0]);
                DEBUG(this->deterioration_time_pi[2][0]);
            }
            // Section 5: Hospitals
            else if (txt_section == 4)
            {
                cout << "Hospitales:\n";
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d;
                    // save internal id for matrix of distances
                }
            }
            // Section 6: Vehicles
            else if (txt_section == 5)
            {
                cout << "Vehiculos:\n";
                for (int line = 0; line < count; line++)
                {
                    cin >> i >> a >> b >> c >> d >> e >> f >> g;
                    // save helicopter origin id for matrix of distances
                }
            }
            // Section 7: Casualties
            else if (txt_section == 6)
            {
                cout << "Victimas:\n";
                for (int line = 0; line < count; line++)
                {
                    cin >> a >> b >> c >> d >> e >> f >> g >> h;
                    //
                }
            }
        }
        cin.rdbuf(cinbuf);
        in_file.close();
        return LOADING_INSTANCE_OK;
    }
    return LOADING_INSTANCE_FAILED;
}