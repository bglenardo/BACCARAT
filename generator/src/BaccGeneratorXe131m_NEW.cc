////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorXe131m_NEW.cc

This is the code file for the Xe131m_NEW generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	C++ includes
//
	#include <iostream>
	#include <stdlib.h>	/* rand */
	#include <fstream>	/* ofstream */
	#include <math.h>       /* log, sqrt */

//
//    GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"
#include "G4Gamma.hh"
	#include "G4Electron.hh"


//
//     Bacc includes
//
#include "BaccGeneratorXe131m_NEW.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorXe131m_NEW()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorXe131m_NEW::BaccGeneratorXe131m_NEW()
{
    name = "Xe131m_NEW";
    activityMultiplier = 1;
    ion = G4GenericIon::Definition();
    gammaDef = G4Gamma::Definition();
	electronDef = G4Electron::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorXe131m_NEW()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorXe131m_NEW::~BaccGeneratorXe131m_NEW() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorXe131m_NEW::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorXe131m_NEW::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{

	G4double probs[] = 	{
			 	0.00000, 
				0.05290, 0.08260, 0.09950, 0.14400, 0.25650, 
				0.54803, 0.54845, 0.56635, 0.58595, 0.61183, 
				0.61393,  
				0.75505, 0.76115, 0.76725, 
				0.89993, 0.96563, 0.97903, 0.98050, 1.00000, 
				1.00100
				};
G4double photon1_Energy[]={ 0., 033.629, 033.562, 034.402, 029.452, 029.452, 029.781, 004.280, 000.000, 004.280, 000.000, 000.000, 000.000, 000.300, 000.000, 000.000, 000.000, 000.000, 000.000, 163.930, 0. }; 
G4double photon2_Energy[]={ 0., 000.000, 000.000, 000.000, 004.280, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 0. };
G4double photon3_Energy[]={ 0., 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 004.280, 004.280, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 0. };	//randomize



G4double electr1_Energy[]={ 0., 129.370, 129.370, 129.370, 129.370, 129.370, 129.370, 129.370, 129.370, 129.370, 129.370, 129.370, 158.790, 158.790, 158.790, 158.790, 162.890, 163.740, 163.910, 000.000, 0. };
G4double electr2_Energy[]={ 0., 000.931, 000.998, 000.158, 000.828, 003.430, 003.430, 024.500, 024.500, 029.200, 024.500, 033.400, 004.090, 004.840, 004.840, 003.430, 001.040, 000.190, 000.020, 000.000, 0. };
G4double electr3_Energy[]={ 0., 000.000, 000.000, 000.000, 000.000, 001.678, 001.349, 000.000, 003.430, 001.080, 003.430, 001.160, 001.050, 000.000, 000.300, 001.710, 000.000, 000.000, 000.000, 000.000, 0. };
G4double electr4_Energy[]={ 0., 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 004.090, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 0. };
G4double electr5_Energy[]={ 0., 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 002.540, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 0. };
G4double electr6_Energy[]={ 0., 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 001.500, 002.350, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 000.000, 0. };	//randomize

        if( !(event->GetEventID()%500) ) { G4cout << "Processing event number " << event->GetEventID() << G4endl; }

	G4double pr_seed =  G4UniformRand();
	G4double flat_rand_one = G4UniformRand() ;
	G4double flat_rand_two = G4UniformRand() ;

    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
            GetRandomDirection() );

    G4double time = (firstNode->timeOfEvent)/ns;
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );

    G4ThreeVector pos = G4ThreeVector(firstNode->pos);
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

	for( G4int n = 1; n < 21; n = n + 1 ) 
	{
		if( pr_seed < probs[n] && pr_seed > probs[n-1] ) 
		{
			if( photon1_Energy[n] != 0. )
			{	
				//--- gamma source 1
				particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );	
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( photon1_Energy[n]*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

				particleGun->GeneratePrimaryVertex( event );
  				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}
			if( photon2_Energy[n] != 0. )
			{	
				//--- gamma source 2
				particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );	
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( photon2_Energy[n]*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

				particleGun->GeneratePrimaryVertex( event );
				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}

			if( photon3_Energy[n] != 0. )
			{	
				G4double rand_gamma_energy = photon3_Energy[n] + .66*(flat_rand_one - flat_rand_two) ;	//Spreads out the input photon energy
				//--- gamma source 3
				particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );	
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( rand_gamma_energy*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

				particleGun->GeneratePrimaryVertex( event );
				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

				//--- electron source 6
				particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( ( electr6_Energy[n]+photon3_Energy[n]-rand_gamma_energy )*keV );	//matches the coresponding electron energy to the spread photon energy
				//G4cout <<  electr6_Energy[n]+photon3_Energy[n]-rand_gamma_energy  << event->GetEventID() << G4endl;
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

				particleGun->GeneratePrimaryVertex( event );
  				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}
			if( electr1_Energy[n] != 0. )
			{	
				//--- electron source 1
				particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( electr1_Energy[n]*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);	

				particleGun->GeneratePrimaryVertex( event );
				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}
			if( electr2_Energy[n] != 0. )
			{	
				//--- electron source 2
				particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( electr2_Energy[n]*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
				
				particleGun->GeneratePrimaryVertex( event );
				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}
			if( electr3_Energy[n] != 0. )
			{	
				//--- electron source 3
				particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( electr3_Energy[n]*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);	

				particleGun->GeneratePrimaryVertex( event );
				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}
			if( electr4_Energy[n] != 0. )
			{	
				//--- electron source 4
				particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( electr4_Energy[n]*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

				particleGun->GeneratePrimaryVertex( event );
				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}
			if( electr5_Energy[n] != 0. )
			{	
				//--- electron source 5
				particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
				particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
				    GetRandomDirection() );
				particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( electr5_Energy[n]*keV );
				particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);	
				
				particleGun->GeneratePrimaryVertex( event );
				baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

			}
			
		}
		else { continue; }
	}
}

