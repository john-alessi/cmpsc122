#include <iostream>
using namespace std;

// List of Process ProcList Information
// This is a simple linked list, all of whose elements record
// information about a process in a simulated process scheduler.
// This list data structure will be used for three different purposes:
// -- a record of the past history of a particular process
// -- a collection for the job scheduler of processes currently wishing to run
// -- a task list for known future events for the simulation

#include "proclist.h"

// First, some helper functions to display the data

ostream &operator<<( ostream &stream, ProcListElement &ele )
{
   stream << "(" << ele.procID << "," << ele.state << "," 
	  << ele.time << ")";
   return stream;
}

ostream &operator<<( ostream &stream, ProcList &hist )
{
   for (ProcListElement *curr = hist.head; curr != NULL; curr = curr->next )
	stream << *curr;
   stream << endl;
   return stream;
}

// And some functionality for the list itself.
// First some easy iterator definitions:
ProcIterator ProcList::begin()
{
    return ProcIterator( this, head );
}

ProcIterator ProcList::end()
{
    return ProcIterator( this, NULL );
}

//  push a new element to the back end of the list
//  Primarily used for First-In, First-Out ordering
void ProcList::pushBack( int procId, int time, char state )
{
   ProcListElement *newEle = new ProcListElement( procId, time, state );

   if(empty()) {
     head = newEle;
     tail = newEle;
   }
   else {
     tail->next = newEle;
     tail = newEle;
   }
   //cout << "\tPUSHBACK METHOD COMPLETE" << endl;

}

//  remove the element at the front end of the list
//  Two reference parameters are provided to hold onto information
//  from the removed element.  The time index is considered unnecessary.
void ProcList::popFront( int &procId, char &state ) {
  ProcListElement* removal = head;
  if(empty()) {
    tail == NULL;
  }
  else {
    head = head->next;
    procId = removal->procID;
    state = removal->state;
    delete removal;
  }
}

//  adds a new element into a sorted linked list
//  which is sorted in increasing order according to the 'time' argument
void ProcList::insert( int procId, int time, char state )
{
  ProcListElement *newEle = new ProcListElement( procId, time, state );
  if (head == NULL) {
    head = newEle;
    tail = newEle;
  }
  else if (time < head->time) {
    newEle->next = head;
    head = newEle;
  }
  else if (time >= tail->time ) {
    tail->next = newEle;
    tail = newEle;
  }
  else {
    ProcListElement *it;
    for (it = head; it->next->time < time; it = it->next) { }
    newEle->next = it->next;
    it->next = newEle;
  }
   //cout << "\tINSERT METHOD COMPLETE" << endl;
}

// EXTRA CREDIT
// Condenses the information in the linked list after it is collected.
// 1) If two adjacent cells have the same time information,
//    the first can be removed (did not actually occupy that state)
// 2) If two adjacent cells have the same state recorded,
//    the second can be removed (did not actually change state)
void ProcList::condense()
//in case of duplicate times, keep last element
//in case of duplicate states, keep first element
{
/*ProcListElement *it = head;
  while(it->next != NULL && it->next->next != NULL) {
    if(it->next->time == it->next->next->time) {
      it->next = it->next->next;
    }
    if(it->state == it->next->state) {
      if(it->next == tail) {
	tail = it;
      }
      it->next = it->next->next;
    }
    it = it->next;
  }
  if(head->time == head->next->time) {
    head = head->next;
    }*/
}

//diagnostic method, not used in solution
void ProcList::dump() {
  cout << endl;
  ProcListElement *it = head;
  while(it->next != NULL) {
    cout << it->state << "\t";
    it = it->next;
  }
  cout << endl;
  it = head;
  while(it->next != NULL) {
    cout << it->time << "\t";
    it = it->next;
  }
  cout << endl;
  cin.get();
}
