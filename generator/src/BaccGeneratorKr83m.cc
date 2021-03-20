////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorKr83m.cc

This is the code file for the Kr83m generator.
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

//
//	Bacc includes
//
#include "BaccGeneratorKr83m.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorKr83m()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorKr83m::BaccGeneratorKr83m()
{
	name = "Kr83m";
	activityMultiplier = 1;
	ionDef = G4GenericIon::Definition();
    electronDef = G4Electron::Definition();
	gammaDef = G4Gamma::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorKr83m()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorKr83m::~BaccGeneratorKr83m() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorKr83m::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorKr83m::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
    // Following arXiv:1209.3658
    // Some energies approximated to fully conserve energy.
  
    G4ThreeVector pos = G4ThreeVector(firstNode->pos);  
    G4double timeDelay = (firstNode->timeOfEvent)/ns;//stored as seconds
    timeDelay *= ns;
    
    G4double transProb = G4UniformRand();
    G4double icEnergy, augerEnergy, gammaEnergy;

    particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
    particleGun->GetCurrentSource()->SetParticleTime( timeDelay );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

    if ( transProb <= 0.0549/100 ) {
      gammaEnergy = 32.1*keV;
      //gamma                                       
      //polarization not set                               
      particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( gammaEnergy );
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }
    else if (transProb <= (9.4/100 + 0.1/100)) {
      icEnergy = 17.9*keV;
      augerEnergy = 10.8*keV;//and two 1.7*keV Auger
      //ic electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(icEnergy);
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
      //auger electrons
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( augerEnergy );
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
      
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( 1.7*keV );// 1.7keV
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
      
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( 1.7*keV );// 1.7keV
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
    }
    else if (transProb <= (11.5/100 + 9.5/100)) {
      icEnergy = 32.1*keV;
      //electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( icEnergy );
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }
    else if (transProb <= (15.3/100 + 21./100)) {
      icEnergy = 17.8*keV;
      gammaEnergy = 12.6*keV;//called "xray" in Manalaysay
      augerEnergy = 1.7*keV;
      //ic electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(icEnergy);
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
      //gamma
      particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );
      particleGun->GetCurrentSource()->GetAngDist()->
        SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
        SetMonoEnergy( gammaEnergy );
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
      //auger electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( augerEnergy );
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }
    else {
      icEnergy = 30.4*keV;
      augerEnergy = 1.7*keV;
      //ic electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(icEnergy);
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
      //auger electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy(augerEnergy);
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }
    
    ////////////////////////////////////////////////////////
    // half life from Kr-83m excited state 154 ns
    G4double timeDelayKr;
    timeDelayKr = - log( 1.-G4UniformRand() )*154./log(2.) *ns  ;//ns
    timeDelayKr += timeDelay;
    
    //timeDelay *= ns;//G4 units
    particleGun->GetCurrentSource()->SetParticleTime( timeDelayKr );
    G4ThreeVector pol; 
    pol[0] = - log( 1.-G4UniformRand() )*154./log(2.); pol[1]=0;pol[2]=0;
    particleGun->GetCurrentSource()->SetParticlePolarization(pol);
    transProb = G4UniformRand();
    
    if ( transProb <= 5.8/100 ) {
      gammaEnergy = 9.4*keV;
      //gamma
      particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( gammaEnergy );
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }
    else if (transProb <= (13.1/100 + 5.8/100)) {
      icEnergy = 9.4*keV;
      //electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
        SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
        SetMonoEnergy( icEnergy );
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }
    else {
      icEnergy = 7.6*keV;
      augerEnergy = 1.8*keV;
      //ic electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(icEnergy);
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
      //auger electron
      particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
      particleGun->GetCurrentSource()->GetAngDist()->
	SetParticleMomentumDirection( GetRandomDirection() );
      particleGun->GetCurrentSource()->GetEneDist()->
	SetMonoEnergy( augerEnergy );      
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }
    
}
