////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorDD.hh
*
* This is the header file for the GammaX generator.
*
********************************************************************************
* Change log
*   2-Feb-15 - Initial submission (Kevin)
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorDD_HH
#define BaccGeneratorDD_HH 1

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
class BaccGeneratorDD : public BaccSource
{
    public:
        BaccGeneratorDD();
        ~BaccGeneratorDD();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);

    private:
        G4ParticleDefinition *ionDef;
        G4ParticleDefinition *neutronDef;
        G4double ddEnergy;
        
};

#endif
