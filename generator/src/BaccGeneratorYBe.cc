////////////////////////////////////////////////////////////////////////////////
/*   BaccGeneratorYBe.cc
*
* This is the code file for the YBe generator.
*
********************************************************************************
* Change log
*   2 May 2014 - Initial submission (Kareem)
*
*/
////////////////////////////////////////////////////////////////////////////////

//
//   General notes on this generator
//
/*

This is a generator for a YBe source. It generates a 1.836 MeV gamma is generated 
9,999 out of 10,000 decays and a 152 keV neutron 1 out of 10,000 decays.

*/

//
//   GEANT4 includes
//
#include "globals.hh"
#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4GenericIon.hh"

//
//   Bacc includes
//
#include "BaccGeneratorYBe.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               BaccGeneratorYBe()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorYBe::BaccGeneratorYBe()
{
   name = "YBe";
   activityMultiplier = 1;
   neutronDef = G4Neutron::Definition();
   gammaDef = G4Gamma::Definition();
   ionDef = G4GenericIon::Definition();
   neutronEnergy = 152.*keV;
   gammaEnergy1 = 1.836063*MeV;
   gammaEnergy2 = .898042*MeV;
   neutronProb = 1e-4;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               ~BaccGeneratorYBe()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorYBe::~BaccGeneratorYBe() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//void BaccGeneratorYBe::GenerateEvent( G4GeneralParticleSource *particleGun,
//      G4Event *event )
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorYBe::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorYBe::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
     //stored in seconds, time in ns
     G4double time = (firstNode->timeOfEvent)/ns;

     G4ThreeVector pos = G4ThreeVector(firstNode->pos);

     G4double prob = G4UniformRand();     
     if(prob < neutronProb) {
     //   generate the neutron
        particleGun->SetParticleDefinition( neutronDef );
        particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
        particleGun->GetCurrentSource()->SetParticleTime( time*ns );
        particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
              GetRandomDirection() );
        G4double singleNeutronEnergy = neutronEnergy;
        particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(
              singleNeutronEnergy*MeV );

        particleGun->GeneratePrimaryVertex( event );
        baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
     }
     else {
     //   generate the 1.836 MeV gamma
        particleGun->SetParticleDefinition( gammaDef );
        particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
        particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( gammaEnergy1 );
        particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection( GetRandomDirection() );

        particleGun->GeneratePrimaryVertex( event );
        baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
     }
     //   generate the 898 gamma
     particleGun->SetParticleDefinition( gammaDef );
     particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
     particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( gammaEnergy2 );
     particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection( GetRandomDirection() );

     particleGun->GeneratePrimaryVertex( event );
     baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}

