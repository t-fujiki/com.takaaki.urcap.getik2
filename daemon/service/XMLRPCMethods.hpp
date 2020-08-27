#pragma once

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>

#include "Data.hpp"

/**
 * Add a XMLRPC adapter (i.e. Hello) for each method that should be exposed through XML-RPC.
 * Relevant XMLRPC-C documentation can be found here: http://xmlrpc-c.sourceforge.net/doc/libxmlrpc_server++.html
 */

class SetURType : public xmlrpc_c::method
{
public:
  SetURType(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetURType(); // Hereby disabled
  Data *data;
};

class SetPose : public xmlrpc_c::method
{
public:
  SetPose(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetPose(); // Hereby disabled
  Data *data;
};

class SetOffset : public xmlrpc_c::method
{
public:
  SetOffset(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetOffset(); // Hereby disabled
  Data *data;
};

class SetCalibration : public xmlrpc_c::method
{
public:
  SetCalibration(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  SetCalibration(); // Hereby disabled
  Data *data;
};

class GetAngles : public xmlrpc_c::method
{
public:
  GetAngles(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  GetAngles(); // Hereby disabled
  Data *data;
};

class GetPattern : public xmlrpc_c::method
{
public:
  GetPattern(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  GetPattern(); // Hereby disabled
  Data *data;
};

class GetPose : public xmlrpc_c::method
{
public:
  GetPose(Data *data);
  void execute(xmlrpc_c::paramList const &paramList, xmlrpc_c::value *const retvalP);

private:
  GetPose(); // Hereby disabled
  Data *data;
};
