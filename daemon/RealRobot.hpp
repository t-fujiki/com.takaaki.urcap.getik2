#pragma once

#include "Pose.hpp"
#include "Eigen/Dense"
using namespace Eigen;

typedef Matrix<double, 6, 6> Matrix6d;
typedef Matrix<double, 6, 1> Vector6d;

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
    Matrix6d getJacobian(vector<double> const &theta) const;
    Matrix6d getInverseOfJacobian(vector<double> const &theta) const;
    Pose solveFK(vector<double> const &theta) const;
    vector<double> solveIK(vector<double> const &theta) const;
};
