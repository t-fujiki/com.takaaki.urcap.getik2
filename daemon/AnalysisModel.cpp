#include <cmath>
#include <iostream>
#include <vector>
#include "AnalysisModel.hpp"
#include "DHParam.hpp"
#include "TransMatrix.hpp"

AnalysisModel::AnalysisModel(int ur, Pose tcp_pose, Pose tcp_offset)
{
    this->ur = ur;
    this->tcp_pose = tcp_pose;
    this->tcp_offset = tcp_offset;
}

vector<double> AnalysisModel::solveIK(int num) const
{
    double theta[6];
    theta[0] = 0;

    int number = num - 1;

    bool plusT5 = (number & 1) == 1;
    number >>= 1;
    bool plusT3 = (number & 1) == 1;
    number >>= 1;
    bool plusT1 = (number & 1) == 1;

    DHParam param(ur);
    TransMatrix t_tcp(tcp_pose);
    TransMatrix t_offset(tcp_offset);
    TransMatrix t06 = t_tcp * t_offset.inverse();

    double *d = param.d;
    double *a = param.a;
    double *alpha = param.alpha;

    double nx = t06.getEntry(0, 0);
    double ny = t06.getEntry(1, 0);
    double nz = t06.getEntry(2, 0);

    double ox = t06.getEntry(0, 1);
    double oy = t06.getEntry(1, 1);
    double oz = t06.getEntry(2, 1);

    double ax = t06.getEntry(0, 2);
    double ay = t06.getEntry(1, 2);
    double az = t06.getEntry(2, 2);

    double px = t06.getEntry(0, 3);
    double py = t06.getEntry(1, 3);
    double pz = t06.getEntry(2, 3);

    //---θ1
    double p05x = px - ax * d[5];
    double p05y = py - ay * d[5];
    double p05z = pz - az * d[5];

    double phi = atan2(p05y, p05x);

    if (plusT1)
        theta[0] = asin(d[3] / sqrt(pow(p05x, 2) + pow(p05y, 2))) + phi;
    else
        theta[0] = -asin(d[3] / sqrt(pow(p05x, 2) + pow(p05y, 2))) + phi + M_PI;

    //---θ5
    double p16z = px * sin(theta[0]) - py * cos(theta[0]);

    if (plusT5)
        theta[4] = acos((p16z - d[3]) / d[5]);
    else
        theta[4] = -acos((p16z - d[3]) / d[5]);

    //---θ6
    double n16z = nx * sin(theta[0]) - ny * cos(theta[0]);
    double o16z = ox * sin(theta[0]) - oy * cos(theta[0]);

    theta[6] = atan2(-o16z / sin(theta[4]), n16z / sin(theta[4]));

    //---θ3
    double n05x = nx * cos(theta[5]) - ox * sin(theta[5]);
    double n05y = ny * cos(theta[5]) - oy * sin(theta[5]);
    double n05z = nz * cos(theta[5]) - oz * sin(theta[5]);

    double o05x = nx * sin(theta[5]) + ox * cos(theta[5]);
    double o05y = ny * sin(theta[5]) + oy * cos(theta[5]);
    double o05z = nz * sin(theta[5]) + oz * cos(theta[5]);

    double p14x = (o05y * d[4] + p05y) * sin(theta[0]) + (o05x * d[4] + p05x) * cos(theta[0]);
    double p14y = o05z * d[4] + p05z - d[0];

    if (plusT3)
        theta[2] = acos((pow(p14x, 2) + pow(p14y, 2) - pow(a[2], 2) - pow(a[2], 2)) / (2 * a[1] * a[2]));
    else
        theta[2] = -acos((pow(p14x, 2) + pow(p14y, 2) - pow(a[2], 2) - pow(a[2], 2)) / (2 * a[1] * a[2]));

    //---θ2
    double c2 = (a[2] * (p14x * cos(theta[2]) + p14y * sin(theta[2])) + a[1] * p14x) / (pow(p14x, 2) + pow(p14y, 2));
    double s2 = (a[2] * (p14y * cos(theta[2]) - p14x * sin(theta[2])) + a[1] * p14y) / (pow(p14x, 2) + pow(p14y, 2));

    theta[1] = atan2(s2, c2);

    //---θ4
    double s234 = n05z * cos(theta[4]) - az * sin(theta[4]);
    double c234 = (n05y * cos(theta[4]) - ay * sin(theta[4])) * sin(theta[0]) + (n05x * cos(theta[4]) - ax * sin(theta[4])) * cos(theta[0]);

    theta[3] = atan2(s234, c234) - theta[1] - theta[2];

    vector<double> theta_vector;

    cout << "<Solved analysis solution>" << endl;

    for (int i = 0; i < 6; i++)
    {
        double s = sin(theta[i]);
        double c = cos(theta[i]);
        theta[i] = atan2(s, c);

        cout << i << ":" << theta[i] << endl;

        theta_vector.push_back(theta[i]);
    }

    return theta_vector;
}

Pose AnalysisModel::solveFK(vector<double> const &theta) const
{
    DHParam param(ur);

    TransMatrix t[6];
    for (int i = 0; i < 6; i++)
    {
        t[i] = TransMatrix(param.a[i], param.d[i], param.alpha[i], theta[i]);
    }

    TransMatrix t_offset = TransMatrix(tcp_offset);
    TransMatrix t_tcp = t[0] * t[1] * t[2] * t[3] * t[4] * t[5] * t_offset;

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

    cout << "<Solved analysis TCP>" << endl;
    cout << "X:" << x << endl;
    cout << "Y:" << y << endl;
    cout << "Z:" << z << endl;
    cout << "Rx:" << rx << endl;
    cout << "Ry:" << ry << endl;
    cout << "Rz:" << rz << endl;

    return tcp;
}

int AnalysisModel::getPattern(vector<double> const &theta) const
{
    vector<double> _theta = theta;

    for (int i = 0; i < 6; i++)
    {
        double s = sin(theta[i]);
        double c = cos(theta[i]);
        _theta[i] = atan2(s, c);
    }

    bool plusT5 = false;
    bool plusT3 = false;
    bool plusT1 = false;

    DHParam param(ur);

    TransMatrix t[6];
    for (int i = 0; i < 6; i++)
    {
        t[i] = TransMatrix(param.a[i], param.d[i], param.alpha[i], _theta[i]);
    }

    TransMatrix t_flange = t[0] * t[1] * t[2] * t[3] * t[4] * t[5];

    double *d = param.d;
    double *a = param.a;
    double *alpha = param.alpha;

    double ax = t_flange.getEntry(0, 2);
    double ay = t_flange.getEntry(1, 2);
    double az = t_flange.getEntry(2, 2);

    double px = t_flange.getEntry(0, 3);
    double py = t_flange.getEntry(1, 3);
    double pz = t_flange.getEntry(2, 3);

    double p05x = px - ax * d[5];
    double p05y = py - ay * d[5];
    double p05z = pz - az * d[5];

    double phi = atan2(p05y, p05x);

    double s = sin(theta[0] - phi);
    double c = cos(theta[0] - phi);
    double q = abs(atan2(s, c));

    if (q < M_PI / 2)
        plusT1 = true;
    if (theta[2] >= 0)
        plusT3 = true;
    if (theta[4] >= 0)
        plusT5 = true;

    int number = (plusT1 ? 4 : 0) + (plusT3 ? 2 : 0) + (plusT5 ? 1 : 0) + 1;

    return number;
}
