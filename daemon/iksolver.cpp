#include <math.h>
#include <cmath>
#include <iostream>
#include "iksolver.hpp"
#include "transmatrix.hpp"
#include "ur.hpp"
#include "dhparam.hpp"
using namespace std;

double withinPI(double angle)
{
    double c = cos(angle);
    double s = sin(angle);

    return atan2(s, c);
}

IKSolver::IKSolver(int ur, double x, double y, double z, double rx, double ry, double rz)
{
    this->ur = ur;
    this->x = x;
    this->y = y;
    this->z = z;
    this->rx = rx;
    this->ry = ry;
    this->rz = rz;

    offset_x = 0;
    offset_y = 0;
    offset_z = 0;
    offset_rx = 0;
    offset_ry = 0;
    offset_rz = 0;

    for (int i = 0; i < 7; i++)
        theta[i] = 0;
}

IKSolver::~IKSolver()
{
}

void IKSolver::setOffset(double x, double y, double z, double rx, double ry, double rz)
{
    offset_x = x;
    offset_y = y;
    offset_z = z;
    offset_rx = rx;
    offset_ry = ry;
    offset_rz = rz;
}

void IKSolver::setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta)
{
    for (int i = 1; i < 7; i++)
    {
        this->delta_a[i] = delta_a[i];
        this->delta_d[i] = delta_d[i];
        this->delta_alpha[i] = delta_alpha[i];
        this->delta_theta[i] = delta_theta[i];
    }
}

void IKSolver::solve(int num)
{
    int number = num - 1;

    bool plusT5 = (number & 1) == 1;
    number >>= 1;
    bool plusT3 = (number & 1) == 1;
    number >>= 1;
    bool plusT1 = (number & 1) == 1;

    DHParam param(ur);
    TransMatrix t_tcp(x, y, z, rx, ry, rz);
    TransMatrix t_offset(offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz);
    TransMatrix t06 = t_tcp * t_offset.inverse();

    double *d = param.d;
    double *a = param.a;
    double *alpha = param.alpha;

    double nx = t06.entry[0][0];
    double ny = t06.entry[1][0];
    double nz = t06.entry[2][0];

    double ox = t06.entry[0][1];
    double oy = t06.entry[1][1];
    double oz = t06.entry[2][1];

    double ax = t06.entry[0][2];
    double ay = t06.entry[1][2];
    double az = t06.entry[2][2];

    double px = t06.entry[0][3];
    double py = t06.entry[1][3];
    double pz = t06.entry[2][3];

    //---θ1
    double p05x = px - ax * d[6];
    double p05y = py - ay * d[6];
    double p05z = pz - az * d[6];

    double phi = atan2(p05y, p05x);

    if (plusT1)
        theta[1] = asin(d[4] / sqrt(pow(p05x, 2) + pow(p05y, 2))) + phi;
    else
        theta[1] = -asin(d[4] / sqrt(pow(p05x, 2) + pow(p05y, 2))) + phi + M_PI;

    //---θ5
    double p16z = px * sin(theta[1]) - py * cos(theta[1]);

    if (plusT5)
        theta[5] = acos((p16z - d[4]) / d[6]);
    else
        theta[5] = -acos((p16z - d[4]) / d[6]);

    //---θ6
    double n16z = nx * sin(theta[1]) - ny * cos(theta[1]);
    double o16z = ox * sin(theta[1]) - oy * cos(theta[1]);

    theta[6] = atan2(-o16z / sin(theta[5]), n16z / sin(theta[5]));

    //---θ3
    double n05x = nx * cos(theta[6]) - ox * sin(theta[6]);
    double n05y = ny * cos(theta[6]) - oy * sin(theta[6]);
    double n05z = nz * cos(theta[6]) - oz * sin(theta[6]);

    double o05x = nx * sin(theta[6]) + ox * cos(theta[6]);
    double o05y = ny * sin(theta[6]) + oy * cos(theta[6]);
    double o05z = nz * sin(theta[6]) + oz * cos(theta[6]);

    double p14x = (o05y * d[5] + p05y) * sin(theta[1]) + (o05x * d[5] + p05x) * cos(theta[1]);
    double p14y = o05z * d[5] + p05z - d[1];

    if (plusT3)
        theta[3] = acos((pow(p14x, 2) + pow(p14y, 2) - pow(a[2], 2) - pow(a[3], 2)) / (2 * a[2] * a[3]));
    else
        theta[3] = -acos((pow(p14x, 2) + pow(p14y, 2) - pow(a[2], 2) - pow(a[3], 2)) / (2 * a[2] * a[3]));

    //---θ2
    double c2 = (a[3] * (p14x * cos(theta[3]) + p14y * sin(theta[3])) + a[2] * p14x) / (pow(p14x, 2) + pow(p14y, 2));
    double s2 = (a[3] * (p14y * cos(theta[3]) - p14x * sin(theta[3])) + a[2] * p14y) / (pow(p14x, 2) + pow(p14y, 2));

    theta[2] = atan2(s2, c2);

    //---θ4
    double s234 = n05z * cos(theta[5]) - az * sin(theta[5]);
    double c234 = (n05y * cos(theta[5]) - ay * sin(theta[5])) * sin(theta[1]) + (n05x * cos(theta[5]) - ax * sin(theta[5])) * cos(theta[1]);

    theta[4] = atan2(s234, c234) - theta[2] - theta[3];

    for (int i = 1; i < 7; i++)
    {
        theta[i] = withinPI(theta[i]);
    }
}

double *IKSolver::getAngle()
{
    return theta;
}

int IKSolver::getPattern(double *angles)
{
    double theta[7];

    for (int i = 1; i < 7; i++)
        theta[i] = withinPI(angles[i - 1]);

    bool plusT5 = false;
    bool plusT3 = false;
    bool plusT1 = false;

    DHParam param(ur);

    TransMatrix t[7];
    for (int i = 1; i < 7; i++)
        t[i] = TransMatrix(param.a[i], param.d[i], param.alpha[i], theta[i]);

    TransMatrix t06 = t[1] * t[2] * t[3] * t[4] * t[5] * t[6];

    double *d = param.d;
    double *a = param.a;
    double *alpha = param.alpha;

    double ax = t06.entry[0][2];
    double ay = t06.entry[1][2];
    double az = t06.entry[2][2];

    double px = t06.entry[0][3];
    double py = t06.entry[1][3];
    double pz = t06.entry[2][3];

    double p05x = px - ax * d[6];
    double p05y = py - ay * d[6];
    double p05z = pz - az * d[6];

    double phi = atan2(p05y, p05x);

    double q = abs((withinPI(theta[1] - phi)));

    if (q < M_PI / 2)
        plusT1 = true;
    if (theta[3] >= 0)
        plusT3 = true;
    if (theta[5] >= 0)
        plusT5 = true;

    int number = (plusT1 ? 4 : 0) + (plusT3 ? 2 : 0) + (plusT5 ? 1 : 0) + 1;

    return number;
}
