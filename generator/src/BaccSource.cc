////////////////////////////////////////////////////////////////////////////////
/*	BaccSource.cc

This is the code file for individual generators. Each generator will inherit 
from this generalized source class.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  17-Nov-2016 - Overloaded a GenerateEventList for muon generator (David W)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccSource.hh"
#include "BaccDetectorComponent.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccSource()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccSource::BaccSource()
{
	baccManager = BaccManager::GetManager();
	UI = G4UImanager::GetUIpointer();
    
    xPos = yPos = zPos = 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccSourceCatalog()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccSource::~BaccSource() {}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CalculatePopulationsInEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::CalculatePopulationsInEventList( G4double, G4double, 
                      G4String )
{}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList() // Default
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::GenerateEventList( G4ThreeVector, G4int, G4int, G4double)
{}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList() // DecayChain
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::GenerateEventList( G4ThreeVector, G4int, G4int, G4String )
{}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList() //SingleDecay
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::GenerateEventList( G4ThreeVector, G4int, G4int, G4int, 
                      G4int, G4double )
{
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList() //SingleParticle
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::GenerateEventList( G4ThreeVector, G4int, G4int, G4String, 
                      G4double, G4double )
{
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList() //Wimp
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::GenerateEventList( G4ThreeVector, G4int, G4int, G4double, 
                      G4double)
{
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList() //MUSUN
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::GenerateEventList( G4double )
{
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSource::GenerateFromEventList( G4GeneralParticleSource*, G4Event*, 
                      decayNode* )
{
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetParticleInfo()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccManager::primaryParticleInfo BaccSource::GetParticleInfo(
                  G4GeneralParticleSource *particleGun )
{
    BaccManager::primaryParticleInfo particle;
    particle.id = particleGun->GetParticleDefinition()->GetParticleName();
    //particle.id = name ;
    particle.energy = particleGun->GetCurrentSource()->GetParticleEnergy(); 
    particle.time = particleGun->GetCurrentSource()->GetParticleTime();	
    particle.position = particleGun->GetCurrentSource()->GetParticlePosition();
    particle.direction = particleGun->GetCurrentSource()->
                                        GetParticleMomentumDirection();
    return particle; 
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetRandomDirection()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4ThreeVector BaccSource::GetRandomDirection()
{
	cosTheta = -1. + 2.*G4UniformRand();
	sinTheta = sqrt( 1. - cosTheta*cosTheta );
	phi = 2.*3.141592653589793238*G4UniformRand();
	xDir = sinTheta * cos( phi );
	yDir = sinTheta * sin( phi );
	zDir = cosTheta;
	G4ThreeVector direction( xDir, yDir, zDir );

	return direction;
}
