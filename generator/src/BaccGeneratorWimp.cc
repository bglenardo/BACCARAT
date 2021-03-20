/////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorWimp.cc

This is the code file for the Wimp generator.
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
#include "G4PhysicalConstants.hh"
//
//	Bacc includes
//
#include "BaccGeneratorWimp.hh"

//
//  Other Includes
//
#include <sstream>

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorWimp()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorWimp::BaccGeneratorWimp()
{
	name = "Wimp";
	activityMultiplier = 5;
	ion = G4GenericIon::Definition();

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorWimp()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorWimp::~BaccGeneratorWimp() {}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					dR()
//------++++++------++++++------++++++------++++++------++++++------++++++------

//This spectrum comes from Phys. Rev. D 82 (2010) 023530 (McCabe)
G4double BaccGeneratorWimp::dR(G4double ER, G4double mWimp, G4double mXenon){
  // We are going to hard code in the astrophysical halo for now.  This may be 
  // something that we make an argument later, but this is good enough to start.
  // Some constants:
  G4double M_p = 0.93827; //Proton mass [GeV]
  G4double N_A = 6.022e23;//Avagadro's number [atoms/mol]
  G4double c = 2.998e10;  //Speed of light [cm/s]
  G4double GeVperAMU = 0.9315;             //Conversion factor
  G4double SecondsPerDay = 60. * 60. * 24.;//Conversion factor
  G4double KiloGramsPerGram = 0.001;       //Conversion factor
  G4double keVperGeV = 1.e6;               //Conversion factor
  G4double cmPerkm = 1.e5;                 //Conversion factor
  G4double SqrtPi = pow(pi, 0.5);
  // Convert all velocities from km/s into cm/s
  G4double v_0   = 220.  * cmPerkm;
  G4double v_esc = 544.  * cmPerkm;
  G4double v_e   = 225.6 * cmPerkm;


  // Define the detector Z and A and the mass of the target nucleus
  G4double Z = 54.;
  G4double A = 131.293;
  G4double M_T = A * GeVperAMU;
  
  // Calculate the number of target nuclei per kg
  G4double N_T = N_A / (A * KiloGramsPerGram);

  // Rescale the recoil energy and the inelastic scattering parameter into GeV
  ER /= keVperGeV;
  G4double delta = 0. / keVperGeV; //Setting this to a nonzero value will allow
                                   //for inelastic dark matter...
  // Set up your dummy WIMP model (this is just to make sure that the numbers 
  // came out correctly for definite values of these parameters, the overall 
  // normalization of this spectrum doesn't matter since we generate a definite 
  // number of events from the macro).
  G4double rho_D = 0.3;      // [GeV/cm^3]
  G4double m_d   = 25.;      // [GeV]
  G4double sigma_n = 1.e-45; //[cm^2]
  // Calculate the other factors in this expression
  G4double mu_ND = mWimp * M_p / (mWimp + M_p);// WIMP-nucleON reduced mass
  G4double mu_TD = mWimp * M_T / (mWimp + M_T);// WIMP-nucleUS reduced mass
  G4double fp = 1.;// Neutron and proton coupling constants for WIMP interactions.
  G4double fn = 1.;

  // Calculate the minimum velocity required to give a WIM P with energy ER
  G4double v_min = 0.;
  if(ER != 0.){
    v_min = c * (((M_T * ER) / mu_TD) + delta) / sqrt(2. * M_T * ER);
  }

  // Start calculating the differential rate for this energy bin, starting 
  // with the velocity integral:
  G4double x_min = v_min / v_0;// Use v_0 to rescale the other velocities
  G4double x_e   = v_e   / v_0;
  G4double x_esc = v_esc / v_0;
  // Calculate overall normalization to the velocity integral
  G4double N = pow(SqrtPi * v_0, 3.) * (erf(x_esc) - ((4. / SqrtPi) * exp(-1. * x_esc * x_esc)) * ((x_esc / 2.) + ((x_esc*x_esc*x_esc) / 3.)));
  // Calculate the part of the velocity integral that isn't a constant
  G4double zeta = 0.;
  G4int thisCase = -1;
  if((x_e + x_min) < x_esc){thisCase = 1;}
  if((x_min > (x_esc - x_e)) && ((x_e + x_esc) > x_min)){thisCase = 2;}
  if(x_e > (x_min + x_esc)){thisCase = 3;}
  if((x_e + x_esc) < x_min){thisCase = 4;}
  switch(thisCase){
    case 1:
      zeta =  erf(x_min + x_e) - erf(x_min - x_e);
      zeta -= (4. * x_e / SqrtPi) * exp(-1. * x_esc * x_esc) * (1. + (x_esc * x_esc) - (x_e * x_e / 3.) - (x_min * x_min * x_min));
      zeta *= pow(SqrtPi,3.) * (v_0 * v_0) / (2. * N * x_e);
      break;
    case 2:
      zeta =  erf(x_esc) + erf(x_e - x_min);
      zeta -= (2. / SqrtPi) * exp(-1. * x_esc * x_esc) * (x_esc + x_e - x_min - (0.33333 * (x_e - (2. * x_esc) - x_min) * (pow(x_esc + x_e - x_min, 2.))));
      zeta *= pow(SqrtPi, 3.) * (v_0 * v_0) / (2. * N * x_e);
      break;
    case 3:
      zeta = 1. / (x_e * v_0);
      break;
    case 4:
      zeta = 0.;
      break;
    default:
      G4cout << "\tThe velocity integral in the WIMP generator broke!!!" << G4endl;
  }
  // Now do the Helm form factor
  G4double hbarc_MeVfm = 197.3; //[MeV fm]
  // Rescale energy to MeV!!!
  G4double MeVperGeV = 1000.;//Conversion factor
  G4double m_T = M_T * MeVperGeV;
  G4double er = ER * MeVperGeV;
  // Momentum transfer
  G4double q = sqrt(2. * m_T * er);
  // R
  G4double C = (1.23 * pow(A, 0.3333)) - 0.6; //[fm]
  G4double a = 0.52;                          //[fm]
  G4double st = 0.9;                           //[fm]
  G4double R = sqrt((C * C) + ((7./3.) * pi * a *a) - (5. * s *s));

  // Evaluate the spherical Bessel function
  G4double qR = q * R / hbarc_MeVfm;
  G4double j1 = 0.;
  if(qR > 0.){
    j1 = (sin(qR) / (qR * qR)) - (cos(qR) / qR);
  }
  // Calculate the form factor at this energy
  G4double FormFactor = 1.;
  G4double qs = q * st / hbarc_MeVfm;
  if(qR > 0.){
    FormFactor = pow(3. * j1 / qR, 2.) * exp(-1. * qs * qs);
  }
  // Rescale for finite momentum transfer
  G4double qRef = 100.; //[MeV]
  FormFactor *= (q * q) / (qRef * qRef);

  // Now, the differential spectrum for this bin!
  G4double dSpec  = 0.5 * (c * c) * N_T * (rho_D / m_d) * (M_T * sigma_n / (mu_ND * mu_ND));
           dSpec *= pow(((Z * fp) + ((A - Z) * fn)) / fn, 2.) * zeta * FormFactor * SecondsPerDay / keVperGeV;

  return dSpec;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorWimp::GenerateEventList( G4ThreeVector position, 
            G4int sourceByVolumeID, G4int sourcesID, G4double wimpMass,
            G4double time)
{
    G4int a=-1; G4int z=-1;
    G4String pname="Wimp";
  	Isotope *currentIso = new Isotope(name, z, a, pname, wimpMass);
    baccManager->RecordTreeInsert(currentIso,time, position,sourceByVolumeID,sourcesID);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorWimp::GenerateFromEventList( G4GeneralParticleSource 
            *particleGun, G4Event *event, decayNode *firstNode )
{
    std::stringstream command (std::stringstream::in | std::stringstream::out);
  
    G4ThreeVector pos = G4ThreeVector(firstNode->pos);
    G4double timeDelay = (firstNode->timeOfEvent)/ns;

    G4double XeData[9][3] = {
    {132, 26.9086, 122.8679},
    {129, 26.4006, 120.074},
    {131, 21.2324, 121.9373},
    {134, 10.4357, 124.7321},
    {136, 8.857, 126.5968},
    {130, 4.071, 121.0043},
    {128, 1.9102, 119.1414},
    {124, .0953, 115.4176},
    {126, .0892, 117.2791},
    };

    particleGun->GetCurrentSource()->SetParticleDefinition( ion );
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
		        GetRandomDirection() );
    particleGun->GetCurrentSource()->SetParticleTime( timeDelay*ns );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);

  	//Determine the isotope of Xenon;
  	G4int isotope = 0;
  	for(G4double i = 100.0*G4UniformRand(); i > XeData[isotope][1]; isotope++){
  		i -= XeData[isotope][1];
  	}

  	//Set the energy
    G4double wimpMass = (firstNode->energy)/GeV;//saved as kev
  	//Set the energy (a many step process)
  	//Find the zero point.
  	G4double zeroPoint = 1;
  	G4double lowerBound = 0;
  	while(dR(zeroPoint,wimpMass,XeData[isotope][2]) > 0) zeroPoint *= 2;
  	for(G4int i = 0; (zeroPoint - lowerBound)/zeroPoint > 1e-13; i++){
  		if(dR((zeroPoint+lowerBound)/2,wimpMass,XeData[isotope][2]) > 0){
  			lowerBound = (zeroPoint+lowerBound)/2;
  		}
  		else{
  			zeroPoint = (zeroPoint+lowerBound)/2;
  		}
  	}
  	zeroPoint = (zeroPoint+lowerBound)/2;

  	//Integrate dR to get the probability distribution
  	G4double Rdist [5000];
  	Rdist[4999] = 0;
  	for(G4int i = 4998; i >= 0; i--){ //integration: trapezoids
    		Rdist[i] = dR(i*zeroPoint/4999.,wimpMass,XeData[isotope][2]);
    		Rdist[i] += dR((i+1)*zeroPoint/4999.,wimpMass,XeData[isotope][2]);
    		Rdist[i] /=2;
    		Rdist[i] *= (zeroPoint/4999.); //width
    		Rdist[i] += Rdist[i+1]; //last bin evaluated
  	}

  	//Normalize the distribution
  	G4double sum = 0;
  	for(G4int i = 0; i < 5000; i++) sum += Rdist[i];
  	for(G4int i = 0; i < 5000; i++) Rdist[i] /= sum;

  	//Find the bin to get the energy from
  	G4double prob = G4UniformRand(); //the value to search the distribution with
  	G4int bin = 0;
  	while(bin < 5000){
  		prob -= Rdist[bin];
  		if(prob <= 0) break;
  		bin++;
  	}

  	//Select an energy from the bin
  	prob = G4UniformRand();
  	G4double recoilEnergy = zeroPoint*((bin+prob)/4999.);

  	//Finally, set the energy
  	particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 
                  recoilEnergy*keV );

  	//Set the ion
    command << "/gps/ion 54 " << XeData[isotope][0] << " 0 0" << std::endl;
  	
  	//Input the command string
  	UI->ApplyCommand(command.str());

  	particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}
