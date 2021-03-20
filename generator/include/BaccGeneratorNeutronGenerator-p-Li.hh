////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorAmBe.hh

This is the header file for the AmBe generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorpLithium_HH
#define BaccGeneratorpLithium_HH 1

//
//    GEANT4 includes
//
#include "globals.hh"

//
//    Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorpLithium : public BaccSource
{
    public:
        BaccGeneratorpLithium();
        ~BaccGeneratorpLithium();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);
        //using BaccSource::GenerateEvent;
        //void GenerateEvent( G4GeneralParticleSource*, G4Event* );

    private:
        void GetNeutronEnergy();


    private:
        G4ParticleDefinition *ionDef;
        G4ParticleDefinition *neutronDef;
        G4ParticleDefinition *gammaDef;
        
        G4double neutronAnglePDF[28000];
        G4double neutronEnergyPDF[28000];
        G4double neutronCountPDF[28000];
    
        G4double energy;
        G4double angle;
    
        G4double neutronEnergy;
        G4double neutronAngle;
    
        G4double testcount;
    
        G4double lowang;
        G4double highang;
        G4double lowangDegrees;
        G4double highangDegrees;

        G4double phistartDegrees;
        G4double phitotalDegrees;
        G4double phistart;
        G4double phitotal;
    
};

#endif
