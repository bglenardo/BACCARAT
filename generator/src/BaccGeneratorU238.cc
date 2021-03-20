////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorU238.cc

This is the code file for the U238 generator.
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
#include "BaccGeneratorU238.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorU238()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorU238::BaccGeneratorU238()
{
    name = "U238";
    activityMultiplier = 14;
    ion = G4GenericIon::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorU238()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorU238::~BaccGeneratorU238() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorU238::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorU238::GenerateFromEventList( G4GeneralParticleSource
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
    
    if( probability < 1./14. ) {
        UI->ApplyCommand( "/gps/ion 92 238 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 238 238 92 92" );
    } else if( probability < 2./14. ) {
        UI->ApplyCommand( "/gps/ion 90 234 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 234 234 90 90" );
    } else if( probability < 3./14. ) {
        UI->ApplyCommand( "/gps/ion 91 234 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 234 234 91 91" );
    } else if( probability < 4./14. ) {
        UI->ApplyCommand( "/gps/ion 92 234 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 234 234 92 92" );
    } else if( probability < 5./14. ) {
        UI->ApplyCommand( "/gps/ion 90 230 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 230 230 90 90" );
    } else if( probability < 6./14. ) {
        UI->ApplyCommand( "/gps/ion 88 226 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 226 226 88 88" );
    } else if( probability < 7./14. ) {
        UI->ApplyCommand( "/gps/ion 86 222 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 222 222 86 86" );
    } else if( probability < 8./14. ) {
        UI->ApplyCommand( "/gps/ion 84 218 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 218 218 84 84" );
    } else if( probability < 9./14. ) {
        UI->ApplyCommand( "/gps/ion 82 214 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 214 214 82 82" );
    } else if( probability < 10./14. ) {
        UI->ApplyCommand( "/gps/ion 83 214 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 214 214 83 83" );
    } else if( probability < 11./14. ) {
        if( G4UniformRand() < .99979) {
            UI->ApplyCommand( "/grdm/nucleusLimits 214 214 84 84" );
            UI->ApplyCommand( "/gps/ion 84 214 0 0" );
        } else {
            UI->ApplyCommand( "/grdm/nucleusLimits 210 210 81 81" );
            UI->ApplyCommand( "/gps/ion 81 210 0 0" );
        }
    } else if( probability < 12./14. ) {
        UI->ApplyCommand( "/gps/ion 82 210 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 210 210 82 82" );
    } else if( probability < 13./14. ) {
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
