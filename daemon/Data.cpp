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

void Data::setCalibrationConfigA(vector<double> delta_a)
{
  this->delta_a = delta_a;

  cout << "<Delta A>" << endl;
  for (int i = 1; i < 7; i++)
    cout << i + 1 << ":" << this->delta_a[i] << endl;
}

void Data::setCalibrationConfigD(vector<double> delta_d)
{
  this->delta_d = delta_d;

  cout << "<Delta D>" << endl;
  for (int i = 1; i < 7; i++)
    cout << i + 1 << ":" << this->delta_d[i] << endl;
}

void Data::setCalibrationConfigAlpha(vector<double> delta_alpha)
{
  this->delta_alpha = delta_alpha;

  cout << "<Delta Alpha>" << endl;
  for (int i = 1; i < 7; i++)
    cout << i + 1 << ":" << this->delta_alpha[i] << endl;
}

void Data::setCalibrationConfigTheta(vector<double> delta_theta)
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
  vector<double> theta = getAnalysisAngle(num);

  RealRobot realRB(ur, tcp_pose, tcp_offset);

  //キャリブレーションデータをセットする。
  realRB.setCalibrationConfig(delta_a, delta_d, delta_alpha, delta_theta);

  //解析解から実ロボットのTCPを計算し、qnearの代わりとする。
  Pose qnear = realRB.solveFK(theta);

  cout << "<Solved qnear TCP>" << endl;
  cout << "X:" << qnear.x << endl;
  cout << "Y:" << qnear.y << endl;
  cout << "Z:" << qnear.z << endl;
  cout << "Rx:" << qnear.rx << endl;
  cout << "Ry:" << qnear.ry << endl;
  cout << "Rz:" << qnear.rz << endl;

  //実TCPとqnearの差からΔqを計算する。
  Vector6d delta_q;
  for (int i = 0; i < 6; i++)
    delta_q(i) = tcp_pose.toVector()[i] - qnear.toVector()[i];

  //ヤコビアン行列
  Matrix6d invJacobian = realRB.getInverseOfJacobian(theta);

  //実角度解を計算する。
  Vector6d d_theta = invJacobian * delta_q;

  for (int i = 1; i < 7; i++)
    theta[i] += d_theta(i - 1);

  cout << "<Solved real solution>" << endl;

  for (int i = 1; i < 7; i++)
    cout << i << ":" << theta[i] << endl;

  return theta;
}

int Data::getPattern(vector<double> theta)
{
  AnalysisRobot analysisRB(ur, tcp_pose, tcp_offset);

  return analysisRB.getPattern(theta);
}
