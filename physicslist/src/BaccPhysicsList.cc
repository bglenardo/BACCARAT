////////////////////////////////////////////////////////////////////////////////
/*	BaccPhysicsList.cc

This code file is part of the Bacc physics list. It uses the QGSP_BIC_HP physics 
list that's included as part of the hadronic physics lists in GEANT4. It was 
also ripped off wholesale from the extended/field/field04 list, with some 
additional input from Peter Gumplinger. It also uses the 
G4RadioactiveDecayPhysics, G4EmExtraPhysics, and the Livermore physics list from 
the extended electromagnetic examples (see, e.g., TestEm1).
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
  2014/09/08 - Changed the long cut lengths from 0.1mm to 0.01mm, and made
               the long cuts the default (Brian)
  2014/09/09 - Added includes for G4SystemOfUnits and G4PhysicalConstants (Brian)

*/
////////////////////////////////////////////////////////////////////////////////

//
//	C/C++ includes
//
#include <iostream>
#include <iomanip>
#include <ctime>

//
//	GEANT4 includes
//
#include "G4VModularPhysicsList.hh"

#include "G4Version.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4PhysListFactory.hh"

#include "G4EmExtraPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmLivermorePhysics.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//
// Particles
//
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

//
//	Bacc includes
//
#include "BaccPhysicsList.hh"
#include "BaccPhysicsStepMax.hh"
#include "BaccPhysicsOpticalPhysics.hh"

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccPhysicsList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPhysicsList::BaccPhysicsList() : G4VModularPhysicsList()
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );

	//	I don't know what this next line does, but it was part of the field04
	//	physics list, so here it is.
	G4LossTableManager::Instance();

	//	Set default cut values

	defaultCutValue = 0.01*mm;
	longCutValue = 0.01*mm;
	shortCutValue = 1*nm;
        //longCutValue = 1.*mm;
        //shortCutValue = 1.*mm;

	cutForProton = defaultCutValue;
	cutForAlpha = defaultCutValue;
	//cutForGenericIon = 1.*nm; //defaultCutValue;
	cutForGenericIon = defaultCutValue;

	SetCutsLong();

	VerboseLevel = 0;
	OpVerbLevel = 0;
	
	SetVerboseLevel( VerboseLevel );

//        cerr << "\n Creating physics objects...\n" << endl;
	
	G4RadioactiveDecayPhysics *radPhys = new G4RadioactiveDecayPhysics();
//	G4EmExtraPhysics *extraPhys = new G4EmExtraPhysics();
	G4EmLivermorePhysics *lowEPhys = new G4EmLivermorePhysics();
	BaccPhysicsOpticalPhysics *optPhys = new BaccPhysicsOpticalPhysics();

//        cerr << "\nRegistering physics...\n" << endl;
	
	G4cout << "RegisterPhysics: " << radPhys->GetPhysicsName() << G4endl;
	RegisterPhysics( radPhys );
//	G4cout << "RegisterPhysics: " << extraPhys->GetPhysicsName() << G4endl;
//	RegisterPhysics( extraPhys );
	G4cout << "RegisterPhysics: " << lowEPhys->GetPhysicsName() << G4endl;
	RegisterPhysics( lowEPhys );
	G4cout << "RegisterPhysics: " << optPhys->GetPhysicsName() << G4endl;
	RegisterPhysics( optPhys );

        cerr << "\nPhysics registered...\n" << endl;

	G4PhysListFactory factory;
//	G4VModularPhysicsList *phys = factory.GetReferencePhysList( "QGSP_BERT_HP");
	G4VModularPhysicsList *phys = factory.GetReferencePhysList( "QGSP_BIC_HP" );
	for( G4int i=0; ; ++i ) {
		G4VPhysicsConstructor *elem =
				const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
		if( elem == NULL ) break;
		if( elem->GetPhysicsName() != "G4EmStandard" ) {
			G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
			RegisterPhysics( elem );
		}
	}
	
	stepMaxProcess = new BaccPhysicsStepMax();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccPhysicsList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPhysicsList::~BaccPhysicsList()
{
	delete stepMaxProcess;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetCuts()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::SetCuts()
{
	SetCutValue( cutForGamma, "gamma" );
	SetCutValue( cutForElectron, "e-" );
	SetCutValue( cutForPositron, "e+" );
	SetCutValue( cutForProton, "proton" );
	SetCutValue( cutForAlpha, "alpha" );
	SetCutValue( cutForGenericIon, "GenericIon" );

	G4int nParticles = 5;
	DumpCutValuesTable( nParticles );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetCutsLong()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::SetCutsLong()
{
	cutForGamma = longCutValue;
	cutForElectron = longCutValue;
	cutForPositron = longCutValue;

	SetCuts();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetCutsShort()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::SetCutsShort()
{
	cutForGamma = shortCutValue;
	cutForElectron = shortCutValue;
	cutForPositron = shortCutValue;

	SetCuts();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ClearPhysics()
//------++++++------++++++------++++++------++++++------++++++------++++++------
/*void BaccPhysicsList::ClearPhysics()
{
	for( G4PhysConstVector::iterator p = physicsVector->begin();
			p != physicsVector->end(); ++p )
		delete( *p );
	
	physicsVector->clear();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					RemoveFromPhysicsList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::RemoveFromPhysicsList( const G4String& name )
{
	G4bool success = false;
	for( G4PhysConstVector::iterator p = physicsVector->begin();
			p != physicsVector->end(); ++p ) {
		G4VPhysicsConstructor *e = (*p);
		if( e->GetPhysicsName() == name ) {
			physicsVector->erase(p);
			success = true;
			break;
		}
	}
	
	if( !success ) {
		std::ostringstream message;
		message << "PhysicsList::RemoveFromEMPhysicsList " << name
				<< " not found";
#if G4VERSION_NUMBER<950
		G4Exception( message.str().c_str() );
#endif
	}
}*/

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ConstructParticle()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::ConstructParticle()
{
	G4VModularPhysicsList::ConstructParticle();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ConstructProcess()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::ConstructProcess()
{
	G4VModularPhysicsList::ConstructProcess();
	AddStepMax();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetStepMax()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::SetStepMax( G4double step )
{
	MaxChargedStep = step;
	stepMaxProcess->SetStepMax( MaxChargedStep );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					AddStepMax()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPhysicsList::AddStepMax()
{
	GetParticleIterator()->reset();
	while( (*GetParticleIterator())() ) {
		G4ParticleDefinition *particle = GetParticleIterator()->value();
		G4ProcessManager *pmanager = particle->GetProcessManager();
		
		if( stepMaxProcess->IsApplicable(*particle)
				&& !particle->IsShortLived() ) {
			if( pmanager )
				pmanager->AddDiscreteProcess( stepMaxProcess );
		}
	}
}
