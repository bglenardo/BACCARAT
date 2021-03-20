////////////////////////////////////////////////////////////////////////////////
/*   BaccGeneratorScintPhotons.hh

This is the header file for the ScintPhoton generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorScintPhoton_HH
#define BaccGeneratorScintPhoton_HH 1

//
//   GEANT4 includes
//
#include "G4ParticleDefinition.hh"
#include "globals.hh"

//
//   Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorScintPhotons : public BaccSource
{
  public:
    BaccGeneratorScintPhotons();
    ~BaccGeneratorScintPhotons();

  public:
    using BaccSource::GenerateEventList;
    void GenerateEventList( G4ThreeVector, G4int, G4int, G4double);
    using BaccSource::GenerateFromEventList;
    void GenerateFromEventList(G4GeneralParticleSource*,G4Event*, decayNode*);
    //using BaccSource::GenerateEvent;
    //void GenerateEvent( G4GeneralParticleSource*, G4Event*);

  private:
    G4ParticleDefinition *photon;
    G4ThreeVector direction, polarization, perp;
    G4double theta, xpol, ypol, zpol, phi, sinp, cosp;

};

#endif
