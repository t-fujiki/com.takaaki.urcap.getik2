#include "GetIK.hpp"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "AbyssServer.hpp"
#include "Data.hpp"

using namespace std;

bool GetIK::quit = false;
int GetIK::exit_value = 0;

// called at exit
void GetIK::shutdown()
{
  cerr << "GetIK::shutdown" << endl;
}

// Handle various signals
void GetIK::handler(int signum)
{

  if (signum == SIGINT)
  {
    cerr << "GetIK::handler caught CTRL-C" << endl;
    GetIK::quit = true;
  }
  else
  {
    cerr << "GetIK::handler caught signal: " << signum << endl;
  }
}

int main(int argc, char *argv[])
{

  // call handler() at CTRL-C (signum = SIGINT)
  if (signal(SIGINT, SIG_IGN) != SIG_IGN)
  {
    signal(SIGINT, GetIK::handler);
  }

  // call shutdown() at program exit
  atexit(GetIK::shutdown);

  // Example data container for domain logic
  Data data;

  // To communicate between URScript and the executable we use the xmlrpc-c library
  // This library is standard available on the robot and in the development toolchain.
  AbyssServer gui(&data);

  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, gui.run, &gui))
  {
    cerr << "Couldn't create pthread" << endl;
    return EXIT_FAILURE;
  }

  cout << "GetIK started" << endl;

  pthread_join(thread_id, NULL);

  cout << "GetIK stopped" << endl;

  return GetIK::exit_value;
}
