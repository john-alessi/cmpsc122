// A quick driver to test the History Display routine
// It just fabricates a single job history to see what it does
//
#include <iostream>
using namespace std;

#include "scheduler.h"
#include "device.h"

int main()
{
   Scheduler *scheds[] = 	    // make some process schedulers
     { new FCFS(), new RoundRobin(), new Priority(), new Preempt(), new ShortestRemainingTime() };

   Process *tasks[20];
   int arrival[20];
   tasks[0] = new Shell(0);
   arrival[0] = 0;
 
   for (int i=0; i<5; i++)//runs simulation for each scheduler type
   {
	scheds[i]->runScheduler( tasks, arrival, 1 );
	displayHistory( tasks, 1, 0, 5000 );
   }
}

