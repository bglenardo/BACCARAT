//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_MigdalNeutronTagging.hh                //
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

#ifndef XeNeu_MigdalNeutronTagging_HH
#define XeNeu_MigdalNeutronTagging_HH 1

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
class XeNeu_MigdalNeutronTagging
{
    
public:
  
  XeNeu_MigdalNeutronTagging();
  ~XeNeu_MigdalNeutronTagging();
  
  inline G4LogicalVolume *GetLogicalVolume() { return taggingDetectorVolume_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  
  G4LogicalVolume *taggingDetectorVolume_log;
};

#endif
