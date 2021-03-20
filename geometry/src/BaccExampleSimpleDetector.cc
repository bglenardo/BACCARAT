////////////////////////////////////////////////////////////////////////////////
/*	BaccExampleSimpleDetector.cc

This is the code file to define an "simple" example detector.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/01/03 - Code cleanup (Vic)
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
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"

//
//	Bacc includes
//
#include "BaccExampleSimpleDetector.hh"
#include "BaccDetectorComponent.hh"
#include "BaccExamplePMTBank.hh"
#include "BaccMaterials.hh"

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BaccExampleSimpleDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExampleSimpleDetector::BaccExampleSimpleDetector( G4String detName,
        G4String outVolName )
{
    detectorName = detName;
    outermostVolumeName = outVolName;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~BaccExampleSimpleDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExampleSimpleDetector::~BaccExampleSimpleDetector() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BuildDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccExampleSimpleDetector::BuildDetector()
{
	//	This simple cryostat is just a titanium can with two banks of
	//	PMTs and a liquid Xe target.

	//	First make the cryostat. Inside it it holds two banks of PMTs 
	//	(12 cmtall each), and the target region is 5 cm, for a cavity 
	//	29 cm high. The cavity is also 15 cm in diameter.
	G4double cavityDiameter = 15.*cm;
	G4double cavityHeight = 29.*cm;
	G4double thickness = 5.*mm;
	
	G4Tubs *cryostat_solid = new G4Tubs( "cryostat_solid", 0*mm,
			cavityDiameter/2 + thickness, cavityHeight/2 + thickness,
			0.*deg, 360.*deg );
	logicalVolume = new G4LogicalVolume( cryostat_solid,
			baccMaterials->Titanium(), "simplecryostat_log" );
	logicalVolume->SetVisAttributes( baccMaterials->TitaniumVis() );
	outermostVolumeName = "cryostat";

	//	Now put a cavity inside the cryostat filled with liquid xenon
	G4Tubs *liquidXeTarget_solid = new G4Tubs( "liquidXeTarget_solid", 0*mm,
			cavityDiameter/2, cavityHeight/2, 0.*deg,360.*deg );
	G4LogicalVolume *liquidXeTarget_log = new G4LogicalVolume(
			liquidXeTarget_solid, baccMaterials->LiquidXe(),
			"liquidXeTarget_log" );
	liquidXeTarget_log->SetVisAttributes( baccMaterials->LiquidXeVis() );
	liquidXeTarget = new BaccDetectorComponent( 0, G4ThreeVector(),
			liquidXeTarget_log, "LiquidXeTarget", logicalVolume, 0, 0, false );
	
	//	Now the two banks of PMTs, starting with the top bank
	banks[0] = new BaccExamplePMTBank();
	banks[1] = new BaccExamplePMTBank();

	G4double xOff = 0, yOff = 0, zOff = cavityHeight/2 - 6.*cm;
	G4RotationMatrix *rotY180 = new G4RotationMatrix();
	rotY180->rotateY(180.*deg);

	topBank = new BaccDetectorComponent( rotY180,
			G4ThreeVector( xOff, yOff, zOff ), "TopPMTHolder",
			banks[0]->GetLogicalVolume(), liquidXeTarget, 0, 0, false );
	stringstream name;
	for( G4int i=0; i<3; i++ ) {
		name.str("");
		name << "Top_PMT_Can_" << i+1;
		banks[0]->GetPMT(i)->SetName( name.str() );
		name.str("");
		name << "Top_PMT_Window_" << i+1;
		banks[0]->GetPMTClass(i)->GetPMTWindow()->SetName( name.str() );
		name.str("");
		name << "Top_PMT_Vacuum_" << i+1;
		banks[0]->GetPMTClass(i)->GetPMTVacuum()->SetName( name.str() );
	}
	
	bottomBank = new BaccDetectorComponent( 0,
			G4ThreeVector( xOff, yOff, -zOff ), "BottomPMTHolder",
			banks[1]->GetLogicalVolume(), liquidXeTarget, 0, 0, false );
	for( G4int i=0; i<3; i++ ) {
		name.str("");
		name << "Bottom_PMT_Can_" << i+1;
		banks[1]->GetPMT(i)->SetName( name.str() );
		name.str("");
		name << "Bottom_PMT_Window_" << i+1;
		banks[1]->GetPMTClass(i)->GetPMTWindow()->SetName( name.str() );
		name.str("");
		name << "Bottom_PMT_Vacuum_" << i+1;
		banks[1]->GetPMTClass(i)->GetPMTVacuum()->SetName( name.str() );
	
	}
}
