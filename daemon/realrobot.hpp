#include "pose.hpp"
#include "Eigen/Dense"
using namespace Eigen;

typedef Matrix<double, 6, 6> Matrix6d;

class RealRobot
{
private:
    int ur;
    Pose tcp_pose;
    Pose tcp_offset;

    double *delta_a;
    double *delta_d;
    double *delta_alpha;
    double *delta_theta;

public:
    RealRobot(int ur, Pose tcp_pose, Pose tcp_offset);
    void setCalibrationConfig(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta);
    Pose solveFK(double *theta);
    Matrix6d getJacobian(double *theta);
    Matrix6d getInverseOfJacobian(double *theta);
};
