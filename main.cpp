#include <iostream>
#include "instance.h"
using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        cout << "Error: Expected more parameters." << '\n';
        cout << "Follow the format: ./CSTPSolver directory name" << '\n';
        cout << "Try: ./CSTPSolver ./instances i1" << '\n';
        return 0;
    }

    Instance cstp(argv[2], argv[1]);
    // Load the data
    int archiveStatus = cstp.loadData();
    if (archiveStatus == LOADING_INSTANCE_FAILED)
    {
        cout << "Error: Failed to load the archive. Check if it is present in the directory.";
        return 0;
    }
    return 0;
}