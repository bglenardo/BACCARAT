#ifndef AluminumTest_HH
#define AluminumTest_HH 1


//      GEANT4 includes
//

#include "globals.hh"

//
//      Project includes
//


//
//      Class forwarding
//

class G4Material;
class G4LogicalVolume;
class BaccDetectorComponent;
class BaccMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class AluminumTest
{

public:

  AluminumTest();
    ~AluminumTest();

      inline G4LogicalVolume *GetLogicalVolume() { return logicalVolume; }


private:

  BaccMaterials *BACCmaterials;

  G4LogicalVolume *logicalVolume;
};

#endif


