////////////////////////////////////////////////////////////////////////////////
/*	BaccPrimaryGeneratorAction.cc

This is the code file to control the GEANT4 side of event generation.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

//
//	Bacc includes
//
#include "BaccPrimaryGeneratorAction.hh"
#include "BaccManager.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccPrimaryGeneratorAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPrimaryGeneratorAction::BaccPrimaryGeneratorAction()
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );
	particleGun = new G4GeneralParticleSource();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccPrimaryGeneratorAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccPrimaryGeneratorAction::~BaccPrimaryGeneratorAction()
{
	delete particleGun;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GeneratePrimaries()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccPrimaryGeneratorAction::GeneratePrimaries( G4Event *event )
{
        if( baccManager->GetUseInputEventSeed() ){
                G4long fEventSeeds[2];
                fEventSeeds[0] = baccManager->GetInputEventSeed1();
                fEventSeeds[1] = baccManager->GetInputEventSeed2();
                CLHEP::HepRandom::setTheSeed( baccManager->GetInputEventSeedIndex() );
                CLHEP::HepRandom::setTheSeeds( fEventSeeds ); //, 
                        //baccManager->GetInputEventSeedIndex() );
                    
        }
        baccManager->SaveEventSeedVals( 
                         CLHEP::HepRandom::getTheSeed(),
                         CLHEP::HepRandom::getTheSeeds()[0],
                         CLHEP::HepRandom::getTheSeeds()[1] );

	//	Have the management class determine which event is next and generate
	//	that event
	if( baccManager->GetTotalSimulationActivity() )
		baccManager->GenerateEvent( particleGun, event );
	else {
	    //BaccManager::primaryParticleInfo particle = GetParticleInfo(particleGun);
        particleGun->GeneratePrimaryVertex( event );
        baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
    }
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                  GetParticleInfo()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccManager::primaryParticleInfo BaccPrimaryGeneratorAction::GetParticleInfo(
        G4GeneralParticleSource *pGun )
{
    BaccManager::primaryParticleInfo particle;
    particle.id = pGun->GetParticleDefinition()->GetParticleName();
    particle.energy = pGun->GetCurrentSource()->GetParticleEnergy();
    particle.time = pGun->GetCurrentSource()->GetParticleTime();
    particle.position = pGun->GetCurrentSource()->GetParticlePosition();
    particle.direction = pGun->GetCurrentSource()->
                            GetParticleMomentumDirection();
    return particle;
}
