//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_DDCastle.hh                //
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

#ifndef XeNeu_DDCastle_HH
#define XeNeu_DDCastle_HH 1

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

//------++++++------++++++------++++++------++++++------++++++------++++++------
class XeNeu_DDCastle
{
    
public:
  
  XeNeu_DDCastle();
  ~XeNeu_DDCastle();
  
  inline G4LogicalVolume *GetLogicalVolume() { return ddCastleVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  BaccDetectorComponent * poly_blocks[7];
  G4LogicalVolume *ddCastleVolume_log;
};

#endif
