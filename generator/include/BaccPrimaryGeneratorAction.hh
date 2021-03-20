////////////////////////////////////////////////////////////////////////////////
/*	BaccPrimaryGeneratorAction.hh

This is the header file to control the GEANT4 side of event generation.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccPrimaryGeneratorAction_HH
#define BaccPrimaryGeneratorAction_HH 1

//
//	GEANT4 includes
//
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

//
//  Bacc includes
//
#include "BaccManager.hh"

//
//	Class forwarding
//
class G4GeneralParticleSource;
class G4Event;
class BaccManager;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		BaccPrimaryGeneratorAction();
		~BaccPrimaryGeneratorAction();

	public:
		void GeneratePrimaries( G4Event* );

    protected:
        BaccManager::primaryParticleInfo GetParticleInfo( G4GeneralParticleSource* );

	private:
		BaccManager *baccManager;
		G4GeneralParticleSource *particleGun;
};

#endif
