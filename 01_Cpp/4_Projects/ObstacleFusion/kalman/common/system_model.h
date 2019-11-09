#ifndef KALMAN_SYSTEM_MODEL_H_
#define KALMAN_SYSTEM_MODEL_H_

#include "eigen3/Eigen/Dense"

namespace kalman {

class SystemModel
{
protected:
    SystemModel(const int& state_dims, const int& control_dims,
                const int& process_noise_dims);
    virtual ~SystemModel(){}
public:

    virtual Eigen::VectorXd f(const Eigen::VectorXd& X, const Eigen::VectorXd& U,
                              const double& dt) const = 0;

    bool Initialized();
    void Init(const Eigen::VectorXd& process_noise);
//    void Init(const Eigen::MatrixXd& process_noise_cov);

    long GetX_dim() const;
    long GetU_dim() const;
    long GetW_dim() const;

    Eigen::MatrixXd GetQ() const;

private:
    void SetQ(const Eigen::VectorXd& process_noise);
    void SetQ(const Eigen::MatrixXd& process_noise_cov);

    bool Inited_;

    long X_dim_;
    long U_dim_;
    long W_dim_;

    Eigen::VectorXd W_;
    Eigen::MatrixXd Q_;

};

}



#endif
