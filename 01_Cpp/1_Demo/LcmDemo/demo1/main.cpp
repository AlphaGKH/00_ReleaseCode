#include <lcm/lcm-cpp.hpp>

#include "exlcm/example_t.hpp"

#include <iostream>
#include <unistd.h>

int main(int argc, char **argv)
{
    lcm::LCM lcm;
    if (!lcm.good())
        return 1;

    exlcm::example_t my_data;
    my_data.timestamp = 0;

    my_data.position[0] = 1;
    my_data.position[1] = 2;
    my_data.position[2] = 3;

    my_data.orientation[0] = 1;
    my_data.orientation[1] = 0;
    my_data.orientation[2] = 0;
    my_data.orientation[3] = 0;

    my_data.num_ranges = 15;
    my_data.ranges.resize(my_data.num_ranges);
    for (int i = 0; i < my_data.num_ranges; i++)
        my_data.ranges[i] = i;

    my_data.name = "example string";
    my_data.enabled = true;

    long count = 0;

    while (true) {
        std::cout << "Publish: " << count << std::endl;
        my_data.timestamp = count;
        count ++;
        lcm.publish("EXAMPLE", &my_data);
        sleep(2);
    }



    return 0;
}


