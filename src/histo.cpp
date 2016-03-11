// History Displayer
// Produces an Ascii-graphic representation of the history of a process
// within a simulation of job scheduling.  The history is recorded as a
// series of time stamps, represented here as integer time values and
// characters representing state.
// Paramaters:
//     state	input char array	what state this job changed to
//     times	input int array		when this job changed state
//     start	input int		beginning of time frame of interest
//     stop	input int		end of time frame of interest
// Pre-Conditions:
//     'state' consists of printable characters, with 'Q' marking end of job
//     'times' consists of increasing positive integers
//     the number of meaningful values in both arrays is equal (including 'Q')
//     'start' and 'stop' are both positive integers, with start < stop
//          and ( stop-start >= 20 )
//     NOTE:  'start' or 'stop' may have values outside the range in 'when'
// Results:
//     A display of between 20 and 50 printable characters representing
//     the job history within the time range of interest, using the
//     characters stored within 'state'.  Events outside the actual range
//     of the job history will be displayed with blanks.

#define NULL 0

#include <iostream>
#include <math.h>

using namespace std;

#include "histo.h"

void displayHistory(Process* history[], int size, int start, int stop) {
  int range = stop - start;
  float charScale = range/50.0;
  for(int i = 0; i < size; i++) {
    for(float t = (float)start; t <= stop; t+= charScale) {
      if(t < history[i]->getLog().leadTime()) {
	cout << " ";
      }
      else {
	ProcIterator it = history[i]->getLog().begin();
	char prev = history[i]->getLog().begin().state();
	while (it != history[i]->getLog().end() && t > it.time()) {
	  prev = it.state();
	  it.advance();
	}
	cout << prev;
      }
    }
    cout << endl;
  }

  float avgTurnaround = 0;
  int nonInteractiveTasks = 0;
  for(int i = 0; i < size; i++) {
    if(history[i]->isInteractive()) {
      avgTurnaround += (history[i]->getLog().tailTime() - history[i]->getLog().leadTime());///
      nonInteractiveTasks++;
    }
  }
  avgTurnaround /= nonInteractiveTasks;

  int responses = 0;
  float avgResponse = 0;
  int responseStart;
  int maxResponse = 0;
  char prev;
  for(int i = 0; i < size; i++) {
    if(history[i]->isInteractive()) {
      ProcIterator it = history[i]->getLog().begin();
      while(it.state() != 'I') {
	it.advance();
      }
      prev = it.state();
      it.advance();
      while(it != history[i]->getLog().end()) {
	if(prev == 'I') {
	  responseStart = it.time();
	}
	else if(it.state() == 'I') {
	  responses++;
	  avgResponse += (it.time() - responseStart);
	  if(maxResponse < it.time() - responseStart) {
	    maxResponse = it.time() - responseStart;
	  }
	}
	prev = it.state();
	it.advance();
      }
    }
  }
  avgResponse /= responses;

  float variance = 0;
  float stdDev = 0;
  for(int i = 0; i < size; i++) {
    if(history[i]->isInteractive()) {
      ProcIterator it = history[i]->getLog().begin();
      while(it.state() != 'I') {
	it.advance();
      }
      prev = it.state();
      it.advance();
      while(it != history[i]->getLog().end()) {
	if(prev == 'I') {
	  responseStart = it.time();
	}
	else if(it.state() == 'I') {
	  variance += (avgResponse - (it.time() - responseStart))*(avgResponse - (it.time() - responseStart));
	}
	prev = it.state();
	it.advance();
      }
    }
  }
  variance /= responses;
  stdDev = sqrt(variance);
  
  cout << "Average turnaround time:\t\t" << avgTurnaround << endl;
  cout << "Average response time:\t\t\t" << avgResponse << endl;
  cout << "Highest response time:\t\t\t" << maxResponse << endl;
  cout << "Standard deviation of response times:\t" << stdDev << endl;
  cout << endl;
  //cout << "\tDISPLAY METHOD COMPLETE" << endl;
}
