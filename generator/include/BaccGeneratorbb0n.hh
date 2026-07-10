////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorbb0n.hh
*
* This is the header file for the bb0n generator.
*
********************************************************************************
* Change log
*   10-Jul-26 - Initial submission (Copilot)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorbb0n_HH
#define BaccGeneratorbb0n_HH 1

//
//    GEANT4 includes
//
#include "G4ParticleDefinition.hh"
#include "globals.hh"

//
//    Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorbb0n : public BaccSource
{
    public:
        BaccGeneratorbb0n();
        ~BaccGeneratorbb0n();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList( G4GeneralParticleSource*, G4Event*, decayNode* );

    private:
        G4ParticleDefinition *ionDef;
        G4ParticleDefinition *electronDef;
        G4ParticleDefinition *gammaDef;
        G4double qValue;
        G4int maxIterations;

    private:
        G4double FermiFunction( G4int z, G4double ke, G4int ffFactor ) const;
        G4double Dbb0nSpectrum( G4double k, G4double d ) const;
        G4double Dbb0nSpectralMax( G4double k ) const;
        G4ThreeVector RotateToDirection( const G4ThreeVector &axis,
                                         G4double theta,
                                         G4double azimuth ) const;
};

#endif
