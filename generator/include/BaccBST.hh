////////////////////////////////////////////////////////////////////////////////
/*  BaccBST.hh

This is the header for the Binary Search Tree
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccBST_HH
#define BaccBST_HH 1

//  GEANT4 includes
//
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

//  Bacc includes
//
#include "BaccIsotope.hh"

//  C++ includes
//
#include <fstream>
#include <iostream>

struct decayNode {
    G4int Z;
    G4int A;
    G4String particleName;//singleParticle
    G4double energy;//singleParticle and WimpMass
    G4double timeOfEvent;//nanoseconds
    G4ThreeVector pos;
    // IDs used to call GenereateFromEventList methods
    G4int sourceByVolumeID;//for manager->geometry
    G4int sourcesID;//geometry->generators
    decayNode *left;
    decayNode *right;
};

class BaccBST
{
  public:
    BaccBST( G4int, G4double, G4double, G4int );
    void Insert( Isotope*, G4double, G4ThreeVector, G4int, G4int);
    decayNode *GetEarliest();
    decayNode *GetLast();
    void PopEarliest();
    void PopLast();
    void PrintNodes();
    inline G4bool HasNodes() { return hasNodes; };
    inline G4int GetNumNonemptyNodes() { return numNonemptyNodes; };
    
    typedef decayNode pubDecayNode;

  private:
    G4int numEvents;
    G4int numNonemptyNodes;
    G4int numNodes;
    G4bool hasNodes;
    decayNode *topNode;
    decayNode *Insert( Isotope*, G4double, G4ThreeVector, G4int, G4int,
        decayNode* );
    void PrintNodes( decayNode* );

    G4int nodeCount;
    G4int warningsWritten,warningsWritten1e4, warningsWritten1e5;
  
};
#endif
