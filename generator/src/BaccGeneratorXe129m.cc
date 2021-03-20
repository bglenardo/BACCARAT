////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorXe129m.cc

This is the code file for the Xe129m generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//    GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"
#include "G4Gamma.hh"

//
//     Bacc includes
//
#include "BaccGeneratorXe129m.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorXe129m()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorXe129m::BaccGeneratorXe129m()
{
    name = "Xe129m";
    activityMultiplier = 1;
    ion = G4GenericIon::Definition();
    gammaDef = G4Gamma::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorXe129m()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorXe129m::~BaccGeneratorXe129m() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorXe129m::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4double time)
{
    G4int a=-1; G4int z=-1; G4double hl=-1;
    Isotope *currentIso = new Isotope(name, z, a, hl);
    baccManager->RecordTreeInsert( currentIso, time, position, 
                  sourceByVolumeID, sourcesID );    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorXe129m::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
    particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
            GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 39.5784*keV );

    G4double time = (firstNode->timeOfEvent)/ns;
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );

    G4ThreeVector pos = G4ThreeVector(firstNode->pos);
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );

    particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
	    GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 196.563*keV );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );

}
