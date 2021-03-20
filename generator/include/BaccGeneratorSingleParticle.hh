////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorSingleParticle.hh

This is the header file for the SingleParticle generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorSingleParticle_HH
#define BaccGeneratorSingleParticle_HH 1

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
class BaccGeneratorSingleParticle : public BaccSource
{
  public:
    BaccGeneratorSingleParticle();
    ~BaccGeneratorSingleParticle();

  public:
    using BaccSource::GenerateEventList;
    void GenerateEventList( G4ThreeVector, G4int, G4int,G4String, 
                            G4double, G4double );
    using BaccSource::GenerateFromEventList;
    void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);

    //using BaccSource::GenerateEvent;
    //void GenerateEvent( G4GeneralParticleSource*, G4Event*, G4String, G4double );

  protected:
    G4double GetEnergy() { return particleEnergy; };
    G4ParticleDefinition *GetParticleDefinition() 
        { return particleName ;};

  private:
    G4ParticleDefinition *particleName;
    G4double particleEnergy;
    std::stringstream uiStream;
    G4String uiString;

};

#endif
