////////////////////////////////////////////////////////////////////////////////
/*	BaccRunAction.cc

This is the code file to control the simulation run-by-run.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	Geant4 includes
//
#include "G4Run.hh"

//
//	Bacc includes
//
#include "BaccRunAction.hh"
#include "BaccManager.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccRunAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccRunAction::BaccRunAction()
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccRunAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccRunAction::~BaccRunAction() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BeginOfRunAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccRunAction::BeginOfRunAction( const G4Run* aRun )
{
	//	Print out the run number, and start the timer
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
	time( &startTime );
	baccManager->InitialiseEventCount();
	
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					EndOfRunAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccRunAction::EndOfRunAction( const G4Run* aRun )
{
	//	Print out the last progress report
	time_t endTime;
	time( &endTime );
	G4double timeDifference = difftime( endTime, startTime );
	G4cout << G4endl << "Run " << aRun->GetRunID() << " time to completion: "
		   << timeDifference << " seconds" << G4endl;

}
