////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorFissionNeutrons.hh

This is the header file for the Cf252 fission neutron generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/09/09 - Included G4SystemOfUnits.hh to be compliant with geant4.10 (Brian)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorCfFission_HH
#define BaccGeneratorCfFission_HH 1

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
class BaccGeneratorCfFission : public BaccSource
{
    public:
        BaccGeneratorCfFission();
        ~BaccGeneratorCfFission();

    public:
      using BaccSource::GenerateEventList;
      void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
      using BaccSource::GenerateFromEventList;
      void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);
      //using BaccSource::GenerateEvent;
      //void GenerateEvent( G4GeneralParticleSource*, G4Event* );

    private:
        G4double GetNeutronEnergy();
        G4double GetGammaEnergy( G4double );

    private:
        G4ParticleDefinition *neutronDef;
        G4ParticleDefinition *gammaDef;
        
        G4double gammaCDF[86];
        G4double gammaEnergy[86];
        
        G4double Z;
        G4double A;
        G4double averageGammaEnergy;
};

#endif
