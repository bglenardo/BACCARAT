////////////////////////////////////////////////////////////////////////////////
/*   BaccStackingAction.cc
*
* This is an implementation of the user stacking action which deals with tracks
* as they come into existence. It is initially created to reset the global time
* for daughter tracks of radioisotopes when using the G4Decay generator.
*
********************************************************************************
* Change log
*   06-Oct-2015 - Initial submission (David W)
*/
////////////////////////////////////////////////////////////////////////////////

//
//GEANT4 includes
//
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"

//
//Bacc includes
//
#include "BaccStackingAction.hh"
#include "BaccManager.hh"


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              BaccSteppingAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccStackingAction::BaccStackingAction(){

    baccManager = BaccManager::GetManager();
    double lastTime = 0;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              ~BaccSteppingAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccStackingAction::~BaccStackingAction() {;}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              ClassifyNewTrack()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4ClassificationOfNewTrack BaccStackingAction::ClassifyNewTrack( const G4Track * aTrack ){
  
  G4ClassificationOfNewTrack result( fUrgent );  
  
  if(baccManager->GetG4DecayBool()){
    lastTime = aTrack->GetGlobalTime();
    //    Reset time for immediate daughter track(s) of an isotope to 1s
    //    This has the effect of having all radioisotopes decay at t=0 with
    //    a decay time of 1s.
    if( aTrack->GetParentID()==baccManager->GetIsotopeTrackID() || aTrack->GetParentID()==1)
        (const_cast<G4Track *>(aTrack))->SetGlobalTime( 1.e9 );
  }
  
  return result;
    
}

