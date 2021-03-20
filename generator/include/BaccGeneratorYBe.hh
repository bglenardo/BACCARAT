/////////////////////////////////////////////////////
/*    BaccGeneratorYBe.hh
*
* This is the header file for the YBe generator.
*
********************************************************************************
* Change log
*    2 May 2014 - Initial submission (Kevin)
*    2014/09/09 - Ported from LUXSim to Baccarat (Brian)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorYBe_HH
#define BaccGeneratorYBe_HH 1

//
//    GEANT4 includes
//
#include "globals.hh"

//
//    Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorYBe : public BaccSource
{
    public:
        BaccGeneratorYBe();
        ~BaccGeneratorYBe();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);
        //using BaccSource::GenerateEvent;
        //void GenerateEvent( G4GeneralParticleSource*, G4Event* );

    private:
        G4ParticleDefinition *ionDef;
        G4ParticleDefinition *neutronDef;
        G4ParticleDefinition *gammaDef;

        G4double neutronEnergy;
        G4double gammaEnergy1;
        G4double gammaEnergy2;
        G4double neutronProb;
        
};

#endif

