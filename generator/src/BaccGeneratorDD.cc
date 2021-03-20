////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorDD.cc
*
* This is the code file for the DD neutron generator.
*
********************************************************************************
* Change log
*   2 Fed 15 - Initial submission (Kevin)
*
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"
#include "G4Neutron.hh"

//
//	Bacc includes
//
#include "BaccGeneratorDD.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorDD()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorDD::BaccGeneratorDD()
{
	name = "DD";
	activityMultiplier = 1;
	ionDef = G4GenericIon::Definition();
       	neutronDef = G4Neutron::Definition();
        ddEnergy = 2.45 * MeV;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorDD()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorDD::~BaccGeneratorDD() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorDD::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4double time)
{
    G4int a=-1; G4int z=-1;
    G4double hl=-1;
    Isotope *currentIso = new Isotope(name, z, a, hl);
    baccManager->RecordTreeInsert( currentIso, time, position,
                  sourceByVolumeID, sourcesID );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorDD::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
  
    G4double timeDelay = (firstNode->timeOfEvent)/ns;//stored as seconds
    timeDelay *= ns;
    
    particleGun->GetCurrentSource()->SetParticleTime( timeDelay );
    
    G4ThreeVector pos = G4ThreeVector(firstNode->pos );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleDefinition( neutronDef );
    particleGun->GetCurrentSource()->GetAngDist()->
      SetParticleMomentumDirection( G4ThreeVector(-35.9166, 337.6137, 0) );
    particleGun->GetCurrentSource()->GetEneDist()->
      SetMonoEnergy( ddEnergy );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

}
