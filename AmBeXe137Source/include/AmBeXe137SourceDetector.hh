////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define AmBeXe137Source geometry
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AmBeXe137SourceDetector_HH
#define AmBeXe137SourceDetector_HH 1

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
#include "AmBeXe137SourceDetectorParameters.hh"

//
//      Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;
class G4NistManager;
class BaccMaterials;
class AmBeXe137SourceMessenger;
class AmBeXe137SourceMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class AmBeXe137SourceDetector : public BaccDetector
{
public:
  AmBeXe137SourceDetector( G4String, G4String );
  ~AmBeXe137SourceDetector();
  
  void BuildDetector();
  G4ThreeVector SetOrigin(BaccDetectorComponent *);
  void InitializeDetector();

  //  Registration methods below

  
private:
  AmBeXe137SourceMessenger *AmBeXe137Sourcemessenger;
  AmBeXe137SourceMaterials *AmBeXe137Sourcematerials;
  G4NistManager *NISTmaterials;
  BaccMaterials *BACCmaterials;
  
};

#endif
