////////////////////////////////////////////////////////////////////////////////
/*	BaccSource.hh

This is the header file for individual generators. Each generator will inherit 
from this generalized source class.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/09/09 - Included G4SystemOfUnits.hh to be compliant with geant4.10 (Brian)
  03 May 2016 - Added a GenerateEvent for the muon generator (David W)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccSource_HH
#define BaccSource_HH 1

//
//	CLHEP includes
//
#include "Randomize.hh"

//
//	GEANT4 includes
//
#include "G4UImanager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
//
//	Bacc includes
//
#include "BaccManager.hh"
#include "BaccBST.hh"
#include "BaccIsotope.hh"

//
//	Class forwarding
//
class BaccDetectorComponent;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccSource
{
	public:
		BaccSource();
		~BaccSource();

	public:
		G4String GetName() { return name; };
		G4double GetActivityMultiplier() { return activityMultiplier; };

        // generate event list for different conditions
        //   default
		virtual void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        //   single decay
		virtual void GenerateEventList( G4ThreeVector, G4int, G4int, 
                            G4int, G4int, G4double );
        //   single particle
		virtual void GenerateEventList( G4ThreeVector, G4int, G4int, 
                G4String, G4double, G4double );
        //   wimp
		virtual void GenerateEventList( G4ThreeVector, G4int, G4int, G4double,
                G4double );
        //   decay chain
        virtual void CalculatePopulationsInEventList( G4double, G4double, 
                            G4String);
        virtual void GenerateEventList( G4ThreeVector, G4int, G4int, G4String );
        virtual G4double GetParentDecayTime() {return 0.;};

        //   musun
        virtual void GenerateEventList( G4double );

        // generate from event list
		virtual void GenerateFromEventList( G4GeneralParticleSource*, G4Event*,
                            decayNode* );

	protected:
		G4UImanager *UI;
		G4String name;
		G4double activityMultiplier;
		G4double probability;
		
	protected:
        BaccManager::primaryParticleInfo GetParticleInfo( 
                        G4GeneralParticleSource* );
		G4ThreeVector GetRandomDirection();
		virtual G4double GetEnergy() { return 1.*MeV; };
		virtual G4ParticleDefinition *GetParticleDefinition()
			{ return G4Gamma::Definition(); };

    protected:
		BaccManager *baccManager;
	
	private:
		G4double xPos, yPos, zPos;
		G4double xDir, yDir, zDir;
		G4double cosTheta, sinTheta, phi;
};

#endif
