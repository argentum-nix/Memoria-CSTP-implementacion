#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <vector>
#include <iostream>

class Hospital
{
public:
    Hospital();
    ~Hospital();
    void setHospitalID(int id);
    void setHospitalLocation(int node_id);
    void setHospitalMaxCapacity(int g, int k);
    void setHospitalCurCapacity(int g, int k);
    void setHospitalAppearTime(int t);

    int getHospitalID();
    int getHospitalMaxCapacity(int g);
    int getHospitalCurCapacity(int g);
    int getHospitalLocation();
    int getHospitalAppearTime();

    void resetLastAssignment();
    void snapshotSolution(int g);
    void printData();
    void clearResetFlag();
    void saveLastAssignment();

private:
    int yet_to_snapshot = 1;
    int hosp_id_h;
    int hosp_location;
    int was_already_reset = 1;
    int hosp_appear_time;
    std::vector<int> hosp_maxcapacity{0, 0, 0};
    std::vector<int> hosp_curcapacity{0, 0, 0};
    std::vector<int> hosp_prev_capacity;
};

#endif