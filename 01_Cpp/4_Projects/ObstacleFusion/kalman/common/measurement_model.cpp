#include "kalman/common/measurement_model.h"
namespace kalman {

MeasurementModel::MeasurementModel(const int& measure_dim,const int& state_dim,
                                   const int& measurement_noise_dim){
   V_dim_ = measurement_noise_dim;
   X_dim_ = state_dim;
   Z_dim_ = measure_dim;
   Inited_ = false;
}

bool MeasurementModel::Initialized(){
    return Inited_;
}

void MeasurementModel::Init(const Eigen::VectorXd &measurement_noise)
{
    if(!Inited_)
    {
        if(measurement_noise.rows() == V_dim_ && measurement_noise.rows() > 0 ){
            V_.resize(V_dim_);
            R_.resize(V_dim_,V_dim_);
            V_ = measurement_noise;
            SetR(measurement_noise);


            Inited_ = true;
        }
    }

}

//void MeasurementModel::Init(const Eigen::MatrixXd &measurement_noise_cov)
//{
//    if(!Inited_)
//    {
//        if(measurement_noise_cov.rows() == V_dim_ && measurement_noise_cov.cols() == V_dim_
//              &&  measurement_noise_cov.rows() > 0 ){
//            V_.resize(V_dim_);
//            R_.resize(V_dim_,V_dim_);
//            SetR(measurement_noise_cov);

//            Inited_ = true;
//        }
//    }
//}

Eigen::MatrixXd MeasurementModel::GetR() const{
    return R_;
}

void MeasurementModel::SetR(const Eigen::VectorXd &measurement_noise){
    if(measurement_noise.rows() != V_dim_ || measurement_noise.rows() <= 0){
        return;
    }

    R_.setZero();

    for (int i = 0; i < V_dim_; i ++) {
        R_(i,i) = measurement_noise(i) * measurement_noise(i);
    }
}

void MeasurementModel::SetR(const Eigen::MatrixXd &measurement_noise_cov){
    if(measurement_noise_cov.rows()!=measurement_noise_cov.cols() ||
            measurement_noise_cov.rows() != V_dim_ ||measurement_noise_cov.rows() <= 0)
    {
        return;
    }
    R_ = measurement_noise_cov;
}

long MeasurementModel::GetV_dim() const{
    return V_dim_;
}

long MeasurementModel::GetX_dim() const{
    return X_dim_;
}

long MeasurementModel::GetZ_dim() const{
    return Z_dim_;
}

}
