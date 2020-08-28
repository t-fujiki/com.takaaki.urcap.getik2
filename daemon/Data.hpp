#pragma once

#include <iostream>
#include "analysisrobot.hpp"
#include "realrobot.hpp"
using namespace std;

typedef Matrix<double, 6, 1> Vector6d;

/**
 * @brief Simple example data container
 * A class like this should be used to implement your domain logic, e.g. a driver.
 */
class Data
{
private:
  int ur;
  Pose tcp_pose;
  Pose tcp_offset;

  vector<double> delta_a;
  vector<double> delta_d;
  vector<double> delta_alpha;
  vector<double> delta_theta;

public:
  Data();
  ~Data();

  void setURType(string type);
  void setTCPPose(Pose tcp_pose);
  void setTCPOffset(Pose tcp_offset);
  void setCalibrationConfigA(vector<double> delta_a);
  void setCalibrationConfigD(vector<double> delta_d);
  void setCalibrationConfigAlpha(vector<double> delta_alpha);
  void setCalibrationConfigTheta(vector<double> delta_theta);
  vector<double> getAnalysisAngle(int num);
  vector<double> getRealAngle(int num);
  int getPattern(vector<double> theta);
};
