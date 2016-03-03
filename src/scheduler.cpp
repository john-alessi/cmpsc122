#include "scheduler.h"

const int DISKTIME = 200;		// how long a disk action requires

//  Request to access the disk (secondary storage)
//  A process may occasionally need to access a file or the
//  virtual memory system.  The disk may only service one
//  process at a time, and its readiness will be simulated
//  with a time index also.
//  Parameters:
//  	pid  	  (input int)		process id (subscript to tasks[])
//  	clock	  (input int)		overall simulation clock time
//  	diskReady (modified int)	when the disk is available for use
//	tasks	  (Process array)	given to update process history
//	future	  (modified ProcList)	a process may proceed after using disk
//

void Scheduler::diskRequest( int pid, int clock, int &diskReady, Process tasks[], ProcList &future )
{
  if(clock < diskReady) {
    //tasks[pid].addLog(clock, '-');
    tasks[pid].addLog(diskReady, 'D');
    tasks[pid].addLog(diskReady + DISKTIME, '-');
    diskReady += DISKTIME;
  }
  else {
    tasks[pid].addLog(clock, 'D');
    tasks[pid].addLog(clock + DISKTIME, '-');
    diskReady = clock + DISKTIME;
  }
  future.insert(pid, diskReady, 'X');
}


//  Scheduler Simulation
//  Simulates a process scheduler for a collecdtion of tasks
//  Parameters:
//  	tasks		(modified process array)description of tasks to run
//  	arrival		(input int array)	arrival in simulation time
//  	size		(input int)		number of elements in arrays
//  	allowance	(input int)		maximal CPU allowance 
//		(a large 'allowance' simulates First Come First Served)
//  Each Process includes a history of its activity which is
//  pupulated during the simulation and displayed afterwards.
//  
//  The scheduler includes a readySet of processes that are currently
//  wishing to use the CPU, and a future list of events that will
//  occur later in the simulation.  A simulation clock advances to
//  identify which events occur in which order.
void Scheduler::runScheduler( Process tasks[], int arrival[], int size, int allowance )
{ 
    int pid;			// process wanting action (same as process's index in tasks[])
    char nextAct;		// and the action it wants
    int diskReady = 0;
    
    for (int i=0; i < size; i++)
    {
	future.insert( i, arrival[i], 'X');	// all want to run
	tasks[i].restart();			// and start at beginning
	tasks[i].addLog( arrival[i], '-');	// might have to wait
    }

    clock = 0;			// initialize simulation clock
    //  repeat while anything is ready to run now or later

    
    while ( !noneReady() || !future.empty() )
    {
      //if any future process is "ready", move it to the ready set
      if(!future.empty() && future.leadTime() <= clock) {
	future.popFront(pid, nextAct);
	addProcess(pid);
      }
      //choose process from ready set and run it
      if(!noneReady()) {
	chooseProcess(pid);//segfaulting when readyset is empty
	tasks[pid].run(clock, allowance, nextAct);
	if(nextAct == 'D') {
	  diskRequest(pid, clock, diskReady, tasks, future);
	}
	else if(nextAct != 'Q') {
	  addProcess(pid);
	}
      }
      else {
	clock = future.leadTime();
      }
    }
    //cout << "\tRUNSCHEDULER METHOD COMPLETE" << endl;
}
