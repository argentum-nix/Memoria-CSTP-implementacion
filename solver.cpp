#include "solver.h"
#include <math.h>
#include <iomanip>
using namespace std;

Solver::Solver(Instance *in)
{
    instance = in;

    float lambda;
    int prev_hospital, g, prev_vehicle, veh_type;

    // for every period...
    for (int p = 1; p <= instance->qty_periods; p++)
    {
        // clean the priority list, because every period should start with new list
        priority_list.clear();
        // Asign time of start of operations, and save current period timestamp
        if (p == 1)
        {
            // not actually used for calculations
            start_time = instance->getPeriod(p);
        }
        // this one is the one used in all calculations
        current_time = instance->getPeriod(p);
        cout << "CURRENT PERIOD: " << p << " TIMESTAMP: " << current_time << endl;
        cout << "STATE OF THE HOSPITALS BEFORE RESET:" << endl;
        for (int h = 1; h <= instance->qty_hospitals; h++)
        {
            cout << "MCC" << h;
            if (instance->getHospitalAppearTime(h) <= current_time)
            {
                cout << " (AVAILABLE)";
            }
            else
            {
                cout << " (UNAVAILABLE)";
            }
            cout << " [" << instance->getHospitalCurCapacity(h, 1) << ", " << instance->getHospitalCurCapacity(h, 2) << ", " << instance->getHospitalCurCapacity(h, 3) << "]" << endl;
        }
        // create new prioririty list with current and prev periods
        for (int i = 1; i <= instance->qty_casualties; i++)
        {
            // if new victim, add directly
            if (instance->getCasualtyAppearTime(i) == current_time)
            {
                cout << "New casualty " << i << " added to the system." << endl;
                lambda = calculatePriority(instance->getCasualtyWaitingTime(i) - instance->getCasualtyAppearTime(i), instance->getCasualtyGravity(i));
                instance->updateCasualtyPriority(i, lambda);
                priority_list.push_back(make_pair(lambda, i));
            }
            // if old victim from prev period that had to wait for pickup
            else if (instance->getCasualtyAppearTime(i) < current_time && instance->getCasualtyWaitingTime(i) != 0)
            {
                // if the wait is so that the victim is still waiting at start of the new period
                // aqui se asume que no habra ninguna victima con espera no nula en primer periodo i=0
                if (instance->getCasualtyWaitingTime(i) >= current_time)
                {
                    cout << "Old casualty " << i << " is ready for new assignment.";
                    // free the occupied bed
                    g = instance->getCasualtyGravity(i);
                    prev_hospital = instance->getCasualtyAssignedHospital(i);
                    instance->updateHospitalBedCapacity(prev_hospital, g, instance->getHospitalCurCapacity(prev_hospital, g) + 1);
                    // de-assign the hospital
                    instance->updateCasualtyHospital(i, -1);
                    cout << " Freed bed at MCC" << prev_hospital << " for G=" << g;
                    // reassign waiting time to the start of new period
                    instance->updateCasualtyWaitingTime(i, current_time);
                    // recalculate the victims gravity using its new wait time (total of seconds of wait)
                    updateCasualtyState(i, instance->getCasualtyWaitingTime(i) - instance->getCasualtyAppearTime(i));
                    // update lambda with new data
                    lambda = calculatePriority(instance->getCasualtyWaitingTime(i) - instance->getCasualtyAppearTime(i), g);
                    instance->updateCasualtyPriority(i, lambda);
                    priority_list.push_back(make_pair(lambda, i));
                    // free the occupied vehicle and make its available time equal to prev assignment
                    prev_vehicle = instance->getCasualtyAssignedVehicle(i);
                    veh_type = instance->getCasualtyAssignedVehicleType(i);
                    // de-assign the vehicle
                    instance->resetVehicleOccuipedUntil(prev_vehicle, veh_type, current_time);
                    if (veh_type == 0)
                    {
                        cout << ". Freed A" << prev_vehicle << endl;
                    }
                    else if (veh_type == 1)
                    {
                        cout << ". Freed H" << prev_vehicle << endl;
                    }
                }
            }
        }
        cout << "STATE OF THE HOSPITALS AFTER RESET:" << endl;
        for (int h = 1; h <= instance->qty_hospitals; h++)
        {
            cout << "MCC" << h;
            if (instance->getHospitalAppearTime(h) <= current_time)
            {
                cout << " (AVAILABLE)";
            }
            else
            {
                cout << " (UNAVAILABLE)";
            }
            cout << " [" << instance->getHospitalCurCapacity(h, 1) << ", " << instance->getHospitalCurCapacity(h, 2) << ", " << instance->getHospitalCurCapacity(h, 3) << "]" << endl;
        }
        // order the list from greater to smaller
        sort(priority_list.rbegin(), priority_list.rend(), [](const auto &l, const auto &r)
             { return (l.first == r.first) ? l.second > r.second : l.first < r.first; });
        // sort(priority_list.rbegin(), priority_list.rend());
        printPriorityList();
        cout << endl;
        // greedy for current slice of victims
        cout << "=GREEDY=" << endl;
        greedyAssignment('H');
    }
}

void Solver::printPriorityList()
{
    cout << "PRIORITY LIST: ";
    for (unsigned int i = 0; i < priority_list.size(); i++)
    {
        cout << "(" << priority_list[i].first << ", " << priority_list[i].second << "), ";
    }
    cout << endl;
}

// Solver class destructor
Solver::~Solver() {}

void Solver::updateCasualtyState(int casualty_id, float te)
{
    int pi1_2 = instance->getDeteriorationTimeValue(1);
    int pi2_3 = instance->getDeteriorationTimeValue(2);
    // Case 1: Victim of LSI 1 already waited enough to be considered LSI 2
    int g = instance->getCasualtyGravity(casualty_id);
    cout << ". Waiting time in minutes is " << te / 60;
    if (g == 1 && te / 60 > pi1_2)
    {
        cout << "V" << casualty_id << " deteriorated to GRAVITY 2" << endl;
        instance->updateCasualtyGravity(casualty_id, 2);
        // instance->updateCasualtyAppearTime(i, instance->getCasualtyAppearTime(i) + instance->getDeteriorationTimeValue(g));
    }
    // Case 1:Victim of LSI 2 already waited enough to be considered LSI 3
    else if (g == 2 && te / 60 > pi2_3)
    {
        cout << "V" << casualty_id << " deteriorated to GRAVITY 3." << endl;
        instance->updateCasualtyGravity(casualty_id, 3);
        // instance->updateCasualtyWaitingTime(i, instance->getCasualtyWaitingTime(i) - TEmax2);
        // instance->updateCasualtyAppearTime(i, instance->getCasualtyAppearTime(i) + instance->getDeteriorationTimeValue(g));
    }
}

float Solver::calculatePriority(float te, int g)
{
    // aqui te que se pasa es cantidad de segundos de espera (no el timestamp)
    // en formula el calculo es en HORAS
    float kappa = instance->getDetFunctParam(0, g);
    float w = instance->getDetFunctParam(1, g);
    float pg = instance->getDetFunctParam(3, g);
    float phi = instance->getDetFunctParam(2, g);
    return pg + kappa * exp(w + phi * te / 3600);
}

pair<int, float> Solver::findClosestHospitalWithBeds(int casualty_id, int id_closest_veh, int veh_type)
{
    int h_capacity = -1;
    int closest_h = -1;
    float actual_time = 0;
    float min_dh = 999999;
    for (int h = 1; h <= instance->qty_hospitals; h++)
    {
        // Search for hospitals that area AVAILABLE in this moment (their appearance time is on or before the current period)
        if (instance->getHospitalAppearTime(h) <= current_time)
        {
            h_capacity = instance->getHospitalCurCapacity(h, instance->getCasualtyGravity(casualty_id));
            // if this hospital can attend such gravity then
            if (h_capacity > 0)
            {
                cout << "[Victim->Hospital ";
                if (veh_type == TYPE_AMBULANCE)
                {
                    cout << "in ambulance] ";
                }
                else if (veh_type == TYPE_HELICOPTER)
                {
                    cout << "in helicopter] ";
                }
                cout << instance->getCasualtyLocation(casualty_id) << " -> " << instance->getHospitalLocation(h) << " = ";
                cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(casualty_id), instance->getHospitalLocation(h), veh_type);
                actual_time = instance->getTimeBetweenNodes(instance->getCasualtyLocation(casualty_id), instance->getHospitalLocation(h), veh_type);
                if (veh_type == TYPE_HELICOPTER)
                {
                    actual_time += instance->getVehicleLandingTime(id_closest_veh, veh_type) + instance->getVehicleTakeoffTime(id_closest_veh, veh_type);
                }
                cout << " and with takeoff/landing: " << actual_time << endl;

                if (actual_time < min_dh)
                {
                    min_dh = actual_time;
                    closest_h = h;
                }
            }
        }
    }
    return make_pair(closest_h, min_dh);
}

pair<int, float> Solver::findFirstAvailableVehicle(int veh_type)
{
    float min_availability = 999999;
    int next_available_v = -1;
    int fleet_qty = 0;
    if (veh_type == TYPE_AMBULANCE)
    {
        fleet_qty = instance->qty_ambulances;
    }
    else if (veh_type == TYPE_HELICOPTER)
    {
        fleet_qty = instance->qty_helicopters;
    }
    for (int v = 1; v <= fleet_qty; v++)
    {
        // searching solo among ambulances available for operations currently
        if (instance->getVehicleAppearTime(v, veh_type) <= current_time)
        {
            if (veh_type == TYPE_AMBULANCE)
            {
                cout << "[Ambulance A";
            }
            else if (veh_type == TYPE_HELICOPTER)
            {
                cout << "[Helicopter H";
            }
            cout << v << "] occupied until " << instance->getVehicleOccupiedUntilTime(v, veh_type) / 60 << endl;
            if (instance->getVehicleOccupiedUntilTime(v, veh_type) < min_availability)
            {
                min_availability = instance->getVehicleOccupiedUntilTime(v, veh_type);
                next_available_v = v;
            }
        }
    }
    return make_pair(next_available_v, min_availability);
}

pair<int, float> Solver::findClosestAvailableVehicle(int casualty_id, int veh_type)
{
    int closest_v = -1;
    float actual_time = 0;
    float min_dv = 999999;
    int available_vehicles = 0;
    int fleet_qty = 0;
    if (veh_type == TYPE_AMBULANCE)
    {
        fleet_qty = instance->qty_ambulances;
    }
    else if (veh_type == TYPE_HELICOPTER)
    {
        fleet_qty = instance->qty_helicopters;
    }
    for (int v = 1; v <= fleet_qty; v++)
    {
        // check if vehicle is not occupied currently and is available in this period
        if (instance->getVehicleOccupiedUntilTime(v, veh_type) <= current_time && instance->getVehicleAppearTime(v, veh_type) <= current_time)
        {
            available_vehicles++;
            cout << "[Pickup->Victim for";
            if (veh_type == TYPE_AMBULANCE)
            {
                cout << " A";
            }
            else if (veh_type == TYPE_HELICOPTER)
            {
                cout << " H";
            }
            cout << v << "] " << instance->getVehicleLocation(v, veh_type) << " -> " << instance->getCasualtyLocation(casualty_id) << " = ";
            cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(casualty_id), instance->getVehicleLocation(v, 0), 0) << endl;
            actual_time = instance->getTimeBetweenNodes(instance->getCasualtyLocation(casualty_id), instance->getVehicleLocation(v, veh_type), veh_type);
            // If helicopter, add landing, takeoff and 2x running to and from the casualty location on foot
            if (veh_type == TYPE_HELICOPTER)
            {
                actual_time += instance->getVehicleLandingTime(v, veh_type) + instance->getVehicleTakeoffTime(v, veh_type);
                actual_time += 2 * instance->getCasualtyTimeToHeliport(casualty_id);
            }
            if (actual_time < min_dv)
            {
                min_dv = instance->getTimeBetweenNodes(instance->getCasualtyLocation(casualty_id), instance->getVehicleLocation(v, 0), 0);
                closest_v = v;
            }
        }
    }
    return make_pair(closest_v, min_dv);
}

void Solver::greedyAssignment(char fleet_mode)
{
    // TODO: Revisar los vehiculos y si ya se desocupan
    pair<int, float> res;
    int first_id;
    int closest_h_amb = -1;
    int closest_h_heli = -1;
    float min_dh_amb, min_dh_heli, min_dv_amb, min_dv_heli;
    int available_vehicles = 0;
    float waiting_till, appear_time;
    int closest_amb = -1;
    int closest_heli = -1;
    float min_availability_amb = 999999;
    int next_available_amb = -1;
    float tot_sec = 0;

    int veh_type = 0;
    if (fleet_mode == 'A')
    {
        veh_type = TYPE_AMBULANCE;
    }
    else if (fleet_mode == 'H')
    {
        veh_type = TYPE_HELICOPTER;
    }

    for (int i = 0; i < int(priority_list.size()); i++)
    {
        // pick the first victim on top of the list
        first_id = priority_list[i].second;
        // Find the closest hospital that has de capacity to attend the casualty
        min_dh_amb = 999999;
        min_dh_heli = 999999;
        closest_h_amb = -1;
        closest_h_heli = -1;
        min_dv_amb = 999999;
        closest_amb = -1;
        available_vehicles = -1;
        min_availability_amb = 999999;
        next_available_amb = -1;

        // Find an available vehicle, that takes the smallest time to get to the victim
        // Case: ambulance
        cout << "Searching for closest vehicle to pick up the casualty..." << endl;
        res = findClosestAvailableVehicle(first_id, veh_type);
        closest_amb = res.first;
        min_dv_amb = res.second;

        if (res.first == -1)
        {
            available_vehicles = 0;
        }
        if (available_vehicles == 0)
        {
            // find a vehicle that will be freed first between all the occupied ones (by default, available for operations currently)
            // and assign the wait time alpha_k
            cout << "All vehicles occcupied." << endl;
            res = findFirstAvailableVehicle(veh_type);
            next_available_amb = res.first;
            min_availability_amb = res.second;

            cout << "->>>>Next available vehicle is A" << next_available_amb << " in " << (min_availability_amb) / 60;
            cout << " (" << int(min_availability_amb) / 3600 << ":" << (int(min_availability_amb) / 60) % 60 << ":" << int(min_availability_amb) % 60 << ")" << endl;
            closest_amb = next_available_amb;
        }

        cout << "Searching for closest hospital with beds..." << endl;
        res = findClosestHospitalWithBeds(first_id, closest_amb, veh_type);
        closest_h_amb = res.first;
        min_dh_amb = res.second;

        if (available_vehicles == 0)
        {
            if (closest_h_amb != -1)
            {
                min_dh_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getHospitalLocation(closest_h_amb), veh_type);
                // Ocupar la cama de hospital asignado
                instance->updateHospitalBedCapacity(closest_h_amb, instance->getCasualtyGravity(first_id), instance->getHospitalCurCapacity(closest_h_amb, instance->getCasualtyGravity(first_id)) - 1);
                min_dv_amb = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(closest_amb, veh_type), veh_type);
                // se asigna hasta que momento en segundos desde el inicio la victima espera su vehiculo
                instance->updateCasualtyWaitingTime(first_id, min_availability_amb);
            }
            else
            {
                cout << "No hospitals can attend this victim. Victim: V" << first_id << " G=" << instance->getCasualtyGravity(first_id) << endl;
                // TODO: Use special hospitals // dummies
                // Skip this iteration, victim recieves no medical attention, F in the chat
                continue;
            }
        }

        // Si es la primera asigancion de veh, se debe agregar el tiempo de preparacion
        if (instance->getVehicleOccupiedUntilTime(closest_amb, veh_type) == instance->getVehicleAppearTime(closest_amb, veh_type))
        {
            min_dh_amb += instance->getVehiclePrepTime(closest_amb, veh_type);
        }

        // Puros prints de tabla de asignacion ambulancia o helicoptero
        cout << left << "V" << first_id << "    ";
        cout << left << instance->getCasualtyGravity(first_id) << "    ";
        cout << left << instance->getCasualtyAge(first_id) << "    ";
        cout << left << priority_list[i].first << "    ";
        cout << left << instance->getCasualtyStabilizationTime(first_id) << "    ";
        if (veh_type == TYPE_AMBULANCE)
        {
            cout << left << "A" << closest_amb << "    ";
        }
        else if (veh_type == TYPE_HELICOPTER)
        {
            cout << left << "H" << closest_amb << "    ";
        }
        cout << left << "ROUND " << instance->getVehicleRound(closest_amb, veh_type) << "    ";
        cout << left << "MCC" << closest_h_amb << "    ";
        appear_time = instance->getCasualtyAppearTime(first_id) / 60;
        cout << left << appear_time << " (" << int(appear_time * 60) / 3600 << ":" << int(appear_time) % 60 << ":" << int(appear_time * 60) % 60 << ")"
             << "    ";
        waiting_till = instance->getCasualtyWaitingTime(first_id) / 60;
        // If there is some kind of wait, it should be added. The moment of assignment is starting time + all the wait
        if (waiting_till * 60 - appear_time * 60 != 0)
        {
            cout << left << waiting_till << " (" << int(waiting_till * 60) / 3600 << ":" << int(waiting_till) % 60 << ":" << int(waiting_till * 60) % 60 << ")"
                 << "    ";
            // If there is any wait, then the vehicle arrival is to be shifted by waiting (not timestamp, but actual duration, hence the -appear_time)
            tot_sec = min_dv_amb * 60 + current_time + (waiting_till * 60 - appear_time * 60);
        }
        // If victim was assigned immediately, do not sum the wait. The moment of assignment is current_time
        else
        {
            cout << left << waiting_till << " (" << int(current_time) / 3600 << ":" << int(current_time / 60) % 60 << ":" << int(current_time) % 60 << ")"
                 << "    ";
            // And the time of arrival of the vehicle in seconds is current_time + the arrival (in min) * 60
            // The difference is that i do not sum the wait time to this value because the assignment was made at the same time as the victim appeared
            tot_sec = min_dv_amb * 60 + current_time;
        }

        cout << left << tot_sec / 60.0 << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        tot_sec += instance->getCasualtyStabilizationTime(first_id) * 60;
        cout << left << tot_sec / 60.0 << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        tot_sec += min_dh_amb * 60;
        cout << left << tot_sec / 60.0 << " (" << int(tot_sec) / 3600 << ":" << (int(tot_sec) / 60) % 60 << ":" << int(tot_sec) % 60 << ")"
             << "    ";
        cout << endl;
        // Asignar el nuevo tiempo hasta cual la ambulancia estara ocupada y aumentar rondas de ambulancia
        instance->updateVehicleOccupiedUntilTime(closest_amb, veh_type, tot_sec);
        instance->addVehicleRound(closest_amb, veh_type);
        // Asignar el hospital y vehiculo a la casualty
        instance->updateCasualtyHospital(first_id, closest_h_amb);

        instance->updateCasualtyAssignedVehicle(first_id, closest_amb, veh_type);
        cout << endl;
    }
}