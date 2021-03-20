//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_DDBoratedPoly_2.hh                //
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

#ifndef XeNeu_DDBoratedPoly_2_HH
#define XeNeu_DDBoratedPoly_2_HH 1

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
class XeNeu_DDBoratedPoly_2
{
    
public:
  
  XeNeu_DDBoratedPoly_2();
  ~XeNeu_DDBoratedPoly_2();
  
  inline G4LogicalVolume *GetLogicalVolume() { return boratedPoly2Volume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  BaccDetectorComponent * b_poly_sheet[2];
  G4LogicalVolume *boratedPoly2Volume_log;
};

#endif
