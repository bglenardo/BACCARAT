//////////////////////////////////////////////////////
//                                                  //
//  TMS_PSD_Detector.hh                //
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

#ifndef TMS_PSD_Detector_HH
#define TMS_PSD_Detector_HH 1

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
class TMS_PSD_Detector
{
    
public:
  
  TMS_PSD_Detector(std::string instance_num="");
  ~TMS_PSD_Detector();
  
  inline G4LogicalVolume *GetLogicalVolume() { return psdDetectorVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  TMSMaterials * TMSmaterials; 
 
  G4LogicalVolume *psdDetectorVolume_log;
};

#endif
