#include "eigen3/Eigen/Dense"
#include "kalman/common/system_model.h"

namespace kalman {

SystemModel::SystemModel(const int& state_dims, const int& control_dims,
                         const int& process_noise_dims){
    X_dim_ = state_dims;
    U_dim_ = control_dims;
    W_dim_ = process_noise_dims;
    Inited_ = false;
}

bool SystemModel::Initialized(){
    return Inited_;
}

void SystemModel::Init(const Eigen::VectorXd& process_noise)
{
    if(!Inited_)
    {
        if(process_noise.rows() == W_dim_ && process_noise.rows() > 0){
            W_.resize(W_dim_,1);
            Q_.resize(W_dim_,W_dim_);
            W_ = process_noise;
            SetQ(process_noise);

            Inited_ = true;
        }
    }
}

//void SystemModel::Init(const Eigen::MatrixXd &process_noise_cov){
//    if(!Inited_)
//    {
//        if(process_noise_cov.rows() == W_dim_ && process_noise_cov.cols() == W_dim_
//                && process_noise_cov.rows() > 0){
//            W_.resize(W_dim_,1);
//            Q_.resize(W_dim_,W_dim_);
//            SetQ(process_noise_cov);

//            Inited_ = true;
//        }
//    }
//}

Eigen::MatrixXd SystemModel::GetQ() const{
    return Q_;
}

void SystemModel::SetQ(const Eigen::VectorXd &process_noise)
{
    if(process_noise.rows() != W_dim_ && process_noise.rows() > 0)
    {
        return;
    }

    Q_.setZero();

    for (int i = 0; i < W_dim_ ; i++) {
        Q_(i,i) = process_noise(i) * process_noise(i);
    }
}

void SystemModel::SetQ(const Eigen::MatrixXd &process_noise_cov)
{
    if(process_noise_cov.rows() != process_noise_cov.cols()
            || process_noise_cov.rows() != W_dim_ || process_noise_cov.rows() <= 0)
    {
        return;
    }
    Q_ = process_noise_cov;
}

long SystemModel::GetX_dim() const{
    return X_dim_;
}

long SystemModel::GetU_dim() const{
    return U_dim_;
}

long SystemModel::GetW_dim() const{
    return W_dim_;
}

}

