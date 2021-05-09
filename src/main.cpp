#include "./WindowManagement.hpp"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    WindowManagement* window_management = new WindowManagement();

    bool err = !window_management -> init("Visualization");

    if(err)
    {
        exit(1);
    }

    window_management->mainloop();

    return 0;
}