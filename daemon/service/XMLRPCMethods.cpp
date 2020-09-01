#include "XMLRPCMethods.hpp"

#include <iostream>
#include "Pose.hpp"

using namespace std;

/*---------------------------
---------------------------*/
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

/*---------------------------
---------------------------*/
SetTCPPose::SetTCPPose(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetTCPPose::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> pose;
  for (int i = 0; i < 6; i++)
    pose.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  Pose tcp_pose(pose);
  data->setTCPPose(tcp_pose);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

/*---------------------------
---------------------------*/
SetTCPOffset::SetTCPOffset(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetTCPOffset::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> pose;
  for (int i = 0; i < 6; i++)
    pose.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  Pose tcp_offset(pose);
  data->setTCPOffset(tcp_offset);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

/*---------------------------
---------------------------*/
SetCalibrationConfigA::SetCalibrationConfigA(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetCalibrationConfigA::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> delta;

  for (int i = 0; i < 6; i++)
    delta.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  data->setCalibrationConfigA(delta);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

/*---------------------------
---------------------------*/
SetCalibrationConfigD::SetCalibrationConfigD(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetCalibrationConfigD::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> delta;

  for (int i = 0; i < 6; i++)
    delta.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  data->setCalibrationConfigD(delta);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

/*---------------------------
---------------------------*/
SetCalibrationConfigAlpha::SetCalibrationConfigAlpha(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetCalibrationConfigAlpha::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> delta;

  for (int i = 0; i < 6; i++)
    delta.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  data->setCalibrationConfigAlpha(delta);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

/*---------------------------
---------------------------*/
SetCalibrationConfigTheta::SetCalibrationConfigTheta(Data *data) : data(data)
{
  this->_signature = "s:dddddd";
}

void SetCalibrationConfigTheta::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> delta;

  for (int i = 0; i < 6; i++)
    delta.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  data->setCalibrationConfigTheta(delta);

  *retvalP = xmlrpc_c::value_string("done"); // XML-RPC void return values are an extension to the protocol and not always available or compatible between languages.
}

/*---------------------------
---------------------------*/
GetAnalysisAngle::GetAnalysisAngle(Data *data) : data(data)
{
  this->_signature = "A:i"; // RPC method signature, which is not mandatory for basic operation, see http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html#howto
}

void GetAnalysisAngle::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  int const num(paramList.getInt(0));
  paramList.verifyEnd(1);

  vector<double> theta = data->getAnalysisAngle(num);

  std::vector<xmlrpc_c::value> res_theta;

  for (int i = 0; i < 6; i++)
    res_theta.push_back(xmlrpc_c::value_double(theta[i]));

  *retvalP = xmlrpc_c::value_array(res_theta);
}

/*---------------------------
---------------------------*/
GetRealAngle::GetRealAngle(Data *data) : data(data)
{
  this->_signature = "A:i"; // RPC method signature, which is not mandatory for basic operation, see http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html#howto
}

void GetRealAngle::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  int const num(paramList.getInt(0));
  paramList.verifyEnd(1);

  vector<double> theta = data->getRealAngle(num);

  std::vector<xmlrpc_c::value> res_theta;

  for (int i = 0; i < 6; i++)
    res_theta.push_back(xmlrpc_c::value_double(theta[i]));

  *retvalP = xmlrpc_c::value_array(res_theta);
}

/*---------------------------
---------------------------*/
GetPattern::GetPattern(Data *data) : data(data)
{
  this->_signature = "i:dddddd"; // RPC method signature, which is not mandatory for basic operation, see http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html#howto
}

void GetPattern::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> theta;

  for (int i = 0; i < 6; i++)
    theta.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  *retvalP = xmlrpc_c::value_int(data->getPattern(theta));
}

/*---------------------------
---------------------------*/
GetPose::GetPose(Data *data) : data(data)
{
  this->_signature = "A:dddddd"; // RPC method signature, which is not mandatory for basic operation, see http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html#howto
}

void GetPose::execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP)
{
  vector<double> theta;

  for (int i = 0; i < 6; i++)
    theta.push_back(paramList.getDouble(i));

  paramList.verifyEnd(6);

  vector<double> pose = data->getPose(theta);

  std::vector<xmlrpc_c::value> res_pose;

  for (int i = 0; i < 6; i++)
    res_pose.push_back(xmlrpc_c::value_double(pose[i]));

  *retvalP = xmlrpc_c::value_array(res_pose);
}
