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

    vector<double> delta_a;
    vector<double> delta_d;
    vector<double> delta_alpha;
    vector<double> delta_theta;

public:
    RealRobot(int ur, Pose tcp_pose, Pose tcp_offset);
    void setCalibrationConfig(vector<double> delta_a, vector<double> delta_d,
                              vector<double> delta_alpha, vector<double> delta_theta);
    Pose solveFK(vector<double> theta);
    Matrix6d getJacobian(vector<double> theta);
    Matrix6d getInverseOfJacobian(vector<double> theta);
};
