#include <cstdlib>
#include <sstream>
#include <string>
#include "Data.hpp"
#include "iksolver.hpp"
#include "fksolver.hpp"
#include "jacobian.hpp"
#include "ur.hpp"

using namespace std;

Data::Data()
{
}

Data::~Data()
{
}

void Data::setURType(string type)
{
  if (type == "UR3")
    ur = UR::UR3;
  else if (type == "UR5")
    ur = UR::UR5;
  else if (type == "UR10")
    ur = UR::UR10;
  else if (type == "UR3e")
    ur = UR::UR3e;
  else if (type == "UR5e")
    ur = UR::UR5e;
  else if (type == "UR10e")
    ur = UR::UR10e;
  else if (type == "UR16e")
    ur = UR::UR16e;
}

void Data::setPose(double x, double y, double z, double rx, double ry, double rz)
{
  this->x = x;
  this->y = y;
  this->z = z;
  this->rx = rx;
  this->ry = ry;
  this->rz = rz;
}

void Data::setOffset(double x, double y, double z, double rx, double ry, double rz)
{
  this->offset_x = x;
  this->offset_y = y;
  this->offset_z = z;
  this->offset_rx = rx;
  this->offset_ry = ry;
  this->offset_rz = rz;
}

void Data::setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta)
{
  for (int i = 1; i < 7; i++)
  {
    this->delta_a[i] = delta_a[i];
    this->delta_d[i] = delta_d[i];
    this->delta_alpha[i] = delta_alpha[i];
    this->delta_theta[i] = delta_theta[i];
  }
}

double *Data::getAngles(int num)
{
  IKSolver solver(ur, x, y, z, rx, ry, rz);
  solver.setOffset(offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz);
  solver.setCalibration(delta_a, delta_d, delta_alpha, delta_theta);
  solver.solve(num);
  double *theta = solver.getAngle();

  for (int i = 0; i < 6; i++)
    angle[i] = theta[i + 1];

  return angle;
}

int Data::getPattern(double *angles)
{
  IKSolver solver(ur, x, y, z, rx, ry, rz);
  return solver.getPattern(angles);
}

double *Data::getPose(double *angles)
{
  double theta[7];

  for (int i = 1; i < 7; i++)
    theta[i] = angles[i - 1];

  FKSolver solver(ur);
  solver.setOffset(offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz);
  solver.setCalibration(delta_a, delta_d, delta_alpha, delta_theta);
  solver.solve(theta);

  double pose[6];
  pose[0] = solver.getX();
  pose[1] = solver.getY();
  pose[2] = solver.getZ();
  pose[3] = solver.getRx();
  pose[4] = solver.getRy();
  pose[5] = solver.getRz();

  Jacobian jacobian(ur);
  jacobian.setOffset(offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz);
  jacobian.setCalibration(delta_a, delta_d, delta_alpha, delta_theta);
  jacobian.solve(theta);
  jacobian.inverse();
  jacobian.calc();

  return pose;
}
