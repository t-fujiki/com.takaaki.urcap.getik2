#include <iostream>
#include <vector>
#include "pose.hpp"
#include "Eigen/Dense"
using namespace Eigen;

class TransMatrix
{
private:
public:
    Matrix4d entry;

    TransMatrix(Pose pose);
    TransMatrix(double a, double d, double alpha, double theta);
    TransMatrix();
    ~TransMatrix();
    TransMatrix operator*(const TransMatrix &tm);
    TransMatrix inverse();
    double setEntry(int i, int j, double value);
    double getEntry(int i, int j);
};
