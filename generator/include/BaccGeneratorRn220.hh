////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorRn220.hh
*
* This is the header file for the Rn220 generator.
*
********************************************************************************
* Change log
*    16-February-2015 - file creation (Simon), copying from Rn222 generator
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorRn220_HH
#define BaccGeneratorRn220_HH 1

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
class BaccGeneratorRn220 : public BaccSource
{
    public:
        BaccGeneratorRn220();
        ~BaccGeneratorRn220();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);
        //using BaccSource::GenerateEvent;
        //void GenerateEvent( G4GeneralParticleSource*, G4Event* );

    private:
        G4ParticleDefinition *ion;

};

#endif
