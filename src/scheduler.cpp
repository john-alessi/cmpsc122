#include "scheduler.h"
#include "device.h"

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
void Scheduler::runScheduler( Process* tasks[], int arrival[], int size)
{
  cout << name << endl;
    int pid;			// process wanting action (same as process's index in tasks[])
    char nextAct;		// and the action it wants
    int diskReady = 0;

    Device* nextDevice;
    
    for (int i=0; i < size; i++)
    {
	future.insert( i, arrival[i], 'X');	// all want to run
	tasks[i]->restart();			// and start at beginning
	tasks[i]->addLog( arrival[i], '-');	// might have to wait
	disk.restart();
	net.restart();
	console.restart();
	cpu.restart();
    }

    clock = 0;			// initialize simulation clock
    //  repeat while anything is ready to run now or later

    while ( !noneReady() || !future.empty() )
    {
      //if any future process is "ready", move it to the ready set
      while(!future.empty() && future.leadTime() <= clock) {
	future.popFront(pid, nextAct);
	addProcess(pid);
      }
      //choose process from ready set and run it
      if(!noneReady()) {
	chooseProcess(pid);
	tasks[pid]->run(clock, allowance(), nextDevice);
	if(nextDevice != NULL) {
	  nextDevice->request(pid, clock, tasks, future);
	}
      }
      else {
	clock = future.leadTime();
      }
    }
    //cout << "\tRUNSCHEDULER METHOD COMPLETE" << endl;
}

Heap::Heap(int size, Process** procs) {
  tasks = procs;
  values = new int[size + 1];
  nextEmpty = 1;
}

void Heap::push(int value) {
  int position = nextEmpty;
  while(position > 1 && tasks[value]->getRemainingTime() < tasks[values[position/2]]->getRemainingTime()) {
    values[position] = values[position/2];
    position /= 2;
  }
  values[position] = value;
  nextEmpty++;
}

int Heap::pop() {
  int value = values[1];
  bool done = false;
  int position = 1;
  nextEmpty--;
  values[position] = values[nextEmpty];
  int child = 2;
  while(child < nextEmpty && !done) {
    if(child + 1 < nextEmpty && values[child+1] < values[child]) {
      child++;
    }
    if(values[position] < values[child]) {
      done = true;
    }
    else {
      int tmp = values[child];
      values[child] = values[position];
      values[position] = tmp;
      //swap values[child] and values[position];
      position = child;
      child = 2*position;
    }
  }
}

bool Heap::isEmpty() {
  return nextEmpty == 1;
}
