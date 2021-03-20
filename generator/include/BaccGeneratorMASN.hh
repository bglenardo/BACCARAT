////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorMASN.hh

This is the header file for the AmBe generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorMASN_HH
#define BaccGeneratorMASN_HH 1

//
//    GEANT4 includes
//
#include "globals.hh"

//
//    Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorMASN : public BaccSource
{
    public:
        BaccGeneratorMASN();
        ~BaccGeneratorMASN();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);
        //using BaccSource::GenerateEvent;
        //void GenerateEvent( G4GeneralParticleSource*, G4Event* );

    private:
        G4double GetMuonEnergy();
        G4double GetMuonAngle();
        G4double GetNeutronEnergy();
        G4double GetNeutronMultiplicity();
        G4double GetNeutronAngle();

    private:
        G4ParticleDefinition *neutronDef;
        G4ParticleDefinition *muonDef;
        
        G4double muonAngleCDF[1001];
        G4double muonAngle[1001];
        G4double muonEnergyCDF[1001];
        G4double muonEnergy[1001];

        G4double neutronAngleCDF[1001];
        G4double neutronAngle[1001];
        G4double neutronMultCDF[1001];
        G4double neutronMult[1001];
        G4double neutronEnergyCDF[1001];
        G4double neutronEnergy[1001];
};

#endif
