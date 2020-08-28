#include <cmath>
#include <iostream>
#include <vector>
#include "realrobot.hpp"
#include "dhparam.hpp"
#include "transmatrix.hpp"

RealRobot::RealRobot(int ur, Pose tcp_pose, Pose tcp_offset)
{
    this->ur = ur;
    this->tcp_pose = tcp_pose;
    this->tcp_offset = tcp_offset;
}

void RealRobot::setCalibrationConfig(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta)
{
    this->delta_a = delta_a;
    this->delta_d = delta_d;
    this->delta_alpha = delta_alpha;
    this->delta_theta = delta_theta;

    for (int i = 1; i < 7; i++)
    {
        cout << "a:" << delta_a[i] << endl;
        cout << "d:" << delta_d[i] << endl;
        cout << "alpha:" << delta_alpha[i] << endl;
        cout << "theta:" << delta_theta[i] << endl;
    }
}

Pose RealRobot::solveFK(double *theta)
{
    DHParam param(ur);

    TransMatrix t[7];
    for (int i = 1; i < 7; i++)
    {
        t[i] = TransMatrix(param.a[i] + delta_a[i], param.d[i] + delta_d[i],
                           param.alpha[i] + delta_alpha[i], theta[i] + delta_theta[i]);
        /* cout << "a:" << delta_a[i] << endl;
        cout << "d:" << delta_d[i] << endl;
        cout << "alpha:" << delta_alpha[i] << endl;
        cout << "theta:" << delta_theta[i] << endl;*/
    }

    TransMatrix t_offset = TransMatrix(tcp_offset);
    TransMatrix t_tcp = t[1] * t[2] * t[3] * t[4] * t[5] * t[6] * t_offset;

    double r11 = t_tcp.getEntry(0, 0);
    double r12 = t_tcp.getEntry(0, 1);
    double r13 = t_tcp.getEntry(0, 2);
    double r21 = t_tcp.getEntry(1, 0);
    double r22 = t_tcp.getEntry(1, 1);
    double r23 = t_tcp.getEntry(1, 2);
    double r31 = t_tcp.getEntry(2, 0);
    double r32 = t_tcp.getEntry(2, 1);
    double r33 = t_tcp.getEntry(2, 2);

    double x = t_tcp.getEntry(0, 3);
    double y = t_tcp.getEntry(1, 3);
    double z = t_tcp.getEntry(2, 3);

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

    double rx = nx * angle;
    double ry = ny * angle;
    double rz = nz * angle;

    Pose tcp(x, y, z, rx, ry, rz);

    cout << "<Solved real TCP>" << endl;
    cout << "X:" << x << endl;
    cout << "Y:" << y << endl;
    cout << "Z:" << z << endl;
    cout << "Rx:" << rx << endl;
    cout << "Ry:" << ry << endl;
    cout << "Rz:" << rz << endl;

    return tcp;
}

Matrix6d RealRobot::getJacobian(double *theta)
{
    Matrix6d jacobian;

    double dt = 10E-6;

    for (int j = 1; j < 7; j++)
    {
        double theta_p[7];
        double theta_m[7];

        for (int k = 1; k < 7; k++)
        {
            theta_p[k] = j == k ? theta[k] + dt : theta[k];
            theta_m[k] = j == k ? theta[k] - dt : theta[k];
        }

        Pose tcp_p = solveFK(theta_p);
        Pose tcp_m = solveFK(theta_m);

        for (int i = 0; i < 6; i++)
            jacobian(i, j - 1) = (tcp_p.toVector().at(i) - tcp_m.toVector().at(i)) / (2 * dt);
    }

    cout << "<Solved jacobian matrix>" << endl;
    cout << jacobian << endl;

    return jacobian;
}

Matrix6d RealRobot::getInverseOfJacobian(double *theta)
{
    Matrix6d jacobian = getJacobian(theta);

    return jacobian.inverse();
}
