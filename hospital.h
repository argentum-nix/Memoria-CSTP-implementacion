#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <vector>
#include <iostream>

class Hospital
{
public:
    Hospital();
    ~Hospital();

    // setters
    void setHospitalID(int id);
    void setHospitalLocation(int node_id);
    void setHospitalMaxCapacity(int g, int k);
    void setHospitalCurCapacity(int g, int k);
    void setHospitalAppearTime(int t);

    // getters
    int getHospitalID();
    int getHospitalMaxCapacity(int g);
    int getHospitalCurCapacity(int g);
    int getHospitalLocation();
    int getHospitalAppearTime();

    void printData();

    // metaheuristic-specific methods
    void clearResetFlag();
    void resetLastAssignment();
    void snapshotSolution(int g);
    void saveLastAssignment();

private:
    // Static values
    int hosp_id_h;
    int hosp_location;
    int hosp_appear_time;

    // Changing values (assignment-specific)
    std::vector<int> hosp_maxcapacity{0, 0, 0};
    std::vector<int> hosp_curcapacity{0, 0, 0};

    // methauristic-specific
    int was_already_reset = 1;
    int yet_to_snapshot = 1;
    std::vector<int> hosp_prev_capacity;
};

#endif