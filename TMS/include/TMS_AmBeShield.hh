////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define TMS geometry
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TMS_AmBeShield_HH
#define TMS_AmBeShield_HH 1

//
//      GEANT4 includes
//
#include "globals.hh"

//
//      BACCARAT includes
//
#include "BaccDetector.hh"
#include "BaccManager.hh"

//
//	Project-specific includes
//
//#include "TMS_AmBeShieldParameters.hh"

//
//      Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;
class G4NistManager;
class BaccMaterials;
class TMSMessenger;
class TMSMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class TMS_AmBeShield
{
public:
  TMS_AmBeShield();
  ~TMS_AmBeShield();
  
  inline G4LogicalVolume *GetLogicalVolume() { return outerVolume_log; }

  
private:
  TMSMaterials *TMSmaterials;
  BaccMaterials *BACCmaterials;

  G4LogicalVolume * outerVolume_log;
 
};

#endif
