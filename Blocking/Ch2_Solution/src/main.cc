#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>
#include "AlarmSystem.hh"

std::function<void()> timeout;

void sigalrm_handler(int signal) {
  timeout();
}

int
main(int argc, char* argv[])
{
  dzn::locator loc;
  dzn::runtime rt;

  loc.set(rt);

  AlarmSystem as(loc);
  as.dzn_meta.name = "AlarmSystem";

  as.iTimer.in.start = [] (int ms) { alarm(ms/1000); }; //ITimer contract is in milliseconds, but alarm is in seconds
  as.iTimer.in.cancel = [&] () { alarm(0); as.iTimer.out.cancelled(); };
  timeout = as.iTimer.out.timeout;
  signal(SIGALRM, sigalrm_handler);

  as.check_bindings();

  std::string input;
  while(std::cin >> input) {
    as.iController.in.passwordEntered(input);
  }
}
