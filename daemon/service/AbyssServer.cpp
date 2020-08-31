#include "AbyssServer.hpp"

#include <memory>
#ifndef PSTREAM
#include <xmlrpc-c/server_abyss.hpp>
#endif

#include "XMLRPCMethods.hpp"

using namespace std;

// Please use a unique port number!
#define SERVER_PORT 56000

AbyssServer::AbyssServer(Data *data) : data(data)
{
  // Add all adapters defined in XMLRPCMethods here
  serviceRegistry.addMethod("set_ur_type", new SetURType(data));
  serviceRegistry.addMethod("set_tcp_pose", new SetTCPPose(data));
  serviceRegistry.addMethod("set_tcp_offset", new SetTCPOffset(data));
  serviceRegistry.addMethod("set_calibration_a", new SetCalibrationConfigA(data));
  serviceRegistry.addMethod("set_calibration_d", new SetCalibrationConfigD(data));
  serviceRegistry.addMethod("set_calibration_alpha", new SetCalibrationConfigAlpha(data));
  serviceRegistry.addMethod("set_calibration_theta", new SetCalibrationConfigTheta(data));
  serviceRegistry.addMethod("get_analysis_angle", new GetAnalysisAngle(data));
  serviceRegistry.addMethod("get_real_angle", new GetRealAngle(data));
  serviceRegistry.addMethod("get_pattern", new GetPattern(data));
  serviceRegistry.addMethod("get_pose", new GetPose(data));
}

AbyssServer::~AbyssServer()
{
}

void *AbyssServer::run(void *aserver)
{

  AbyssServer *ptr = static_cast<AbyssServer *>(aserver);

  xmlrpc_c::serverAbyss server(
      xmlrpc_c::serverAbyss::constrOpt().portNumber(SERVER_PORT).registryP(&(ptr->serviceRegistry)));

  server.run();

  return NULL;
}
