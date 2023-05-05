#include <iostream>
#include "instance.h"
using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        cout << "Error: Expected more parameters." << '\n';
        cout << "Follow the format: ./CSTPSolver directory instance_txt_name network_txt_name" << '\n';
        cout << "Try: ./CSTPSolver ./instances/ instance1 network" << '\n';
        return 0;
    }

    Instance cstp(argv[1], argv[2], argv[3]);
    // Load the data
    int archiveStatus;
    archiveStatus = cstp.loadNetwork();
    if (archiveStatus == LOADING_FAILED)
    {
        cout << "[ERROR] Failed to load the network txt." << endl;
        return 0;
    }
    archiveStatus = cstp.loadInstance();
    if (archiveStatus == LOADING_FAILED)
    {
        cout << "[ERROR]: Failed to load the instance txt." << endl;
        return 0;
    }
    return 0;
}