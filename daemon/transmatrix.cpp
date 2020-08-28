#include <cstdlib>
#include <cmath>
#include "transmatrix.hpp"

TransMatrix::TransMatrix(Pose pose)
{
    double angle = sqrt(pose.rx * pose.rx + pose.ry * pose.ry + pose.rz * pose.rz);

    double r11 = 1;
    double r12 = 0;
    double r13 = 0;
    double r21 = 0;
    double r22 = 1;
    double r23 = 0;
    double r31 = 0;
    double r32 = 0;
    double r33 = 1;

    if (abs(sin(angle)) > 10E-10)
    {
        double nx = pose.rx / angle;
        double ny = pose.ry / angle;
        double nz = pose.rz / angle;

        double s = sin(angle);
        double c = cos(angle);

        double m = 1 - cos(angle);

        r11 = c + nx * nx * m;
        r12 = nx * ny * m - nz * s;
        r13 = nx * nz * m + ny * s;

        r21 = ny * nx * m + nz * s;
        r22 = c + ny * ny * m;
        r23 = ny * nz * m - nx * s;

        r31 = nz * nx * m - ny * s;
        r32 = nz * ny * m + nx * s;
        r33 = c + nz * nz * m;
    }

    entry(0, 0) = r11;
    entry(0, 1) = r12;
    entry(0, 2) = r13;
    entry(0, 3) = pose.x;

    entry(1, 0) = r21;
    entry(1, 1) = r22;
    entry(1, 2) = r23;
    entry(1, 3) = pose.y;

    entry(2, 0) = r31;
    entry(2, 1) = r32;
    entry(2, 2) = r33;
    entry(2, 3) = pose.z;

    entry(3, 0) = 0;
    entry(3, 1) = 0;
    entry(3, 2) = 0;
    entry(3, 3) = 1;
}

TransMatrix::TransMatrix(double a, double d, double alpha, double theta)
{

    entry(0, 0) = cos(theta);
    entry(0, 1) = -sin(theta) * cos(alpha);
    entry(0, 2) = sin(theta) * sin(alpha);
    entry(0, 3) = a * cos(theta);

    entry(1, 0) = sin(theta);
    entry(1, 1) = cos(theta) * cos(alpha);
    entry(1, 2) = -cos(theta) * sin(alpha);
    entry(1, 3) = a * sin(theta);

    entry(2, 0) = 0;
    entry(2, 1) = sin(alpha);
    entry(2, 2) = cos(alpha);
    entry(2, 3) = d;

    entry(3, 0) = 0;
    entry(3, 1) = 0;
    entry(3, 2) = 0;
    entry(3, 3) = 1;
}

TransMatrix::TransMatrix()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            entry(i, j) = 0;
}

TransMatrix::~TransMatrix()
{
}

TransMatrix TransMatrix::operator*(const TransMatrix &tm)
{
    TransMatrix matrix;

    matrix.entry = this->entry * tm.entry;

    return matrix;
}

TransMatrix TransMatrix::inverse()
{
    TransMatrix invMatrix;

    invMatrix.entry = this->entry.inverse();

    return invMatrix;
}

double TransMatrix::setEntry(int i, int j, double value)
{
    entry(i, j) = value;
}

double TransMatrix::getEntry(int i, int j)
{
    return entry(i, j);
}
