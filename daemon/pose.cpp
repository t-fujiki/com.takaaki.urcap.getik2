#include "pose.hpp"

Pose::Pose(double x, double y, double z, double rx, double ry, double rz)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->rx = rx;
    this->ry = ry;
    this->rz = rz;
}

Pose::Pose(vector<double> pose_vector)
{
    this->x = pose_vector.at(0);
    this->y = pose_vector.at(1);
    this->z = pose_vector.at(2);
    this->rx = pose_vector.at(3);
    this->ry = pose_vector.at(4);
    this->rz = pose_vector.at(5);
}

Pose::Pose(double *pose)
{
    this->x = pose[0];
    this->y = pose[1];
    this->z = pose[2];
    this->rx = pose[3];
    this->ry = pose[4];
    this->rz = pose[5];
}

vector<double> Pose::toVector()
{
    vector<double> vector;

    vector.push_back(x);
    vector.push_back(y);
    vector.push_back(z);
    vector.push_back(rx);
    vector.push_back(ry);
    vector.push_back(rz);

    return vector;
}

double *Pose::toDoubleArray()
{
    double pose[6];

    pose[0] = x;
    pose[1] = y;
    pose[2] = z;
    pose[3] = rx;
    pose[4] = ry;
    pose[5] = rz;

    return pose;
}