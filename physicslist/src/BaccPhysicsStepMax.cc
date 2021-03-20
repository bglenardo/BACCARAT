////////////////////////////////////////////////////////////////////////////////
/*	BaccPhysicsStepMax.cc

This code file comes from Peter Gumplinger, and is intended to be a part of the 
extended example field04 (which should be part of GEANT4.9.3). It turns on 
physics for optical models.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "G4Track.hh"
#include "G4VParticleChange.hh"

//
//	Bacc includes
//
#include "BaccPhysicsStepMax.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccPhysicsStepMax()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPhysicsStepMax::BaccPhysicsStepMax( const G4String& aName )
  : G4VDiscreteProcess(aName), MaxChargedStep(DBL_MAX)
{
	if( verboseLevel>0 )
		G4cout << GetProcessName() << " is created "<< G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccPhysicsStepMax()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPhysicsStepMax::BaccPhysicsStepMax( BaccPhysicsStepMax& right ) : G4VDiscreteProcess(right) { }

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccPhysicsStepMax()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPhysicsStepMax::~BaccPhysicsStepMax() { }

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					PostStepGetPhysicalInteractionLength()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccPhysicsStepMax::PostStepGetPhysicalInteractionLength(
                                              const G4Track&,
                                              G4double,
                                              G4ForceCondition* condition)
{
	// condition is set to "Not Forced"
	*condition = NotForced;

	G4double ProposedStep = DBL_MAX;
	if ( MaxChargedStep > 0.) ProposedStep = MaxChargedStep ;
	return ProposedStep;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					PostStepDoIt()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4VParticleChange* BaccPhysicsStepMax::PostStepDoIt(const G4Track& aTrack,
                                         const G4Step&         )
{
	// do nothing
	aParticleChange.Initialize(aTrack);
	return &aParticleChange;
}
