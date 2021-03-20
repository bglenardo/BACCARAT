////////////////////////////////////////////////////////////////////////////////
/*	BaccLBNLAlphaCounter.cc

This defines the geometry for the LBL prototype alpha counter.
********************************************************************************
Change log
  2014/04/08 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	C/C++ includes
//
#include <sstream>

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"


//
//	Bacc includes
//
#include "BaccLBNLAlphaCounter.hh"
#include "BaccDetectorComponent.hh"
#include "BaccExamplePMTBank.hh"
#include "BaccMaterials.hh"

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BaccLBNLAlphaCounter
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccLBNLAlphaCounter::BaccLBNLAlphaCounter( G4String detName,
        G4String outVolName )
{
    detectorName = detName;
    outermostVolumeName = outVolName;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~BaccLBNLAlphaCounter
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccLBNLAlphaCounter::~BaccLBNLAlphaCounter() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BuildDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccLBNLAlphaCounter::BuildDetector()
{
// The alpha counter is just an aluminum box with vacuum in it.  Inside that 
// vacuum is a plastic ring holding a silicon wafer to serve as the alpha 
// detector mount and physical detector respectively.

// Set up the dimmensions of the various detector components here.
    // First, the vacuum chamber:
    G4double ChamberWidth  = 15.24 * cm;
    G4double ChamberDepth  = 15.24 * cm;
    G4double ChamberHeight =  5.08 * cm;//outer dimensions for 6" x 6" x 2"
    G4double ChamberWall   =  0.50 * cm;//wall thickness
    // Next, alpha counter active element:
    G4double DetectorWidth     = 12.70 * cm;//5" diameter wafer
    G4double DetectorThickness =  0.3 * cm;
    //Finally, the plastic holder dimmensions:
    G4double HolderWidth     = 0.3 * cm;                    //radial thickness
    G4double HolderThickness = DetectorThickness + 0.2 * cm;//height

// Build up the detector out of Geant4 geometry primitives.
    // Start with the outer sample chamber (recall that G4Box is specified by half-dimensions!):
    G4Box *ChamberSolid = new G4Box("ChamberSolid", 
                                    0.5 * ChamberWidth, 
                                    0.5 * ChamberDepth, 
                                    0.5 * ChamberHeight);
    logicalVolume = new G4LogicalVolume(ChamberSolid, baccMaterials->Aluminum(), 
                                        "ChamberLogical");
    logicalVolume->SetVisAttributes(baccMaterials->AluminumVis());
    outermostVolumeName = "Chamber";

    // Make a vacuum space for the inside of the chamber:
    G4Box *VacuumSolid = new G4Box("VacuumSolid", 
                                   (0.5 * ChamberWidth)  - ChamberWall, 
                                   (0.5 * ChamberDepth)  - ChamberWall, 
                                   (0.5 * ChamberHeight) - ChamberWall);
    G4LogicalVolume *VacuumLogical = new G4LogicalVolume(VacuumSolid, 
                                                         baccMaterials->Vacuum(), 
                                                         "VacuumLogical");	
	VacuumVolume = new BaccDetectorComponent(0, G4ThreeVector(), VacuumLogical, 
	                                         "VacuumVolume", logicalVolume, 0, 0, false);
    // Now create the detector element itself:
    G4Tubs *DetectorSolid = new G4Tubs("DetectorSolid", 0., 0.5 * DetectorWidth,
                                       0.5 * DetectorThickness, 0., 360.*deg);
    G4LogicalVolume *DetectorLogical = new G4LogicalVolume(DetectorSolid,
                                                           baccMaterials->Silicon(), 
                                                           "DetectorLogical");
    DetectorLogical->SetVisAttributes(baccMaterials->BerylliumVis());
	DetectorVolume = new BaccDetectorComponent(0, G4ThreeVector(), DetectorLogical, 
	                                           "DetectorVolume", VacuumLogical, 0, 0, false );
    // Finally, create the plastic holder for the detector:
    G4Tubs *HolderSolid = new G4Tubs("HolderSolid", 0.5 * DetectorWidth, (0.5 * DetectorWidth) + HolderWidth,
                                       0.5 * HolderThickness, 0., 360.*deg);
    G4LogicalVolume *HolderLogical = new G4LogicalVolume(HolderSolid,
                                                         baccMaterials->Peek(), 
                                                         "HolderLogical");
    HolderLogical->SetVisAttributes(baccMaterials->PeekVis());
	HolderVolume = new BaccDetectorComponent(0, G4ThreeVector(), HolderLogical, 
	                                         "HolderVolume", VacuumLogical, 0, 0, false );
    //G4cout << "\tGot to the end of BaccLBNLAlphaCounter::BuildDetector().  Whee!!!" << G4endl;
}
