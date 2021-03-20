//////////////////////////////////////////////////////
//                                                  //
//  TMS_NaI_Detector_5in.hh                //
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

#ifndef TMS_NaI_Detector_5in_HH
#define TMS_NaI_Detector_5in_HH 1

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
class TMS_NaI_Detector_5in
{
    
public:
  
  TMS_NaI_Detector_5in(std::string instance_num="");
  ~TMS_NaI_Detector_5in();
  
  inline G4LogicalVolume *GetLogicalVolume() { return naiDetectorVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  TMSMaterials * TMSmaterials; 
 
  G4LogicalVolume *naiDetectorVolume_log;
};

#endif
