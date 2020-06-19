#include <iostream>
#include <string>

#include "pbconfig/pbconfig.h"
#include "planner.pb.h"


using namespace std;

int main()
{
    Configurer<PlanningConfig> cfger;
    PlanningConfig planning_config;

    cfger.GetPbMsgFromCfgFile(&planning_config,
                               "../conf/planning_config.pb.txt");
    cout << planning_config.planner_type() << endl;

    for (auto task : planning_config.em_planner_config().task()){
        cout << task << endl;
    }

    cout << planning_config.em_planner_config().dp_poly_path_config().sample_points_num_each_level()
         << endl;

    cfger.SetPbMsgToASCIIFile(planning_config,"../out/planning_out.txt");
    cfger.SetPbMsgToBinaryFile(planning_config,"../out/out.bin");

    cout << "Hello World!" << endl;
    return 0;
}
