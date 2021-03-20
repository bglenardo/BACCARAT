////////////////////////////////////////////////////////////////////////////////
/*	BaccPhysicsOpticalPhysics.cc

This code file comes from Peter Gumplinger, and is intended to be a part of the 
extended example field04 (which should be part of GEANT4.9.3). It turns on 
physics for optical models.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4Version.hh"

#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThermalElectron.hh"
#include "G4S2Light.hh"

//
//	Bacc includes
//
#include "BaccPhysicsOpticalPhysics.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccPhysicsOpticalPhysics()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPhysicsOpticalPhysics::BaccPhysicsOpticalPhysics()
    : G4VPhysicsConstructor("Optical")
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccPhysicsOpticalPhysics()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPhysicsOpticalPhysics::~BaccPhysicsOpticalPhysics() { }

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ConstructParticle()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsOpticalPhysics::ConstructParticle()
{
	G4OpticalPhoton::OpticalPhotonDefinition();
	G4ThermalElectron::ThermalElectronDefinition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ConstructProcess()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsOpticalPhysics::ConstructProcess()
{
	G4cout << "BaccPhysicsOpticalPhysics:: Add Optical Physics Processes"
	<< G4endl;
	
	theScintProcess = new G4S1Light();
    G4S2Light *theLuminProcess = new G4S2Light( "S2", fElectromagnetic,
            theScintProcess);
	theCerenkovProcess = new G4Cerenkov();
	G4int MaxNumPhotons = 300;
	theCerenkovProcess->SetTrackSecondariesFirst(false);
	theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);
	
	G4OpAbsorption* theAbsorptionProcess = new G4OpAbsorption();
	G4OpRayleigh* theRayleighScattering = new G4OpRayleigh();
	G4OpBoundaryProcess* theBoundaryProcess = new G4OpBoundaryProcess();
	G4OpWLS* theWLSProcess = new G4OpWLS();

	G4ProcessManager* pManager = 
	G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

	if( !pManager ) {
		std::ostringstream o;
		o << "Optical Photon without a Process Manager";
		G4Exception("BaccPhysicsOpticalPhysics::ConstructProcess()","",
		FatalException,o.str().c_str());
	}


	pManager->AddDiscreteProcess(theAbsorptionProcess);
	pManager->AddDiscreteProcess(theRayleighScattering);

	//  theBoundaryProcess->SetModel(unified);
	//  theBoundaryProcess->SetModel(glisur);

	pManager->AddDiscreteProcess(theBoundaryProcess);

	theWLSProcess->UseTimeProfile("delta");
	//  theWLSProcess->UseTimeProfile("exponential");

	pManager->AddDiscreteProcess(theWLSProcess);

	//theScintProcess->SetScintillationYieldFactor(1.);
	//theScintProcess->SetScintillationExcitationRatio(0.06);
	//theScintProcess->SetTrackSecondariesFirst(false);


        #if G4VERSION_NUMBER>=1000
	GetParticleIterator()->reset();

	while( (*GetParticleIterator())() ) {
		G4ParticleDefinition* particle = GetParticleIterator()->value();
		G4String particleName = particle->GetParticleName();

		pManager = particle->GetProcessManager();
		if (!pManager) {
			std::ostringstream o;
			o << "Particle " << particleName << "without a Process Manager";
			G4Exception("BaccPhysicsOpticalPhysics::ConstructProcess()","",
			FatalException,o.str().c_str());
		}

		if( theCerenkovProcess->IsApplicable(*particle) ) {
			pManager->AddProcess(theCerenkovProcess);
			pManager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
		}
		if( theScintProcess->IsApplicable(*particle) )
		  pManager->AddProcess(theScintProcess,ordDefault,ordInActive,ordDefault);
		if( theLuminProcess->IsApplicable(*particle) )
		  pManager->AddProcess(theLuminProcess,ordDefault,ordInActive,ordDefault);
	}
        #endif
        #if G4VERSION_NUMBER<1000
	GetParticleIterator()->reset();

	while( (*GetParticleIterator())() ) {
		G4ParticleDefinition* particle = GetParticleIterator()->value();
		G4String particleName = particle->GetParticleName();

		pManager = particle->GetProcessManager();
		if (!pManager) {
			std::ostringstream o;
			o << "Particle " << particleName << "without a Process Manager";
			G4Exception("BaccPhysicsOpticalPhysics::ConstructProcess()","",
			FatalException,o.str().c_str());
		}

		if( theCerenkovProcess->IsApplicable(*particle) ) {
			pManager->AddProcess(theCerenkovProcess);
			pManager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
		}
		if( theScintProcess->IsApplicable(*particle) )
		  pManager->AddProcess(theScintProcess,ordDefault,ordInActive,ordDefault);
		if( theLuminProcess->IsApplicable(*particle) )
		  pManager->AddProcess(theLuminProcess,ordDefault,ordInActive,ordDefault);
	}
        #endif
        std::cerr << "\nFinished adding optical stuff...\n" << std::endl;

}
