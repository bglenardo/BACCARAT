//////////////////////////////////////////////////////
//                                                  //
//  XeNeuDT_VacuumBox.cc                             //
//                                                  //
//  This is the class implementation for the XeNeu  //
//  xenon detector, for use in the LLNL xenon       //
//  recoil experiments                              //
//                                                  //
//////////////////////////////////////////////////////
/*
********************************************************************************
*
*	Change log
*	
*	2017-11-29 - Initial submission (Brian Lenardo)
*
*
********************************************************************************
*/

//
//	C/C++ includes
//

#include <sstream>
#include <string>

//
//	GEANT4 includes
//

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Ellipsoid.hh"

//
//      BACCARAT includes
//
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"

//
//	LZSystemTest includes
//
#include "XeNeuDT_VacuumBox.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuDT_VacuumBox::XeNeuDT_VacuumBox()
{


  double vacuumBoxHalfLength = 22 * 2.54 * cm/2.;
  double vacuumBoxHalfWidth = 11. * 2.54 * cm/2.;
  double vacuumBoxHalfHeight = 6. * 2.54 * cm/2.;
  double vacuumBoxWallThickness = 0.25 * 2.54 * cm;

  double holeRadius = 4. * 2.54 * cm;

  double flangeOuterRadius = 3. * 2.54 * cm;
  double flangeInnerRadius = (4. - 2. * 1./16.) * 2.54 * cm / 2.;
  double flangeThickness = 0.79 * 2.54 * cm;

    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

  //example box -- make it into a minimal working example
   G4Box * vacuumBoxVolume_box = new G4Box("vacuum_box_box", 
                                             vacuumBoxHalfWidth, 
                                             vacuumBoxHalfLength, 
                                             vacuumBoxHalfHeight);
  vacuumBoxVolume_log = new G4LogicalVolume( vacuumBoxVolume_box, BACCmaterials->Vacuum(), "vacuumBoxVolume");
  vacuumBoxVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );


  // This piece models the stainless steel vacuum box in which all the instrumentation and connections are. 
  // None of that is implemented, it's just an empty stainless box with a hole in the bottom where the cryostat connects.
  G4Box * steel_box_solid = new G4Box("steel_box_solid",
                                             vacuumBoxHalfWidth, 
                                             vacuumBoxHalfLength, 
                                             vacuumBoxHalfHeight);
  G4Box * steel_box_cutout = new G4Box("steel_box_cutout",
                                             vacuumBoxHalfWidth - vacuumBoxWallThickness, 
                                             vacuumBoxHalfLength - vacuumBoxWallThickness, 
                                             vacuumBoxHalfHeight - vacuumBoxWallThickness);
  G4Tubs * steel_box_hole = new G4Tubs("steel_box_hole",
                                             0.*cm,
                                             holeRadius,
                                             vacuumBoxWallThickness + 1.*cm,
                                             0.*deg,360.*deg);
  // Make the vacuum space
  G4SubtractionSolid * steel_box_with_cutout = new G4SubtractionSolid("steel_box_with_cutout",
                                                 steel_box_solid,
                                                 steel_box_cutout,
                                                 0,G4ThreeVector(0.,0.,0.));

  // Make the bottom hole where the detector volume is connected
  G4SubtractionSolid * steel_box_final_solid = new G4SubtractionSolid("steel_box_final_solid",
                                                 steel_box_with_cutout,
                                                 steel_box_hole,
                                                 0,G4ThreeVector(0.,-vacuumBoxHalfLength + 5.*2.54*cm,-vacuumBoxHalfHeight));


  G4LogicalVolume * steel_box_log = new G4LogicalVolume( steel_box_final_solid, BACCmaterials->Steel(),"steel_box_log");
  steel_box_log->SetVisAttributes( BACCmaterials->SteelVis() );  
  //steel_box_log->SetVisAttributes( BACCmaterials->VacuumVis() );  

  BaccDetectorComponent * steel_vacuum_box = new BaccDetectorComponent(0,
                                               G4ThreeVector(0.,0.,0.),
                                               steel_box_log,
                                               "steel_vacuum_box",
                                               vacuumBoxVolume_log,
                                               0,0,true);

 
  // This piece models the two conflat flanges that make up the top of the xenon cryostat. The one on bottom has a 4" hole, 
  // the other is modeled as a solid cylinder
  G4Tubs * steel_flange_solid = new G4Tubs("steel_flange_solid",
                                           0.*cm,
                                           flangeOuterRadius,
                                           flangeThickness/2.,
                                           0.*deg,360.*deg);
  G4Tubs * steel_flange_ring = new G4Tubs("steel_flange_ring",
                                           flangeInnerRadius,
                                           flangeOuterRadius,
                                           flangeThickness/2.,
                                           0.*deg,360.*deg);
  G4UnionSolid * steel_flanges_both_part = new G4UnionSolid("steel_flanges_both_part",
                                           steel_flange_solid,
                                           steel_flange_ring,
                                           0,G4ThreeVector(0.,0.,-flangeThickness));
  G4LogicalVolume * steel_flanges_log = new G4LogicalVolume( steel_flanges_both_part,
                                           BACCmaterials->Steel(),
                                           "steel_flanges_log");
  BaccDetectorComponent * steel_CF_flanges = new BaccDetectorComponent(0,
                                           G4ThreeVector(0., vacuumBoxHalfLength - 5.*2.54*cm,-vacuumBoxHalfHeight+flangeThickness+flangeThickness/2.),
                                           steel_flanges_log,
                                           "steel_CF_flanges",
                                           vacuumBoxVolume_log,
                                           0,0,true);


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeuDT_VacuumBox
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuDT_VacuumBox::~XeNeuDT_VacuumBox(){}
