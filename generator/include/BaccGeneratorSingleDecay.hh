////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorSingleDecay.hh

This is the header file for the SingleDecay generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorSingleDecay_HH
#define BaccGeneratorSingleDecay_HH 1

//
//    GEANT4 includes
//
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"
#include "G4GenericIon.hh"

//
//    Bacc includes
//
#include "BaccBST.hh"
#include "BaccIsotope.hh"
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorSingleDecay : public BaccSource
{
    public:
        BaccGeneratorSingleDecay();
        ~BaccGeneratorSingleDecay();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4int, 
                            G4int, G4double);
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);

        //using BaccSource::GenerateEvent;
        //void GenerateEvent( G4GeneralParticleSource*, G4Event*, G4int, G4int );

    protected:
        G4double GetEnergy() {return 0.*keV; };
        G4ParticleDefinition *GetParticleDefinition() 
            { return G4GenericIon::Definition(); };

    private:
        G4ParticleDefinition *ion ;
        //std::stringstream uiStream;
        G4String uiString;
        G4int nucleusA;
        G4int nucleusZ;


};

#endif
