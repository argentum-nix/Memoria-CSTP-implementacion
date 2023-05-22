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

    int getHospitalID();
    int getHospitalMaxCapacity(int g);
    int getHospitalCurCapacity(int g);
    int getHospitalLocation();

    void printData();

private:
    int hosp_id_h;
    int hosp_location;
    std::vector<int> hosp_maxcapacity{0, 0, 0};
    std::vector<int> hosp_curcapacity{0, 0, 0};
};

#endif