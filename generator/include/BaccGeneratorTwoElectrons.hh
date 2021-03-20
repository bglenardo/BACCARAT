////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorTwoElectrons.hh
*
* This is the header file for the two electron generator.
*
********************************************************************************
* Change log
*   22-July-13 - Initial submission (Matthew)
*   
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorTwoElectrons_HH
#define BaccGeneratorTwoElectrons_HH 1

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
class BaccGeneratorTwoElectrons : public BaccSource
{
    public:
        BaccGeneratorTwoElectrons();
        ~BaccGeneratorTwoElectrons();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);

    private:
        G4ParticleDefinition *ionDef;
        G4ParticleDefinition *electronDef;
        G4ParticleDefinition *gammaDef;
        
};

#endif

