////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorSingleParticle.cc

This is the code file for the SingleParticle generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//  C/C++ includes
//
#include <sstream>

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"

//
//	Bacc includes
//
#include "BaccGeneratorSingleParticle.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorSingleParticle()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorSingleParticle::BaccGeneratorSingleParticle()
{
	name = "SingleParticle";
	activityMultiplier = 1;
    particleEnergy = 0*keV;
    particleName = G4Geantino::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorSingleParticle()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorSingleParticle::~BaccGeneratorSingleParticle() {;}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorSingleParticle::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4String pname, 
                G4double penergy, G4double time)
{

    G4int z=-1; G4int a=-1;
  	Isotope *currentIso = new Isotope(name, z, a, pname, penergy);
  	baccManager->RecordTreeInsert( currentIso, time, position, 
                  sourceByVolumeID, sourcesID );	
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorSingleParticle::GenerateFromEventList( 
    G4GeneralParticleSource *particleGun, G4Event *event, decayNode *firstNode )
{

    G4String particlestring = firstNode->particleName; //name=SingleParticle
    G4ParticleTable *pTable = G4ParticleTable::GetParticleTable();
    particleName = pTable->FindParticle( particlestring ) ;
    if(particleName){
        particleGun->GetCurrentSource()->SetParticleDefinition( particleName );
    }

    G4double time = (firstNode->timeOfEvent)/ns;//time in ns
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );

    G4ThreeVector pos = G4ThreeVector(firstNode->pos );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

  	particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
			GetRandomDirection() );

    G4double penergy = firstNode->energy;
	particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( penergy );

   	particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

}

