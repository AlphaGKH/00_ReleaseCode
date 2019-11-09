#include "kalman/common/system_model.h"
#include "kalman/common/measurement_model.h"

#include "eigen3/Eigen/Dense"

class ObstacleSystem : public kalman::SystemModel
{
public:
    ObstacleSystem(const int& state_dim, const int& proc_nos_dim):
        kalman::SystemModel (state_dim,0,proc_nos_dim){}
    ~ObstacleSystem() override {}

    Eigen::VectorXd f(const Eigen::VectorXd& augment_X, const Eigen::VectorXd& U,
                      const double& dt ) const override;

};

class ObstacleMeasurement : public kalman::MeasurementModel
{
public:
    ObstacleMeasurement(const int& meas_dim, const int& state_dim, const int& meas_nos_dim):
        kalman::MeasurementModel (meas_dim,state_dim,meas_nos_dim){}
    ~ObstacleMeasurement() override{}

    Eigen::VectorXd h(const Eigen::VectorXd& X) const override;
};

struct ObstacleMeasureType
{
    long long timestamp;
    double r;
    double phi;
    double r_dot;

    ObstacleMeasureType() {
        timestamp = 0;
        r = 0;
        phi = 0;
        r_dot = 0;
    }
};

struct ObstacleTrueType
{
    long long timestamp;
    double x;
    double y;
    double v;

    ObstacleTrueType() {
        timestamp = 0;
        x = 0;
        y = 0;
        v = 0;
    }
};
