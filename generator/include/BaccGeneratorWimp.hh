////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorWimp.hh

This is the header file for the Wimp generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorWimp_HH
#define BaccGeneratorWimp_HH 1

//
//	GEANT4 includes
//
#include "G4ParticleDefinition.hh"
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorWimp : public BaccSource
{
	public:
        BaccGeneratorWimp();
        ~BaccGeneratorWimp();

	public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4double, 
                                    G4double );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList( G4GeneralParticleSource*, G4Event*,
                    decayNode*);

    private:
        G4double dR(G4double, G4double, G4double);
        G4ParticleDefinition *ion;

};

#endif
