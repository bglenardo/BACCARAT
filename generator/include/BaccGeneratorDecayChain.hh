////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorDecayChain.hh

This is the header file.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeneratorDecayChain_HH
#define BaccGeneratorDecayChain_HH 1

//
//    GEANT4 includes
//
#include "G4ParticleDefinition.hh"
#include "globals.hh"

//
//    Bacc includes
//
#include "BaccBST.hh"
#include "BaccIsotope.hh"
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeneratorDecayChain : public BaccSource
{
    public:
        BaccGeneratorDecayChain();
        ~BaccGeneratorDecayChain();

    public:
        using BaccSource::GenerateEventList;
        void GenerateEventList( G4ThreeVector, G4int, G4int, G4String );
        using BaccSource::CalculatePopulationsInEventList;
        void CalculatePopulationsInEventList( G4double, G4double, G4String );
        using BaccSource::GenerateFromEventList;
        void GenerateFromEventList( G4GeneralParticleSource*, G4Event*, 
                                decayNode* );
        G4double GetParentDecayTime() {return originalTime_s;};
   
    private:
        //Th232
        G4double halflivesTh[11];
        G4double lambdaTh232[11];
        //U238
        G4double halflivesU238[19];
        G4double lambdaU238[19];

    private:
        //Th232 with 11 isotopes (10 levels)
        G4int CalculatePopulationsTh232( G4double, G4double);
        G4double CalculateRatesTh232( );
        G4double ReducePopulationsTh232( G4double);
        // U238 with 19 isotopes (14 levels)
        G4int CalculatePopulationsU238( G4double, G4double);
        G4double CalculateRatesU238( );
        G4double ReducePopulationsU238( G4double);

    private:
        //Th232
        G4double populationTh[11], ratesTh[11];
        Isotope *isoArrayTh[11];
        //U238
        G4double populationU[19], ratesU[19];
        Isotope *isoArrayU[19];

    private:
        G4ParticleDefinition *ion;
        G4double originalRate;
        G4double originalTime_s;
};

    
#endif
