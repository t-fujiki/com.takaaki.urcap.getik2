#include <iostream>
#include <vector>
#include "pose.hpp"

class TransMatrix
{
private:
    double entry[4][4];

public:
    TransMatrix(Pose const *pose);
    TransMatrix(double a, double d, double alpha, double theta);
    TransMatrix();
    ~TransMatrix();
    TransMatrix operator*(const TransMatrix &tm);
    TransMatrix inverse();
    double setEntry(int i, int j, double value);
    double getEntry(int i, int j);
};
