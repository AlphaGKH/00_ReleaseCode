#include <stdio.h>

#include <lcm/lcm-cpp.hpp>
#include "exlcm/example_t.hpp"

#include "exlcm/expb.pb.h"

class Handler {
  public:
    ~Handler() {}
    void handleMessage(const lcm::ReceiveBuffer *rbuf, const std::string &chan,
                       const exlcm::example_t *msg)
    {
        int i;
        printf("Received message on channel \"%s\":\n", chan.c_str());
        printf("  timestamp   = %lld\n", (long long) msg->timestamp);
        printf("  position    = (%f, %f, %f)\n", msg->position[0], msg->position[1],
               msg->position[2]);
        printf("  orientation = (%f, %f, %f, %f)\n", msg->orientation[0], msg->orientation[1],
               msg->orientation[2], msg->orientation[3]);
        printf("  ranges:");
        for (i = 0; i < msg->num_ranges; i++)
            printf(" %d", msg->ranges[i]);
        printf("\n");
        printf("  name        = '%s'\n", msg->name.c_str());
        printf("  enabled     = %d\n", msg->enabled);
    }

    void handlePbMessage(const lcm::ReceiveBuffer* rubf, const std::string& chan){
        example::pos p1;
        p1.ParseFromArray(rubf->data,rubf->data_size);
        std::cout << "index: " << p1.index() << std::endl;
        std::cout << "x: " << p1.x() << std::endl;
        std::cout << "y: " << p1.y() << std::endl;

    }
};

int main(int argc, char **argv)
{
    lcm::LCM lcm;

    if (!lcm.good())
        return 1;

    Handler handlerObject;
    // lcm.subscribe("EXAMPLE", &Handler::handleMessage, &handlerObject);
    lcm.subscribe("EXAMPLE", &Handler::handlePbMessage, &handlerObject);

    while (0 == lcm.handle()) {
        // Do nothing
    }

    return 0;
}