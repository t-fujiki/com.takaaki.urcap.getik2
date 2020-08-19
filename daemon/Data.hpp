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
  double *getAngles(int num);
  int getPattern(double *angles);

private:
  double x, y, z, rx, ry, rz;
  double offset_x, offset_y, offset_z, offset_rx, offset_ry, offset_rz;
  double angle[6];
  int ur;
};
