////////////////////////////////////////////////////////////////////////////////
/*	BaccEventAction.hh

This is the header file to control the simulation event-by-event.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccEventAction_HH
#define BaccEventAction_HH 1

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4UserEventAction.hh"

//
//	Class forwarding
//
class G4Event;
class BaccManager;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccEventAction : public G4UserEventAction
{
	public:
		BaccEventAction();
		~BaccEventAction();

		void BeginOfEventAction( const G4Event* );
		void EndOfEventAction( const G4Event* );
		
		void SetManager( BaccManager *man ) { baccManager = man; };
		
		inline G4double GetTimeSinceStart() { return simStartTime; };
                inline void SetTimeSinceStart( G4double t ) { simStartTime = (time_t)(t); };
		
		inline G4int GetEventNum() { return eventNum; };
		
		inline void SetRadioactivePrimaryTime( G4double val )
				{ radioactivePrimaryTime = val; };
		inline G4double GetRadioactivePrimaryTime()
				{ return radioactivePrimaryTime; };
			
	private:
		time_t simStartTime, simEndTime;
		G4int eventNum;
		G4double radioactivePrimaryTime;
	
		BaccManager *baccManager;
};

#endif
