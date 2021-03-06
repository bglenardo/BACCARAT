////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define TestShield geometry
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TestShieldDetector_HH
#define TestShieldDetector_HH 1

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
class TestShieldMessenger;
class TestShieldMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class TestShieldDetector : public BaccDetector
{
public:
  TestShieldDetector( G4String, G4String );
  ~TestShieldDetector();
  
  void BuildDetector();
  G4ThreeVector SetOrigin(BaccDetectorComponent *);
  void InitializeDetector();

  //  Registration methods below
  void SetShieldingRadius( G4double val ) { shieldingRadius = val; }
  
private:
  TestShieldMessenger *TestShieldmessenger;
  TestShieldMaterials *TestShieldmaterials;
  G4NistManager *NISTmaterials;
  BaccMaterials *BACCmaterials;
 
  G4double shieldingRadius;
  
};

#endif
