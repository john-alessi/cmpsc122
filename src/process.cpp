#include "process.h"
#include "device.h"

//  Run a Process for some time
//  The process would like to complete its current CPU burst,
//  but is currently only allowed what is granted to it.
//  Parameters:
//      clock           (modified int)  time on simulation clock
//      allowance       (input int)     time allowed to run with
//      next            (output Device) what device should be used next
//next is from { disk, net, console, cpu )
//  Post-Condition:
//      the clock will have advanced until either the allowed time
//      has been used up, or the current CPU burst is complete
//      (whichever happens earlier).  There will be no idle CPU time.
//  Also:  The history log for this Process will record what is known
//      at this time
void Process::run( int &clock, int allowance, Device *&next)
{
  addLog(clock, 'X');
  if(allowance < remainingTime) {
    clock += allowance;
    remainingTime -= allowance;
    next = &cpu;
  }
  else {
    clock += remainingTime;
    if(currentCycle < bursts) {
      next = nextRequest[currentCycle++];
      remainingTime = usages[currentCycle];
    }
    else {
      next = NULL;
    }
    /*    clock += remainingTime;
    next = nextRequest[currentCycle];
    if(next == NULL) {
      addLog(clock, 'Q');
    }
    currentCycle++;
    remainingTime = usages[currentCycle];*/
  }
  addLog(clock, '-');
  //cout << "\tRUN METHOD COMPLETE" << endl;
}

Computation::Computation( int id )
{
  myId = id;
  setType("computation");
  bursts = 4 + rand() % 10;// several lengthy CPU bursts
  for (int i=0; i < bursts; i++)
    {
      usages[i] = 200 + rand() % 120;
      nextRequest[i] = &disk;// some disk activity
    }
  nextRequest[bursts-1] = NULL;// all done!
}

Download::Download( int id )
{
  myId = id;
  setType("download");
  bursts = 15;// 4 chances to move data, then wrap up
  for (int i=0; i < bursts; i++)
    {
      usages[i] = 40 + rand() % 20;   // not much CPU needed
      if (i%2 == 0)
	nextRequest[i] = &net;// alternate network
      else
	nextRequest[i] = &disk;// and disk
    }
  nextRequest[bursts-1] = NULL;// all done!
}

Interact::Interact( int id )
{
  setType("interact");
  myId = id;
  bursts = 10;// enough to simulate till others are all done
  for (int i=0; i < bursts; i++)
    {
      usages[i] = 30 + rand() % 20;
      nextRequest[i] = &console;// work with console
    }
  nextRequest[bursts-1] = NULL;// all done!
}

Shell::Shell(int id) {
  myId = id;
  bursts = 15; //heavily affects runtime of the simulation
  for (int i=0; i < bursts; i++)
    {
      usages[i] = 30 + rand() % 20;
      nextRequest[i] = &user; //not yet implemented
    }
  nextRequest[bursts-1] = NULL;// all done!
}
