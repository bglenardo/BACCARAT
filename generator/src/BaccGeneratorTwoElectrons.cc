////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorTwoElectrons.cc
*
* This is the code file for the Two Electron generator.
*
********************************************************************************
* Change log
*   22 Jul 13 - Initial submission, for Two Electron event generation. (Matthew)
*
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"
#include "G4Electron.hh"
#include "G4PhysicalConstants.hh"

//
//	Bacc includes
//
#include "BaccGeneratorTwoElectrons.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorTwoElectrons()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorTwoElectrons::BaccGeneratorTwoElectrons()
{
	name = "TwoElectrons";
	activityMultiplier = 1;
	ionDef = G4GenericIon::Definition();
        electronDef = G4Electron::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorTwoElectrons()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorTwoElectrons::~BaccGeneratorTwoElectrons() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorTwoElectrons::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorTwoElectrons::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
  
    G4ThreeVector pos;
    G4double phiAngle = twopi * G4UniformRand();
    G4double angleInRad = 23. * cm * sqrt(G4UniformRand());
    G4double Z = 5.6 * cm + 48.32 * cm * G4UniformRand();
    pos[0] = angleInRad*cos(phiAngle); pos[1] = angleInRad*sin(phiAngle);
    pos[2] = Z;
    
    G4double timeDelay = (firstNode->timeOfEvent)/ns;//stored as seconds
    timeDelay *= ns;
    
    G4double scatterEnergy1 = baccManager->GetTwoElectronsEmin() + (baccManager->GetTwoElectronsEmax() - baccManager->GetTwoElectronsEmin()) * G4UniformRand();
    G4double scatterEnergy2 = baccManager->GetTwoElectronsEmin() + (baccManager->GetTwoElectronsEmax() - baccManager->GetTwoElectronsEmin()) * G4UniformRand();

    particleGun->GetCurrentSource()->SetParticleTime( timeDelay );
    
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
    particleGun->GetCurrentSource()->GetAngDist()->
      SetParticleMomentumDirection( GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->
      SetMonoEnergy( scatterEnergy1  );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

    phiAngle = twopi * G4UniformRand();
    angleInRad = 23. * cm * sqrt(G4UniformRand());
    pos[0] = angleInRad*cos(phiAngle); pos[1] = rad*sin(phiAngle);
    pos[2] = Z + baccManager->GetTwoElectronsDeltaZMax() * (G4UniformRand() - .5);
    
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
    particleGun->GetCurrentSource()->GetAngDist()->
      SetParticleMomentumDirection( GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->
      SetMonoEnergy( scatterEnergy2 );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;


}
