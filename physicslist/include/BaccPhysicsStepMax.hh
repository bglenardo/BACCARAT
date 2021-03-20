////////////////////////////////////////////////////////////////////////////////
/*	BaccPhysicsStepMax.hh

This header file is part of the Bacc physics list. It is based on the extended 
field example field04, with some additional improvements from Peter Gumplinger.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccPhysicsStepMax_h
#define BaccPhysicsStepMax_h 1

//
//  GEANT4 includes
//
#include "globals.hh"
#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccPhysicsStepMax : public G4VDiscreteProcess
{
	public:
		BaccPhysicsStepMax( const G4String& processName = "UserStepMax" );
		BaccPhysicsStepMax( BaccPhysicsStepMax & );

		~BaccPhysicsStepMax();

		G4bool IsApplicable( const G4ParticleDefinition& particle ) {
			return( particle.GetPDGCharge() != 0. ); };

		void SetStepMax( G4double step ) { MaxChargedStep = step; };
		G4double GetStepMax() { return MaxChargedStep; };

		G4double PostStepGetPhysicalInteractionLength(const G4Track& track,
                                                  G4double previousStepSize,
                                                  G4ForceCondition* condition);

		G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

	protected:
		G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*)
			{ return 0.; };

	private:

		// hide assignment operator as private
		BaccPhysicsStepMax & operator=(const BaccPhysicsStepMax &right);
		BaccPhysicsStepMax( const BaccPhysicsStepMax& );

	private: 
		G4double MaxChargedStep;
};

#endif
