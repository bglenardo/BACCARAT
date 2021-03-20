////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorRn220.cc
*
* This is the code file for the Rn220 generator.
*
********************************************************************************
* Change log
*	16-February-2015 - file creation (Simon), copying from Rn222 generator
*
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"

//
//	Bacc includes
//
#include "BaccGeneratorRn220.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorRn220()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorRn220::BaccGeneratorRn220()
{
	name = "Rn220";
	activityMultiplier = 5;
	ion = G4GenericIon::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorRn220()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorRn220::~BaccGeneratorRn220() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//void BaccGeneratorRn220::GenerateEvent( G4GeneralParticleSource *particleGun,
//		G4Event *event )
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorRn220::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorRn220::GenerateFromEventList( G4GeneralParticleSource
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

  	//radon220
  	if( probability < 1./5. ) {
  		UI->ApplyCommand( "/gps/ion 86 220 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 220 220 86 86" );
  	}
  	//polonium216
  	else if( probability < 2./5. ) {
  		UI->ApplyCommand( "/gps/ion 84 216 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 216 216 84 84" );
  	}
  	//lead212
  	else if( probability < 3./5. ) {
  		UI->ApplyCommand( "/gps/ion 82 212 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 212 212 82 82" );
  	} 
  	//bismuth212
  	else if( probability < 4./5. ) {
  		UI->ApplyCommand( "/gps/ion 83 212 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 212 212 83 83" );
  	} 		
  	else {
  	//thalium208
        	if( G4UniformRand() < 0.3594 ) {
           	 UI->ApplyCommand( "/gps/ion 81 208 0 0" );
           	 UI->ApplyCommand( "/grdm/nucleusLimits 208 208 81 81" );
            	 } 
  	//polonium212
		else {
           	 UI->ApplyCommand( "/gps/ion 84 212 0 0" );
           	 UI->ApplyCommand( "/grdm/nucleusLimits 212 212 84 84" );
            	}
	}

    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}


