////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorEventsFile.hh
*
* This is the header file for the EventsFile generator.
*
********************************************************************************
* Change log
*   12-May-14 - Initial submission (Kevin)
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorEventsFile_HH
#define BaccGeneratorEventsFile_HH 1

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
class BaccGeneratorEventsFile : public BaccSource
{
    public:
        BaccGeneratorEventsFile();
        ~BaccGeneratorEventsFile();

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

