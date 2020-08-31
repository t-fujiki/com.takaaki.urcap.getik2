#include "Pose.hpp"

class AnalysisModel
{
private:
    int ur;
    Pose tcp_pose;
    Pose tcp_offset;

public:
    AnalysisModel(int ur, Pose tcp_pose, Pose tcp_offset);
    vector<double> solveIK(int num) const;
    Pose solveFK(vector<double> const &theta) const;
    int getPattern(vector<double> const &theta) const;
};
