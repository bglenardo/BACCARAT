////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorPb210.hh

This is the header file for the Pb210 generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorPb210_HH
#define BaccGeneratorPb210_HH 1

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
class BaccGeneratorPb210 : public BaccSource
{
  public:
    BaccGeneratorPb210();
    ~BaccGeneratorPb210();

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
