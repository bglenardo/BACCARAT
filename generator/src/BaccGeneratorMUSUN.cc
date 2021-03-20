////////////////////////////////////////////////////////////////////////////////
/*   BaccGeneratorMUSUN.cc
*
* This is the code file for the MUSUN generator.
*
********************************************************************************
* Change log
*   03-May-2016 - Initial submission (David)
*
*
////////////////////////////////////////////////////////////////////////////////

//
//   General notes on this generator
//
//

This C++ code is a port of the musun-surf.f and test-musun-surf.f code written by Vitaly Kudryavtsev of the University of Sheffield. It generates muonswith energy, position, and direction specific to the Davis cavern at the Sanford Underground Research Facility in Lead, South Dakota.
This C++ code was ported by Kareem Kazkaz, kareem@llnl.gov, (925) 422-7208 and edited by David Woodward, dwoodward1@sheffield.ac.uk.

Here are the notes from Vitaly:

c       The code samples single atmospheric muons at the SURF
c       underground laboratory (Davis' cavern)
c       (taking into account the slant depth distribution)
c       in the energy range E1-E2, zenith angle range theta1-theta2 (0-90 degrees)
c       and azimuthal angle range phi1-phi2 (0-360 degrees).
c       At present only the following ranges of parameters are supported:
c       E1 = 1 GeV, E2 = 10^6 GeV, theta1 = 0, theta2 = 90 deg, phi1 = 0, phi2 = 360 deg.     
c
c       Program uses muon energy spectra at various depths and zenith
c       angles obtained with MUSIC code for muon propagation and Gaisser's
c       formula for muon spectrum at sea level
c       (T.K.Gaisser, Cosmic Rays and Particle Physics, Cambridge
c       University Press, 1990) modified for large zenith angles and
c       prompt muon flux with normalisation and spectral index
c       that fit LVD data: gamma = 2.77, A = 0.14.
c       Density of rock is assumed to be 2.70 g/cm^3 but can be changed
c       during initialisation (previous step, ask the author).
c
c       Muon flux through a sphere (Chao's coordinates) = 6.33x10^(-9) cm^(-2) s^(-1) (gamma=2.77) - old
c       Muon flux through a sphere (Martin's coordinates) = 6.16x10^(-9) cm^(-2) s^(-1) (gamma=2.77) - new
c       Muon flux through the cuboid (30x22x24 m^3) = 0.0588 s^(-1) (gamma=2.77)
c
c       Note: the muon spectrum at sea level does not take into account
c       the change of the primary spectrum slope at and above the knee
c       region (3*10^15-10^16 eV).
c
c       Program uses the tables of muon energy spectra at various
c       zenith and azimuthal angles at SURF
c       calculated with the muon propagation code MUSIC and the
c       angular distribution of muon intensities at SURF (4850 ft level).
c
c       Coordinate system for the muon intensities
c       is determined by the mountain profile provided
c       by Chao Zhang (USD, South Dakota): x-axis is pointing to the East.
c       Muons are sampled on a surface of a rectangular parallelepiped,
c       the planes of which are parallel to the walls of the cavern.
c       The long side of the cavern is pointing at 6.6 deg from the North
c       to the East (or 90 - 6.6 deg from the East to the North).
c       Muon coordinates and direction cosines are then given in the
c       coordinate system related to the cavern with x-axis
c       pointing along the long side of the cavern at 6.6 deg from the
c       North to the East.
c       The angle phi is measured from the positive x-axis towards
c       positive y-axis (0-360 deg).
c       Z-axis is pointing upwards.   
*/

//
//   GEANT4 includes
//
#include "globals.hh"
#include "G4Neutron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Gamma.hh"
#include "G4GenericIon.hh"
#include "G4UnitsTable.hh"

//
//   Bacc includes
//
#include "BaccGeneratorMUSUN.hh"

//
//   C++ includes & definitions
//
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#define PI 3.14159265358979312
#define DEBUGGING 0

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               BaccGeneratorMUSUN()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorMUSUN::BaccGeneratorMUSUN()
{
   name = "MUSUN";
   activityMultiplier = 1;
   
   neutronDef = G4Neutron::Definition();
   muonplusDef = G4MuonPlus::Definition();
   muonminusDef = G4MuonMinus::Definition();
   gammaDef = G4Gamma::Definition();
   ionDef = G4GenericIon::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               ~BaccGeneratorMUSUN()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorMUSUN::~BaccGeneratorMUSUN() {}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorMUSUN::GenerateEventList(G4double time) 
{
  G4ThreeVector position( 0,0,0 );
  G4int a=-1; G4int z=-1;
  G4double hl=-1;
  Isotope *currentIso = new Isotope(name, z, a, hl);
  G4int sourceByVolumeID = 0;
  G4int sourcesID = 0;
  baccManager->RecordTreeInsert( currentIso, time, position,
				sourceByVolumeID, sourcesID );
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorMUSUN::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
  CalculateMuonVariables();
  
  //--- Start time = 0
  G4double time = 0/ns; 

  //--- Get muon position
  //    Convert cm --> mm 
  G4double x0 = GetMuonX0()*10; 
  G4double y0 = GetMuonY0()*10;
  G4double z0 = GetMuonZ0()*10;
  G4ThreeVector pos( x0, y0, z0 ); 
  
  //--- Get muon direction
  G4double cosineX = GetMuonCx();
  G4double cosineY = GetMuonCy();
  G4double cosineZ = GetMuonCz();
  G4ThreeVector directionalCosine( cosineX, cosineY, cosineZ );

  //--- Get muon energy
  G4double muonEnergy = GetMuonE();

  //--- Get muon sign, 10 = pos muon, 11 = neg muon
  G4int muonSign = GetMuonSign();

  //--- Generate the Muon
  if(muonSign==10) particleGun->SetParticleDefinition( muonplusDef );
  else particleGun->SetParticleDefinition ( muonminusDef );
  particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords( pos );
  particleGun->GetCurrentSource()->SetParticleTime( time*ns );
  particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection( directionalCosine );
  particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( muonEnergy*GeV );  

  if(DEBUGGING){
    cout << "\n ==> Generating muon \n_____________________" << endl; 
    cout << "\t Position = " << G4BestUnit(pos[0], "Length") << "," << G4BestUnit(pos[1], "Length") << G4BestUnit(pos[2], "Length") << G4endl;
    cout << "\t Direction = " << directionalCosine << G4endl;
    cout << "\t Energy = " << G4BestUnit(muonEnergy*GeV, "Energy") << endl;    
  }
  
  particleGun->GeneratePrimaryVertex( event );
  baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
     
}

//------++++++------++++++------++++++------++++++------++++++------++++++------                                     
//                    CalculateMuonVariables()
//------++++++------++++++------++++++------++++++------++++++------++++++------    
void BaccGeneratorMUSUN::CalculateMuonVariables()
{
   
  // Note:          // Muon energy range is 1 GeV to 1e6 GeV  
  m_theta1 = 0.;    // Range of zenith angles (degrees): theta1-theta2
  m_theta2 = 90.;   // (theta2>=theta1)
  m_phi1 = 0;       // Range of azimuthal angles (degrees): phi1-phi2
  m_phi2 = 360.;    // (phi2>=phi1)
    
  srand(time(NULL));
  
  //  Muons are sampled on the surface of a sphere with a unit area
  //  perpendicular to the muon flux.
  //  Zenith and azimuthal angles are sampled from the slant depth
  //  distribution (muon intensity distribution) without taking
  //  into account the detector or cavern geometries.

  double s_hor = 1.;
  double s_ver1 = 1.;
  double s_ver2 = 1.;

  //  Alternative way is to sample muons on a surface of a parallelepiped.
  //  If you want muons to be sampled on the surface of a parallelepiped
  //  decomment the following lines and specify the size (the areas of
  //  the planes) of a parallelepiped in cm.
    
  bool igflag = true;
    
  //  You have to change all these dimensions if you change the size of the
  //  parallelepiped. The dimensions are given in cm. The example shown here
  //  is for the lab+rock having the size of 30 x 24 x 24 m^3 in x, y and z,
  //  respectively. The parallelepiped on the surface of which the muons are
  //  sampled is extended by 5 m in all directions into rock, except top where
  //  the extension is 7m. The centre of the coordinate system is the centre
  //  of the water tank (approximately). **This is wrong, the centre of the 
  //  coordinate system is actually the cathode wire. This means that
  //  the values of zh1 and zh2 are changed from -800 to -675 and 1600 to 1725 
  //  respectively.**
  
  double zh1 = -675.;
  double zh2 = +1725.;
  double xv1 = -1400.;
  double xv2 = +1600.;
  double yv1 = -1200.;
  double yv2 = +1200.;
    
  // area of the horizontal plane of the parallelepiped
  s_hor = (xv2-xv1)*(yv2-yv1);
  // area of the vertical plane of the parallelepiped, perpendicular to x-axis
  s_ver1 = (yv2-yv1)*(zh2-zh1);
  // area of the vertical plane of the parallelepiped, perpendicular to y-axis
  s_ver2 = (xv2-xv1)*(zh2-zh1);   
    
  //---- perform the intialisation; 
  //     theta1 & theta2 are muon zenith bounds 
  //     phi1 & phi2 and azimuthal bounds, s_hor, s_ver1 & s_ver2 are related to the direction 
  //     components of the muons on the surface of the sphere.
  static bool firstInit=true;
  if ( firstInit ) Initialization(igflag,s_hor,s_ver1,s_ver2); firstInit=false;
  
  double E, theta, phi, dep;
  Sampling( &E, &theta, &phi, &dep );
    
  m_theta1 = theta;
  theta *= PI / 180.;

  //  changing the angle phi so x-axis is positioned along the long side
  //  of the cavern pointing at 14 deg from the North to the East from
  //  Josh Whillhite
  double phi_cavern = phi - 90. + 14.; //--- azimuth relative to the cavern
  phi += (-90. - 14.0);                //--- azimuth relative to the original LUXSim coordinate system
  if( phi >= 360. )
    phi -= 360.;
  if( phi < 0 )
    phi += 360.;
  m_phi1 = phi;
  phi *= PI / 180.;
  
  //  Muon sign (particle ID - GEANT3 definition)
  int id_part = 10;   //  positive muon
  if( G4UniformRand() < 1./2.38 )
    id_part = 11;     //  mu+/mu- ratio is 1.38; change this if necessary
 
  //  The minus sign is for z-axis pointing up, so the z-momentum
  //  is always pointing down.  
  double cx = -sin(theta)*cos(phi);
  double cy = -sin(theta)*sin(phi);
  double cz = -cos(theta);
  
  //  Muon coordinates
  double sh1 = s_hor * cos(theta);
  double sv1 = s_ver1 * sin(theta) * fabs(cos(phi_cavern));
  double sv2 = s_ver2 * sin(theta) * fabs(sin(phi_cavern));
  double ss = sh1 + sv1 + sv2;
  double x0, y0, z0;
  double yfl1 = G4UniformRand();

  //--- pick a surface with a probability weighted by the angle of the muon, then pick a random point on this surface.
  if( yfl1 <= sh1/ss ) {
    x0 = (xv2 - xv1)*G4UniformRand() + xv1;
    y0 = (yv2 - yv1)*G4UniformRand() + yv1;
    z0 = zh2;
  } else if( yfl1 <= (sh1+sv1)/ss ) {
    if( cx >= 0 ) 
      x0 = xv1;
    else
      x0 = xv2;
    y0 = (yv2 - yv1)*G4UniformRand() + yv1;
    z0 = (zh2 - zh1)*G4UniformRand() + zh1;
  } else {
    x0 = (xv2 - xv1)*G4UniformRand() + xv1;
    if( cy >= 0 )
      y0 = yv1;
    else
      y0 = yv2;
    z0 = (zh2 - zh1)*G4UniformRand() + zh1;
  }
  
  //--- convert the x, y, z to the original LUXSim coordinate system
  double azimuth_conversion = 28*(PI/180);
  double xi=x0;
  double yi=y0;
  x0=xi*cos(azimuth_conversion) + yi*sin(azimuth_conversion);
  y0=-xi*sin(azimuth_conversion) + yi*cos(azimuth_conversion);

  //      E - total muon energy in GeV assuming ultrarelativistic muons
  //      x0, y0, z0 - muon coordinates on the surface of parallelepiped
  //          specified above; x-axis and y-axis are pointing in the
  //          directions such that the angle phi (from the slant depth
  //          distribution files) is measured from x to y. z-axis is
  //          pointing upwards.
  //      cx, cy, cz - direction cosines.
  
  //--- set the variables
  SetMuonX0(x0);
  SetMuonY0(y0);
  SetMuonZ0(z0);
  SetMuonE(E);
  SetMuonSign(id_part);
  SetMuonCx(cx);
  SetMuonCy(cy);
  SetMuonCz(cz);

}


//------++++++------++++++------++++++------++++++------++++++------++++++------                                     
//                    Initialization()                                                                         
//------++++++------++++++------++++++------++++++------++++++------++++++------    
void BaccGeneratorMUSUN::Initialization( bool igflag, double s_hor, double s_ver1, double s_ver2 )
{
  //
  // Read in the data files
  //
  //--- muint-davis-mr-new.dat contains muon intensities (units cm^-2 s^-1 sr^-1) expressed as log(I).
  //    91 zenith angles (0 - 90) and 360 azimuth angles (0 - 359)
  //--- musp-davis-mr-new.dat contains energy spectra (integrated number of muons) for values of theta and depth.
  //    121 energy values expressed as log(E) from E = 1 GeV to 1e6 GeV (width = 0.05) << first value for each spectra is 0 so there are 120 'bins'
  //    62 depth values in m w.e. from 3200 m w.e. to 15400 m w.e. (width = 200 m w.e.)
  //    51 theta values expressed as cos(theta) from 1 to 0 (width = 0.02)
  //--- depth array contains conents of depth-davis-mr-new.dat (muon depths - each array is a zenith angle and there are 360 azimuthal angles)
  
  int lineNumber = 0, index = 0;
  char inputLine[10000]; 
  
  FILE *infile;
  if ((infile=fopen("generator/data/muint-davis-mr-new.dat","r")) == NULL){
    G4cout << "Error: Cannot find the muint-davis-mr-new.dat file" << G4endl;
  }
  ifstream file3( "generator/data/muint-davis-mr-new.dat", ios::in ); 
  while( file3.good() ) { 
    file3.getline( inputLine, 9999 ); 
    char *token;
    token = strtok( inputLine, " " ); 
    while( token != NULL ) {
      fmu[index][lineNumber] = atof( token );       
      token = strtok( NULL, " " );
      index++;
      if( index == 360 ) {
	index = 0;
	lineNumber++;
      }
    }
  }
  file3.close();   
  
  ifstream file2( "generator/data/musp-davis-mr-new.dat", ios::binary|ios::in );
  int i1 = 0, i2 = 0, i3 = 0;
  int nEBins = 121;
  int nDepBins = 62;
  int nThetaBins = 51;
  float readVal;
  while( file2.good() ) {
    file2.read((char *)(&readVal), sizeof(float));
    spmu[i1][i2][i3] = readVal;    
    i1++;
    if( i1 == nEBins ) {
      i2++;
      i1 = 0;
    }
    if( i2 == nDepBins ) {
      i3++;
      i2 = 0;
    }
  }
  file2.close();
  //--- this fix is neccessary to properly align values to the correct indicies
  for( int i=0; i<nEBins-1; i++ )
    for( int j=0; j<nDepBins; j++ )
      for( int k=0; k<nThetaBins; k++ )
	spmu[i][j][k] = spmu[i+1][j][k];
  spmu[1][1][0] = 0.000853544;
    
  ifstream file1( "generator/data/depth-davis-mr-new.dat", ios::in );
  lineNumber = index = 0;
  while( file1.good() ) {
    file1.getline( inputLine, 9999 );
    char *token;
    token = strtok( inputLine, " " );
    while( token != NULL ) {
      depth[index][lineNumber] = atof( token );
      token = strtok( NULL, " " );
      index++;
      if( index == 360 ) {
	index = 0;
	lineNumber++;
      }
    }
  }
  file1.close();

 
  //
  // Set up muon cdf to sample theta & phi
  // 
  double theta = m_theta1;
  double phi = m_phi1;
  double dt = 1.; //--- theta interval
  double dp = 1.; //--- phi interval
  double theta_rad = 0., phi_rad = 0.;
  double ctheta = 0.;
  double mu_flux = 0.;
  double mu_sec = 0.;
  double area_hor, area_ver01, area_ver02, area_ver1, area_ver2, area_tot;
  int ii_cdf = 1;

  while(theta < m_theta2-dt/2.){     //--- loop from theta1 --> theta2
    theta+=dt/2.;                    //--- increment theta
    theta_rad = PI/180.*theta;       
    ctheta=cos(theta_rad);
    area_hor = s_hor*ctheta;
    area_ver01 = s_ver1*sqrt(1. - ctheta*ctheta);
    area_ver02 = s_ver2*sqrt(1. - ctheta*ctheta);
    
    int itheta = (int)theta; //--- round down

    phi = m_phi1;
    while( phi < m_phi2-dp/2.){
      phi+=dp/2.;
      
      //--- the long side of the cavern is pointing 14 degs from the north
      //--- the next few lines changes the cooridnate system so that it is relative to the cavern
      phi_rad = PI/180.*(phi - 90. + 14.);
      area_ver1 = area_ver01 * fabs(cos(phi_rad));
      area_ver2 = area_ver02 * fabs(sin(phi_rad));
      area_tot = 1.;
      if(igflag) area_tot = area_hor + area_ver1 + area_ver2; //--- total area muons pass through
      
      int iphi = (int)phi; //--- round down

      mu_flux = 0.;        //--- initialize mu_flux = 0

      for( int ii=0; ii<4; ii++){
	int ii_theta = itheta+ii/2;  //--- +0, +0, +1, +1
	int ii_phi = iphi+ii%2;      //--- +0, +1, +0, +1	
	if(ii_phi==360) ii_phi=0;

	//--- find the 4 nearest flux values and take average
	if(fmu[ii_phi][ii_theta] < 0)
	  mu_flux = mu_flux + pow(10.,fmu[ii_phi][ii_theta]) / 4;
      }
      mu_sec = mu_sec + mu_flux * area_tot * dp * PI/180. * sin(theta_rad) * dt * PI/180.; //--- mu_flux is given in units muons/cm^2/sr/s so mu_sec is muons/sec for this angular bin 

      mu_cdf[ii_cdf] = mu_sec;  //--- cumulative muons/s
      ii_cdf+=1;                //--- total size of muon CDF

      phi = phi+dp/2.;
    }

    theta = theta+dp/2.;
  }

  //--- create a muon cumulative distribution function
  int cdf_size = ii_cdf;
  for(ii_cdf=0; ii_cdf<cdf_size; ii_cdf++)
    mu_cdf[ii_cdf] = mu_cdf[ii_cdf]/mu_cdf[cdf_size-1];


  G4cout << "Finished initialization..." << G4endl;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               Sampling()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorMUSUN::Sampling( double *E, double *theta, double *phi, double *dep )
{
  // Sample theta and phi from the muon flux CDF. There are 90 x 360 + 1 elements in a 1D array.
  // This corresponds to the 90 bins in theta and 360 bins in phi, with the first bin in the array
  // set to equal 0.
  double ranNum = G4UniformRand();
  int i=1; //--- start at 1 because mu_cdf[0] = 0
  while( ranNum>mu_cdf[i] ) ++i;
  int theta_bin = (i-1)/360;
  int phi_bin = (i-1)-theta_bin*360;  

  ranNum = G4UniformRand();
  *theta = m_theta1 + 1.*((double)theta_bin+ranNum);
  ranNum = G4UniformRand();
  *phi = m_phi1 + 1.*((double)phi_bin+ranNum);
  *dep = depth[phi_bin][theta_bin] * 2.70;
  //---


  // Sample muon energy from muon flux CDF. This is a 3D array; energy, depth, cos(theta).
  theta_bin = cos(M_PI/180.**theta) * 50.; //--- cos(theta) runs from 0 - 1 in 0.02 bins
  int dep_bin = *dep / 200. - 16;          //--- dep runs from 3200 m w.e. to 15400 m w.e. in 200 m w.e. bins  
  if(dep_bin>61) dep_bin = 61;             //--- max. depth bin is 61 (15400 m w.e.)

  ranNum = G4UniformRand();
  int j = 0;
  while ( ranNum > spmu[j][dep_bin][theta_bin] ) ++j;

  //--- spmu = 0 for j=0, so j=N gives integral from 10^((N-1)*0.05) GeV to 10^(N*0.05) GeV
  double En1 = 0.05 * (j-1);               
  double En2 = 0.05 * (j);
  *E = pow(10.,En1 + (En2 - En1)*G4UniformRand());

  return;

}
