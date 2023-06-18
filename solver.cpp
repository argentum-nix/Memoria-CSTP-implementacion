#include "solver.h"
#include <math.h>
#include <iomanip>
using namespace std;

int Solver::checkIfHighST(int casualty_id)
{
    int g = instance->getCasualtyGravity(casualty_id);
    int a = instance->getCasualtyAge(casualty_id);
    if (g == 3 || (g == 2 && a == 1))
    {
        if (instance->getCasualtyAssignedVehicleType(casualty_id) == TYPE_HELICOPTER)
        {
            return 1;
        }
    }
    return 0;
}

Solver::Solver(Instance *in)
{
    instance = in;

    float lambda;
    int prev_hospital, g, prev_vehicle, veh_type;

    // for every period...
    // change to instance->qty_periods
    for (int p = 1; p <= 1; p++)
    {
        // clean the priority list, because every period should start with new list
        priority_list.clear();
        // Asign time of start of operations, and save current period timestamp
        if (p == 1)
        {
            start_time = instance->getPeriod(p);
        }
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
            // if old victim from prev period
            else if (instance->getCasualtyAppearTime(i) < current_time)
            {
                // if the wait is so that the victim is still waiting at start of the new period
                if (instance->getCasualtyWaitingTime(i) >= current_time)
                {
                    cout << "Old casualty " << i << " is ready for new assignment.";
                    // free the occupied bed
                    g = instance->getCasualtyGravity(i);
                    prev_hospital = instance->getCasualtyAssignedHospital(i);
                    if (prev_hospital != -1)
                    {
                        instance->updateHospitalBedCapacity(prev_hospital, g, instance->getHospitalCurCapacity(prev_hospital, g) + 1);
                        // de-assign the hospital
                        instance->updateCasualtyHospital(i, -1);
                        cout << " Freed bed at MCC" << prev_hospital << " for G=" << g;
                    }
                    else
                    {
                        cout << "This victim could not be assigned a hospital on prev iteration";
                    }
                    // reassign waiting time to the start of new period
                    instance->updateCasualtyWaitingTime(i, current_time);
                    // reset the casualty gravity (if it was changed on current time or later in previous assignment)
                    instance->resetCasualtyGravity(i, current_time);
                    // recalculate the victims gravity using its new wait time (total of seconds of wait)
                    updateCasualtyState(i, instance->getCasualtyWaitingTime(i) - instance->getCasualtyAppearTime(i), current_time);
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
                        cout << ".Freed A" << prev_vehicle << endl;
                    }
                    else if (veh_type == 1)
                    {
                        cout << ".Freed H" << prev_vehicle << endl;
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
        greedyAssignment('M', 0);

        // METAHEURISTIC PROCEDURE
        int cursor, amb_loc, cas_loc, choosen_amb, closest_h;
        int prev_v, prev_v_type, prev_h, prev_g;
        float closeness, availability, amb_priority;
        float min_availability_veh, min_dv_veh, min_dh_veh, h_admit_timestamp;
        float appear_time, waiting_till, veh_arrival_time, cas_st_timestamp;
        float min_amb_priority = 9999999;
        vector<pair<float, int>> ambulances;
        pair<int, float> res;
        cout << "Terminated greedy routes. Entering in Metaheuristic" << endl;
        for (int k = 0; k < int(priority_list.size()); k++)
        {
            // FIRST: CLEAN ALL THE FLAGS FOR RESETTING SOLUTIONS
            for (int i = 1; i < instance->qty_casualties; i++)
            {
                instance->clearCasualtyResetFlag(i);
            }
            for (int i = 1; i < instance->qty_ambulances; i++)
            {
                instance->clearVehicleResetFlag(i, TYPE_AMBULANCE);
            }
            for (int i = 1; i < instance->qty_helicopters; i++)
            {
                instance->clearVehicleResetFlag(i, TYPE_HELICOPTER);
            }

            cursor = priority_list[k].second;
            cout << "Trying for victim " << cursor << endl;
            if (checkIfHighST(cursor))
            {
                float prevsolq = calculateSolutionQuality();
                cout << "Eligible for reasignment. Current solution quality: " << prevsolq << endl;
                // liberate all resources - vehicles and hospitals
                for (int u = k; u < int(priority_list.size()); u++)
                {
                    prev_g = instance->getCasualtyGravity(priority_list[u].second);
                    prev_v = instance->getCasualtyAssignedVehicle(priority_list[u].second);
                    prev_v_type = instance->getCasualtyAssignedVehicleType(priority_list[u].second);
                    prev_h = instance->getCasualtyAssignedHospital(priority_list[u].second);
                    instance->temporaryDeassignHospital(prev_h, prev_g);
                    instance->snapshotVehicleLastAssignment(prev_v, prev_v_type);
                }
                // TODO: make this a function that returns an ordered vector of pairs of ambs and their priority
                // So that the algorithm can try with several ambulances, not only the head one
                cas_loc = instance->getCasualtyLocation(cursor);
                for (int z = 1; z <= instance->qty_ambulances; z++)
                {
                    amb_loc = instance->getVehicleLocation(z, 0);
                    closeness = instance->getTimeBetweenNodes(amb_loc, cas_loc, 0);
                    // add prepare time to vehicle if it is its first asignment
                    if (instance->getVehicleOccupiedUntilTime(z, TYPE_AMBULANCE) == instance->getVehicleAppearTime(z, TYPE_AMBULANCE))
                    {
                        closeness += instance->getVehiclePrepTime(z, TYPE_AMBULANCE);
                    }
                    availability = instance->getVehicleOccupiedUntilTime(z, 0);
                    amb_priority = closeness * 0.2 + availability * 0.8;
                    // search for min value ambulance that is available in this period
                    if (amb_priority < min_amb_priority && instance->getVehicleAppearTime(z, TYPE_AMBULANCE) <= current_time)
                    {
                        min_amb_priority = amb_priority;
                        min_availability_veh = availability;
                        min_dv_veh = closeness;
                        choosen_amb = z;
                    }
                }
                cout << "Choosing ambulance A" << choosen_amb << " with min_av= " << min_availability_veh << " cl= " << min_dv_veh << " of priority " << min_amb_priority << endl;
                // asign to current casualty the ambulance we chose, just how Greedy does it
                cout << "Assigning the new route to the eligible casualty V" << cursor << ". Before:" << endl;
                printCasualtyRouteRow(cursor);
                appear_time = instance->getCasualtyAppearTime(cursor);
                waiting_till = min_availability_veh;
                // Assign the vehicle arrival time - it will either be based on period start if the victim is assigned immediately
                // Or it will be based on shifted value of appearance+wait, which was saved in waiting_till at some point
                veh_arrival_time = min_dv_veh * 60 + waiting_till;
                // If casualty deteoriates when vehicle arrives, change its state to calculate correct stabilization time and assign hospital
                updateCasualtyState(cursor, veh_arrival_time - appear_time, veh_arrival_time);
                // Get the timestamp at which the casualty is stabilized (based on real gravity at arrival of medical group)
                cas_st_timestamp = veh_arrival_time + instance->getCasualtyStabilizationTime(cursor) * 60;
                res = findClosestHospitalWithBeds(cursor, choosen_amb, TYPE_AMBULANCE);
                closest_h = res.first;
                min_dh_veh = res.second;
                // STEP THREE: CALCULATE Hospital-related timestamp
                h_admit_timestamp = cas_st_timestamp + min_dh_veh * 60;
                // STEP FOUR: SAVE THE ASSIGNMENT
                // Asignar el nuevo tiempo hasta cual la ambulancia estara ocupada y aumentar rondas de ambulancia
                instance->updateVehicleLocation(choosen_amb, TYPE_AMBULANCE, closest_h);
                instance->updateVehicleOccupiedUntilTime(choosen_amb, TYPE_AMBULANCE, h_admit_timestamp);
                instance->addVehicleRound(choosen_amb, TYPE_AMBULANCE);
                // Asignar el hospital y vehiculo a la casualty
                instance->updateCasualtyHospital(cursor, closest_h);
                // Ocupar la cama de hospital asignado
                instance->updateHospitalBedCapacity(closest_h, instance->getCasualtyGravity(cursor), instance->getHospitalCurCapacity(closest_h, instance->getCasualtyGravity(cursor)) - 1);
                // Guardar la asignacion de vehiculo
                instance->updateCasualtyAssignedVehicle(cursor, choosen_amb, TYPE_AMBULANCE);
                // Guardar los tiempos
                instance->updateCasualtyRouteTimes(cursor, waiting_till, veh_arrival_time, cas_st_timestamp, h_admit_timestamp);

                cout << "After: " << endl;
                printCasualtyRouteRow(cursor);
                cout << endl;

                cout << "STARTING SLICED GREEDY FROM VICTIM V" << cursor << ", PRIORITY LIST POSITION =" << k + 1 << endl;
                // run greedy with a slice of victims
                greedyAssignment('M', k + 1);
                // compare the solutions
                float cursolq = calculateSolutionQuality();
                if (cursolq > prevsolq)
                {
                    cout << "Current solution is WORSE than prev sol: " << cursolq << " vs " << prevsolq << endl;
                    for (int c = k; c < int(priority_list.size()); c++)
                    {
                        cout << "Resetting victim V" << priority_list[c].second << " on index " << c << " in priority list." << endl;
                        prev_g = instance->getCasualtyGravity(priority_list[c].second);
                        prev_v = instance->getCasualtyAssignedVehicle(priority_list[c].second);
                        prev_v_type = instance->getCasualtyAssignedVehicleType(priority_list[c].second);
                        prev_h = instance->getCasualtyAssignedHospital(priority_list[c].second);

                        cout << "BEFORE: " << endl;
                        printCasualtyRouteRow(priority_list[c].second);

                        instance->resetTemporaryDeassignHospital(prev_h, prev_g);
                        // should be reassigned only once
                        instance->resetVehicleLastAssignment(prev_v, prev_v_type);
                        instance->resetCasualtyLastAssignment(priority_list[c].second);
                    }
                    cout << "AFTER: " << endl;
                    for (int c = k; c < int(priority_list.size()); c++)
                    {
                        printCasualtyRouteRow(priority_list[c].second);
                    }
                }
                else
                {
                    cout << "Current solution is BETTER than prev sol: " << cursolq << " vs " << prevsolq << endl;
                    for (int c = k; c < int(priority_list.size()); c++)
                    {
                        // save the new solution as current solution
                        instance->saveCasualtyLastAssignment(priority_list[c].second);
                        prev_v = instance->getCasualtyAssignedVehicle(priority_list[c].second);
                        prev_v_type = instance->getCasualtyAssignedVehicleType(priority_list[c].second);
                        instance->saveVehicleLastAssignment(prev_v, prev_v_type);
                    }
                }
            }
        }
    }
    // para ver victimas que quedaron sin hospital al final de todos los periodos
    int count_unatt = 0;
    cout << "Victims without assignment:" << endl;
    for (int i = 1; i <= instance->qty_casualties; i++)
    {
        if (instance->getCasualtyAssignedHospital(i) == -1)
        {
            cout << "UNATTENDED VICTIM V" << i << endl;
            count_unatt++;
        }
    }
    cout << "TOTAL: " << count_unatt << endl;
}

void Solver::printCasualtyRouteRow(int casualty_id)
{
    cout << left << "V" << casualty_id << "    ";
    cout << left << instance->getCasualtyGravity(casualty_id) << "    ";
    cout << left << instance->getCasualtyAge(casualty_id) << "    ";
    cout << left << instance->getCasualtyPriority(casualty_id) << "    ";
    cout << left << instance->getCasualtyStabilizationTime(casualty_id) << "    ";
    if (instance->getCasualtyAssignedVehicleType(casualty_id) == TYPE_HELICOPTER)
    {
        cout << left << "H" << instance->getCasualtyAssignedVehicle(casualty_id) << "    ";
    }
    else if (instance->getCasualtyAssignedVehicleType(casualty_id) == TYPE_AMBULANCE)
    {
        cout << left << "A" << instance->getCasualtyAssignedVehicle(casualty_id) << "    ";
    }
    else
    {
        cout << left << "X" << instance->getCasualtyAssignedVehicle(casualty_id) << "    ";
    }
    cout << left << "ROUND " << instance->getVehicleRound(instance->getCasualtyAssignedVehicle(casualty_id), instance->getCasualtyAssignedVehicleType(casualty_id)) << "    ";
    cout << left << "MCC" << instance->getCasualtyAssignedHospital(casualty_id) << "    ";
    printTimestamp(instance->getCasualtyAppearTime(casualty_id));
    printTimestamp(instance->getCasualtyWaitingTime(casualty_id));
    printTimestamp(instance->getCasualtyVehArrivedTime(casualty_id));
    printTimestamp(instance->getCasualtyStabilizedTime(casualty_id));
    printTimestamp(instance->getCasualtyAdmittedAtHopsitalTime(casualty_id));
    cout << endl;
}

float Solver::calculateSolutionQuality()
{
    float quality = 0;
    float p;
    int c;
    for (unsigned int i = 0; i < priority_list.size(); i++)
    {
        p = priority_list[i].first;
        c = priority_list[i].second;
        quality += p * (instance->getCasualtyAdmittedAtHopsitalTime(c) / 60.0 - instance->getCasualtyAppearTime(c) / 60.0);
    }
    return quality;
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

void Solver::printTimestamp(float t)
{
    cout << left << t / 60.0 << " (" << int(t) / 3600 << ":" << (int(t) / 60) % 60 << ":" << int(t) % 60 << ")"
         << "    ";
}

void Solver::updateCasualtyState(int casualty_id, float te, float change_timestamp)
{
    int pi1_2 = instance->getDeteriorationTimeValue(1);
    int pi2_3 = instance->getDeteriorationTimeValue(2);
    // Case 1: Victim of LSI 1 already waited enough to be considered LSI 2
    int g = instance->getCasualtyGravity(casualty_id);
    cout << "Waiting time in minutes is " << te / 60 << ".";
    if (g == 1 && te / 60 > pi1_2)
    {
        cout << "V" << casualty_id << " deteriorated to GRAVITY 2 AT TIMESTAMP ";
        printTimestamp(change_timestamp);
        cout << endl;
        if (change_timestamp > current_time)
        {
            cout << "IT WAS AN IN-ROUTE GRAVITY CHANGE!" << endl;
            instance->updateCasualtyGravity(casualty_id, 2, change_timestamp, 1);
        }
        else
        {
            instance->updateCasualtyGravity(casualty_id, 2, change_timestamp, 0);
        }
    }
    // Case 1:Victim of LSI 2 already waited enough to be considered LSI 3
    else if (g == 2 && te / 60 > pi2_3)
    {
        cout << "V" << casualty_id << " deteriorated to GRAVITY 3 AT TIMESTAMP ";
        printTimestamp(change_timestamp);
        cout << endl;
        if (change_timestamp > current_time)
        {
            cout << "IT WAS AN IN-ROUTE GRAVITY CHANGE!" << endl;
            instance->updateCasualtyGravity(casualty_id, 3, change_timestamp, 1);
        }
        else
        {
            instance->updateCasualtyGravity(casualty_id, 3, change_timestamp, 0);
        }
    }
    else
    {
        cout << "OK NO CHANGE NEEDED";
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
            cout << instance->getTimeBetweenNodes(instance->getCasualtyLocation(casualty_id), instance->getVehicleLocation(v, veh_type), veh_type) << endl;
            actual_time = instance->getTimeBetweenNodes(instance->getCasualtyLocation(casualty_id), instance->getVehicleLocation(v, veh_type), veh_type);
            // If helicopter, add landing, takeoff and running to the casualty location on foot
            if (veh_type == TYPE_HELICOPTER)
            {
                actual_time += instance->getVehicleLandingTime(v, veh_type) + instance->getVehicleTakeoffTime(v, veh_type);
                actual_time += instance->getCasualtyTimeToHeliport(casualty_id);
            }
            // if its first operating of the vehicle, add the prep time too
            if (instance->getVehicleOccupiedUntilTime(v, veh_type) == instance->getVehicleAppearTime(v, veh_type))
            {
                actual_time += instance->getVehiclePrepTime(v, veh_type);
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

void Solver::greedyAssignment(char fleet_mode, int cursor)
{
    // TODO: Revisar los vehiculos y si ya se desocupan
    pair<int, float> res;
    int first_id, closest_h, closest_veh, available_vehicles, veh_type;
    float min_dh_veh, min_dv_veh, waiting_till, appear_time;
    float min_availability_amb, min_availability_heli, min_availability_veh;
    int next_available_amb, next_available_heli, next_available_veh;

    float cas_st_timestamp, veh_arrival_time, h_admit_timestamp;

    for (int i = cursor; i < int(priority_list.size()); i++)
    {
        cout << endl;
        // Pick the first victim on top of the list
        first_id = priority_list[i].second;
        // Reset the values
        closest_h = -1;
        min_dv_veh = 999999;
        min_dh_veh = 999999;
        closest_veh = -1;
        available_vehicles = -1;
        min_availability_amb = 999999;
        min_availability_heli = 999999;
        min_availability_veh = 999999;
        next_available_amb = -1;
        next_available_heli = -1;

        // 1. Ambulance mode. Only use ambulances for all trips.
        if (fleet_mode == 'A')
        {
            veh_type = TYPE_AMBULANCE;
        }
        // 2. Helicopter mode. Only use helicopters for all trips.
        else if (fleet_mode == 'H')
        {
            veh_type = TYPE_HELICOPTER;
        }
        // 3. Preferential mode. Use helicopters only when casualty is of gravity 3. Use ambulance eoc.
        else if (fleet_mode == 'P')
        {
            if (instance->getCasualtyGravity(first_id) == 3)
            {
                cout << "[NOTICE: Preferential mode ON.]" << endl;
                veh_type = TYPE_HELICOPTER;
            }
            else
            {
                veh_type = TYPE_AMBULANCE;
            }
        }
        // 4. Mixed mode. Use helicopters when available, if not, use ambulances. If none available, choose whataver vehicle that is freed first
        else if (fleet_mode == 'M')
        {
            veh_type = TYPE_HELICOPTER;
        }

        // STEP ONE: Find a vehicle to pick up the casualty
        cout << "Searching for closest vehicle to pick up the casualty..." << endl;
        if (fleet_mode == 'M')
        {
            cout << "[NOTICE: Mixed mode ON]" << endl;
            res = findClosestAvailableVehicle(first_id, veh_type);
            // If no helicopters are currently available, try to find an ambulance
            if (res.first == -1)
            {
                veh_type = TYPE_AMBULANCE;
                res = findClosestAvailableVehicle(first_id, veh_type);
            }
        }
        // for H and A cases, use the corresponding type of vehicle directly
        else
        {
            res = findClosestAvailableVehicle(first_id, veh_type);
        }

        closest_veh = res.first;
        min_dv_veh = res.second;

        if (res.first == -1)
        {
            available_vehicles = 0;
        }

        // If no vehicles are available
        if (available_vehicles == 0)
        {
            // find a vehicle that will be freed first between all the occupied ones (by default, available for operations currently)
            cout << "All vehicles are occcupied." << endl;

            if (fleet_mode == 'M')
            {
                // Find next available helicopter
                res = findFirstAvailableVehicle(TYPE_HELICOPTER);
                next_available_heli = res.first;
                min_availability_heli = res.second;
                // Find next available ambulance
                res = findFirstAvailableVehicle(TYPE_AMBULANCE);
                next_available_amb = res.first;
                min_availability_amb = res.second;
                // Select the vehicle that is freed first, save its type.
                if (min_availability_amb < min_availability_heli)
                {
                    veh_type = TYPE_AMBULANCE;
                    next_available_veh = next_available_amb;
                    min_availability_veh = min_availability_amb;
                }
                else
                {
                    veh_type = TYPE_HELICOPTER;
                    next_available_veh = next_available_heli;
                    min_availability_veh = min_availability_heli;
                }
            }
            // Pick the first available vehicle of the corresponding mode type
            else
            {
                res = findFirstAvailableVehicle(veh_type);
                next_available_veh = res.first;
                min_availability_veh = res.second;
            }

            cout << "->>>>Next available vehicle is";
            if (veh_type == TYPE_HELICOPTER)
            {
                cout << " H";
            }
            else
            {
                cout << " A";
            }
            cout << next_available_veh << " in " << (min_availability_veh) / 60;
            cout << " (" << int(min_availability_veh) / 3600 << ":" << (int(min_availability_veh) / 60) % 60 << ":" << int(min_availability_veh) % 60 << ")" << endl;
            closest_veh = next_available_veh;
            min_dv_veh = instance->getTimeBetweenNodes(instance->getCasualtyLocation(first_id), instance->getVehicleLocation(closest_veh, veh_type), veh_type);
        }
        // STEP TWO: Calculate Vehicle-arrival related timestamps
        veh_arrival_time = 0;
        cas_st_timestamp = 0;
        appear_time = 0;
        waiting_till = 0;
        appear_time = instance->getCasualtyAppearTime(first_id);
        // if casualty had to wait, set its wait to min availability, else use the casualty wait value directly
        if (min_availability_veh == 999999)
        {
            waiting_till = instance->getCasualtyWaitingTime(first_id);
        }
        else
        {
            waiting_till = min_availability_veh;
        }
        // Assign the vehicle arrival time - it will either be based on period start if the victim is assigned immediately
        // Or it will be based on shifted value of appearance+wait, which was saved in waiting_till at some point
        veh_arrival_time = min_dv_veh * 60 + waiting_till;
        // If casualty deteoriates when vehicle arrives, change its state to calculate correct stabilization time and assign hospital
        updateCasualtyState(first_id, veh_arrival_time - appear_time, veh_arrival_time);
        // Get the timestamp at which the casualty is stabilized (based on real gravity at arrival of medical group)
        cas_st_timestamp = veh_arrival_time + instance->getCasualtyStabilizationTime(first_id) * 60;

        // STEP THREE: Find a hospital (using the actual gravity of the victim at arrival of the medical group, not the initial one)
        cout << endl;
        cout << "Searching for closest hospital with beds..." << endl;
        res = findClosestHospitalWithBeds(first_id, closest_veh, veh_type);
        closest_h = res.first;
        min_dh_veh = res.second;

        // No hospitals available
        if (closest_h == -1)
        {
            cout << "No hospitals can attend this victim. Victim: V" << first_id << " G=" << instance->getCasualtyGravity(first_id) << endl;
            // TODO: Use special hospitals // dummies // or count
            continue;
        }

        // STEP THREE: CALCULATE Hospital-related timestamp
        h_admit_timestamp = cas_st_timestamp + min_dh_veh * 60;
        // if heli, add the running back with victim to the heliport, takeoff, and landing at hospital time
        if (veh_type == TYPE_HELICOPTER)
        {
            h_admit_timestamp += instance->getCasualtyTimeToHeliport(first_id) * 60;
            h_admit_timestamp += instance->getVehicleLandingTime(closest_veh, veh_type) * 60;
            h_admit_timestamp += instance->getVehicleTakeoffTime(closest_veh, veh_type) * 60;
        }

        // STEP FOUR: SAVE THE ASSIGNMENT
        // Asignar el nuevo tiempo hasta cual la ambulancia estara ocupada y aumentar rondas de ambulancia
        instance->updateVehicleLocation(closest_veh, veh_type, closest_h);
        instance->updateVehicleOccupiedUntilTime(closest_veh, veh_type, h_admit_timestamp);
        instance->addVehicleRound(closest_veh, veh_type);
        // Asignar el hospital y vehiculo a la casualty
        instance->updateCasualtyHospital(first_id, closest_h);
        // Ocupar la cama de hospital asignado
        instance->updateHospitalBedCapacity(closest_h, instance->getCasualtyGravity(first_id), instance->getHospitalCurCapacity(closest_h, instance->getCasualtyGravity(first_id)) - 1);
        // Guardar la asignacion de vehiculo
        instance->updateCasualtyAssignedVehicle(first_id, closest_veh, veh_type);
        cout << endl;

        // STEP FIVE: PRINT THE FINAL ASSIGNMENT
        cout << left << "V" << first_id << "    ";
        cout << left << instance->getCasualtyGravity(first_id) << "    ";
        cout << left << instance->getCasualtyAge(first_id) << "    ";
        cout << left << instance->getCasualtyPriority(first_id) << "    ";
        cout << left << instance->getCasualtyStabilizationTime(first_id) << "    ";
        if (veh_type == TYPE_AMBULANCE)
        {
            cout << left << "A" << closest_veh << "    ";
        }
        else if (veh_type == TYPE_HELICOPTER)
        {
            cout << left << "H" << closest_veh << "    ";
        }

        cout << left << "ROUND " << instance->getVehicleRound(closest_veh, veh_type) << "    ";
        cout << left << "MCC" << closest_h << "    ";
        // time in which victim appears in the system
        printTimestamp(appear_time);
        printTimestamp(waiting_till);
        // timestamp at which a vehicle comes to rescue the victim
        printTimestamp(veh_arrival_time);
        // timestamp at which victim was stabilized
        printTimestamp(cas_st_timestamp);
        // timestamp at which victim is admitted to the hospital
        printTimestamp(h_admit_timestamp);
        cout << endl;
        cout << endl;
        instance->updateCasualtyRouteTimes(first_id, waiting_till, veh_arrival_time, cas_st_timestamp, h_admit_timestamp);
    }
}
