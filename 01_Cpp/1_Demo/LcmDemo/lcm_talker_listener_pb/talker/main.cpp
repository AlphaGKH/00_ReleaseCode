#include <iostream>

#include <lcm/lcm-cpp.hpp>

#include "exlcm/example_t.hpp"
#include "exlcm/expb.pb.h"

template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename MessageT>
void Print(const enable_if_t<
           !std::is_base_of<google::protobuf::Message, MessageT>::value,MessageT>& msg){
    std::cout << "lcm" << std::endl;
    return;
}

//void Print(const google::protobuf::Message& msg){
//    std::cout << "proto" << std::endl;
//    return;
//}



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
    // for (int i = 0; i < my_data.num_ranges; i++)
    //     my_data.ranges[i] = i;

    // my_data.name = "example string";
    // my_data.enabled = true;

    // lcm.publish("EXAMPLE", &my_data);

    example::pos p;
    p.set_index(1);
    p.set_x(1.6);
    p.set_y(3.9);
//    std::cout << "type size: " << p.ByteSize() << std::endl;

//    void* data = malloc(p.ByteSize());
//    p.SerializeToArray(data, p.ByteSize());
    
//    lcm.publish("EXAMPLE",data, uint64_t(p.ByteSize()));

//    delete [] data;

//    Print(my_data);

//    unsigned int datalen = my_data.getEncodedSize();
//    uint8_t *buf = new uint8_t[datalen];
//    my_data.encode(buf, 0, datalen);

    Print(p);
//    Print<exlcm::example_t>(my_data);
    

    return 0;
}
