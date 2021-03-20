////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorXe129m.hh

This is the header file for the Xe129m generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorXe129m_HH
#define BaccGeneratorXe129m_HH 1

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
class BaccGeneratorXe129m : public BaccSource
{
  public:
    BaccGeneratorXe129m();
    ~BaccGeneratorXe129m();

  public:
    using BaccSource::GenerateEventList;
    void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
    using BaccSource::GenerateFromEventList;
    void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);
    //using BaccSource::GenerateEvent;
    //void GenerateEvent( G4GeneralParticleSource*, G4Event* );

  private:
    G4ParticleDefinition *ion;
    G4ParticleDefinition *gammaDef;

};

#endif
