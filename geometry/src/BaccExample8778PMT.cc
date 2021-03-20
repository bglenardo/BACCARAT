////////////////////////////////////////////////////////////////////////////////
/*	BaccExample8778PMT.cc

This is the code file to define the 8778 model PMT by Hamamatsu.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

//
//	Bacc includes
//
#include "BaccExample8778PMT.hh"
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BaccExample8778PMT
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExample8778PMT::BaccExample8778PMT()
{
  // Get the BaccMaterials pointer
  BaccMaterials *baccMaterials = BaccMaterials::GetMaterials();

  // This PMT is very simply modeled. It is a hollow stainless steel can with
  // a quartz window. The steel is 2.65 mm thick, and the quartz is 2.5 mm.

  // First make the body of the PMT
  outerDiameter = 55.6 * mm;
  height = 12. * cm;
  G4double thickness = 2.65 * mm;
  G4Tubs *PMT8778_solid = new G4Tubs("PMT8778_solid", 
                                     0 * mm, 0.5 * outerDiameter, 
                                     0.5 * height, 0. * deg, 360. * deg);
  logicalVolume = new G4LogicalVolume(PMT8778_solid, baccMaterials->Steel(), 
                                     "PMT8778_log");
  logicalVolume->SetVisAttributes(baccMaterials->SteelVis());

  // Now put a vacuum inside the PMT
  G4double vacuumRadius = (0.5 * outerDiameter) - thickness;
  G4double vacuumHeight = height - thickness;
  G4Tubs *PMTvacuum_solid = new G4Tubs("PMT8778Vacuum_solid", 
                                       0 * mm, vacuumRadius, 0.5 * vacuumHeight, 
                                       0. * deg, 360. * deg);
  G4LogicalVolume *PMTvacuum_log = new G4LogicalVolume(PMTvacuum_solid,  
                                                       baccMaterials->Vacuum(),
                                                       "PMT8778Vacuum_log" );
  PMTvacuum_log->SetVisAttributes(baccMaterials->VacuumVis());
  G4double xOff = 0, yOff = 0, zOff = 0.5 * thickness;
  PMTvacuum = new BaccDetectorComponent(0, G4ThreeVector(xOff, yOff, zOff), 
                                        PMTvacuum_log, "PMT8778Vacuum", 
                                        logicalVolume, 0, 0, false );
	
  // Put a quartz window atop the PMT
  G4double windowThickness = 2.5 * mm;
  G4double windowRadius = vacuumRadius;
  zOff = 0.5 * (vacuumHeight - windowThickness);
  G4Tubs *PMTwindow_solid = new G4Tubs("PMT8778Window_solid", 
                                       0. * mm, windowRadius, 
                                       0.5 * windowThickness, 
                                       0. * deg, 360. * deg);
  G4LogicalVolume *PMTwindow_log = new G4LogicalVolume(PMTwindow_solid, 
                                                       baccMaterials->Quartz(), 
                                                       "PMT8778Window_log");
  PMTwindow_log->SetVisAttributes(baccMaterials->QuartzVis());
  PMTWindow = new BaccDetectorComponent(0, G4ThreeVector(xOff, yOff, zOff), 
                                        "PMT8778Window", PMTwindow_log, 
                                        PMTvacuum, 0, 0, false);
	
//	Put flashing embedded in the inside surface of the PMT window, so that
//	it sits between the window and the photocathode

//	Now put the aluminum traces on the back surface of the active window.
//	These traces are actually a very thin layer embedded in the quartz to
//	ensure that optical photons are properly reflected. There are several
//	parts to the traces:
//		The outer ring (IR = 23.9 mm, OD = 25.15 mm)
//		4 long traces (19 mm x 1 mm)
//		4 short traces (10 mm x 1 mm)
//	The total mass in the traces is 0.25 g, so the thickness is 100 microns.
/*  G4double traceRingInnerR = 22.65 * mm,
           traceThickness = 0.1 * mm,
           traceLineWidth = 1. * mm,
           traceLineLengthLong = 19. * mm,
           traceLineLengthShort = 10. * mm;
  // Solid primatives for the traces
  G4Tubs *alTrace_RingSolid = new G4Tubs("alTrace_RingSolid", 
                                         traceRingInnerR, windowRadius, 
                                         0.5 * traceThickness, 
                                         0. * deg, 360. * deg);
  G4Box *alTrace_LineSolid = new G4Box("alTrace_LineSolid", 
                                       traceRingInnerR + (1. * mm), 
                                       0.5 * traceLineWidth, 
                                       0.5 * traceThickness);
  G4Box *alTrace_LongLineCutoutSolid = new G4Box("alTrace_LongLineCutoutSolid", 
                                                 traceRingInnerR - traceLineLengthLong,
                                                 0.5 * traceLineWidth, 
                                                 0.5 * traceThickness);
  G4Box *alTrace_ShortLineCutoutSolid = new G4Box("alTrace_ShortLineCutoutSolid", 
                                                  traceRingInnerR - traceLineLengthShort,
                                                  0.5 * traceLineWidth, 
                                                  0.5 * traceThickness);
  G4SubtractionSolid *alTrace_2LongLineSolid = new G4SubtractionSolid("alTrace_2LongLineSolid",
                                                                      alTrace_LineSolid,
                                                                      alTrace_LongLineCutoutSolid);
  G4SubtractionSolid *alTrace_2ShortLineSolid = new G4SubtractionSolid("alTrace_2ShortLineSolid",
                                                                      alTrace_LineSolid,
                                                                      alTrace_ShortLineCutoutSolid);
  // Some rotation matricies for clocking the line traces into position
  G4RotationMatrix *rot000Z = new G4RotationMatrix; rot000Z->rotateZ(0.*deg);
  G4RotationMatrix *rot045Z = new G4RotationMatrix; rot045Z->rotateZ(45.*deg);
  G4RotationMatrix *rot090Z = new G4RotationMatrix; rot090Z->rotateZ(90.*deg);
  G4RotationMatrix *rot135Z = new G4RotationMatrix; rot135Z->rotateZ(135.*deg);
  // A running sum of the Al traces to get the flashing, first add the ring and the first two long 
  // line traces...
  G4UnionSolid *alTrace_SumSolid1 = new G4UnionSolid("alTrace_SumSolid1", alTrace_RingSolid, 
                                                     alTrace_2LongLineSolid, rot000Z, G4ThreeVector());
  // And the second two long line traces...
  G4UnionSolid *alTrace_SumSolid2 = new G4UnionSolid("alTrace_SumSolid2", alTrace_SumSolid1, 
                                                     alTrace_2LongLineSolid, rot090Z, G4ThreeVector());
  // Add the first two short line traces...
  G4UnionSolid *alTrace_SumSolid3 = new G4UnionSolid("alTrace_SumSolid3", alTrace_SumSolid2,
                                                     alTrace_2ShortLineSolid, rot045Z, G4ThreeVector());
  // Add the second two short line traces and we're done.
  G4UnionSolid *PMTflashing_solid = new G4UnionSolid("PMTflashing_solid", alTrace_SumSolid3, 
                                                     alTrace_2ShortLineSolid, rot135Z, G4ThreeVector());
  // And finish putting this thing in the simulation...
  G4LogicalVolume *PMTflashing_log = new G4LogicalVolume(PMTflashing_solid,
			                                             baccMaterials->Aluminum(), 
                                                         "PMT8778Flashing_log" );
  G4VisAttributes *flashingVis = new G4VisAttributes( G4Color::Black() );
  PMTflashing_log->SetVisAttributes( flashingVis );
  zOff = 0.5 * traceThickness;
  PMTFlashing = new BaccDetectorComponent(0, G4ThreeVector(xOff, yOff, zOff), 
                                          "PMTFlashing", PMTflashing_log, 
                                          PMTWindow, 0, 0, false );
  //Optical boundary between the Al flashing and window
  G4LogicalBorderSurface *flashingWindowSurface;
  flashingWindowSurface = new G4LogicalBorderSurface("flashingWindowSurface", 
                                                     PMTWindow, PMTFlashing, 
		                                             baccMaterials->AlQuartzSurface());
*/

//The next several lines of code, until the "Make the "photocathode"" comment are the simplified flasher
// parts. I took them with some modifications from LUXSimR11410PMT.cc - WHL

         	G4double flashingThickness = 0.1*mm;	//	Value based on the R8778 PMTs
		G4double windowDiameter = windowRadius*2;
		G4double traceThickness = flashingThickness; 
		G4double flashInnerDiameter = windowRadius*2-4*mm;
		G4double photocathodeThickness2 = 20*nm;
		//	Put flashing in the PMT window, right next to the photocathode
		G4Tubs *pmtFlashing_solid = new G4Tubs( "pmtFlashing_sol", 
				flashInnerDiameter/2., windowDiameter/2., flashingThickness/2,
				0.*deg, 360.*deg );
		G4LogicalVolume *pmtFlashing_log = new G4LogicalVolume(
				pmtFlashing_solid, baccMaterials->Aluminum(), "pmtFlashing_log");
		pmtFlashing_log->SetVisAttributes( baccMaterials->AluminumVis() );
		
		zOff = photocathodeThickness2/2 + flashingThickness/2;
		PMTFlashing = new BaccDetectorComponent( 0,
				G4ThreeVector(0,0,zOff), "PMTAlFlashing", 
				pmtFlashing_log, PMTWindow, 0, 0, false );
		
		//	Optical boundary between the Al flashing and window
		G4LogicalBorderSurface *flashingWindowSurface;
		flashingWindowSurface = new G4LogicalBorderSurface(
				"flashingWindowSurface", PMTWindow, PMTFlashing, 
				baccMaterials->AlQuartzSurface() );

  // Make the "photocathode" nothing more than a volume of quartz wholly
  // inside the PMT window, located behind the aluminum flashing.
  G4double photocathodeRadius = windowRadius;
  G4double photocathodeThickness = 20 * nm;
  zOff = (-0.5 * photocathodeThickness) - traceThickness - (1.14 * mm) 
          - (8. * um) - (0.00198 * mm);
  G4Tubs *PMTphotocathode_solid = new G4Tubs("PMT8778Photocathode_solid", 
                                             0, photocathodeRadius, 
                                             0.5 * photocathodeThickness, 
                                             0. * deg, 360. * deg);
  G4LogicalVolume *PMTphotocathode_log = new G4LogicalVolume(PMTphotocathode_solid, 
                                                             baccMaterials->Quartz(),
                                                             "PMT8778Photocathode_log");
  G4VisAttributes *photocathodeVis = new G4VisAttributes( G4Color::Magenta());
  PMTphotocathode_log->SetVisAttributes(photocathodeVis);
  PMTPhotocathode = new BaccDetectorComponent(0, G4ThreeVector(xOff, yOff, zOff), 
                                              "PMT8778Photocathode", 
                                              PMTphotocathode_log, PMTWindow, 
                                              0, 0, false);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~BaccExample8778PMT
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExample8778PMT::~BaccExample8778PMT() {}

