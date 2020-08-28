#include <cmath>
#include <iostream>
#include <vector>
#include "analysisrobot.hpp"
#include "dhparam.hpp"
#include "transmatrix.hpp"

AnalysisRobot::AnalysisRobot(int ur, Pose tcp_pose, Pose tcp_offset)
{
    this->ur = ur;
    this->tcp_pose = tcp_pose;
    this->tcp_offset = tcp_offset;
}

vector<double> AnalysisRobot::solveIK(int num)
{
    double theta[7];
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

    cout << t_tcp.entry << endl;
    cout << t_offset.entry << endl;
    cout << t06.entry << endl;

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

    vector<double> theta_vector;
    theta_vector.push_back(0);

    cout << "<Solved analysis solution>" << endl;

    for (int i = 1; i < 7; i++)
    {
        double s = sin(theta[i]);
        double c = cos(theta[i]);
        theta[i] = atan2(s, c);

        cout << i << ":" << theta[i] << endl;

        theta_vector.push_back(theta[i]);
    }

    return theta_vector;
}

Pose AnalysisRobot::solveFK(double *theta)
{
    DHParam param(ur);

    TransMatrix t[7];
    for (int i = 1; i < 7; i++)
    {
        cout << i << ":" << theta[i] << endl;

        t[i] = TransMatrix(param.a[i], param.d[i], param.alpha[i], theta[i]);
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

    cout << "<Solved analysis TCP>" << endl;
    cout << "X:" << x << endl;
    cout << "Y:" << y << endl;
    cout << "Z:" << z << endl;
    cout << "Rx:" << rx << endl;
    cout << "Ry:" << ry << endl;
    cout << "Rz:" << rz << endl;

    return tcp;
}

int AnalysisRobot::getPattern(double *theta)
{
    double _theta[7];

    for (int i = 1; i < 7; i++)
    {
        double s = sin(theta[i]);
        double c = cos(theta[i]);
        _theta[i] = atan2(s, c);
    }

    bool plusT5 = false;
    bool plusT3 = false;
    bool plusT1 = false;

    DHParam param(ur);

    TransMatrix t[7];
    for (int i = 1; i < 7; i++)
    {
        t[i] = TransMatrix(param.a[i], param.d[i], param.alpha[i], _theta[i]);
    }

    TransMatrix t_flange = t[1] * t[2] * t[3] * t[4] * t[5] * t[6];

    double *d = param.d;
    double *a = param.a;
    double *alpha = param.alpha;

    double ax = t_flange.getEntry(0, 2);
    double ay = t_flange.getEntry(1, 2);
    double az = t_flange.getEntry(2, 2);

    double px = t_flange.getEntry(0, 3);
    double py = t_flange.getEntry(1, 3);
    double pz = t_flange.getEntry(2, 3);

    double p05x = px - ax * d[6];
    double p05y = py - ay * d[6];
    double p05z = pz - az * d[6];

    double phi = atan2(p05y, p05x);

    double s = sin(_theta[1] - phi);
    double c = cos(_theta[1] - phi);
    double q = abs(atan2(s, c));

    if (q < M_PI / 2)
        plusT1 = true;
    if (_theta[3] >= 0)
        plusT3 = true;
    if (_theta[5] >= 0)
        plusT5 = true;

    int number = (plusT1 ? 4 : 0) + (plusT3 ? 2 : 0) + (plusT5 ? 1 : 0) + 1;

    return number;
}
