//////////////////////////////////////////////////////
//                                                  //
//  TMS_LSDetector.hh                //
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

#ifndef TMS_LSDetector_HH
#define TMS_LSDetector_HH 1

#include <string>

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
class TMSMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class TMS_LSDetector
{
    
public:
  
  TMS_LSDetector(std::string instance_num="");
  ~TMS_LSDetector();
  
  inline G4LogicalVolume *GetLogicalVolume() { return lsDetectorVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  TMSMaterials * TMSmaterials;
  
  G4LogicalVolume *lsDetectorVolume_log;
};

#endif
