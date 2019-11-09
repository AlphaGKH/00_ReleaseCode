#ifndef KALMAN_UNSCENTED_KALMAN_H_
#define KALMAN_UNSCENTED_KALMAN_H_

#include <eigen3/Eigen/Dense>
#include "kalman/common/system_model.h"
#include "kalman/common/measurement_model.h"

#include <memory>
#include <vector>

namespace kalman {

class UnscentedKalman
{
public:
    UnscentedKalman(SystemModel* system_model, MeasurementModel* measurement_model);
    ~UnscentedKalman();

    bool IsInited();

    //init
    void Init(const Eigen::VectorXd& init_X, const Eigen::MatrixXd& init_P,
              const long long& time_stamp);

    void Proc(const Eigen::VectorXd& current_U,
              const Eigen::VectorXd& current_Z,
              const long long& time_stamp);

    Eigen::VectorXd GetX();
    Eigen::MatrixXd GetP();
    Eigen::VectorXd GetPrioriX();





private:


    // predict
    Eigen::MatrixXd SimpleAugementSigmaPointsOnX(const Eigen::VectorXd& current_X,
                                      const Eigen::MatrixXd& current_P);

    Eigen::MatrixXd PredictOnSigmaPointsX(const Eigen::MatrixXd& augment_sigma_points,
                                       const Eigen::VectorXd& current_U,
                                       const double& dt);

    Eigen::VectorXd Predict_X(const Eigen::MatrixXd& predicted_sigma_points);

    Eigen::MatrixXd Predict_P_XX(const Eigen::MatrixXd& predicted_sigma_points,
                              const Eigen::VectorXd& predicted_X);

    void Predict(const Eigen::VectorXd& current_X,const Eigen::VectorXd& current_U,
                 const Eigen::MatrixXd& current_P, const double& dt);

    //measurement
    Eigen::MatrixXd SimpleSigmaPointsOnZ(const Eigen::MatrixXd& predicted_sigma_points_X);

    Eigen::VectorXd MeasurePredict_Z(const Eigen::MatrixXd& sigma_points_Z);

    Eigen::MatrixXd MeasurePredict_P_ZZ(const Eigen::MatrixXd& sigma_points_Z,
                                     const Eigen::MatrixXd& measure_predicted_Z);

    void Measure(const Eigen::MatrixXd& predicted_sigma_points_X);


    //update
    Eigen::MatrixXd MeasurePredict_P_XZ(const Eigen::MatrixXd& predicted_sigma_points_X,
                                        const Eigen::VectorXd& predicted_priori_X,
                                        const Eigen::MatrixXd& sigma_points_Z,
                                        const Eigen::VectorXd& measure_predicted_Z);

    Eigen::MatrixXd ComputeK(const Eigen::MatrixXd& P_XZ, const Eigen::MatrixXd& P_ZZ);

    Eigen::VectorXd ComputeX(const Eigen::VectorXd& predicted_priori_X,
                             const Eigen::MatrixXd& K,
                             const Eigen::VectorXd& measure_Z,
                             const Eigen::VectorXd& measure_predicted_Z);

    Eigen::MatrixXd ComputeP(const Eigen::MatrixXd& predicted_priori_P_XX,
                             const Eigen::MatrixXd& K,
                             const Eigen::MatrixXd& measure_predicted_P_ZZ);

    void Update(const Eigen::MatrixXd& predicted_sigma_points_X,
                const Eigen::VectorXd& predicted_priori_X,
                const Eigen::MatrixXd& sigma_points_Z,
                const Eigen::VectorXd& measure_predicted_Z,
                const Eigen::MatrixXd& measure_predicted_P_ZZ,
                const Eigen::VectorXd& measure_Z, // from sensor
                const Eigen::MatrixXd& predicted_priori_P_XX);


private:

    bool inited_;

    // for sigma point
    long NX;
    long NAUGMENT;
    long COLSIGAM;
    double ALPHA = 0.65465367070797709;
    double BETA = 2; // for Gaussian distribution, don't change
    double KAPPA = 0;
    double LAMBDA;

    std::vector<double> weight_meanvalue;//
    std::vector<double> weight_covariance;//

    long NZ;

    //predict
    SystemModel* sys_;////
    long X_dim_;////
    long U_dim_;////
    long W_dim_;////
    Eigen::MatrixXd predicted_sigma_points_X_;//
    Eigen::VectorXd predicted_priori_X_;//
    Eigen::MatrixXd predicted_priori_P_XX_;//

    Eigen::MatrixXd Q_;////

    //measurement
    MeasurementModel* measure_;////
    long Z_dim_;////
    long V_dim_;////
    Eigen::MatrixXd sigma_points_Z_;//
    Eigen::VectorXd measure_predicted_Z_;//
    Eigen::MatrixXd measure_predicted_P_ZZ_;//

    Eigen::MatrixXd R_;////

    //update
    Eigen::VectorXd X_;
    Eigen::MatrixXd P_;

    long long time_stamp_;

    long Counter = 0;

};

}

#endif
