#ifndef Bacc2EVTTRIGGER_H
#define Bacc2EVTTRIGGER_H 1

#include "Bacc2evtPulse.hh"

using namespace std;

class Bacc2evtTrigger {
  public:
    //double something;
    vector<int> dropped_events;
    bool trigger_active;

    Bacc2evtTrigger();
    ~Bacc2evtTrigger();

    //unsigned int GetPulseNumber ();

    //void SetPulseNumber (unsigned int pn); 
    bool Phase1Trigger(vector< vector< double> > & timingInfoVecEvent, unsigned int event);
    bool Phase2Trigger(vector< vector<Bacc2evtPulse*> > & pod_pulseEvent, unsigned int event);

    vector<int> GetDroppedEvents();
    void EnableTrigger();
    void DisableTrigger();

};

#endif

