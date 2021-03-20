////////////////////////////////////////////////////////////////////////////////
/*	BaccRunAction.hh

This is the header file to control the simulation run-by-run.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccRunAction_HH
#define BaccRunAction_HH 1

//
//	GEANT4 includes
//
#include "G4UserRunAction.hh"
#include "globals.hh"

//
//	Class forwarding
//
class BaccManager;
class G4Run;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccRunAction : public G4UserRunAction
{
	public:
		BaccRunAction();
		~BaccRunAction();

	public:
		void BeginOfRunAction( const G4Run* );
		void EndOfRunAction( const G4Run* );
		
	private:
		BaccManager *baccManager;
		time_t startTime;
		
};

#endif
