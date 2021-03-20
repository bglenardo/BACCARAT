//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_VacuumBox.hh                //
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

#ifndef XeNeu_VacuumBox_HH
#define XeNeu_VacuumBox_HH 1

//
//	GEANT4 includes
//

#include "globals.hh"

//
//	Project includes
//


//
//	Class forwarding
//

class G4Material;
class G4LogicalVolume;
class BaccDetectorComponent;
class BaccMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class XeNeu_VacuumBox
{
    
public:
  
  XeNeu_VacuumBox();
  ~XeNeu_VacuumBox();
  
  inline G4LogicalVolume *GetLogicalVolume() { return vacuumBoxVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  
  G4LogicalVolume *vacuumBoxVolume_log;
};

#endif
