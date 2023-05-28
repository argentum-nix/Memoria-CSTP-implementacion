#include "solver.h"
#include <math.h>
#include <iomanip>
using namespace std;

Solver::Solver(Instance *in)
{
    instance = in;

    float lambda;
    for (int i = 1; i <= instance->qty_casualties; i++)
    {
        // Poblate the priority list with incoming victims
        // TODO: check if id "i" already was instanced, if so, update its lambda or skip it entirely
        lambda = calculatePriority(instance->getCasualtyWaitingTime(i), instance->getCasualtyGravity(i));
        instance->updateCasualtyPriority(i, lambda);
        // add new victim to priority_list
        priority_list.push_back(make_pair(lambda, i));
        // update waiting time
        updateWaitingTime(i);
    }
    // order the list from greater to smaller

    sort(priority_list.rbegin(), priority_list.rend(), [](const auto &l, const auto &r)
         { return (l.first == r.first) ? l.second > r.second : l.first < r.first; });
    // sort(priority_list.rbegin(), priority_list.rend());
    printPriorityList();
    cout << endl;
    cout << "=GREEDY=" << endl;
    greedyAssignment();
}

void Solver::printPriorityList()
{
    cout << "PRIORITY LIST: ";
    for (int i = 0; i < instance->qty_casualties; i++)
    {
        cout << "(" << priority_list[i].first << ", " << priority_list[i].second << "), ";
    }
    cout << endl;
}

// Solver class destructor
Solver::~Solver() {}

// update TE
void Solver::updateWaitingTime(int casualty_id)
{
    // TODO: 5 es el valor de inicio de periodo TT, instancia todavia no maneja estos valores.
    // Deberian venir dentro de la misma instancia.
    int TT = 5;
    int TA = instance->getCasualtyAppearTime(casualty_id);
    instance->updateCasualtyWaitingTime(casualty_id, TT - TA);
}

void Solver::updateStatesOfCasualties()
{
    // TODO: tiempo de espera maxima para pasar a siguiente LSI debe venir dentro de la instancia...
    int TEmax1 = 1140;
    int TEmax2 = 360;
    for (int i = 1; i <= instance->qty_casualties; i++)
    {
        // Case 1: Any victim of LSI 1 already waited enough to be considered LSI 2
        int g = instance->getCasualtyGravity(i);
        int te = instance->getCasualtyWaitingTime(i);
        if (g == 1 && te > TEmax1)
        {
            instance->updateCasualtyGravity(i, 2);
            instance->updateCasualtyAppearTime(i, instance->getCasualtyAppearTime(i) + instance->getDeteriorationTimeValue(g));
        }
        // Case 1: Any victim of LSI 2 already waited enough to be considered LSI 3
        else if (g == 2 && te > TEmax2)
        {
            instance->updateCasualtyGravity(i, 3);
            instance->updateCasualtyWaitingTime(i, instance->getCasualtyWaitingTime(i) - TEmax2);
            instance->updateCasualtyAppearTime(i, instance->getCasualtyAppearTime(i) + instance->getDeteriorationTimeValue(g));
        }
    }
}

void Solver::updatePriorityOfCasualties()
{
    // TODO: tiempo de espera maxima para pasar a siguiente LSI debe venir dentro de la instancia...
    int TEmax1 = 1140;
    int TEmax2 = 360;
    for (int i = 1; i <= instance->qty_casualties; i++)
    {
        int g = instance->getCasualtyGravity(i);
        int te = instance->getCasualtyWaitingTime(i);
        if (g == 1 && te <= TEmax1)
        {
            instance->updateCasualtyPriority(i, calculatePriority(instance->getCasualtyWaitingTime(i), 1));
        }
        else if (g == 2 && te <= TEmax2)
        {
            instance->updateCasualtyPriority(i, calculatePriority(instance->getCasualtyWaitingTime(i), 2));
        }
        else if (g == 3)
        {
            instance->updateCasualtyPriority(i, calculatePriority(instance->getCasualtyWaitingTime(i), 3));
        }
    }
}

float Solver::calculatePriority(float te, int g)
{
    // diferencia de resultados de prioridad es debido a que en modelo usaron euler 2.718281
    if (g == 1)
    {
        return 0.1 + 0.1 * exp(1 + 0.048 * te / 60);
    }
    else if (g == 2)
    {
        return 0.4 + 0.1 * exp(1.6 + 0.374 * te / 60);
    }
    else if (g == 3)
    {
        return 4.5 + 0.1 * exp(1.8 + 0.4 * te / 60);
    }
    return 0;
}

void Solver::greedyAssignment()
{
    // TODO: Revisar los vehiculos y si ya se desocupan
    int first_id, g, h_capacity;
    int closest_h_amb = -1;
    int closest_h_heli = -1;
    float min_dh_amb = 999999;
    float min_dh_heli = 999999;
    float min_dv_amb = 999999;
    float min_dv_heli = 999999;
    int available_vehicles = 0;
    float waiting_till = 0;
    int closest_amb = -1;
    int closest_heli = -1;
    float min_availability_amb = 999999;
    int next_available_amb = -1;
    float actual_time = -1;
    float tot_sec = 0;

    for (int i = 0; i < int(priority_list.size()); i++)
    {
        // pick the first victim on top of the list
        first_id = priority_list[i].second;
        g = instance->getCasualtyGravity(first_id);
        // Find the closest hospital that has de capacity to attend the casualty
        min_dh_amb = 999999;
        min_dh_heli = 999999;
        closest_h_amb = -1;
        closest_h_heli = -1;
        cout << "Searching for closest hospital with beds..." << endl;
        for (int h = 1; h <= instance->qty_hospitals; h++)
        {
            h_capacity = instance->getHospitalCurCapacity(h, g);
            // if this hospital can attend such gravity then
            if (h_capacity > 0)
            {
                // find closest hospital for AMBULANCE trip
                cout << "[Victim->Hospital in ambulance] " << instance->getCasualtyLocation(first_id) << " -> " << instance->getHospitalLocation(h) << " = ";
                cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 0) << endl;
                if (instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 0) < min_dh_amb)
                {
                    min_dh_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 0);
                    closest_h_amb = h;
                }
                /*
                // find closest hospital for HELICOPTER trip
                cout << "[Victim->Hospital in helicopter] " << instance->getCasualtyLocation(first_id) << " -> " << instance->getHospitalLocation(h) << " = ";
                cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 1);
                // time with landing and takeoff times added
                actual_time =  instance->getVehicleLandingTime(closest_heli, 1) + instance->getVehicleTakeoffTime(closest_heli, 1) + instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(h), 1);
                cout << " and with takeoff/landing: " << actual_time << endl;
                if (actual_time < min_dh_heli)
                {
                    min_dh_heli = actual_time;
                    closest_h_heli = h;
                }
                */
            }
        }
        // Find an available vehicle, that takes the smallest time to get to the victim
        // Case: ambulance
        cout << "Searching for closest vehicle to pick up the casualty..." << endl;
        min_dv_amb = 999999;
        closest_amb = -1;
        available_vehicles = 0;
        for (int a = 1; a <= instance->qty_ambulances; a++)
        {
            // check if vehicle is not occupied
            if (instance->getVehicleOccupiedUntilTime(a, 0) < start_time)
            {
                available_vehicles++;
                cout << "[Pickup->Victim for A" << a << "] " << instance->getVehicleLocation(a, 0) << " -> " << instance->getCasualtyLocation(first_id) << " = ";
                cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(a, 0), 0) << endl;
                if (instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(a, 0), 0) < min_dv_amb)
                {
                    min_dv_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(a, 0), 0);
                    closest_amb = a;
                }
            }
        }
        min_availability_amb = 999999;
        next_available_amb = -1;
        if (available_vehicles == 0)
        {
            // find a vehicle that will be available first between all the occupied ones
            // and assign the wait time alpha_k as the time between (vehicle's fin of route - start of period)
            for (int a = 1; a <= instance->qty_ambulances; a++)
            {
                if (instance->getVehicleOccupiedUntilTime(a, 0) < min_availability_amb)
                {
                    min_availability_amb = instance->getVehicleOccupiedUntilTime(a, 0);
                    next_available_amb = a;
                }
            }

            cout << "All vehicles occcupied. Next available vehicle is A" << next_available_amb << " in " << (min_availability_amb) / 60;
            cout << " (" << int(min_availability_amb) / 3600 << ":" << (int(min_availability_amb) / 60) % 60 << ":" << int(min_availability_amb) % 60 << ")" << endl;
            closest_amb = next_available_amb;
            // asignar valores de tiempos de viaje desde victima la hospital, y hacia la victima
            // caso: con hospital disponible, sin vehiculos disponibles
            if (closest_h_amb != -1)
            {
                min_dh_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(closest_h_amb), 0);
            }
            // caso: sin hospitales disponibles, sin vehiculos disponibles
            else
            {
                cout << "No hospitals can attend this victim." << endl;
            }
            min_dv_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(closest_amb, 0), 0);
            // se asigna hasta que momento en segundos desde el inicio la victima espera su vehiculo
            instance->updateCasualtyWaitingTime(first_id, min_availability_amb);
        }

        /*
        // Mismo para helicoptero
        min_dv_heli = 999999;
        closest_heli = -1;
        // Case: helicopter
        for (int h = 1; h <= instance->qty_helicopters; h++)
        {
            cout << "[Pickup->Victim for H" << h << "] " << instance->getVehicleLocation(h, 1) << " -> " << instance->getCasualtyLocation(first_id) << " = ";
            cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(h, 1), 1);
            actual_time = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(h, 1), 1) + instance->getVehicleLandingTime(closest_heli, 1) + instance->getVehicleTakeoffTime(closest_heli, 1) + 2 * instance->getCasualtyTimeToHeliport(first_id);
            cout << " and with takeoff/landing and 2x running: " << actual_time << endl;
            if (actual_time < min_dv_heli)
            {
                min_dv_heli = actual_time;
                closest_heli = h;
            }
        }
        */

        // Si es la primera asigancion de ambulancia, se debe agregar el tiempo de preparacion
        if (instance->getVehicleOccupiedUntilTime(closest_amb, 0) == 0)
        {
            min_dh_amb += instance->getVehiclePrepTime(closest_amb, 0);
        }

        /*
        // mismo para helicoptero + tiempos extra de helicoptero
        if (instance->getVehicleOccupiedUntilTime(closest_heli, 1) == 0)
        {
            min_dh_heli += instance->getVehiclePrepTime(closest_heli, 1);
        }
        */

        // Puros prints de tabla de asignacion ambulancia o helicoptero
        cout << left << "V" << first_id << "    ";
        cout << left << instance->getCasualtyGravity(first_id) << "    ";
        cout << left << instance->getCasualtyAge(first_id) << "    ";
        cout << left << priority_list[i].first << "    ";
        cout << left << instance->getCasualtyStabilizationTime(first_id) << "    ";
        cout << left << "A" << closest_amb << "    ";
        cout << left << "ROUND " << instance->getVehicleRound(closest_amb, 0) << "    ";
        cout << left << "MCC" << closest_h_amb << "    ";
        // current time == init period time in seconds
        waiting_till = instance->getCasualtyWaitingTime(first_id) / 60;
        // If there is some kind of wait, it should be added. The moment of assignment is starting time + all the wait
        if (waiting_till != 0)
        {
            tot_sec = min_dv_amb * 60 + start_time + (waiting_till * 60 - start_time);
            cout << left << waiting_till << " (" << int(waiting_till * 60) / 3600 << ":" << int(waiting_till) % 60 << ":" << int(waiting_till * 60) % 60 << ")"
                 << "    ";
        }
        // If victim was assigned immediately, do not sum the wait. The moment of assignment is current_time
        else
        {
            tot_sec = min_dv_amb * 60 + start_time;
            cout << left << waiting_till << " (" << int(current_time) / 3600 << ":" << int(current_time / 60) % 60 << ":" << int(current_time) % 60 << ")"
                 << "    ";
        }
        cout << left << min_dv_amb * 60 + waiting_till << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        tot_sec += instance->getCasualtyStabilizationTime(first_id) * 60;
        cout << left << tot_sec / 60.0 << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        tot_sec += min_dh_amb * 60;
        cout << left << tot_sec / 60.0 << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        cout << endl;
        cout << endl;

        // Asignar el nuevo tiempo hasta cual la ambulancia estara ocupada, ocupar la cama, y aumentar rondas de ambulancia
        instance->updateVehicleOccupiedUntilTime(closest_amb, 0, tot_sec);
        instance->updateHospitalBedCapacity(closest_h_amb, g, instance->getHospitalCurCapacity(closest_h_amb, g) - 1);
        instance->addVehicleRound(closest_amb, 0);

        /*
        cout << left << "V" << first_id << "    ";
        cout << left << "H" << closest_heli << "     ";
        cout << left << "MCC" << closest_h_heli << "    ";
        tot_sec = min_dv_heli * 60 +  start_time;
        cout << left << min_dv_heli << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        tot_sec += instance->getCasualtyStabilizationTime(first_id) * 60;
        cout << left << tot_sec / 60.0 << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        tot_sec += min_dh_heli * 60;
        cout << left << tot_sec / 60.0 << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        cout << endl;
        cout << "================================" << endl;
        cout << endl;

        // Asignar el tiempo hasta cual el helicoptero estara ocupado y ocupar la cama
        instance->updateVehicleOccupiedUntilTime(closest_heli, 1, tot_sec);
        instance->updateHospitalBedCapacity(closest_h_heli, g, instance->getHospitalCurCapacity(closest_h_heli, g) - 1);
        instance->addVehicleRound(closest_heli, 1);
        */
    }
}