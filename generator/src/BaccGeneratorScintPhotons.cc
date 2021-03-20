////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorScintPhotons.cc

This is the code file for the ScintPhotons generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/09/09 - Added include for G4PhysicalConstraints.hh (Brian)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicalConstants.hh"
//
//	Bacc includes
//
#include "BaccGeneratorScintPhotons.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorScintPhotons()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorScintPhotons::BaccGeneratorScintPhotons()
{
	name = "ScintPhotons";
	activityMultiplier = 1;
    photon = G4OpticalPhoton::OpticalPhotonDefinition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorScintPhotons()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorScintPhotons::~BaccGeneratorScintPhotons() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorScintPhotons::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4double time)
{
    G4int z=1; G4int a=1; G4double hl=1;
  	Isotope *currentIso = new Isotope(name, z, a, hl);//
  	baccManager->RecordTreeInsert( currentIso, time, position, 
                  sourceByVolumeID, sourcesID );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorScintPhotons::GenerateFromEventList(G4GeneralParticleSource 
      *particleGun,	G4Event *event, decayNode *firstNode )
{
	particleGun->GetCurrentSource()->SetParticleDefinition( photon );
    G4ThreeVector pos = G4ThreeVector(firstNode->pos);
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

    G4double time = (firstNode->timeOfEvent)/ns;
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );
    
    //determine momentum direction
    direction = GetRandomDirection();
    theta = direction.theta();
    phi = direction.phi();
    //determine polarization (same as G4Scintillation.cc)
    xpol = std::cos( theta) * std::cos( phi );
    ypol = std::cos( theta) * std::sin( phi );
    zpol = -sin ( theta );
    polarization = G4ThreeVector( xpol, ypol, zpol );
    perp = direction.cross(polarization);
    phi = twopi*G4UniformRand();
    sinp = std::sin(phi);
    cosp = std::cos(phi);
    polarization = cosp*polarization + sinp*perp;
    polarization = polarization.unit();

    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
        direction );
    particleGun->GetCurrentSource()->SetParticlePolarization( polarization );
        particleGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Gauss");
	particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 6.97*eV );
	particleGun->GetCurrentSource()->GetEneDist()->SetBeamSigmaInE (0.23*eV);
	
   	particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
}
