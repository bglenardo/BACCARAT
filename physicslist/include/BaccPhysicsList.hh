////////////////////////////////////////////////////////////////////////////////
/*	BaccPhysicsList.hh

This header file is part of the Bacc physics list. See the code file for 
additional comments on the contents of the physics list.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccPhysicsList_h
#define BaccPhysicsList_h 1

//
//  GEANT4 includes
//
#include "G4VModularPhysicsList.hh"
#include "globals.hh"

//
//  Bacc includes
//
#include "BaccManager.hh"

//
//	Class forwarding
//
class G4VPhysicsConstructor;
class BaccPhysicsStepMax;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccPhysicsList: public G4VModularPhysicsList
{
	public:
		BaccPhysicsList();
		virtual ~BaccPhysicsList();

	public:
		void SetCuts();
		void SetCutsLong();
		void SetCutsShort();
		
		void ClearPhysics();
		void RemoveFromPhysicsList( const G4String& );
		
		void ConstructParticle();
		void ConstructProcess();

		void SetStepMax( G4double );
		void AddStepMax();
		BaccPhysicsStepMax *GetStepMaxProcess() { return stepMaxProcess; };
		
	private:
		BaccManager *baccManager;
	
		G4int VerboseLevel;
		G4int OpVerbLevel;

		G4double defaultCutValue;
		G4double shortCutValue;
		G4double longCutValue;

		G4double cutForGamma;
		G4double cutForElectron;
		G4double cutForPositron;
		G4double cutForProton;
		G4double cutForAlpha;
		G4double cutForGenericIon;
		
		G4double MaxChargedStep;
		BaccPhysicsStepMax *stepMaxProcess;
		
};

#endif
