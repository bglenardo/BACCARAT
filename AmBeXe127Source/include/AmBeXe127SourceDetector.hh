////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define AmBeXe127Source geometry
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AmBeXe127SourceDetector_HH
#define AmBeXe127SourceDetector_HH 1

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
#include "AmBeXe127SourceDetectorParameters.hh"

//
//      Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;
class G4NistManager;
class BaccMaterials;
class AmBeXe127SourceMessenger;
class AmBeXe127SourceMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class AmBeXe127SourceDetector : public BaccDetector
{
public:
  AmBeXe127SourceDetector( G4String, G4String );
  ~AmBeXe127SourceDetector();
  
  void BuildDetector();
  G4ThreeVector SetOrigin(BaccDetectorComponent *);
  void InitializeDetector();

  //  Registration methods below

  
private:
  AmBeXe127SourceMessenger *AmBeXe127Sourcemessenger;
  AmBeXe127SourceMaterials *AmBeXe127Sourcematerials;
  G4NistManager *NISTmaterials;
  BaccMaterials *BACCmaterials;
  
};

#endif
