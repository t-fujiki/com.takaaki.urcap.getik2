#include <cstdlib>
#include <sstream>
#include <string>
#include "Data.hpp"
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

  cout << "URType:" << type << endl;
}

void Data::setTCPPose(Pose tcp_pose)
{
  this->tcp_pose = tcp_pose;

  cout << "<TCP>" << endl;
  cout << "X:" << this->tcp_pose.x << endl;
  cout << "Y:" << this->tcp_pose.y << endl;
  cout << "Z:" << this->tcp_pose.z << endl;
  cout << "Rx:" << this->tcp_pose.rx << endl;
  cout << "Ry:" << this->tcp_pose.ry << endl;
  cout << "Rz:" << this->tcp_pose.rz << endl;
}

void Data::setTCPOffset(Pose tcp_offset)
{
  this->tcp_offset = tcp_offset;

  cout << "<TCP Offset>" << endl;
  cout << "X:" << this->tcp_offset.x << endl;
  cout << "Y:" << this->tcp_offset.y << endl;
  cout << "Z:" << this->tcp_offset.z << endl;
  cout << "Rx:" << this->tcp_offset.rx << endl;
  cout << "Ry:" << this->tcp_offset.ry << endl;
  cout << "Rz:" << this->tcp_offset.rz << endl;
}

void Data::setCalibrationConfigA(double *delta_a)
{
  this->delta_a = delta_a;

  cout << "<Delta A>" << endl;
  for (int i = 1; i < 7; i++)
    cout << i + 1 << ":" << this->delta_a[i] << endl;
}

void Data::setCalibrationConfigD(double *delta_d)
{
  this->delta_d = delta_d;

  cout << "<Delta D>" << endl;
  for (int i = 1; i < 7; i++)
    cout << i + 1 << ":" << this->delta_d[i] << endl;
}

void Data::setCalibrationConfigAlpha(double *delta_alpha)
{
  this->delta_alpha = delta_alpha;

  cout << "<Delta Alpha>" << endl;
  for (int i = 1; i < 7; i++)
    cout << i + 1 << ":" << this->delta_alpha[i] << endl;
}

void Data::setCalibrationConfigTheta(double *delta_theta)
{
  this->delta_theta = delta_theta;

  cout << "<Delta Theta>" << endl;
  for (int i = 1; i < 7; i++)
    cout << i + 1 << ":" << this->delta_theta[i] << endl;
}

vector<double> Data::getAnalysisAngle(int num)
{
  AnalysisRobot analysisRB(ur, tcp_pose, tcp_offset);

  vector<double> theta_vector = analysisRB.solveIK(num);

  return theta_vector;
}

vector<double> Data::getRealAngle(int num)
{
  //解析解を取得する。
  vector<double> theta_a_vector = getAnalysisAngle(num);

  double theta_a[7];

  for (int i = 0; i < 7; i++)
    theta_a[i] = theta_a_vector.at(i);

  RealRobot realRB(ur, tcp_pose, tcp_offset);

  for (int i = 1; i < 7; i++)
  {
    cout << "a:" << delta_a[i] << endl;
    cout << "d:" << delta_d[i] << endl;
    cout << "alpha:" << delta_alpha[i] << endl;
    cout << "theta:" << delta_theta[i] << endl;
  }
  realRB.setCalibrationConfig(delta_a, delta_d, delta_alpha, delta_theta);

  //解析解から実ロボットのTCPを計算し、qnearの代わりとする。
  Pose qnear = realRB.solveFK(theta_a);

  //実TCPとqnearの差からΔqを計算する。
  Vector6d delta_q;
  for (int i = 0; i < 6; i++)
    delta_q(i) = tcp_pose.toVector().at(i) - qnear.toVector().at(i);

  //ヤコビアン行列
  Matrix6d invJacobian = realRB.getInverseOfJacobian(theta_a);

  //実角度解を計算する。
  Vector6d real_theta = invJacobian * delta_q;

  vector<double> theta_vector;
  theta_vector.push_back(0);

  for (int i = 1; i < 7; i++)
    theta_vector.push_back(real_theta(i - 1));

  cout << "<Solved real solution>" << endl;

  for (int i = 1; i < 7; i++)
    cout << i << ":" << theta_vector.at(i);

  return theta_vector;
}

int Data::getPattern(double *theta)
{
  AnalysisRobot analysisRB(ur, tcp_pose, tcp_offset);

  return analysisRB.getPattern(theta);
}
