////////////////////////////////////////////////////////////////////////////////
/*	BaccDetector.hh

This is the header file to define the Baccarat detector. This "detector" is just 
the detector part, it does not include the water shield or the rock.
********************************************************************************
Change log
  2013/09/18 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccDetector_HH
#define BaccDetector_HH 1

//
//  Geant4 includes
//
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

//
//	Bacc includes
//
#include "BaccDetector.hh"
#include "BaccDetectorConstruction.hh"
#include "BaccGeometryCatalog.hh"
#include "BaccDetectorComponent.hh"

//
//	Class forwarding
//
class BaccDetectorConstruction;
class BaccGeometryCatalog;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccDetector
{
	public:
        BaccDetector();
		~BaccDetector();
		
		inline G4LogicalVolume *GetLogicalVolume() { return logicalVolume; };
    
        virtual void BuildDetector();
    virtual G4ThreeVector SetOrigin(BaccDetectorComponent *ExpHall);
    
        inline G4String GetName() { return detectorName; };
        inline G4String GetVolumeName() { return outermostVolumeName; };
		
	protected:
		G4LogicalVolume *logicalVolume;
		
		BaccManager *baccManager;
        BaccMaterials *baccMaterials;
        BaccGeometryCatalog *baccGeometryCatalog;
		
        G4String detectorName;
        G4String outermostVolumeName;
};

#endif
