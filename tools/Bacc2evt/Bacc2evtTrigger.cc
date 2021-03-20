#include <vector>
#include <iostream>
#include <math.h>

#include "Bacc2evtTrigger.hh"
#include "Bacc2evtMethods.hh"

using namespace std;

// Constructors
Bacc2evtTrigger::Bacc2evtTrigger() {}
// Destructors
Bacc2evtTrigger::~Bacc2evtTrigger() {
}

// "Get" functions.
//unsigned int Bacc2evtTrigger::GetPulseNumber () {return pulse_number;}

// "Set" functions.
//void Bacc2evtTrigger::SetPulseNumber (unsigned int pn) {pulse_number=pn;}


// The good stuff

//============================================================================
//            Bacc2evtTrigger::SOMEFUNC
//============================================================================
//void Bacc2evtTrigger::SOMEFUNC() {
//}


//============================================================================
//            Bacc2evtTrigger::Phase1Trigger
//============================================================================
bool Bacc2evtTrigger::Phase1Trigger(
        vector< vector<double> > & timingInfoVecEvent, unsigned int event) {
  // This is where the trigger logic goes. This is a basic trigger that says
  // at least two pmts must have light above 3 phe.
  if(!trigger_active)
    return true;
  int num_of_triggered_pmts = 0;
  int majority_thresh = 2;
  unsigned int phe_thresh = 3;
  for(unsigned int pmt=0; pmt<122; pmt++) {
    if(timingInfoVecEvent[pmt].size() > phe_thresh) {
      num_of_triggered_pmts++;
    }
    if(num_of_triggered_pmts > majority_thresh)
      return true;
  }
  // Keep a record of which events were dropped.
  dropped_events.push_back(event);
  // Clear the vector that has the photon arrival times.
  for(unsigned int pmt=0; pmt<122; pmt++) {
    timingInfoVecEvent[pmt].clear();
  }
  return false;

}


//============================================================================
//            Bacc2evtTrigger::Phase2Trigger
//============================================================================
bool Bacc2evtTrigger::Phase2Trigger(
     vector< vector<Bacc2evtPulse*> > & pod_pulseEvent, unsigned int event) {

  if(!trigger_active)
    return true;

  // This basic trigger looks at having light in at least 1 pmt. It doesn't
  // do anything for now, but is just a framework.
  for(unsigned int pmt=0; pmt<122; pmt++) {
    if (pod_pulseEvent[pmt].size() >=1) {
      return true;
    }
    for(unsigned int pod=0; pod<pod_pulseEvent[pmt].size(); pod++) { }
  }
  // Keep a record of which events were dropped.
  dropped_events.push_back(event);
  // Clear the vector that has the photon arrival times.
  for(unsigned int pmt=0; pmt<122; pmt++) {
    for(unsigned int pod=0; pod<pod_pulseEvent[pmt].size(); pod++) {
      pod_pulseEvent[pmt][pod]->data.clear();
      pod_pulseEvent[pmt][pod]->SetPulseLength(0);
    }
  }
  return false;

}

//============================================================================
//            Bacc2evtTrigger::GetDroppedEvents
//============================================================================
vector<int> Bacc2evtTrigger::GetDroppedEvents() {
  return dropped_events;
}

//============================================================================
//            Bacc2evtTrigger::EnableTrigger
//============================================================================
void Bacc2evtTrigger::EnableTrigger() { trigger_active = true; }

//============================================================================
//            Bacc2evtTrigger::DisableTrigger
//============================================================================
void Bacc2evtTrigger::DisableTrigger() { trigger_active = false; }
