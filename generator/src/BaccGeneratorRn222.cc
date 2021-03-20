////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorRn222.cc

This is the code file for the Rn222 generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
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
#include "BaccGeneratorRn222.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorRn222()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorRn222::BaccGeneratorRn222()
{
	name = "Rn222";
	activityMultiplier = 5;
	ion = G4GenericIon::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorRn222()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorRn222::~BaccGeneratorRn222() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//void BaccGeneratorRn222::GenerateEvent( G4GeneralParticleSource *particleGun,
//		G4Event *event )
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorRn222::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorRn222::GenerateFromEventList( G4GeneralParticleSource
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
  	//radon222
  	if( probability < 1./5. ) {
  		UI->ApplyCommand( "/gps/ion 86 222 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 222 222 86 86" );
  	}
  	//polonium21
  	else if( probability < 2./5. ) {
  		UI->ApplyCommand( "/gps/ion 84 218 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 218 218 84 84" );
  	}
  	//lead214
  	else if( probability < 3./5. ) {
  		UI->ApplyCommand( "/gps/ion 82 214 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 214 214 82 82" );
  	} 
  	//bimuth214
  	else if( probability < 4./5. ) {
  		UI->ApplyCommand( "/gps/ion 83 214 0 0" );
  		UI->ApplyCommand( "/grdm/nucleusLimits 214 214 83 83" );
  	} 		
  	//polonium214
  	else{
  			UI->ApplyCommand( "/grdm/nucleusLimits 214 214 84 84" );
  			UI->ApplyCommand( "/gps/ion 84 214 0 0" );
  	}
  	//ends at decay Po decay to Lead-210

  	particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}


