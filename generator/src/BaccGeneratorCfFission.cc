////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorFissionNeutrons.cc

This is the code file for the Cf252 fission neutron generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	General notes on this generator
//
/*
	This generator only produces fission neutrons from a Cf252 source. There are
	no accompanying gammas, nor is there the regular Cf252 alpha decay. These
	are all all fission neutrons, all the time.

	This generator assumes a Watt spectrum of neutrons, and so it's based simply
	on an equation:
	
			exp(-E/1.209) * sinh( sqrt(0.836 E) )
	
	where E is the neutron energy. The reference for this equation and the
	choice of parameters is W. Mannhart, "Status of the Cf-252 fission neutron
	spectrum evaluation with regard to recent experiments", available from
	http://www-nds.iaea.org/reports-new/indc-reports/indc-nds/indc-nds-0220.pdf
	Specifically, see absolute pages 331-333 (reference pages 329-331).
	
	The range of neutron energies is taken to be between 1 meV and 15 MeV. To
	obtain the appropriate energy, two random values are chosen, x and y. If
	this point is below the energy curve, that x value is the neutron energy. If
	the point is above the curve, another random point is generated.
	
	To make this work, the maximum value of the curve must be known. That value
	turns out to be a little over 0.47, so the randomly-generated y values are
	selected up to 0.48.
	
	The code for the gamma multiplicity and total energy comes from Timothy E.
	Valentine, "Evaluation of Prompt Fission Gamma Rays for use in Simulating
	Nuclear Safeguard Measurements", ORNL/TM-1999/300, Oak Ridge National
	Laboratory (1999).

	The fission gamma spectrum for a 252Cf source comes from Fig. 8 of
	Verbinski, Weber, and Sund, "Prompt Gamma Rays from 235U(n,f), 239Pu(n,f),
	and Spontaneous Fission of 252Cf", Physical Review C, 7(3) (1973) p.1173-85.
	It was digitized to create a PDF. It was technically in units of photons
	per MeV per fission, but here it's simply being used as a spectrum to sample
	from.

*/

//
//	C/C++ includes
//
#include <vector>

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4Poisson.hh"
#include "Randomize.hh"

//
//	Bacc includes
//
#include "BaccGeneratorCfFission.hh"

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorCfFission()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorCfFission::BaccGeneratorCfFission()
{
	name = "CfFission";
	activityMultiplier = 1;
	neutronDef = G4Neutron::Definition();
	gammaDef = G4Gamma::Definition();

	Z = 98;
	A = 252;
	averageGammaEnergy = -1.33 + 119.6*pow(Z,1./3)/A;
	
	G4double gammaInfo[] = {
			0.141, 3.952,
			0.156, 5.091,
			0.171, 7.718,
			0.194, 8.609,
			0.251, 9.773,
			0.270, 8.303,
			0.301, 7.448,
			0.316, 7.722,
			0.339, 8.927,
			0.361, 10.700,
			0.377, 11.504,
			0.425, 9.035,
			0.448, 8.819,
			0.477, 8.736,
			0.499, 9.773,
			0.535, 8.799,
			0.556, 8.303,
			0.575, 8.455,
			0.594, 9.256,
			0.640, 7.722,
			0.667, 6.927,
			0.689, 6.927,
			0.697, 6.214,
			0.738, 5.998,
			0.776, 5.094,
			0.815, 4.171,
			0.853, 3.355,
			0.929, 2.798,
			0.968, 2.555,
			1.018, 2.305,
			1.079, 2.137,
			1.154, 2.026,
			1.235, 1.946,
			1.258, 1.757,
			1.350, 1.509,
			1.388, 1.378,
			1.426, 1.236,
			1.503, 1.149,
			1.579, 1.049,
			1.679, 0.950,
			1.743, 0.893,
			1.834, 0.855,
			1.880, 0.819,
			1.912, 0.761,
			1.970, 0.720,
			2.016, 0.689,
			2.133, 0.654,
			2.247, 0.620,
			2.267, 0.566,
			2.343, 0.566,
			2.400, 0.526,
			2.446, 0.474,
			2.611, 0.455,
			2.690, 0.383,
			2.840, 0.347,
			2.916, 0.317,
			3.018, 0.269,
			3.117, 0.230,
			3.222, 0.212,
			3.260, 0.190,
			3.356, 0.174,
			3.467, 0.159,
			3.520, 0.147,
			3.649, 0.123,
			3.785, 0.113,
			3.876, 0.103,
			3.966, 0.095,
			4.090, 0.085,
			4.178, 0.078,
			4.284, 0.070,
			4.420, 0.062,
			4.601, 0.050,
			4.738, 0.040,
			4.919, 0.034,
			5.100, 0.030,
			5.237, 0.024,
			5.373, 0.020,
			5.554, 0.017,
			5.736, 0.014,
			5.872, 0.011,
			6.053, 0.009,
			6.235, 0.007,
			6.507, 0.004,
			6.734, 0.003,
			7.051, 0.002,
			7.414, 4.522e-4 };
	
	G4int numPoints = sizeof(gammaInfo)/sizeof(G4double)/2;
	G4double *gammaPDF, totalArea = 0.;
	gammaPDF = new G4double[numPoints];
	for( G4int i=0; i<numPoints; i++ ) {
		gammaEnergy[i] = gammaInfo[i*2];
		gammaPDF[i] = gammaInfo[i*2 + 1];
		totalArea += gammaPDF[i];
	}
	
	for( G4int i=0; i<numPoints; i++ )
		gammaPDF[i] /= totalArea;
	
	gammaCDF[0] = 0.;
	for( G4int i=1; i<numPoints; i++ )
		gammaCDF[i] = gammaCDF[i-1] + gammaPDF[i-1];
	gammaCDF[numPoints-1] = 1.;
	
	delete [] gammaPDF;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorCfFission()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorCfFission::~BaccGeneratorCfFission() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//void BaccGeneratorCfFission::GenerateEvent(
//		G4GeneralParticleSource *particleGun, G4Event *event )
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorCfFission::GenerateEventList( G4ThreeVector position,
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
void BaccGeneratorCfFission::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
    //stored in seconds, time in ns
    G4double time = (firstNode->timeOfEvent)/ns;
    G4ThreeVector pos = G4ThreeVector(firstNode->pos);

  	//	First generate the neutrons
  	particleGun->SetParticleDefinition( neutronDef );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );

  	//	The multiplicity comes from Valentine
  	G4int multiplicity = G4Poisson( 3.757 );
  	for( G4int neutronCount = 0; neutronCount<multiplicity; neutronCount++ ) {
  		particleGun->GetCurrentSource()->GetAngDist()->
  				SetParticleMomentumDirection( GetRandomDirection() );
  		particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(
  				GetNeutronEnergy()*MeV );

  		particleGun->GeneratePrimaryVertex( event );
  		baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
  	}

  	//	Then create the gammas
  	particleGun->SetParticleDefinition( gammaDef );
  	
  	//	The total energy of the gammas is 6.95 +/- 0.3 MeV. I take this to mean
  	//	there is at least a 4% uncertainty in the total gamma energy. This
  	//	generator therefore smears out the total energy by 4% so that we don't
  	//	get a well-defined total gamma energy for any given multiplicity.
  	G4double totalEnergy = (2.51 - 1.13e-5*Z*Z*sqrt(A))*multiplicity + 4.;
  	totalEnergy = G4RandGauss::shoot( totalEnergy, totalEnergy*0.3/6.95 );
  	
  	G4double numGammas1 =  totalEnergy / averageGammaEnergy;
  	G4int numGammas2 = G4Poisson( numGammas1 );
  	G4double runningTotal = 0;
  	vector<G4double> gammaEnergies;
  	for( G4int i=0; i<numGammas2; i++ ) {
  		particleGun->GetCurrentSource()->GetAngDist()->
  				SetParticleMomentumDirection( GetRandomDirection() );
  				
  		G4double singleEnergy;
  		if( i == numGammas2-1 )
  			singleEnergy = totalEnergy;
  		else
  			singleEnergy = GetGammaEnergy( totalEnergy );
  		
  		runningTotal += singleEnergy;		
  		totalEnergy -= singleEnergy;
  		
  		particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(
  				singleEnergy );

  		particleGun->GeneratePrimaryVertex( event );
  		baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
  	}
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetNeutronEnergy()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorCfFission::GetNeutronEnergy()
{
	G4double energy = G4UniformRand() * 15 + 1e-9;
	G4double height = G4UniformRand() * 0.48;

	while( height > (exp(-energy/1.209) * sinh( sqrt(0.836*energy))) ) {
		energy = G4UniformRand() * 15;
		height = G4UniformRand() * 0.48;
	}
	
	return( energy );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetGammaEnergy()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorCfFission::GetGammaEnergy( G4double limit )
{
	G4double energy = limit + 1;

	if( limit < gammaEnergy[0] )
		energy = limit;
	else while( energy > limit ) {
		G4double prob = G4UniformRand();
		G4int indexLo = 0, indexHi = 86;

		while( !(gammaCDF[indexLo+1] > prob && gammaCDF[indexHi-1] < prob) ) {
			if( gammaCDF[(indexLo+indexHi)/2] < prob )
				indexLo = (indexLo + indexHi)/2;
			else
				indexHi = (indexLo + indexHi)/2;
		}
		
		G4double split = (prob - gammaCDF[indexLo]) /
				(gammaCDF[indexHi] - gammaCDF[indexLo]);
		energy = gammaEnergy[indexLo] +
				split*(gammaEnergy[indexHi] - gammaEnergy[indexLo]);
	}
	
	return( energy );
}
