#include "pose.hpp"

class RealRobot
{
private:
    int ur;
    Pose const *tcp_pose;
    Pose const *tcp_offset;

    double const *delta_a;
    double const *delta_d;
    double const *delta_alpha;
    double const *delta_theta;

public:
    RealRobot(int ur, Pose const *tcp_pose, Pose const *tcp_offset);
    void setCalibrationConfig(double const *delta_a, double const *delta_d, double const *delta_alpha, double const *delta_theta);
    double *solveIK(int num);
    Pose solveFK(double const *theta);
};
