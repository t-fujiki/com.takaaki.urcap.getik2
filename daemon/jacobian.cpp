#include <math.h>
#include <cmath>
#include <iostream>
#include "jacobian.hpp"
#include "fksolver.hpp"
#include "transmatrix.hpp"
#include "ur.hpp"
#include "dhparam.hpp"
using namespace std;

Jacobian::Jacobian(int ur)
{

    this->ur = ur;
}

Jacobian::~Jacobian()
{
}

void Jacobian::setOffset(double x, double y, double z, double rx, double ry, double rz)
{
    offset_x = x;
    offset_y = y;
    offset_z = z;
    offset_rx = rx;
    offset_ry = ry;
    offset_rz = rz;
}

void Jacobian::setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta)
{
    for (int i = 1; i < 7; i++)
    {
        this->delta_a[i] = delta_a[i];
        this->delta_d[i] = delta_d[i];
        this->delta_alpha[i] = delta_alpha[i];
        this->delta_theta[i] = delta_theta[i];
    }
}

void Jacobian::solve(double *theta)
{

    double dt = 10E-6;

    for (int j = 1; j < 7; j++)
    {
        FKSolver solver_p(ur);
        FKSolver solver_m(ur);

        double theta_p[7];
        double theta_m[7];

        solver_p.setOffset(offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz);
        solver_p.setCalibration(delta_a, delta_d, delta_alpha, delta_theta);

        solver_m.setOffset(offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz);
        solver_m.setCalibration(delta_a, delta_d, delta_alpha, delta_theta);

        for (int k = 1; k < 7; k++)
        {
            theta_p[k] = j == k ? theta[k] + dt : theta[k];
            theta_m[k] = j == k ? theta[k] - dt : theta[k];
        }

        solver_p.solve(theta_p);
        solver_m.solve(theta_m);

        entry[0][j - 1] = (solver_p.getX() - solver_m.getX()) / (2 * dt);
        entry[1][j - 1] = (solver_p.getY() - solver_m.getY()) / (2 * dt);
        entry[2][j - 1] = (solver_p.getZ() - solver_m.getZ()) / (2 * dt);
        entry[3][j - 1] = (solver_p.getRx() - solver_m.getRx()) / (2 * dt);
        entry[4][j - 1] = (solver_p.getRy() - solver_m.getRy()) / (2 * dt);
        entry[5][j - 1] = (solver_p.getRz() - solver_m.getRz()) / (2 * dt);
    }
}

void Jacobian::inverse()
{
    double ent[6][6];

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            ent[i][j] = entry[i][j];
            inv_entry[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    //掃き出し法
    for (int i = 0; i < 6; i++)
    {
        double buf = 1 / ent[i][i];
        for (int j = 0; j < 6; j++)
        {
            ent[i][j] *= buf;
            inv_entry[i][j] *= buf;
        }
        for (int j = 0; j < 6; j++)
        {
            if (i != j)
            {
                buf = ent[j][i];
                for (int k = 0; k < 6; k++)
                {
                    ent[j][k] -= ent[i][k] * buf;
                    inv_entry[j][k] -= inv_entry[i][k] * buf;
                }
            }
        }
    }
}

void Jacobian::calc()
{
    double t[6][6];
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
        {
            t[i][j] = 0;
            for (int k = 0; k < 6; k++)
                t[i][j] += entry[i][k] * inv_entry[k][j];

            cout << t[i][j] << endl;
        }
}
