#ifndef HOSPITAL_H
#define HOSPITAL_H

class Hospital
{
public:
    Hospital();
    ~Hospital();
    void setHospitalID();
    void setHospitalLocation();
    void setHospitalMaxCapacity();
    void setHospitalCurCapacity(int g);

    int getHospitalID();
    int getHospitalLocation();
    int getHospitalMaxCapacity();
    int getHospitalCurCapacity(int g);

private:
    int hosp_id_h;
    int hosp_location;
    int hosp_maxcap_K;
    int hosp_curcap_g1;
    int hosp_curcap_g2;
    int hosp_curcap_g3;
};

#endif