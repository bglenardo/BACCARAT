////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorSingleDecay.cc

This is the code file for the SingleDecay generator.
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
#include "G4GenericIon.hh"

//
//	Bacc includes
//
#include "BaccGeneratorSingleDecay.hh"
#include "BaccBST.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorSingleDecay()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorSingleDecay::BaccGeneratorSingleDecay()
{
	name = "SingleDecay";
	activityMultiplier = 1;
	ion = G4GenericIon::Definition();
    nucleusA = 60;
    nucleusZ = 27;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorSingleDecay()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorSingleDecay::~BaccGeneratorSingleDecay() {}



//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorSingleDecay::GenerateEventList( G4ThreeVector position,
     G4int sourceByVolumeID, G4int sourcesID, G4int a, G4int z, G4double time)
{

    //name, z, e, particlename, energy
    G4double halflife=0;
  	Isotope *currentIso = new Isotope(name, z, a, halflife);
  	baccManager->RecordTreeInsert( currentIso, time, position, 
                  sourceByVolumeID, sourcesID );
		
	
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorSingleDecay::GenerateFromEventList( G4GeneralParticleSource 
        *particleGun, G4Event *event, decayNode *firstNode  )
{

    nucleusZ = firstNode->Z;
    nucleusA = firstNode->A;
    //G4int nucleusZ = firstNode->GetZ();
    //G4int nucleusA = firstNode->GetA();
    G4ThreeVector pos = G4ThreeVector(firstNode->pos );
    G4double time = (firstNode->timeOfEvent)/ns;

	particleGun->GetCurrentSource()->SetParticleDefinition( ion );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );

    //G4IsotopeProperty* G4RIsotopeTable::GetIsotope(G4int Z, G4int A, G4double E)
    //    isoproperty = GetIsotope( nucleusZ, nucleusA, 0. );
    //    isoproperty->GetLifeTime();
    //G4double G4RIsotopeTable::GetMeanLifeTime(G4int Z, G4int A, G4double& aE)
   
    std::stringstream uiStream ;
    uiStream.str("") ;
    uiStream << "/gps/ion " << nucleusZ << " " << nucleusA << " 0 0" ;
    uiString = uiStream.str() ;
	UI->ApplyCommand( uiString );
    
    uiStream.clear() ;  //removes error cache
    uiStream.str("") ;
    uiStream << "/grdm/nucleusLimits " << nucleusA << " " << nucleusA << " " << 
        nucleusZ << " " << nucleusZ ;
    uiString = uiStream.str() ;
	UI->ApplyCommand( uiString ) ;
    
	particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
			GetRandomDirection() );
	particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 0.*keV );

   	particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
}
