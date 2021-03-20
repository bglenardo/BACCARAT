////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorTh232.cc

This is the code file for the Th232 generator.
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
//    Bacc includes
//
#include "BaccGeneratorTh232.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorTh232()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorTh232::BaccGeneratorTh232()
{
    name = "Th232";
    activityMultiplier = 10;
    ion = G4GenericIon::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorTh232()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorTh232::~BaccGeneratorTh232() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//void BaccGeneratorTh232::GenerateEvent( G4GeneralParticleSource *particleGun,
//        G4Event *event )
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorTh232::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorTh232::GenerateFromEventList( G4GeneralParticleSource
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

      
    if( probability < 1./10. ) {
        UI->ApplyCommand( "/gps/ion 90 232 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 232 232 90 90" );
    } else if( probability < 2./10. ) {
        UI->ApplyCommand( "/gps/ion 88 228 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 228 228 88 88" );
    } else if( probability < 3./10. ) {
        UI->ApplyCommand( "/gps/ion 89 228 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 228 228 89 89" );
    } else if( probability < 4./10. ) {
        UI->ApplyCommand( "/gps/ion 90 228 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 228 228 90 90" );
    } else if( probability < 5./10. ) {
        UI->ApplyCommand( "/gps/ion 88 224 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 224 224 88 88" );
    } else if( probability < 6./10. ) {
        UI->ApplyCommand( "/gps/ion 86 220 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 220 220 86 86" );
    } else if( probability < 7./10. ) {
        UI->ApplyCommand( "/gps/ion 84 216 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 216 216 84 84" );
    } else if( probability < 8./10. ) {
        UI->ApplyCommand( "/gps/ion 82 212 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 212 212 82 82" );
    } else if( probability < 9./10. ) {
        UI->ApplyCommand( "/gps/ion 83 212 0 0" );
        UI->ApplyCommand( "/grdm/nucleusLimits 212 212 83 83" );
    } else {
        if( G4UniformRand() < 0.3594 ) {
            UI->ApplyCommand( "/grdm/nucleusLimits 208 208 81 81" );
            UI->ApplyCommand( "/gps/ion 81 208 0 0" );
        } else {
            UI->ApplyCommand( "/grdm/nucleusLimits 212 212 84 84" );
            UI->ApplyCommand( "/gps/ion 84 212 0 0" );
        }
    }

    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}


