#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Pose
{
public:
    double x;
    double y;
    double z;
    double rx;
    double ry;
    double rz;

    Pose(double x, double y, double z, double rx, double ry, double rz);
    Pose(vector<double> pose_vector);
    Pose(double *pose);
    Pose();
    vector<double> toVector();
};
