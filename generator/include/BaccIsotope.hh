////////////////////////////////////////////////////////////////////////////////
/*    BaccBST.hh

This is the header for the Isotope information
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccIsotope_HH
#define BaccIsotope_HH 1

//#include "TROOT.h"

#include <vector>
//#include <iostream>
//#include <stdlib.h>
//#include <string>

//
//    GEANT4 includes
//
#include "globals.hh"

//
//    CLHEP includes
//
#include "Randomize.hh"
//class TRandom3;

class Isotope
{
    public:
      Isotope( G4String, G4int, G4int, G4double );//All Generators except...
      Isotope( G4String, G4int, G4int, G4String, G4double );//SingleParticle
      
      inline G4String Name() { return name; };
      void AddDaughter( Isotope*, G4double );
      Isotope *GetNextDaughter();
      inline G4int GetZ() { return Z; };
      inline G4int GetA() { return A; };
      inline G4double GetHalflife() { return halflife; };
      inline G4String GetParticleName() { return particleName; };
      inline G4double GetEnergy() { return energy; };
      inline void SetPopulation( G4double pop ) { population = pop; };
      inline G4double GetPopulation() { return population; };
    
    private:
      G4String name;
      G4double halflife;
      G4int Z;
      G4int A;
      G4String particleName;
      G4double energy;
      std::vector<Isotope*> daughters;
      std::vector<G4double> branchRatio;
      G4double population;
    
};

#endif
