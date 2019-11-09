#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>


#include <eigen3/Eigen/Dense>

#include "kalman/unscented/unscented_kalman.h"

#include "example/obstacle.h"
//#include <math.h>

#include <sys/time.h>

int main()
{
    std::string in_filename = "/home/igs/cpp/ObstacleFusion/example/data/RadarData.log";

    std::string out_filename = "/home/igs/cpp/ObstacleFusion/example/data/OutData.log";

    std::ifstream in_file(in_filename.c_str(), std::ifstream::in);
    std::ofstream out_file(out_filename.c_str(), std::ofstream::out);

    std::string seneor_type;

    std::vector<ObstacleMeasureType> obstacle_sensor_data;
    std::vector<ObstacleTrueType> obstacle_true_data;


    ObstacleMeasureType sensor_data;
    ObstacleTrueType true_data;

    std::string line;

    double vx;
    double vy;

    while (getline(in_file,line)) {
        std::istringstream iss(line);
        iss >> seneor_type;
        if(seneor_type.compare("R") == 0){
            iss >> sensor_data.r;
            iss >> sensor_data.phi;
            iss >> sensor_data.r_dot;
            iss >> sensor_data.timestamp;

            iss >> true_data.x;
            iss >> true_data.y;
            iss >> vx;
            iss >> vy;
            true_data.v = sqrt(vx*vx + vy*vy);

            obstacle_sensor_data.emplace_back(sensor_data);
            obstacle_true_data.emplace_back(true_data);
        }
    }

    out_file << "time_stamp" << "\t";
    out_file << "x" << "\t";
    out_file << "y" << "\t";
    out_file << "v" << "\t";
    out_file << "theta" << "\t";
    out_file << "omega" << "\t";

    out_file << "measure_x" << "\t";
    out_file << "measure_y" << "\t";

    out_file << "predict_x" << "\t";
    out_file << "predict_y" << "\t";
    out_file << "predict_v" << "\t";
    out_file << "predict_theta" << "\t";
    out_file << "predict_omega" << "\t";

    out_file << "true_x" << "\t";
    out_file << "true_y" << "\t";
    out_file << "true_v" << "\n";


    int state_dim = 5;
    int process_noise_dim = 2;
    ObstacleSystem obs_sys(state_dim,process_noise_dim);
    Eigen::Vector2d w;
    w << 0.9,0.6;
    //w << 0.0,0.0;

    int measure_dim = 3;
    int measurement_noise_dim = 3;
    ObstacleMeasurement obs_mea(measure_dim,state_dim,measurement_noise_dim);
    Eigen::Vector3d v;
    v << 0.3,0.03,0.3;
    //v << 0.0,0.0,0.0;

    obs_sys.Init(w);
    obs_mea.Init(v);

    if(!obs_sys.Initialized() || !obs_mea.Initialized()){
        return 0;
    }

    kalman::UnscentedKalman ukf(&obs_sys,&obs_mea);

    Eigen::VectorXd state = Eigen::VectorXd(state_dim, 1);

    Eigen::Vector2d U;
    U << 0,0;

    Eigen::VectorXd Z = Eigen::VectorXd(measure_dim);

    for (unsigned int i = 0;i < obstacle_sensor_data.size(); i++) {
        double r = obstacle_sensor_data.at(i).r;
        double phi = obstacle_sensor_data.at(i).phi;
        double r_dot = obstacle_sensor_data.at(i).r_dot;

        Z << r,phi,r_dot;

        double measure_x = r * cos(phi);
        double measure_y = r * sin(phi);

        //std::cout << Z << std::endl;

        long long t = obstacle_sensor_data.at(i).timestamp;

        if(!ukf.IsInited()){
            state << measure_x,measure_y,0,0,0;
            ukf.Init(state, ukf.GetP(), t);
        }
        else {
            ukf.Proc(U,Z,t);

            state = ukf.GetX();
        }

        //std::cout << ukf.GetX() << std::endl;

        out_file << t << "\t";
        out_file << state(0) << "\t";
        out_file << state(1) << "\t";
        out_file << state(2) << "\t";
        out_file << state(3) << "\t";
        out_file << state(4) << "\t";

        out_file << measure_x << "\t";
        out_file << measure_y << "\t";

        state.setZero();
        state = ukf.GetPrioriX();
        out_file << state(0) << "\t";
        out_file << state(1) << "\t";
        out_file << state(2) << "\t";
        out_file << state(3) << "\t";
        out_file << state(4) << "\t";

        out_file << obstacle_true_data.at(i).x << "\t";
        out_file << obstacle_true_data.at(i).y << "\t";
        out_file << obstacle_true_data.at(i).v << "\n";


    }

    std::cout << "Hello World!" << std::endl;
    return 0;
}
