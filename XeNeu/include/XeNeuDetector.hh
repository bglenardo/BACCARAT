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

#ifndef XeNeuDetector_HH
#define XeNeuDetector_HH 1

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
//#include "XeNeuDetectorParameters.hh"

//
//      Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;
class G4NistManager;
class BaccMaterials;
class XeNeuMessenger;
class XeNeuMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class XeNeuDetector : public BaccDetector
{
public:
  XeNeuDetector( G4String, G4String );
  ~XeNeuDetector();
  
  void BuildDetector();
  G4ThreeVector SetOrigin(BaccDetectorComponent *);
  void InitializeDetector();
  void SetXeNeuDDShielding( G4bool val ) { ddShieldingOn = val; }
  void SetXeNeuMigdalNeutronTagging( G4bool val ) { migdal_neutron_tagging = val; }
  void SetXeNeuMigdalNaITagging( G4bool val ) { migdal_nai_tagging = val; }
  void SetXeNeuTOFCalibration( G4bool val ) { tof_calibration_on = val; }
  void SetXeNeuTOFCalibrationDistance( G4double val ) { tof_calibration_distance = val; }
  //  Registration methods below
  //
  G4bool GetXeNeuDDShielding() { return ddShieldingOn; }
  
private:
  XeNeuMessenger *XeNeumessenger;
  XeNeuMaterials *XeNeumaterials;
  G4NistManager *NISTmaterials;
  BaccMaterials *BACCmaterials;
  
  G4bool ddShieldingOn;
  G4bool migdal_neutron_tagging;
  G4bool migdal_nai_tagging;
  G4bool tof_calibration_on;
  G4double tof_calibration_distance; 

};

#endif
