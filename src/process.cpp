#include "process.h"

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

Computation::Computation( int id )
{
  myId = id;
  bursts = 4 + rand() % 3;// several lengthy CPU bursts
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
  bursts = 9;// 4 chances to move data, then wrap up
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
  myId = id;
  bursts = 4;// enough to simulate till others are all done
  for (int i=0; i < bursts; i++)
    {
      usages[i] = 30 + rand() % 20;
      nextRequest[i] = &console;// work with console
    }
  nextRequest[bursts-1] = NULL;// all done!
}
