#pragma once

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>

#include "Data.hpp"

/**
 * Add a XMLRPC adapter (i.e. Hello) for each method that should be exposed through XML-RPC.
 * Relevant XMLRPC-C documentation can be found here: http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html
 */

/*---------------------------
---------------------------*/
class SetURType : public xmlrpc_c::method
{
public:
  SetURType(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetURType(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class SetTCPPose : public xmlrpc_c::method
{
public:
  SetTCPPose(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetTCPPose(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class SetTCPOffset : public xmlrpc_c::method
{
public:
  SetTCPOffset(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetTCPOffset(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class SetCalibrationConfigA : public xmlrpc_c::method
{
public:
  SetCalibrationConfigA(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetCalibrationConfigA(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class SetCalibrationConfigD : public xmlrpc_c::method
{
public:
  SetCalibrationConfigD(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetCalibrationConfigD(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class SetCalibrationConfigAlpha : public xmlrpc_c::method
{
public:
  SetCalibrationConfigAlpha(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetCalibrationConfigAlpha(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class SetCalibrationConfigTheta : public xmlrpc_c::method
{
public:
  SetCalibrationConfigTheta(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetCalibrationConfigTheta(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class GetAnalysisAngle : public xmlrpc_c::method
{
public:
  GetAnalysisAngle(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  GetAnalysisAngle(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class GetRealAngle : public xmlrpc_c::method
{
public:
  GetRealAngle(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  GetRealAngle(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/

class GetPattern : public xmlrpc_c::method
{
public:
  GetPattern(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  GetPattern(); // Hereby disabled
  Data *data;
};

/*---------------------------
---------------------------*/
class GetPose : public xmlrpc_c::method
{
public:
  GetPose(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  GetPose(); // Hereby disabled
  Data *data;
};
