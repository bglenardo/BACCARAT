////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorAmBe.hh

This is the header file for the AmBe generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/09/09 - Included G4SystemOfUnits.hh to comply with geant4.10 (Brian)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorAmBe_HH
#define BaccGeneratorAmBe_HH 1

//
//    GEANT4 includes
//
#include "globals.hh"
#include "G4SystemOfUnits.hh"
//
//    Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorAmBe : public BaccSource
{
    public:
        BaccGeneratorAmBe();
        ~BaccGeneratorAmBe();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);
        //using BaccSource::GenerateEvent;
        //void GenerateEvent( G4GeneralParticleSource*, G4Event* );

    private:
        G4double GetNeutronEnergy();
        G4double GetGammaAngle();

    private:
        G4ParticleDefinition *ionDef;
        G4ParticleDefinition *neutronDef;
        G4ParticleDefinition *gammaDef;
        
        G4double neutronCDF[758];
        G4double neutronEnergy[758];
        
        G4double gammaCDF[5001];
        G4double gammaAngle[5001];
};

#endif
