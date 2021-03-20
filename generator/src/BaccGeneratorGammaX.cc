////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorGammaX.cc

This is the code file for the Gamma-X generator.
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
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
//
//	Bacc includes
//
#include "BaccGeneratorGammaX.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorGammaX()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorGammaX::BaccGeneratorGammaX()
{
	name = "gamma-X";
	activityMultiplier = 1;
	ionDef = G4GenericIon::Definition();
    electronDef = G4Electron::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorGammaX()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorGammaX::~BaccGeneratorGammaX() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorGammaX::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorGammaX::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
  
    G4ThreeVector pos;
    G4double phi = twopi * G4UniformRand();
    G4double rad = baccManager->GetGammaXFiducialR() * sqrt(G4UniformRand());
    G4double fiducialHeight = baccManager->GetGammaXFiducialHiZ() - baccManager->GetGammaXFiducialLoZ();
    pos[0] = rad*cos(phi); pos[1] = rad*sin(phi);
    pos[2] = baccManager->GetGammaXFiducialLoZ() + fiducialHeight * G4UniformRand();   
 
    G4double timeDelay = (firstNode->timeOfEvent)/ns;//stored as seconds
    timeDelay *= ns;
    
    G4double upperScatter = G4UniformRand() * baccManager->GetGammaXMaxEnergy();
    G4double lowerScatter = G4UniformRand() * baccManager->GetGammaXMaxEnergy();
 
    particleGun->GetCurrentSource()->SetParticleTime( timeDelay );
    
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
    particleGun->GetCurrentSource()->GetAngDist()->
      SetParticleMomentumDirection( GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->
      SetMonoEnergy( upperScatter );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

    G4double reverseFieldHeight = baccManager->GetGammaXReverseFieldHiZ() - baccManager->GetGammaXReverseFieldLoZ();
    phi = twopi * G4UniformRand();
    rad = baccManager->GetGammaXReverseFieldR() * sqrt(G4UniformRand());    
    pos[0] = rad*cos(phi); pos[1] = rad*sin(phi);
    pos[2] = baccManager->GetGammaXReverseFieldLoZ() + reverseFieldHeight * G4UniformRand();    

    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
    particleGun->GetCurrentSource()->GetAngDist()->
      SetParticleMomentumDirection( GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->
      SetMonoEnergy( lowerScatter );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;


}
