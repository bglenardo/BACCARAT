////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define XeNeu geometry
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef XeNeuDTDetector_HH
#define XeNeuDTDetector_HH 1

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
//#include "XeNeuDTDetectorParameters.hh"

//
//      Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;
class G4NistManager;
class BaccMaterials;
class XeNeuDTMessenger;
class XeNeuDTMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class XeNeuDTDetector : public BaccDetector
{
public:
  XeNeuDTDetector( G4String, G4String );
  ~XeNeuDTDetector();
  
  void BuildDetector();
  G4ThreeVector SetOrigin(BaccDetectorComponent *);
  void InitializeDetector();
  void SetXeNeuDDShielding( G4bool val ) { ddShieldingOn = val; }
  void SetXeNeuDTShielding( G4bool val ) { dtShieldingOn = val; } 
  //  Registration methods below
  //
  G4bool GetXeNeuDDShielding() { return ddShieldingOn; }
  G4bool GetXeNeuDTShielding() { return dtShieldingOn; }  
private:
  XeNeuDTMessenger *XeNeumessenger;
  XeNeuDTMaterials *XeNeumaterials;
  G4NistManager *NISTmaterials;
  BaccMaterials *BACCmaterials;
  
  G4bool ddShieldingOn;
  G4bool dtShieldingOn;
};

#endif