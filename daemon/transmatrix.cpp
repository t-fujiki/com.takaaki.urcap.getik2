#include <cstdlib>
#include <math.h>
#include "transmatrix.hpp"

TransMatrix::TransMatrix(double x, double y, double z, double rx, double ry, double rz)
{

    double angle = sqrt(rx * rx + ry * ry + rz * rz);

    double r11 = 1;
    double r12 = 0;
    double r13 = 0;
    double r21 = 0;
    double r22 = 1;
    double r23 = 0;
    double r31 = 0;
    double r32 = 0;
    double r33 = 1;

    if (abs(angle) > 10E-10)
    {
        double nx = rx / angle;
        double ny = ry / angle;
        double nz = rz / angle;

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

    entry[0][0] = r11;
    entry[0][1] = r12;
    entry[0][2] = r13;
    entry[0][3] = x;

    entry[1][0] = r21;
    entry[1][1] = r22;
    entry[1][2] = r23;
    entry[1][3] = y;

    entry[2][0] = r31;
    entry[2][1] = r32;
    entry[2][2] = r33;
    entry[2][3] = z;

    entry[3][0] = 0;
    entry[3][1] = 0;
    entry[3][2] = 0;
    entry[3][3] = 1;
}

TransMatrix::TransMatrix(double a, double d, double alpha, double theta)
{

    entry[0][0] = cos(theta);
    entry[0][1] = -sin(theta) * cos(alpha);
    entry[0][2] = sin(theta) * sin(alpha);
    entry[0][3] = a * cos(theta);

    entry[1][0] = sin(theta);
    entry[1][1] = cos(theta) * cos(alpha);
    entry[1][2] = -cos(theta) * sin(alpha);
    entry[1][3] = a * sin(theta);

    entry[2][0] = 0;
    entry[2][1] = sin(alpha);
    entry[2][2] = cos(alpha);
    entry[2][3] = d;

    entry[3][0] = 0;
    entry[3][1] = 0;
    entry[3][2] = 0;
    entry[3][3] = 1;
}

TransMatrix::TransMatrix()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            entry[i][j] = 0;
}

TransMatrix::~TransMatrix()
{
}

TransMatrix TransMatrix::operator*(const TransMatrix &tm)
{
    TransMatrix matrix;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                matrix.entry[i][j] += entry[i][k] * tm.entry[k][j];

    return matrix;
}

TransMatrix TransMatrix::inverse()
{
    TransMatrix invMatrix;

    double A2323 = entry[2][2] * entry[3][3] - entry[2][3] * entry[3][2];
    double A1323 = entry[2][1] * entry[3][3] - entry[2][3] * entry[3][1];
    double A1223 = entry[2][1] * entry[3][2] - entry[2][2] * entry[3][1];
    double A0323 = entry[2][0] * entry[3][3] - entry[2][3] * entry[3][0];
    double A0223 = entry[2][0] * entry[3][2] - entry[2][2] * entry[3][0];
    double A0123 = entry[2][0] * entry[3][1] - entry[2][1] * entry[3][0];
    double A2313 = entry[1][2] * entry[3][3] - entry[1][3] * entry[3][2];
    double A1313 = entry[1][1] * entry[3][3] - entry[1][3] * entry[3][1];
    double A1213 = entry[1][1] * entry[3][2] - entry[1][2] * entry[3][1];
    double A2312 = entry[1][2] * entry[2][3] - entry[1][3] * entry[2][2];
    double A1312 = entry[1][1] * entry[2][3] - entry[1][3] * entry[2][1];
    double A1212 = entry[1][1] * entry[2][2] - entry[1][2] * entry[2][1];
    double A0313 = entry[1][0] * entry[3][3] - entry[1][3] * entry[3][0];
    double A0213 = entry[1][0] * entry[3][2] - entry[1][2] * entry[3][0];
    double A0312 = entry[1][0] * entry[2][3] - entry[1][3] * entry[2][0];
    double A0212 = entry[1][0] * entry[2][2] - entry[1][2] * entry[2][0];
    double A0113 = entry[1][0] * entry[3][1] - entry[1][1] * entry[3][0];
    double A0112 = entry[1][0] * entry[2][1] - entry[1][1] * entry[2][0];

    double det = entry[0][0] * (entry[1][1] * A2323 - entry[1][2] * A1323 + entry[1][3] * A1223) - entry[0][1] * (entry[1][0] * A2323 - entry[1][2] * A0323 + entry[1][3] * A0223) + entry[0][2] * (entry[1][0] * A1323 - entry[1][1] * A0323 + entry[1][3] * A0123) - entry[0][3] * (entry[1][0] * A1223 - entry[1][1] * A0223 + entry[1][2] * A0123);
    det = 1 / det;

    invMatrix.entry[0][0] = det * (entry[1][1] * A2323 - entry[1][2] * A1323 + entry[1][3] * A1223);
    invMatrix.entry[0][1] = det * -(entry[0][1] * A2323 - entry[0][2] * A1323 + entry[0][3] * A1223);
    invMatrix.entry[0][2] = det * (entry[0][1] * A2313 - entry[0][2] * A1313 + entry[0][3] * A1213);
    invMatrix.entry[0][3] = det * -(entry[0][1] * A2312 - entry[0][2] * A1312 + entry[0][3] * A1212);
    invMatrix.entry[1][0] = det * -(entry[1][0] * A2323 - entry[1][2] * A0323 + entry[1][3] * A0223);
    invMatrix.entry[1][1] = det * (entry[0][0] * A2323 - entry[0][2] * A0323 + entry[0][3] * A0223);
    invMatrix.entry[1][2] = det * -(entry[0][0] * A2313 - entry[0][2] * A0313 + entry[0][3] * A0213);
    invMatrix.entry[1][3] = det * (entry[0][0] * A2312 - entry[0][2] * A0312 + entry[0][3] * A0212);
    invMatrix.entry[2][0] = det * (entry[1][0] * A1323 - entry[1][1] * A0323 + entry[1][3] * A0123);
    invMatrix.entry[2][1] = det * -(entry[0][0] * A1323 - entry[0][1] * A0323 + entry[0][3] * A0123);
    invMatrix.entry[2][2] = det * (entry[0][0] * A1313 - entry[0][1] * A0313 + entry[0][3] * A0113);
    invMatrix.entry[2][3] = det * -(entry[0][0] * A1312 - entry[0][1] * A0312 + entry[0][3] * A0112);
    invMatrix.entry[3][0] = det * -(entry[1][0] * A1223 - entry[1][1] * A0223 + entry[1][2] * A0123);
    invMatrix.entry[3][1] = det * (entry[0][0] * A1223 - entry[0][1] * A0223 + entry[0][2] * A0123);
    invMatrix.entry[3][2] = det * -(entry[0][0] * A1213 - entry[0][1] * A0213 + entry[0][2] * A0113);
    invMatrix.entry[3][3] = det * (entry[0][0] * A1212 - entry[0][1] * A0212 + entry[0][2] * A0112);

    return invMatrix;
}
