////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorbb0n.cc
*
* This is the code file for the bb0n generator.
*
********************************************************************************
* Change log
*   10-Jul-26 - Initial submission (Copilot)
*/
////////////////////////////////////////////////////////////////////////////////

//
//    GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4PhysicalConstants.hh"

#include <algorithm>
#include <cmath>

//
//    Bacc includes
//
#include "BaccGeneratorbb0n.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorbb0n()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorbb0n::BaccGeneratorbb0n()
{
    name = "bb0n";
    activityMultiplier = 1;
    ionDef = G4GenericIon::Definition();
    electronDef = G4Electron::Definition();
    gammaDef = G4Gamma::Definition();
    qValue = 2457.8 * keV;
    maxIterations = 100;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorbb0n()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorbb0n::~BaccGeneratorbb0n() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorbb0n::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4double time )
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
void BaccGeneratorbb0n::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode )
{
    particleGun->SetParticleDefinition( electronDef );

    // Time is stored in seconds in the event list and converted back to ns.
    G4double timeDelay = (firstNode->timeOfEvent)/ns;
    timeDelay *= ns;

    G4ThreeVector decayVertex = G4ThreeVector(firstNode->pos);
    particleGun->GetCurrentSource()->SetParticleTime( timeDelay );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(decayVertex);

    // K = T1 + T2 in units of electron mass.
    G4double t0 = qValue / electron_mass_c2;
    G4double k = t0;
    G4double t1 = 0.;
    G4double t2 = 0.;

    G4double dSpectralMax = Dbb0nSpectralMax(k);

    G4int n = 0;
    while ( n < maxIterations ) {
        G4double d = k * (2.*G4UniformRand() - 1.);
        G4double dSpectral = dSpectralMax * G4UniformRand();

        if ( dSpectral <= Dbb0nSpectrum(k, d) ) {
            t1 = 0.5 * (k + d);
            t2 = 0.5 * (k - d);
            break;
        }
        n++;
    }

    t1 *= electron_mass_c2;
    t2 *= electron_mass_c2;

    // Allow optional external scaling through user variable 1.
    G4double eScale = 1.;
    if ( baccManager->GetUserVar1() > 0. )
        eScale = baccManager->GetUserVar1() / qValue;

    // Generate first electron isotropically.
    G4double cosTheta1 = 2.*G4UniformRand() - 1.;
    G4double sinTheta1 = sqrt(1. - cosTheta1*cosTheta1);
    G4double phi1 = twopi * G4UniformRand();
    G4ThreeVector u1( sinTheta1*cos(phi1), sinTheta1*sin(phi1), cosTheta1 );

    particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( t1 * eScale );
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection( u1 );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );

    // Generate second electron with 1 - beta1*beta2*cos(theta12) distribution.
    G4double e1 = t1 + electron_mass_c2;
    G4double e2 = t2 + electron_mass_c2;
    G4double p1 = sqrt(e1*e1 - electron_mass_c2*electron_mass_c2);
    G4double p2 = sqrt(e2*e2 - electron_mass_c2*electron_mass_c2);
    G4double beta1 = p1 / e1;
    G4double beta2 = p2 / e2;

    G4double theta12 = 0.;
    G4double phi2 = 0.;
    n = 0;
    while ( n < maxIterations ) {
        G4double cosTheta12 = 2.*G4UniformRand() - 1.;
        theta12 = acos(cosTheta12);
        G4double y = (1. - beta1*beta2) + 2.*beta1*beta2*G4UniformRand();

        if ( y <= (1. - beta1*beta2*cosTheta12) ) {
            phi2 = twopi * G4UniformRand();
            break;
        }
        n++;
    }

    G4ThreeVector u2 = RotateToDirection(u1, theta12, phi2);

    particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( t2 * eScale );
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection( u2 );
    particleGun->GeneratePrimaryVertex( event );
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    Dbb0nSpectrum()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorbb0n::Dbb0nSpectrum( G4double k, G4double d ) const
{
    if( fabs(d) > k ) return 0.;

    G4int z = 54;

    G4double e1 = 0.5*(k+d) + 1.0;
    G4double e2 = 0.5*(k-d) + 1.0;

    G4double p1 = sqrt(e1*e1 - 1.0);
    G4double p2 = sqrt(e2*e2 - 1.0);

    G4double t1 = e1 - 1.0;
    G4double t2 = e2 - 1.0;

    return p1*e1*FermiFunction(z, t1*electron_mass_c2, 1) *
           p2*e2*FermiFunction(z, t2*electron_mass_c2, 1);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    Dbb0nSpectralMax()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorbb0n::Dbb0nSpectralMax( G4double k ) const
{
    G4int z = 54;
    G4int nbins = 100;

    G4double a = -k;
    G4double b =  k;
    G4double dD = (b-a)/nbins;

    G4double phaseSpace[101] = {0.};

    for (G4int n = 0; n < nbins+1; n++) {

        G4double d;

        if (n != nbins) {
            d = a + n*dD;
        } else {
            d = b;
        }

        G4double e1 = 0.5*(k+d)+1.0;
        G4double e2 = 0.5*(k-d)+1.0;

        G4double p1 = sqrt(e1*e1-1.0);
        G4double p2 = sqrt(e2*e2-1.0);

        G4double t1 = e1 - 1.0;
        G4double t2 = e2 - 1.0;

        phaseSpace[n] = p1*e1*FermiFunction(z, t1*electron_mass_c2, 1) *
                        p2*e2*FermiFunction(z, t2*electron_mass_c2, 1);
    }

    G4double dSpectralMax = 0.;

    for( G4int i=0; i<nbins+1; i++ ) {
        if( phaseSpace[i] > dSpectralMax )
            dSpectralMax = phaseSpace[i];
    }

    return dSpectralMax;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    FermiFunction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorbb0n::FermiFunction( G4int z,
                                           G4double ke,
                                           G4int ffFactor ) const
{
    // From Schenter+Vogel in Nucl.Sci.Eng,83,393(1983).
    // W is full energy in electron-mass units.
    G4double alpha = 7.2974e-3;

    G4double totalEnergy = ke + electron_mass_c2;
    G4double w = totalEnergy / electron_mass_c2;

    G4double z0 = z + 2*ffFactor;

    if ( w <= 1. ) w = 1. + 1e-4;

    G4double a = -0.811 + 4.46e-2*z0 + 1.08e-4*z0*z0;
    G4double b = 0.673 - 1.82e-2*z0 + 6.38e-5*z0*z0;

    if ( w > 2.2 ) {
        a = -8.46e-2 + 2.48e-2*z0 + 2.37e-4*z0*z0;
        b = 1.15e-2 + 3.58e-4*z0 - 6.17e-5*z0*z0;
    }

    G4double x = sqrt(w-1.);
    G4double p = sqrt(w*w-1.);

    G4double result = exp(a + b*x) * w / p;

    // Keep compatibility with the legacy behavior.
    if( p <= 0. ) result = 1.;

    if (ffFactor == -1) {
        G4double v = p / w;
        G4double y = 2*pi*z0*alpha/v;
        G4double yy = 1./exp(y);
        result = result*yy;
    }

    return result;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    RotateToDirection()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4ThreeVector BaccGeneratorbb0n::RotateToDirection( const G4ThreeVector &axis,
                                                     G4double theta,
                                                     G4double azimuth ) const
{
    G4ThreeVector zhat = axis.unit();
    G4ThreeVector ref(0., 0., 1.);
    if( fabs(zhat.dot(ref)) > 0.99 ) ref = G4ThreeVector(1., 0., 0.);

    G4ThreeVector xhat = ref.cross(zhat).unit();
    G4ThreeVector yhat = zhat.cross(xhat).unit();

    return sin(theta)*cos(azimuth)*xhat + sin(theta)*sin(azimuth)*yhat + cos(theta)*zhat;
}
