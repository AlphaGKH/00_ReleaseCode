#include "kalman/unscented/unscented_kalman.h"
#include <iostream>

#include <math.h>

double normalize(const double a){
  return (fabs(a) > M_PI) ? std::remainder(a, 2. * M_PI) : a;
}

namespace kalman {

UnscentedKalman::UnscentedKalman(SystemModel* system_model,
                                 MeasurementModel* measurement_model){
    if(system_model != nullptr && measurement_model != nullptr && !inited_){
        sys_ = system_model;
        measure_ = measurement_model;

        X_dim_ = sys_->GetX_dim();
        U_dim_ = sys_->GetU_dim();
        W_dim_ = sys_->GetW_dim();

        V_dim_ = measure_->GetV_dim();
        Z_dim_ = measure_->GetZ_dim();

        NX = X_dim_;
        NAUGMENT = X_dim_ + W_dim_;
        COLSIGAM = 2 * NAUGMENT + 1;

        NZ = Z_dim_;

        predicted_sigma_points_X_.resize(NX,COLSIGAM);
        predicted_sigma_points_X_.setZero();

        predicted_priori_P_XX_.resize(NX,NX);
        predicted_priori_P_XX_.setZero();

        predicted_priori_X_.resize(NX,1);
        predicted_priori_X_.setZero();

        Q_.resize(W_dim_,W_dim_);
        Q_ = sys_->GetQ();

        R_.resize(V_dim_,V_dim_);
        R_ = measure_->GetR();


        LAMBDA = 3/*ALPHA * ALPHA * (NAUGMENT + KAPPA)*/ - NAUGMENT;

        double wm0 = LAMBDA/(NAUGMENT + LAMBDA);
        double wc0 = wm0 /*+ 1 - ALPHA * ALPHA + BETA*/;
        double w = 0.5/(NAUGMENT + LAMBDA);

        weight_meanvalue.emplace_back(wm0);
        weight_covariance.emplace_back(wc0);
        for (int i = 1; i < COLSIGAM; i++) {
            weight_meanvalue.emplace_back(w);
            weight_covariance.emplace_back(w);
        }

        sigma_points_Z_.resize(NZ,COLSIGAM);
        sigma_points_Z_.setZero();

        measure_predicted_Z_.resize(NZ,1);
        measure_predicted_Z_.setZero();

        measure_predicted_P_ZZ_.resize(NZ,NZ);
        measure_predicted_P_ZZ_.setZero();

        X_.resize(NX,1);
        X_.setZero();

        P_.resize(NX,NX);
        P_.setIdentity();

        inited_ = false;
    }
}

UnscentedKalman::~UnscentedKalman(){
    inited_ = false;
    sys_ = nullptr;
    measure_ = nullptr;
}

bool UnscentedKalman::IsInited(){
    return inited_;
}

void UnscentedKalman::Init(const Eigen::VectorXd& init_X,const Eigen::MatrixXd& init_P,
                           const long long& time_stamp){
    this->X_ = init_X;
//    std::cout << this->X_ << std::endl;
    this->P_ = init_P;
    this->time_stamp_ = time_stamp;
    inited_ = true;
}

Eigen::MatrixXd UnscentedKalman::SimpleAugementSigmaPointsOnX(const Eigen::VectorXd &current_X,
                                                              const Eigen::MatrixXd &current_P)
{
    Eigen::VectorXd augmented_X;
    augmented_X.resize(NAUGMENT);
    augmented_X.setZero();

    Eigen::MatrixXd augmented_sigma_points;
    augmented_sigma_points.resize(NAUGMENT,COLSIGAM);
    augmented_sigma_points.setZero();

    Eigen::MatrixXd augmented_P;
    augmented_P.resize(NAUGMENT,NAUGMENT);
    augmented_P.setZero();

    augmented_X.head(NX) = current_X;

    augmented_P.topLeftCorner(NX,NX) = current_P;

    augmented_P.bottomRightCorner(NAUGMENT - NX,NAUGMENT - NX) = Q_;

    const Eigen::MatrixXd L = augmented_P.llt().matrixL();


    augmented_sigma_points.col(0) = augmented_X;


    for (int col = 0; col < NAUGMENT; col++) {
        int i = col + 1;
        augmented_sigma_points.col(i) = augmented_X + sqrt(LAMBDA + NAUGMENT) * L.col(col);
        augmented_sigma_points.col(i+NAUGMENT) = augmented_X - sqrt(LAMBDA + NAUGMENT) * L.col(col);
    }

  //  std::cout << augmented_sigma_points << std::endl;
    return augmented_sigma_points;
}

Eigen::MatrixXd UnscentedKalman::PredictOnSigmaPointsX(
        const Eigen::MatrixXd& augment_sigma_points,
        const Eigen::VectorXd& current_U,const double& dt)
{
    Eigen::MatrixXd predicted_sigam_points;
    predicted_sigam_points.resize(NX,COLSIGAM);

    Eigen::VectorXd cur_col;
    cur_col.resize(NAUGMENT,1);
    cur_col.setZero();

    Eigen::VectorXd cur_col_pridect_X;
    cur_col_pridect_X.resize(NX,1);
    cur_col_pridect_X.setZero();

    for (int col = 0; col < COLSIGAM; col++) {
        cur_col = augment_sigma_points.col(col);
        cur_col_pridect_X = sys_->f(cur_col,current_U,dt);
        predicted_sigam_points.col(col) = cur_col_pridect_X;
    }

//    std::cout << "predicted_sigma: " << Counter << std::endl;
//    std::cout << predicted_sigam_points << std::endl;

    return predicted_sigam_points;
}

Eigen::VectorXd UnscentedKalman::Predict_X(const Eigen::MatrixXd &predicted_sigma_points)
{
    Eigen::VectorXd predicted_X;
    predicted_X.resize(NX,1);
    predicted_X.setZero();
    for (unsigned int i = 0; i < COLSIGAM; i++) {
        predicted_X += weight_meanvalue.at(i) * predicted_sigma_points.col(i);
        //std::cout << weight_meanvalue.at(i) << std::endl;
    }

    return predicted_X;
}

Eigen::MatrixXd UnscentedKalman::Predict_P_XX(const Eigen::MatrixXd &predicted_sigma_points,
                                              const Eigen::VectorXd &predicted_X){
    Eigen::MatrixXd predicted_P;
    predicted_P.resize(NX,NX);
    predicted_P.setZero();

    Eigen::VectorXd dX;
    dX.resize(NX,1);
    dX.setZero();
    for (unsigned int i = 0; i < COLSIGAM; i++) {
        dX = predicted_sigma_points.col(i) - predicted_X;
        dX(3) = normalize(dX(3));
        predicted_P += weight_covariance.at(i) * dX * dX.transpose();
    }
    return predicted_P;
}

void UnscentedKalman::Predict(const Eigen::VectorXd &current_X, const Eigen::VectorXd& current_U,
                              const Eigen::MatrixXd &current_P, const double& dt){
    Eigen::MatrixXd augment_sigma_points;
    augment_sigma_points.resize(NAUGMENT,COLSIGAM);
    augment_sigma_points.setZero();

    augment_sigma_points = SimpleAugementSigmaPointsOnX(current_X,current_P);

    //std::cout << augment_sigma_points << std::endl;

    this->predicted_sigma_points_X_ = PredictOnSigmaPointsX(augment_sigma_points,current_U,dt);
    //std::cout << predicted_sigma_points_X_ << std::endl;
    this->predicted_priori_X_ = Predict_X(this->predicted_sigma_points_X_);
    //std::cout << predicted_priori_X_ << std::endl;
    this->predicted_priori_P_XX_ = Predict_P_XX(this->predicted_sigma_points_X_,this->predicted_priori_X_);

    //std::cout << this->predicted_priori_P_XX_ << std::endl;

}

Eigen::MatrixXd UnscentedKalman::SimpleSigmaPointsOnZ(const Eigen::MatrixXd
                                                      &predicted_sigma_points_X){
    Eigen::MatrixXd sigma_Z;
    sigma_Z.resize(NZ,COLSIGAM);
    sigma_Z.setZero();

    Eigen::VectorXd cur_col;
    cur_col.resize(NX,1);
    cur_col.setZero();

    Eigen::VectorXd cur_col_sigma_Z;
    cur_col_sigma_Z.resize(NZ,1);
    cur_col_sigma_Z.setZero();

    for (int col = 0; col < COLSIGAM; col++) {
        cur_col = predicted_sigma_points_X.col(col);
        cur_col_sigma_Z = measure_->h(cur_col);
        sigma_Z.col(col) = cur_col_sigma_Z;
    }

    return sigma_Z;
}

Eigen::VectorXd UnscentedKalman::MeasurePredict_Z(const Eigen::MatrixXd &sigma_points_Z){
    Eigen::VectorXd mp_Z;
    mp_Z.resize(NZ,1);
    mp_Z.setZero();

    for (unsigned int i = 0; i < COLSIGAM; i ++) {
        mp_Z += sigma_points_Z.col(i) * weight_meanvalue.at(i);
    }

    return mp_Z;
}

Eigen::MatrixXd UnscentedKalman::MeasurePredict_P_ZZ(const Eigen::MatrixXd &sigma_points_Z,
                                                     const Eigen::MatrixXd &measure_predicted_Z)
{
    Eigen::MatrixXd mp_P;
    mp_P.resize(NZ,NZ);
    mp_P.setZero();

    Eigen::VectorXd dZ;
    dZ.resize(NZ,1);
    dZ.setZero();

    for (unsigned int i = 0; i < COLSIGAM; i++) {
        dZ = sigma_points_Z.col(i) - measure_predicted_Z;
        dZ(1) = normalize(dZ(1));
        mp_P += weight_covariance.at(i) * dZ * dZ.transpose();
    }

    mp_P += this->R_;

//    std::cout << sigma_points_Z << std::endl;
//    std::cout << measure_predicted_Z << std::endl;
//    std::cout << R_ << std::endl;

//    std::cout << mp_P << std::endl;

    return mp_P;
}

void UnscentedKalman::Measure(const Eigen::MatrixXd &predicted_sigma_points_X){
    this->sigma_points_Z_ = SimpleSigmaPointsOnZ(predicted_sigma_points_X);
    this->measure_predicted_Z_ = MeasurePredict_Z(this->sigma_points_Z_);
    this->measure_predicted_P_ZZ_ = MeasurePredict_P_ZZ(this->sigma_points_Z_,
                                                        this->measure_predicted_Z_);

//        std::cout << this->sigma_points_Z_ << std::endl
//                  << this->measure_predicted_Z_  <<  std::endl
//                  << this->measure_predicted_P_ZZ_ << std::endl;
}

Eigen::MatrixXd UnscentedKalman::MeasurePredict_P_XZ(const Eigen::MatrixXd &predicted_sigma_points_X,
                                                     const Eigen::VectorXd &predicted_priori_X,
                                                     const Eigen::MatrixXd &sigma_points_Z,
                                                     const Eigen::VectorXd &measure_predicted_Z){
    Eigen::MatrixXd P_XZ;
    P_XZ.resize(NX,NZ);
    P_XZ.setZero();

    Eigen::VectorXd dX;
    dX.resize(NX,1);
    dX.setZero();

    Eigen::VectorXd dZ;
    dZ.resize(NZ,1);
    dZ.setZero();

    for (unsigned int i = 0; i < COLSIGAM; i++) {
        dX = predicted_sigma_points_X.col(i) - predicted_priori_X;
        dX(3) = normalize(dX(3));
        dZ = sigma_points_Z.col(i) - measure_predicted_Z;
        dZ(1) = normalize(dZ(1));
        P_XZ += weight_covariance.at(i) * dX * dZ.transpose();
    }

    //std::cout << P_XZ << std::endl;
    return P_XZ;
}

Eigen::MatrixXd UnscentedKalman::ComputeK(const Eigen::MatrixXd &P_XZ,
                                          const Eigen::MatrixXd &P_ZZ){
    Eigen::MatrixXd K;
    K.resize(NX,NZ);
    K.setZero();

    K = P_XZ * P_ZZ.inverse();

//    std::cout << P_XZ << std::endl
//              << P_ZZ << std::endl;

//    std::cout << K << std::endl;

    return K;
}

Eigen::VectorXd UnscentedKalman::ComputeX(const Eigen::VectorXd& predicted_priori_X,
                                          const Eigen::MatrixXd& K,
                                          const Eigen::VectorXd& measure_Z,
                                          const Eigen::VectorXd& measure_predicted_Z){
    Eigen::VectorXd X;
    X.resize(NX,1);
    X.setZero();

    Eigen::VectorXd dZ;
    dZ.resize(NZ,1);
    dZ.setZero();

    dZ = measure_Z - measure_predicted_Z;
    dZ(1) = normalize(dZ(1));

//    std::cout << Counter << std::endl;

//    std::cout << dZ << std::endl;

//    std::cout << K << std::endl;

//    std::cout << predicted_priori_X + K * dZ << std::endl;

    X = predicted_priori_X + K * dZ;

//    std::cout << Counter << std::endl;
//    std::cout << K << std::endl;
//    std::cout << X << std::endl;
    return X;
}

Eigen::MatrixXd UnscentedKalman::ComputeP(const Eigen::MatrixXd &predicted_priori_P_XX,
                                          const Eigen::MatrixXd &K,
                                          const Eigen::MatrixXd &measure_predicted_P_ZZ){
    Eigen::MatrixXd P;
    P.resize(NX,NX);
    P.setIdentity();

    P = predicted_priori_P_XX - K * measure_predicted_P_ZZ * K.transpose();

    return P;
}

void UnscentedKalman::Update(const Eigen::MatrixXd& predicted_sigma_points_X,
                             const Eigen::VectorXd& predicted_priori_X,
                             const Eigen::MatrixXd& sigma_points_Z,
                             const Eigen::VectorXd& measure_predicted_Z,
                             const Eigen::MatrixXd& measure_predicted_P_ZZ,
                             const Eigen::VectorXd& measure_Z,
                             const Eigen::MatrixXd& predicted_priori_P_XX){

    Eigen::MatrixXd P_XZ;
    P_XZ.resize(NX,NZ);
    P_XZ.setZero();

    P_XZ = MeasurePredict_P_XZ(predicted_sigma_points_X,predicted_priori_X,sigma_points_Z,
                               measure_predicted_Z);

    Eigen::MatrixXd K;
    K.resize(NX,NZ);
    K.setZero();

    K = ComputeK(P_XZ,measure_predicted_P_ZZ);

    this->X_ = ComputeX(predicted_priori_X,K,measure_Z,measure_predicted_Z);

    this->P_ = ComputeP(predicted_priori_P_XX,K,measure_predicted_P_ZZ);

//    std::cout << K << std::endl;
//    std::cout << this->X_ << std::endl;
//    std::cout << this->P_ << std::endl;

}

Eigen::VectorXd UnscentedKalman::GetX(){
    return this->X_;
}

Eigen::MatrixXd UnscentedKalman::GetP(){
    return this->P_;
}

Eigen::VectorXd UnscentedKalman::GetPrioriX(){
    return this->predicted_priori_X_;
}

void UnscentedKalman::Proc(const Eigen::VectorXd& current_U,
                           const Eigen::VectorXd& current_Z,
                           const long long& time_stamp){

    double dt = (time_stamp - this->time_stamp_)/1.0e6;

    //std::cout << dt << ":" << Counter << std::endl;

    this->time_stamp_ = time_stamp;

//    std::cout << this->X_ << std::endl
//              << current_U << std::endl
//              << this->P_ << std::endl
//              << dt << std::endl;

    Predict(X_,current_U,P_,dt);

//    std::cout << this->predicted_priori_X_ << std::endl;
//    std::cout << this->predicted_priori_P_XX_ << std::endl;

    Measure(this->predicted_sigma_points_X_);

    //std::cout << current_Z << std::endl;
    //std::cout << this->measure_predicted_Z_ << std::endl;
    //std::cout << this->measure_predicted_P_ZZ_ << std::endl;

    Update(this->predicted_sigma_points_X_,
           this->predicted_priori_X_,
           this->sigma_points_Z_,
           this->measure_predicted_Z_,
           this->measure_predicted_P_ZZ_,
           current_Z,
           this->predicted_priori_P_XX_);

    Counter ++;
}

}

