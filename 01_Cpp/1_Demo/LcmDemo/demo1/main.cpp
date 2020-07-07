#include <iostream>
#include <thread>

#include "lcm/lcm-cpp.hpp"

#include "exlcm/example_t.hpp"

#include "time/rate.h"

int main()
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

    my_data.name = "example string";
    my_data.enabled = true;

    for (int i = 0; i < my_data.num_ranges; i++)
        my_data.ranges[i] = i;

    Rate rate(10.0);

    while (true) {
        std::cout << "Publish" << std::endl;

        my_data.timestamp = Time::Now().ToNanosecond();

        lcm.publish("EXAMPLE", &my_data);

        rate.Sleep();
    }

    return 0;
}


