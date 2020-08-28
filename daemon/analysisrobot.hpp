#include "pose.hpp"

class AnalysisRobot
{
private:
    int ur;
    Pose tcp_pose;
    Pose tcp_offset;

public:
    AnalysisRobot(int ur, Pose tcp_pose, Pose tcp_offset);
    vector<double> solveIK(int num);
    Pose solveFK(vector<double> theta);
    int getPattern(vector<double> theta);
};
