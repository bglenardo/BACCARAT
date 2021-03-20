//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_PMTCage.cc                             //
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
#include "XeNeu_PMTCage.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_PMTCage::XeNeu_PMTCage()
{


  double al_box_half_width = 2.337 * 2.54 * cm / 2.;
  double al_box_half_height = 2.375 * 2.54 * cm / 2.;
  double al_box_wall_thickness = 0.056 * 2.54 * cm;
  double al_box_lip_thickness = 0.125 * 2.54 * cm;
  double al_box_lip_half_width = al_box_half_width - 2 * al_box_wall_thickness - 0.125 * 2.54 * cm;

  double glass_plate_half_thickness = 0.1 * 2.54 * cm / 2.;
  double glass_plate_half_width = 2.1 * 2.54 * cm / 2.;

  double teflon_casing_lip_thickness = 0.1 * 2.54 * cm;
  double teflon_casing_wall_thickness = 0.065 * 2.54 * cm;
  double teflon_casing_half_width = al_box_half_width - al_box_wall_thickness - 0.01*cm;
  double teflon_casing_half_height = al_box_half_height - al_box_lip_thickness/2. - glass_plate_half_thickness;
  double teflon_pmt_hole_half_width = 0.88/2. * 2.54 * cm;

  double anode_holder_thickness = 0.1 * 2.54 * cm;
  double anode_holder_half_width = al_box_half_width;


    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

  //example box -- make it into a minimal working example
   G4Box * pmtCageVolume_box = new G4Box("detector", 
                                             al_box_half_width, 
                                             al_box_half_width, 
                                             al_box_half_height + anode_holder_thickness/2.);
  pmtCageVolume_log = new G4LogicalVolume( pmtCageVolume_box, BACCmaterials->GasXe(), "pmtCageVolume");
  pmtCageVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );


  // PMT cage aluminum box
  G4Box * al_box_solid = new G4Box("al_box_solid",
                                             al_box_half_width, 
                                             al_box_half_width, 
                                             al_box_half_height);
  G4Box * al_box_cutout = new G4Box("al_box_cutout",
                                             al_box_half_width - al_box_wall_thickness, 
                                             al_box_half_width - al_box_wall_thickness, 
                                             al_box_half_height);
  G4Box * al_box_hole = new G4Box("al_box_hole",
                                             al_box_lip_half_width,
                                             al_box_lip_half_width,
                                             al_box_half_height + 1.*cm);

  G4SubtractionSolid * al_box_with_cutout = new G4SubtractionSolid("al_box_with_cutout",
                                                 al_box_solid,
                                                 al_box_cutout,
                                                 0,G4ThreeVector(0.,0.,al_box_lip_thickness));
  G4SubtractionSolid * al_box_final_solid = new G4SubtractionSolid("al_box_final_solid",
                                                 al_box_with_cutout,
                                                 al_box_hole,
                                                 0,G4ThreeVector(0.,0.,0.));

  G4LogicalVolume * al_box_log = new G4LogicalVolume( al_box_final_solid, BACCmaterials->Aluminum(),"al_box_log");
  al_box_log->SetVisAttributes( BACCmaterials->AluminumVis() );  

  BaccDetectorComponent * pmt_al_box = new BaccDetectorComponent(0,
                                               G4ThreeVector(0.,0.,anode_holder_thickness/2.),
                                               al_box_log,
                                               "pmt_al_box",
                                               pmtCageVolume_log,
                                               0,0,true);

  // PMT cage glass slide
  double glass_slide_Z = -al_box_half_height + al_box_lip_thickness + glass_plate_half_thickness + anode_holder_thickness/2.;

  G4Box * glass_slide_solid = new G4Box("glass_slide_solid",
                                          glass_plate_half_width,
					  glass_plate_half_width,
                                          glass_plate_half_thickness);
  G4LogicalVolume * glass_slide_log = new G4LogicalVolume( glass_slide_solid, BACCmaterials->Quartz(), "glass_slide_log");
  glass_slide_log->SetVisAttributes( BACCmaterials->QuartzVis() );
 
  BaccDetectorComponent * glass_slide = new BaccDetectorComponent(0,
                                             G4ThreeVector(0,0,glass_slide_Z),
                                             glass_slide_log,
                                             "glass_slide",
                                             pmtCageVolume_log,
                                             0,0,true);
  // PMT cage teflon spacer
  double spacer_Z = al_box_lip_thickness/2. + glass_plate_half_thickness + anode_holder_thickness/2.;
  double pmt_hole_center_xy = 0.52 * 2.54 * cm;  

  G4Box * teflon_solid  = new G4Box("teflon_solid",
                                      teflon_casing_half_width, 
                                      teflon_casing_half_width, 
                                      teflon_casing_half_height);
  G4Box * teflon_cutout = new G4Box("teflon_cutout",
                                      teflon_casing_half_width - teflon_casing_wall_thickness, 
                                      teflon_casing_half_width - teflon_casing_wall_thickness, 
                                      teflon_casing_half_height);
  G4Box * teflon_pmt_hole = new G4Box("teflon_pmt_hole",
                                      teflon_pmt_hole_half_width,
                                      teflon_pmt_hole_half_width,
                                      teflon_casing_half_height + 1.*cm);
                                       
  G4SubtractionSolid * teflon_solid_with_cutout = new G4SubtractionSolid("teflon_solid_with_cutout",
                                      teflon_solid,
                                      teflon_cutout,
                                      0,G4ThreeVector(0.,0.,teflon_casing_lip_thickness)); 
  G4SubtractionSolid * teflon_solid_with_cutout_and_1_holes = new G4SubtractionSolid("teflon_solid_with_cutout_and_1_holes",
                                      teflon_solid_with_cutout,
                                      teflon_pmt_hole,
                                      0,G4ThreeVector(pmt_hole_center_xy,pmt_hole_center_xy,0.));
  G4SubtractionSolid * teflon_solid_with_cutout_and_2_holes = new G4SubtractionSolid("teflon_solid_with_cutout_and_2_holes",
                                      teflon_solid_with_cutout_and_1_holes,
                                      teflon_pmt_hole,
                                      0,G4ThreeVector(pmt_hole_center_xy,-pmt_hole_center_xy,0.));
  G4SubtractionSolid * teflon_solid_with_cutout_and_3_holes = new G4SubtractionSolid("teflon_solid_with_cutout_and_3_holes",
                                      teflon_solid_with_cutout_and_2_holes,
                                      teflon_pmt_hole,
                                      0,G4ThreeVector(-pmt_hole_center_xy,-pmt_hole_center_xy,0.));
  G4SubtractionSolid * teflon_solid_with_cutout_and_4_holes = new G4SubtractionSolid("teflon_solid_with_cutout_and_4_holes",
                                      teflon_solid_with_cutout_and_3_holes,
                                      teflon_pmt_hole,
                                      0,G4ThreeVector(-pmt_hole_center_xy,pmt_hole_center_xy,0.));
  
  G4LogicalVolume * pmt_teflon_casing_log = new G4LogicalVolume( teflon_solid_with_cutout_and_4_holes,
                                                                 BACCmaterials->Teflon(),
                                                                 "pmt_teflon_casing_log");
  BaccDetectorComponent * pmt_teflon_casing = new BaccDetectorComponent(0,
                                                    G4ThreeVector(0.,0.,spacer_Z),
                                                    pmt_teflon_casing_log,
                                                    "pmt_teflon_casing",
                                                    pmtCageVolume_log,
                                                    0,0,true);
  
 // PEEK anode holder part
 double anode_holder_Z = -al_box_half_height;

 G4Box * anode_holder_solid = new G4Box("anode_holder_solid",
                                          anode_holder_half_width,
                                          anode_holder_half_width,
                                          anode_holder_thickness/2.);
 G4Tubs * anode_holder_cutout = new G4Tubs("anode_holder_cutout",
                                          0,
                                          2.1 * 2.54 * cm /2.,
                                          anode_holder_thickness * 2.,
                                          0. * deg, 360. * deg);
 G4SubtractionSolid * anode_holder_final_solid = new G4SubtractionSolid("anode_holder_final_solid",
                                                                anode_holder_solid, 
                                                                anode_holder_cutout,
                                                                0,G4ThreeVector(0.,0.,0.));
 G4LogicalVolume * anode_holder_log = new G4LogicalVolume( anode_holder_final_solid,
                                                           BACCmaterials->Peek(),
                                                           "anode_holder_log");
 anode_holder_log->SetVisAttributes( BACCmaterials->PeekVis() ); 
 BaccDetectorComponent * anode_holder = new BaccDetectorComponent(0,
                                                  G4ThreeVector(0.,0.,anode_holder_Z),
                                                  anode_holder_log,
                                                  "anode_holder",
                                                  pmtCageVolume_log,
                                                  0,0,true);

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_PMTCage
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_PMTCage::~XeNeu_PMTCage(){}
