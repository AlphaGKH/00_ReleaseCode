#include <iostream>
#include "modules/planning/proto/planning.pb.h"

int main() {
    dharma::common::Header header;
    header.set_module_name("Planning");
    header.set_timestamp_sec(1.8);

     dharma::planning::ADCTrajectory traj;
     traj.mutable_header()->CopyFrom(header);

     std::cout << traj.header().module_name()
               << " time is: "
               << traj.header().timestamp_sec() << std::endl;

    return 0;
}
