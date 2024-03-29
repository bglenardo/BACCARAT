//////////////////////////////////////////////////////
//                                                  //
//  XeNeuDT_LSDetector.hh                //
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

#ifndef XeNeuDT_LSDetector_HH
#define XeNeuDT_LSDetector_HH 1

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
class XeNeuDT_LSDetector
{
    
public:
  
  XeNeuDT_LSDetector(int det_num);
  ~XeNeuDT_LSDetector();
  
  inline G4LogicalVolume *GetLogicalVolume() { return lsDetectorVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  
  G4LogicalVolume *lsDetectorVolume_log;
};

#endif
