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
  serviceRegistry.addMethod("set_pose", new SetPose(data));
  serviceRegistry.addMethod("set_offset", new SetOffset(data));
  serviceRegistry.addMethod("get_angles", new GetAngles(data));
  serviceRegistry.addMethod("get_pattern", new GetPattern(data));
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
