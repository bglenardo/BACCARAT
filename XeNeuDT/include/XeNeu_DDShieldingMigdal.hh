//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_DDShieldingMigdal.hh                //
//                                                  //
//  This is the class definition for the actve RF   //
//  region of liquid xenon that is in the LZ        //
//  System Test (as of 3 Nov 2016).                 //
//                                                  //
//////////////////////////////////////////////////////
/*
********************************************************************************
*
*	Change log
*	
*	2016-11-7 - Initial submission (Ryan Linehan)
*
*
********************************************************************************
*/

#ifndef XeNeu_DDShieldingMigdal_HH
#define XeNeu_DDShieldingMigdal_HH 1

//
//	GEANT4 includes
//

#include "globals.hh"

//
//	Project includes
//
#include "BaccDetectorComponent.hh"

//
//	Class forwarding
//

class G4Material;
class G4LogicalVolume;
//class BaccDetectorComponent;
class BaccMaterials;
class XeNeuDTMessenger;
class XeNeuDTMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class XeNeu_DDShieldingMigdal
{
    
public:
  
  XeNeu_DDShieldingMigdal();
  ~XeNeu_DDShieldingMigdal();
  
  inline G4LogicalVolume *GetLogicalVolume() { return full_shield_assembly_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  XeNeuDTMaterials *XeNeumaterials;
  G4LogicalVolume *full_shield_assembly_log;
};

#endif
