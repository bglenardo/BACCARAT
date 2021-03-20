////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorXe131m_NEW.hh

This is the header file for the Xe131m_NEW generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorXe131m_NEW_HH
#define BaccGeneratorXe131m_NEW_HH 1

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
class BaccGeneratorXe131m_NEW : public BaccSource
{
  public:
    BaccGeneratorXe131m_NEW();
    ~BaccGeneratorXe131m_NEW();

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
	G4ParticleDefinition *electronDef;

};

#endif
