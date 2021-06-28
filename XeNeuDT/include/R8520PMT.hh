/*
* R8520PMT.hh
*
* This is the header file to define the R8520 model PMT by Hamamatsu.
*
********************************************************************************
* Change log
*
*       2019-12-13 - Initial submission (Daniel N.)
*
********************************************************************************
*/

#ifndef R8520_HH
#define R8520_HH 1

#include <stdio.h>

#include "BaccMaterials.hh"
#include "globals.hh"




//      Class forwarding

class G4LogicalVolume;
class BaccDetectorComponent;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class R8520PMT

{
public:
    R8520PMT(G4bool buildGeometry = "true");  
    ~R8520PMT();

    G4LogicalVolume* GetLogicalVolume() { return logicalVolume; }

/*
    BaccDetectorComponent* GetSkinPMTPhotocathode();
    BaccDetectorComponent* GetSkinPMTWindow();
    BaccDetectorComponent* GetSkinPMTVacuum();
    G4double GetSkinPMTWidth();
    G4double GetSkinPMTHeight();
*/
// inline G4LogicalVolume *GetLogicalVolume() { return logical_volume; }



private:
    BaccMaterials *BACCmaterials;
    G4LogicalVolume* logicalVolume;
  /*
    BaccDetectorComponent* skinPMTphotocathode;
    BaccDetectorComponent* skinPMTwindow;
    BaccDetectorComponent* skinPMTvacuum;
    G4double bodyWidth;
    G4double pmtHeight;
*/

  //  G4LogicalVolume *full_shield_assembly_1_log;
};
#endif /* defined(____BaccSkinPMT__) */


