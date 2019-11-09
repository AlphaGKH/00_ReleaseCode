#ifndef KALMAN_MEASUREMENT_MODEL_H_
#define KALMAN_MEASUREMENT_MODEL_H_

#include <eigen3/Eigen/Dense>

namespace kalman {


class MeasurementModel
{
public:
    MeasurementModel(const int& measure_dim,const int& state_dim,
                     const int& measurement_noise_dim);
    virtual ~MeasurementModel() {}

    virtual Eigen::VectorXd h(const Eigen::VectorXd& predicted_priori_X) const = 0;

    bool Initialized();

    void Init(const Eigen::VectorXd& measurement_noise);
    //void Init(const Eigen::MatrixXd& measurement_noise_cov);

    long GetZ_dim() const;
    long GetX_dim() const;
    long GetV_dim() const;

    Eigen::MatrixXd GetR() const;

private:
    void SetR(const Eigen::VectorXd& measurement_noise);
    void SetR(const Eigen::MatrixXd& measurement_noise_cov);

    bool Inited_;

    long Z_dim_;
    long X_dim_;
    long V_dim_;

    Eigen::VectorXd V_;
    Eigen::MatrixXd R_;

};


}


#endif
