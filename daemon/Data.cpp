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

void Data::setTCPPose(Pose const *tcp_pose)
{
  this->tcp_pose = tcp_pose;
}

void Data::setTCPOffset(Pose const *tcp_offset)
{
  this->tcp_offset = tcp_offset;
}

void Data::setCalibrationConfig(double const *delta_a, double const *delta_d, double const *delta_alpha, double const *delta_theta)
{
  this->delta_a = delta_a;
  this->delta_d = delta_d;
  this->delta_alpha = delta_alpha;
  this->delta_theta = delta_theta;
}

double *Data::getAnalysisAngle(int num)
{
  analysisRB = new AnalysisRobot(ur, tcp_pose, tcp_offset);

  double *theta = analysisRB->solveIK(num);

  double res[7];
  for (int i = 1; i < 7; i++)
    res[i] = theta[i];

  delete analysisRB;

  return res;
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
