//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_TOFDetector.hh                //
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

#ifndef XeNeu_TOFDetector_HH
#define XeNeu_TOFDetector_HH 1

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
class XeNeu_TOFDetector
{
    
public:
  
  XeNeu_TOFDetector(int det_num);
  ~XeNeu_TOFDetector();
  
  inline G4LogicalVolume *GetLogicalVolume() { return lsDetectorVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  
  G4LogicalVolume *lsDetectorVolume_log;
};

#endif
