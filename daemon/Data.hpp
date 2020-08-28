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

  double *delta_a;
  double *delta_d;
  double *delta_alpha;
  double *delta_theta;

public:
  Data();
  ~Data();

  void setURType(string type);
  void setTCPPose(Pose tcp_pose);
  void setTCPOffset(Pose tcp_offset);
  void setCalibrationConfigA(double *delta_a);
  void setCalibrationConfigD(double *delta_d);
  void setCalibrationConfigAlpha(double *delta_alpha);
  void setCalibrationConfigTheta(double *delta_theta);
  vector<double> getAnalysisAngle(int num);
  vector<double> getRealAngle(int num);
  int getPattern(double *theta);
};
