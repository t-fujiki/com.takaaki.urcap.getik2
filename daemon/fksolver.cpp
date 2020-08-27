#include <math.h>
#include <cmath>
#include <iostream>
#include "fksolver.hpp"
#include "transmatrix.hpp"
#include "ur.hpp"
#include "dhparam.hpp"
using namespace std;

FKSolver::FKSolver(int ur)
{
    this->ur = ur;
}

FKSolver::~FKSolver()
{
}

void FKSolver::setOffset(double x, double y, double z, double rx, double ry, double rz)
{
    offset_x = x;
    offset_y = y;
    offset_z = z;
    offset_rx = rx;
    offset_ry = ry;
    offset_rz = rz;
}

void FKSolver::setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta)
{
    for (int i = 1; i < 7; i++)
    {
        this->delta_a[i] = delta_a[i];
        this->delta_d[i] = delta_d[i];
        this->delta_alpha[i] = delta_alpha[i];
        this->delta_theta[i] = delta_theta[i];
    }
}

void FKSolver::solve(double *theta)
{

    DHParam param(ur);

    TransMatrix t[7];
    for (int i = 1; i < 7; i++)
        t[i] = TransMatrix(param.a[i] + delta_a[i], param.d[i] + delta_d[i], param.alpha[i] + delta_alpha[i], theta[i] + delta_theta[i]);

    TransMatrix t_offset = TransMatrix(offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz);

    TransMatrix t_tcp = t[1] * t[2] * t[3] * t[4] * t[5] * t[6] * t_offset;

    double r11 = t_tcp.entry[0][0];
    double r12 = t_tcp.entry[0][1];
    double r13 = t_tcp.entry[0][2];
    double r21 = t_tcp.entry[1][0];
    double r22 = t_tcp.entry[1][1];
    double r23 = t_tcp.entry[1][2];
    double r31 = t_tcp.entry[2][0];
    double r32 = t_tcp.entry[2][1];
    double r33 = t_tcp.entry[2][2];

    x = t_tcp.entry[0][3];
    y = t_tcp.entry[1][3];
    z = t_tcp.entry[2][3];

    double angle = acos((r11 + r22 + r33 - 1) / 2);

    double nx, ny, nz;
    if (abs(sin(angle)) > 10E-6)
    {
        nx = (r32 - r23) / (2 * sin(angle));
        ny = (r13 - r31) / (2 * sin(angle));
        nz = (r21 - r12) / (2 * sin(angle));

        if (nx < 0)
        {
            angle = -acos((r11 + r22 + r33 - 1) / 2) + 2 * M_PI;
            nx = (r32 - r23) / (2 * sin(angle));
            ny = (r13 - r31) / (2 * sin(angle));
            nz = (r21 - r12) / (2 * sin(angle));
        }
    }
    else
    {
        nx = sqrt((r11 - cos(angle)) / (1 - cos(angle)));
        ny = sqrt((r22 - cos(angle)) / (1 - cos(angle)));
        nz = sqrt((r33 - cos(angle)) / (1 - cos(angle)));
    }

    rx = nx * angle;
    ry = ny * angle;
    rz = nz * angle;
}

double FKSolver::getX()
{
    return x;
}

double FKSolver::getY()
{
    return y;
}

double FKSolver::getZ()
{
    return z;
}

double FKSolver::getRx()
{
    return rx;
}

double FKSolver::getRy()
{
    return ry;
}

double FKSolver::getRz()
{
    return rz;
}
