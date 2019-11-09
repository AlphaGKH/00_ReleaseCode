#include "example/obstacle.h"
#include <iostream>
#include <math.h>

double normalize1(const double a){
  return (fabs(a) > M_PI) ? std::remainder(a, 2. * M_PI) : a;
}

Eigen::VectorXd ObstacleSystem::f(const Eigen::VectorXd& augment_X, const Eigen::VectorXd& U,
                                  const double& dt) const
{
    const double THRESH = 0.001;
    Eigen::VectorXd Priori_X;

    const double x = augment_X(0);
    const double y = augment_X(1);
    const double v = augment_X(2);

    const double theta = augment_X(3);
    const double omega = augment_X(4);

    const double v_noise = augment_X(5);
    const double omega_noise = augment_X(6);

    const double cos_theta = cos(theta);
    const double sin_theta = sin(theta);
    const double dt2 = dt * dt;

    const double position_noise = 0.5 * v_noise *dt2;
    const double theta_noise = 0.5 * omega_noise * dt2;

    const double dtheta = omega * dt;
    const double dpos = v * dt;

    const double predict_v = v + v_noise * dt;
    const double predict_theta = theta + dtheta + theta_noise;
    const double predict_omega = omega + omega_noise * dt;

    double predict_x, predict_y;

    if(fabs(omega) <= THRESH){
        predict_x = x + dpos * cos_theta + position_noise * cos_theta;
        predict_y = y + dpos * sin_theta + position_noise * sin_theta;
    }
    else{
        const double r = v/omega;
        const double new_theta = theta + dtheta;

        predict_x = x + r * (sin(new_theta) - sin_theta) + position_noise * cos_theta;
        predict_y = y + r * (cos_theta - cos(new_theta)) + position_noise * sin_theta;
    }

//    std::cout << position_noise * cos_theta << std::endl
//              << position_noise * sin_theta << std::endl
//              << v_noise << std::endl
//              << theta_noise << std::endl
//              << omega_noise << std::endl;

    Priori_X.resize(GetX_dim(),1);
    Priori_X(0) = predict_x;
    Priori_X(1) = predict_y;
    Priori_X(2) = predict_v;

    Priori_X(3) = normalize1(predict_theta);
    Priori_X(4) = predict_omega;

    return Priori_X;

}

Eigen::VectorXd ObstacleMeasurement::h(const Eigen::VectorXd &predicted_priori_X) const{
    const double THRESH = 1e-4;
    Eigen::VectorXd MeasurePredict_Z;
    MeasurePredict_Z.resize(GetZ_dim(),1);
    MeasurePredict_Z.setZero();

    const double x = predicted_priori_X(0);
    const double y = predicted_priori_X(1);
    const double v = predicted_priori_X(2);
    const double theta = predicted_priori_X(3);

    const double v_x = cos(theta) * v;
    const double v_y = sin(theta) * v;

    const double r = sqrt(x * x + y * y);
    const double phi = atan2(y, x);
    const double dot_r = (r > THRESH) ? ((x * v_x + y * v_y) / r) : 0.0;

    MeasurePredict_Z(0) = r;
    MeasurePredict_Z(1) = normalize1(phi);
    MeasurePredict_Z(2) = dot_r;

    return MeasurePredict_Z;

}
