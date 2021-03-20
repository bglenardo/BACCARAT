////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorPb210.cc

This is the code file for the Pb210 generator.
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

//
//     Bacc includes
//
#include "BaccGeneratorPb210.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorPb210()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorPb210::BaccGeneratorPb210()
{
    name = "Pb210";
    activityMultiplier = 3;
    ion = G4GenericIon::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorPb210()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorPb210::~BaccGeneratorPb210() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorPb210::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorPb210::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
    particleGun->GetCurrentSource()->SetParticleDefinition( ion );
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
            GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 0.*keV );

    G4double time = (firstNode->timeOfEvent)/ns;
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );

    G4ThreeVector pos = G4ThreeVector(firstNode->pos);
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    probability = G4UniformRand();
    
    if( probability < 1./3. ) {
        UI->ApplyCommand( "/gps/ion 82 210 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 210 210 82 82" );
    } else if( probability < 2./3. ) {
        UI->ApplyCommand( "/grdm/nucleusLimits 210 210 83 83" );
        UI->ApplyCommand( "/gps/ion 83 210 0 0" );
    } else {
        if( G4UniformRand() < .9999987) {
            UI->ApplyCommand( "/grdm/nucleusLimits 210 210 84 84" );
            UI->ApplyCommand( "/gps/ion 84 210 0 0" );
        } else {
            UI->ApplyCommand( "/grdm/nucleusLimits 206 206 81 81" );
            UI->ApplyCommand( "/gps/ion 81 206 0 0" );
        }
    }

    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}
