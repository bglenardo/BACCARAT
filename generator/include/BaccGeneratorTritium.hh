////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorTritium.hh

This is the header file for the tritium generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorTritium_HH
#define BaccGeneratorTritium_HH 1

//
//    GEANT4 includes
//
#include "globals.hh"

//
//    Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorTritium : public BaccSource
{
    public:
        BaccGeneratorTritium();
        ~BaccGeneratorTritium();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList(G4GeneralParticleSource*,G4Event*,decayNode*);

    private:
        G4double GetElectronEnergy();

    private:
		G4double Q;
		G4double m_e;
		G4double pi;
		G4double a;
		G4double Z;
		G4double xmax;
		G4double ymax;
        G4ParticleDefinition *electronDef;        
};

#endif
