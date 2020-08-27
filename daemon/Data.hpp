#pragma once

#include <iostream>
using namespace std;

/**
 * @brief Simple example data container
 * A class like this should be used to implement your domain logic, e.g. a driver.
 */
class Data
{
public:
  Data();
  ~Data();

  void setURType(string type);
  void setPose(double x, double y, double z, double rx, double ry, double rz);
  void setOffset(double x, double y, double z, double rx, double ry, double rz);
  void setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta);
  double *getAngles(int num);
  int getPattern(double *angles);
  double *getPose(double *theta);

private:
  double x,
      y,
      z,
      rx,
      ry,
      rz;
  double offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz;
  double angle[6];
  double delta_a[7], delta_d[7], delta_alpha[7], delta_theta[7];
  int ur;
};
