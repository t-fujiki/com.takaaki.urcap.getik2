#include "XMLRPCMethods.hpp"

#include <iostream>

using namespace std;

//------
SetURType::SetURType(Data *data) : data(data)
{
  this->_signature = "s:s";
}

void SetURType::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  string const urtype(paramList.getString(0));
  paramList.verifyEnd(1);

  data->setURType(urtype);

  *retvalP = xmlrpc_c::value_string(urtype); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

//------
SetPose::SetPose(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetPose::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  double const x(paramList.getDouble(0));
  double const y(paramList.getDouble(1));
  double const z(paramList.getDouble(2));
  double const rx(paramList.getDouble(3));
  double const ry(paramList.getDouble(4));
  double const rz(paramList.getDouble(5));
  paramList.verifyEnd(6);

  data->setPose(x, y, z, rx, ry, rz);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

//------
SetOffset::SetOffset(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetOffset::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  double const x(paramList.getDouble(0));
  double const y(paramList.getDouble(1));
  double const z(paramList.getDouble(2));
  double const rx(paramList.getDouble(3));
  double const ry(paramList.getDouble(4));
  double const rz(paramList.getDouble(5));
  paramList.verifyEnd(6);

  data->setOffset(x, y, z, rx, ry, rz);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

//------
SetCalibration::SetCalibration(Data *data) : data(data)
{
  this->_signature = "s:dddddddddddddddddddddddd";
}

void SetCalibration::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  double delta_a[7], delta_d[7], delta_alpha[7], delta_theta[7];

  for (int i = 1; i < 7; i++)
    delta_a[i] = paramList.getDouble(i - 1);

  for (int i = 1; i < 7; i++)
    delta_d[i] = paramList.getDouble(i - 1 + 6);

  for (int i = 1; i < 7; i++)
    delta_alpha[i] = paramList.getDouble(i - 1 + 12);

  for (int i = 1; i < 7; i++)
    delta_theta[i] = paramList.getDouble(i - 1 + 18);

  paramList.verifyEnd(24);

  data->setCalibration(delta_a, delta_d, delta_alpha, delta_theta);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

//------
GetAngles::GetAngles(Data *data) : data(data)
{
  this->_signature = "A:i"; // RPC method signature, which is not mandatory for basic operation, see http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html#howto
}

void GetAngles::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  int const num(paramList.getInt(0));
  paramList.verifyEnd(1);

  double *theta = data->getAngles(num);

  std::vector<xmlrpc_c::value> theta_vector;

  for (int i = 0; i < 6; i++)
    theta_vector.push_back(xmlrpc_c::value_double(theta[i]));

  *retvalP = xmlrpc_c::value_array(theta_vector);
}

GetPattern::GetPattern(Data *data) : data(data)
{
  this->_signature = "i:dddddd"; // RPC method signature, which is not mandatory for basic operation, see http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html#howto
}

void GetPattern::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  double pose[6];

  for (int i = 0; i < 6; i++)
    pose[i] = paramList.getDouble(i);

  paramList.verifyEnd(6);

  data->getPattern(pose);

  *retvalP = xmlrpc_c::value_int(data->getPattern(pose));
}

GetPose::GetPose(Data *data) : data(data)
{
  this->_signature = "A:dddddd"; // RPC method signature, which is not mandatory for basic operation, see http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html#howto
}

void GetPose::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  double theta[6];
  ;
  for (int i = 0; i < 6; i++)
    theta[i] = paramList.getDouble(i);

  paramList.verifyEnd(6);

  double *pose = data->getPose(theta);

  std::vector<xmlrpc_c::value> pose_vector;

  for (int i = 0; i < 6; i++)
    pose_vector.push_back(xmlrpc_c::value_double(pose[i]));

  *retvalP = xmlrpc_c::value_array(pose_vector);
}