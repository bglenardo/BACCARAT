////////////////////////////////////////////////////////////////////////////////
/*	BaccExampleComplexDetector.cc

This is the code file to define the example "complex" detector.
********************************************************************************
* Change log
*	2013/11/20 - Initial submission (Vic)
*   2014/01/03 - Code cleanup (Vic)
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
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"

//
//	Bacc includes
//
#include "BaccExampleComplexDetector.hh"
#include "BaccDetectorComponent.hh"
#include "BaccExamplePMTBank.hh"
#include "BaccMaterials.hh"

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BaccExampleComplexDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExampleComplexDetector::BaccExampleComplexDetector( G4String detName,
        G4String outVolName )
{
    detectorName = detName;
    outermostVolumeName = outVolName;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~BaccExampleComplexDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExampleComplexDetector::~BaccExampleComplexDetector()
{}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BuildDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccExampleComplexDetector::BuildDetector()
{
	//	This complex cryostat is not much more complex than the simple
	//	cryostat. I include it just as an example of how a geometry can
	//	be set at run time using the BaccMessenger commands. This 
	//	crysostat is a copper can inside a titanium can, and the inner 
	//	can holds two banks of PMTs and a liquid Xe target.

	//	First make the outer cryostat.
	G4Tubs *outerCryostat_solid = new G4Tubs( "outerCryostat_solid", 0*mm,
			21.*cm/2, 40.*cm/2, 0.*deg, 360.*deg );
	logicalVolume = new G4LogicalVolume( outerCryostat_solid,
			baccMaterials->Titanium(), "simplecryostat_log" );
	logicalVolume->SetVisAttributes( baccMaterials->TitaniumVis() );
	outermostVolumeName = "cryostat";
	
	//	Next comes the vacuum inside the outer cryostat.
	G4Tubs *cryostatVacuum_solid = new G4Tubs( "cryostatVacuum_solid", 0*mm,
			20.*cm/2, 39.*cm/2, 0.*deg, 360.*deg );
	G4LogicalVolume *cryostatVacuum_log = new G4LogicalVolume(
			cryostatVacuum_solid, baccMaterials->Vacuum(),
			"cryostatVacuum_log" );
	cryostatVacuum_log->SetVisAttributes( baccMaterials->VacuumVis() );
	cryostatVacuum = new BaccDetectorComponent( 0, G4ThreeVector(),
			cryostatVacuum_log, "cryostatVacuum", logicalVolume,0,0,false);
	
	//	Next make the inner cryostat. It holds two banks of PMTs (12 cm
	//	tall each), and the target region is 5 cm, for a cavity 29 cm 
	//	high. The cavity is also 15 cm in diameter.
	G4double cavityDiameter = 15.*cm;
	G4double cavityHeight = 29.*cm;
	G4double thickness = 5.*mm;
	
	G4Tubs *innerCryostat_solid = new G4Tubs( "innerCryostat_solid", 0*mm,
			cavityDiameter/2 + thickness, cavityHeight/2+thickness,
			0.*deg, 360.*deg );
	G4LogicalVolume *innerCryostat_log = new G4LogicalVolume(
			innerCryostat_solid, baccMaterials->Copper(),"innerCryostat_log");
	innerCryostat_log->SetVisAttributes( baccMaterials->CopperVis() );
	innerCryostat = new BaccDetectorComponent( 0, G4ThreeVector(),
			"InnerCryostat", innerCryostat_log, cryostatVacuum, 0, 0, false );
	
	//	Optical boundary between the inner cryostat and the vacuum
	G4OpticalSurface *innerCryoVacuumOpSurface = new G4OpticalSurface(
			"innerCryoVacuumOpSurface", unified, polished, dielectric_metal);
	innerCryoVacuumOpSurface->SetMaterialPropertiesTable(
			baccMaterials->CopperTable() );
	G4LogicalBorderSurface *innerCryoVacuumSurface;
	innerCryoVacuumSurface = new G4LogicalBorderSurface(
			"innerCryoVacuumSurface", cryostatVacuum, innerCryostat,
			innerCryoVacuumOpSurface );
	
	//	Now put a cavity inside the cryostat filled with liquid xenon
	G4Tubs *liquidXeTarget_solid = new G4Tubs( "liquidXeTarget_solid", 0*mm,
		cavityDiameter/2, cavityHeight/2, 0.*deg,360.*deg );
	G4LogicalVolume *liquidXeTarget_log = new G4LogicalVolume(
			liquidXeTarget_solid, baccMaterials->LiquidXe(),
			"liquidXeTarget_log" );
	liquidXeTarget_log->SetVisAttributes( baccMaterials->LiquidXeVis() );
	liquidXeTarget = new BaccDetectorComponent( 0, G4ThreeVector(),
			"LiquidXeTarget", liquidXeTarget_log, innerCryostat, 0, 0, false );
	
	//	Optical boundary between the inner cryostat and the LXe
	G4OpticalSurface *innerCryoLXeOpSurface = new G4OpticalSurface(
			"innerCryoLXeOpSurface", unified, polishedbackpainted,
			dielectric_metal);
	innerCryoLXeOpSurface->SetMaterialPropertiesTable(
			baccMaterials->TeflonTable() );
	G4LogicalBorderSurface *innerCryoLXeSurface;
	innerCryoLXeSurface = new G4LogicalBorderSurface(
			"innerCryoLXeSurface", liquidXeTarget, innerCryostat,
			innerCryoLXeOpSurface );
	
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

	//	Optical boundary between the top bank and the LXe
	G4OpticalSurface *topBankLXeOpSurface = new G4OpticalSurface(
			"topBankLXeOpSurface", unified, polished, dielectric_metal);
	topBankLXeOpSurface->SetMaterialPropertiesTable(
			baccMaterials->CopperTable() );
	G4LogicalBorderSurface *topBankLXeSurface;
	topBankLXeSurface = new G4LogicalBorderSurface(
			"topBankLXeSurface", liquidXeTarget, topBank,
			topBankLXeOpSurface );
	
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
			
	//	Optical boundary between the bottom bank and the LXe
	G4OpticalSurface *bottomBankLXeOpSurface = new G4OpticalSurface(
			"bottomBankLXeOpSurface", unified, polished, dielectric_metal);
	bottomBankLXeOpSurface->SetMaterialPropertiesTable(
			baccMaterials->CopperTable() );
	G4LogicalBorderSurface *bottomBankLXeSurface;
	bottomBankLXeSurface = new G4LogicalBorderSurface(
			"bottomBankLXeSurface", liquidXeTarget, bottomBank,
			bottomBankLXeOpSurface );
}
