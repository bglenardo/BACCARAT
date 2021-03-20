////////////////////////////////////////////////////////////////////////////////
/*	BaccSteppingAction.hh

This is the header file to control the simulation step-by-step.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccSteppingAction_HH
#define BaccSteppingAction_HH 1

//
//	GEANT4 includes
//
#include "G4UserSteppingAction.hh"
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccManager.hh"

//
//	Class forward declarations
//
class G4UserEventAction;
class G4Track;
class G4Material;

class BaccEventAction;
class BaccDetectorComponent;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccSteppingAction : public G4UserSteppingAction
{
	public:
		BaccSteppingAction();
		~BaccSteppingAction();

		void UserSteppingAction( const G4Step *theStep );

                //      Primary particle information
                BaccManager::primaryParticleInfo primaryParticles;

	
	private:
		G4Track *theTrack;
		G4ThreeVector trackPosition;
		G4ThreeVector particleDirection;
		
		G4int recordLevel;
		G4int optPhotRecordLevel;
		G4int thermElecRecordLevel;	

		BaccManager *baccManager;
		BaccManager::stepRecord aStepRecord;
		
		G4Material *blackiumMat;
  
                std::map<G4int,bool> radIsoMap;
                std::map<G4int,bool>::iterator itMap;
  
                int currentZ;
                int currentA;


};

#endif
