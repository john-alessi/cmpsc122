#include "process.h"

//  Process Constructor
//  Initalizes a process as a single burst of CPU usage,
//  which may or may not be allowed to run all at once.
//  When that CPU time is complete, so is the process.

Process::Process( int id )  // a constructor
{
    myId = id;
    bursts = 4 + rand() % 3;    // several CPU bursts
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 80 + rand() % 120;
        nextState[i] = 'D';     // some disk activity
    }
    nextState[bursts-1] = 'Q';  // all done!
}

//  Run a Process for some time
//  The process would like to complete its current CPU burst,
//  but is currently only allowed what is granted to it.
//  Parameters:
//  	clock		(modified int)	time on simulation clock
//  	allowance	(input int)	time allowed to run with
//	next		(output char)	the process next state after this
//		'Q' = complete;  'X' = wishes more CPU;  'D' == wishes disk
//  Post-Condition:
//  	the clock will have advanced until either the allowed time
//  	has been used up, or the current CPU burst is complete
//  	(whichever happens earlier).  There will be no idle CPU time.
//  Also:  The history log for this Process will record what is known
//  	at this time
void Process::run( int &clock, int allowance, char &next )
{
  addLog(clock, 'X');
  if(allowance < remainingTime) {
    clock += allowance;
    remainingTime -= allowance;
    next = 'X';
  }
  else {
    clock += remainingTime;
    next = nextState[currentCycle];
    if(next == 'Q') {
      addLog(clock, 'Q');
    }
    currentCycle++;
    remainingTime = usages[currentCycle];
  }
  addLog(clock, '-');
  //cout << "\tRUN METHOD COMPLETE" << endl;
}

