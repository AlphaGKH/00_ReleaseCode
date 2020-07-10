#include <iostream>
#include "modules/control/proto/control.pb.h"

int main() {
    dharma::common::Header header;
    header.set_module_name("Control");
    header.set_timestamp_sec(2.8);

    dharma::control::Command cmd;
    cmd.mutable_header()->CopyFrom(header);

    std::cout << cmd.header().module_name()
              << " time is: "
              << cmd.header().timestamp_sec() << std::endl;

    return 0;
}
