#pragma once

#include <iostream>
#include "pose.hpp"
#include "analysisrobot.hpp"
#include "realrobot.hpp"
using namespace std;

/**
 * @brief Simple example data container
 * A class like this should be used to implement your domain logic, e.g. a driver.
 */
class Data
{
private:
  int ur;
  AnalysisRobot *analysisRB;
  RealRobot *realRB;
  Pose const *tcp_pose;
  Pose const *tcp_offset;

  double const *delta_a;
  double const *delta_d;
  double const *delta_alpha;
  double const *delta_theta;

public:
  Data();
  ~Data();

  void setURType(string type);
  void setTCPPose(Pose const *tcp_pose);
  void setTCPOffset(Pose const *tcp_offset);
  void setCalibrationConfig(double const *delta_a, double const *delta_d, double const *delta_alpha, double const *delta_theta);
  double *getAnalysisAngle(int num);
  double *getRealAngle(int num);

  int getPattern(double *angles);
  double *getPose(double *theta);
};
