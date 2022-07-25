////////////////////////////////////////////////////////////////////////////////
/*	BaccEventAction.cc

This is the code file to control the simulation event-by-event.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/09/09 - Changed trj->DrawTrajectory( 0 ) to trj->DrawTrajectory( )
               (Brian)
  2017/02/21 - Initialises and clears storage variables for S2 vertex positions (TD and AC)

*/
////////////////////////////////////////////////////////////////////////////////

//
//	C/C++ includes
//
#include <ctime>

//      CLHEP includes
#include "Randomize.hh"

//
//	GEANT4 includes
//
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"

//
//	Bacc includes
//
#include "BaccEventAction.hh"
#include "BaccManager.hh"





//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccEventAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccEventAction::BaccEventAction()
: simStartTime(0), simEndTime(0), eventNum(-1)
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccEventAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccEventAction::~BaccEventAction()
{}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BeginOfEventAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccEventAction::BeginOfEventAction(const G4Event* evt )
{
	eventNum = evt->GetEventID();
	if( !eventNum ) {
		baccManager->SetRunEndedCleanly( false );
		time( &simStartTime );
	}
	
	radioactivePrimaryTime = 0;
	baccManager->SetCurrentS2Index(0);

	if( baccManager->GetUseInputEventSeed() ){
		long fEventSeeds[2];
		fEventSeeds[0] = baccManager->GetInputEventSeed1();
		fEventSeeds[1] = baccManager->GetInputEventSeed2();
		CLHEP::HepRandom::setTheSeeds( fEventSeeds, 
			baccManager->GetInputEventSeedIndex() );
	}
	baccManager->SaveEventSeedVals( 
			CLHEP::HepRandom::getTheSeed(),
			CLHEP::HepRandom::getTheSeeds()[0],
			CLHEP::HepRandom::getTheSeeds()[1] );
	//4,1689174239,709645262	
	//std::vector<G4int> fEventSeeds{1689174239, 709645262};
	//long fEventSeeds[2];
	//fEventSeeds[0] = 1689174239;
	//fEventSeeds[1] = 709645262;
	//CLHEP::HepRandom::setTheSeeds(fEventSeeds,4);

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					EndOfEventAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccEventAction::EndOfEventAction( const G4Event* evt )
{
	//	Check to see if all the events have been processed, and if so, set the
	//	"run ended cleanly" flag
	if( eventNum == (baccManager->GetNumEvents() - 1) )
		baccManager->SetRunEndedCleanly( true );

	// Print out periodic progress reports
	if( eventNum% (baccManager->GetEventProgressFreqnecy()) == 0 ) {
		time( &simEndTime );
		G4cout << "\n\tProcessing event " << eventNum << " at "
			   << difftime( simEndTime, simStartTime ) << " seconds.";
		G4cout.flush();
	} else if( baccManager->GetEventProgressFreqnecy() >= 10 &&
                eventNum % (baccManager->GetEventProgressFreqnecy()/10) == 0 ) {
		G4cout << ".";
        G4cout.flush();
	}

	if( !baccManager->GetG4DecayBool() ){
	  baccManager->RecordValues( eventNum );
	  baccManager->ClearRecords();
	} else {
	  baccManager->IterateEventCount();
	  baccManager->RecordValues( baccManager->GetEventCount() );
	  baccManager->ClearRecords();
	  baccManager->SetG4DecayOutputBool(false);
	}
	

	//	Visualization support. The follow chunk of code draws the trajectories
	//	if the macro opens a visualization.
	if( eventNum < 100 && G4VVisManager::GetConcreteInstance() ) {
		G4TrajectoryContainer *trajectoryContainer =
				evt->GetTrajectoryContainer();
		G4int n_trajectories = 0;
		if( trajectoryContainer )
			n_trajectories = trajectoryContainer->entries();
		for( G4int i=0; i<n_trajectories; i++ ) {
			G4Trajectory* trj =
					(G4Trajectory *)( (*(evt->GetTrajectoryContainer()))[i] );
			trj->DrawTrajectory( );

		}
	}

	baccManager->ClearS2Positions();
	
}
