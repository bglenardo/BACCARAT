////////////////////////////////////////////////////////////////////////////////
/*	Bacc7081PMT.cc

This is the code file to define the R7081 model PMT by Hamamatsu. M.Sweany 
(sweany1@llnl.gov)
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/01/03 - Code cleanup (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
//
//	Bacc includes
//
#include "Bacc7081PMT.hh"
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				Bacc7081PMT
//------++++++------++++++------++++++------++++++------++++++------++++++------
Bacc7081PMT::Bacc7081PMT()
{
	//	Get the BaccMaterials pointer
	BaccMaterials *baccMaterials = BaccMaterials::GetMaterials();

	//	This PMT is extremely simply modeled: it is a spherical vacuum 
	//	with a quartz semi-sphere window

	//	First make the glass window of the PMT
	G4double radius = 13.67*cm;
	G4double thickness = 1.*mm;

	G4Sphere *PMT7081_solid = new G4Sphere( "PMT7081_solid", 0.*cm,radius,
			0.*deg, 360.*deg, 0.*deg, 90.*deg );
	PMT7081_log = new G4LogicalVolume( PMT7081_solid,
			baccMaterials->Vacuum(), "PMT7081_log" );
	PMT7081_log->SetVisAttributes( baccMaterials->VacuumVis() );


	//	Now put a quartz window inside the vacuum
	G4Sphere *window_solid = new G4Sphere( "window_solid",radius-thickness,
			radius,0.*deg, 360.*deg, 0.*deg, 90.*deg );
	G4LogicalVolume *window_log = new G4LogicalVolume( window_solid,
			baccMaterials->Quartz(), "PMT7081Window_log" );
	window_log->SetVisAttributes( baccMaterials->QuartzVis() );
	PMT7081window = new BaccDetectorComponent( 0,G4ThreeVector(), 
			window_log,"PMT7081Window", PMT7081_log, 0, 0, false );
			
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~Bacc7081PMT
//------++++++------++++++------++++++------++++++------++++++------++++++------
Bacc7081PMT::~Bacc7081PMT() {}
