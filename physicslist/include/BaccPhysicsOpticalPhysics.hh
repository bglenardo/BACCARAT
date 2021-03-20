////////////////////////////////////////////////////////////////////////////////
/*	BaccPhysicsOpticalPhysics.hh

This header file is part of the Bacc physics list. It is based on the extended 
field example field04, with some additional improvements from Peter Gumplinger.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccPhysicsOpticalPhysics_h
#define BaccPhysicsOpticalPhysics_h 1

//
//  GEANT4 includes
//
#include "globals.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4S1Light.hh"
#include "G4Cerenkov.hh"

//
//  Bacc includes
//
#include "BaccManager.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccPhysicsOpticalPhysics : public G4VPhysicsConstructor
{
	private:
		BaccManager *baccManager;

		G4S1Light *theScintProcess;
		G4Cerenkov *theCerenkovProcess;

	public:

		BaccPhysicsOpticalPhysics();
		virtual ~BaccPhysicsOpticalPhysics();

		virtual void ConstructParticle();
		virtual void ConstructProcess();
		
		G4S1Light *GetScintillation() { return theScintProcess; };
		G4Cerenkov *GetCerenkov() { return theCerenkovProcess; };
};
#endif
