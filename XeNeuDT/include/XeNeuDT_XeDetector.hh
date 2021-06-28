//////////////////////////////////////////////////////
//                                                  //
//  XeNeuDT_XeDetector.hh                //
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

#ifndef XeNeuDT_XeDetector_HH
#define XeNeuDT_XeDetector_HH 1

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
class XeNeuDT_XeDetector
{
    
public:
  
  XeNeuDT_XeDetector();
  ~XeNeuDT_XeDetector();
  
  inline G4LogicalVolume *GetLogicalVolume() { return detectorVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  
  G4LogicalVolume *detectorVolume_log;
};

#endif
