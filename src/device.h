// Device Driver Simulation
// A simplified version of the behavior of a device driver.
// It is notified of when a request arrives, and then it
// determines when that action begins and ends.

#include "proclist.h"

class Process;
class Device
{
  friend class Process; // ADDED BY ME FOR DEBUG COUT; REMOVE
protected:
  ProcList requests;
  char action;   // letter used for display
  int duration;  // how long the operation might take
  int readyTime; // when next operation may start
public:
  Device( char a, int d ) : action(a), duration(d), readyTime(0) { }
  void restart()
  {
    readyTime = 0;
  }
  virtual void request( int, int, Process* [], ProcList & );
};

class User : public Device
{
public:
  User(char a, int d) : Device(a, d) { }
  virtual void request(int, int, Process* [], ProcList &);
};

extern Device disk, net, console, cpu;
extern User user;
