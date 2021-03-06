////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define ThermalFluxCalc geometry
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ThermalFluxCalcDetector_HH
#define ThermalFluxCalcDetector_HH 1

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

//
//      Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;
class G4NistManager;
class BaccMaterials;
class ThermalFluxCalcMessenger;
class ThermalFluxCalcMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class ThermalFluxCalcDetector : public BaccDetector
{
public:
  ThermalFluxCalcDetector( G4String, G4String );
  ~ThermalFluxCalcDetector();
  
  void BuildDetector();
  G4ThreeVector SetOrigin(BaccDetectorComponent *);
  void InitializeDetector();

  //  Registration methods below
  void SetShieldingRadius( G4double val ) { shieldingRadius = val; }
  
private:
  ThermalFluxCalcMessenger *ThermalFluxCalcmessenger;
  ThermalFluxCalcMaterials *ThermalFluxCalcmaterials;
  G4NistManager *NISTmaterials;
  BaccMaterials *BACCmaterials;
 
  G4double shieldingRadius;
  
};

#endif
