#include <iostream>
#include <string>

#include "pbconfig/pbconfig.h"
#include "proto/planner.pb.h"


using namespace std;

//string cfg_file_name = "/home/igs/cpp/learn/pbconf/conf/planning_config.pb.txt";


int main()
{
    Configurer<planning::PlanningConfig> cfger;
    planning::PlanningConfig planning_config;

    cfger.GetPbMsgFromCfgFile(&planning_config,
                               "/home/kh/Code/1_CXX/pbconf/conf/planning_config.pb.txt");
    cout << planning_config.planner_type() << endl;

    for (auto task : planning_config.em_planner_config().task()){
        cout << task << endl;
    }

    cout << planning_config.em_planner_config().dp_poly_path_config().sample_points_num_each_level()
         << endl;

    cfger.SetPbMsgToASCIIFile(planning_config,"/home/kh/Code/1_CXX/pbconf/out/planning_out.txt");
    cfger.SetPbMsgToBinaryFile(planning_config,"/home/kh/Code/1_CXX/pbconf/out/out.bin");

    cout << "Hello World!" << endl;
    return 0;
}
