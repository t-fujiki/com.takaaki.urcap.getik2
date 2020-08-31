#include <cmath>
#include <iostream>
#include <vector>
#include "RealRobot.hpp"
#include "DHParam.hpp"
#include "TransMatrix.hpp"

RealRobot::RealRobot(int ur, Pose tcp_pose, Pose tcp_offset)
{
    this->ur = ur;
    this->tcp_pose = tcp_pose;
    this->tcp_offset = tcp_offset;
}

void RealRobot::setCalibrationConfig(vector<double> delta_a, vector<double> delta_d,
                                     vector<double> delta_alpha, vector<double> delta_theta)
{
    this->delta_a = delta_a;
    this->delta_d = delta_d;
    this->delta_alpha = delta_alpha;
    this->delta_theta = delta_theta;
}

Matrix6d RealRobot::getJacobian(vector<double> const &theta) const
{
    Matrix6d jacobian;

    double dt = 1E-6;

    for (int j = 1; j < 7; j++)
    {
        vector<double> theta_p = theta;
        vector<double> theta_m = theta;

        theta_p.at(j) = theta.at(j) + dt;
        theta_m.at(j) = theta.at(j) - dt;

        Pose tcp_p = solveFK(theta_p);
        Pose tcp_m = solveFK(theta_m);

        for (int i = 0; i < 6; i++)
        {
            jacobian(i, j - 1) = (tcp_p.toVector()[i] - tcp_m.toVector()[i]) / (2 * dt);
        }
    }

    cout << "<Solved jacobian matrix>" << endl;
    cout << jacobian << endl;

    return jacobian;
}

Matrix6d RealRobot::getInverseOfJacobian(vector<double> const &theta) const
{
    Matrix6d jacobian = getJacobian(theta);

    Matrix6d inv = jacobian.inverse();

    cout << "<Solved jacobian inverse matrix>" << endl;
    cout << inv << endl;

    return inv;
}

Pose RealRobot::solveFK(vector<double> const &theta) const
{
    DHParam param(ur);

    TransMatrix t[7];
    for (int i = 1; i < 7; i++)
    {
        t[i] = TransMatrix(param.a[i] + delta_a[i], param.d[i] + delta_d[i],
                           param.alpha[i] + delta_alpha[i], theta[i] + delta_theta[i]);
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
    if (abs(sin(angle)) > 1E-2)
    {
        nx = (r32 - r23) / (2 * sin(angle));
        ny = (r13 - r31) / (2 * sin(angle));
        nz = (r21 - r12) / (2 * sin(angle));
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
    return tcp;
}

vector<double> RealRobot::solveIK(vector<double> const &theta) const
{
    //解析解から実ロボットのTCPを計算し、qnearの代わりとする。
    Pose near_pose = solveFK(theta);

    cout << "<Solved qnear TCP>" << endl;
    cout << "X:" << near_pose.x << endl;
    cout << "Y:" << near_pose.y << endl;
    cout << "Z:" << near_pose.z << endl;
    cout << "Rx:" << near_pose.rx << endl;
    cout << "Ry:" << near_pose.ry << endl;
    cout << "Rz:" << near_pose.rz << endl;

    //実TCPとqnearの差からΔqを計算する。
    Pose tartget = tcp_pose;
    Vector6d delta_q;
    for (int i = 0; i < 6; i++)
        delta_q(i) = tartget.toVector()[i] - near_pose.toVector()[i];

    //ヤコビアン行列
    Matrix6d invJacobian = getInverseOfJacobian(theta);

    //実角度解を計算する。
    Vector6d d_theta = invJacobian * delta_q;

    vector<double> solve_theta = theta;

    for (int i = 1; i < 7; i++)
        solve_theta[i] += d_theta(i - 1);

    cout << "<Solved real solution>" << endl;

    for (int i = 1; i < 7; i++)
        cout << i << ":" << solve_theta[i] << endl;

    return solve_theta;
}
