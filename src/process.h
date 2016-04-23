#include "proclist.h"
#include <stdlib.h>

// A description of a process, executed in the process scheduling simulation
class Device;
class Process
{
protected:
  int myId; // an identifier for the larger picture
  string type;
  void setType(string s) {
    type = s;
  }

  int bursts; // total number of CPU bursts (<= 10)
  int usages[10]; // lengths of each burst
  Device *nextRequest[10]; // what to do after each burst

  int currentCycle; // which burst is next ro run or continue
  int remainingTime; // how much time left on current burst

  ProcList log;
public:

  string getType() {
    return type;
  }
  
  void restart() // start at the very beginning
  {
    currentCycle = 0;
    remainingTime = usages[0];
    log.clear(); // empty the log
  }

  void addLog( int time, char state )
  {
    log.pushBack( myId, time, state );
  }

  ProcList &getLog()
  {
    return log; // get summarized results at end
  }

  virtual bool isInteractive()
  {
    return false; // assume a background job
  }

  int getProcId() {
    return myId;
  }

  int getRemainingTime() {
    return remainingTime;
  }

  void run( int &, int, Device *& );
};

class Computation : public Process
{
public:
  Computation( int id );
};

class Download : public Process
{
public:
  Download( int id );
};

class Interact : public Process
{
public:
  Interact( int id );
  bool isInteractive()
  {
    return true;
  }
};	

class Shell : public Process//rework to extend Interact instead of Process
{
public:
  Shell(int id);
  bool isInteractive()
  {
    return true;
  }
};

