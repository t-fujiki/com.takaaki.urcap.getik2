#include "pose.hpp"

class AnalysisRobot
{
private:
    int ur;
    Pose const *tcp_pose;
    Pose const *tcp_offset;

public:
    AnalysisRobot(int ur, Pose const *tcp_pose, Pose const *tcp_offset);
    double *solveIK(int num);
    Pose solveFK(double const *theta);
};
