//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_XeDetector.cc                             //
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
#include <cmath>

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
#include "XeNeu_XeDetector.hh"
#include "XeNeu_PMTCage.hh"
#include "XeNeu_XeDetectorParameters.hh"
//
//		Definitions
//
using namespace std;
using namespace XeNeuDetectorParameters;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_XeDetector::XeNeu_XeDetector()
{

    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

  //example box -- make it into a minimal working example
//  G4Box * detectorVolume_box = new G4Box("detector", 10. * m, 10. * m, 10. * m);
  G4Tubs * detectorVolume_tubs = new G4Tubs("detector",
                                             0.*cm,
                                             outerCanOuterRadius,
                                             outerCanHalfHeight,
                                             0.*deg,360.*deg);
  detectorVolume_log = new G4LogicalVolume( detectorVolume_tubs, BACCmaterials->Vacuum(), "detector");
  detectorVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );

  // Outer Detector Can
  G4Tubs * outer_can_tubs = new G4Tubs("outer_can",
                                          outerCanInnerRadius,
                                          outerCanOuterRadius,
                                          outerCanHalfHeight,
                                           0.*deg, 360.*deg);
  G4LogicalVolume * outer_can_log = new G4LogicalVolume(outer_can_tubs, BACCmaterials->Steel(), "outer_can");
  //outer_can_log->SetVisAttributes( BACCmaterials->SteelVis() ); 
  outer_can_log->SetVisAttributes( BACCmaterials->VacuumVis() ); 
  BaccDetectorComponent * outer_can = new BaccDetectorComponent(0,
                                                                 G4ThreeVector(0.,0.,0.), //0.64*2.54*cm),
                                                                 outer_can_log,
                                                                 "outer_can",
                                                                 detectorVolume_log,
                                                                 0,0,true);

  // vacuum insulation space
  G4Tubs * vacuum_space_tubs = new G4Tubs("vacuum_space",
                                          innerCanOuterRadius,
                                          outerCanInnerRadius,
                                          outerCanHalfHeight,
                                          0.*deg, 360.*deg);
  G4LogicalVolume * vacuum_space_log = new G4LogicalVolume(vacuum_space_tubs, BACCmaterials->Vacuum(),"vacuum_space");
  vacuum_space_log->SetVisAttributes( BACCmaterials->VacuumVis() ); 
  BaccDetectorComponent * vacuum_space = new BaccDetectorComponent(0,
                                                                 G4ThreeVector(0.,0.,0.),//0.64*2.54*cm),
                                                                 vacuum_space_log,
                                                                 "vacuum_space",
                                                                 detectorVolume_log,
                                                                 0,0,true);
//
  // Inner detector can
  G4Tubs * inner_can_tubs = new G4Tubs("inner_can",
                                          0*cm,
                                          innerCanOuterRadius,
                                          innerCanHalfHeight,
                                           0.*deg, 360.*deg);
  G4LogicalVolume * inner_can_log = new G4LogicalVolume(inner_can_tubs, BACCmaterials->Steel(), "inner_can");
  //inner_can_log->SetVisAttributes( BACCmaterials->SteelVis() ); 
  inner_can_log->SetVisAttributes( BACCmaterials->VacuumVis() ); 
  BaccDetectorComponent * inner_can = new BaccDetectorComponent(0,
                                                                 G4ThreeVector(0.,0.,0.), //0.64*2.54*cm),
                                                                 inner_can_log,
                                                                 "inner_can",
                                                                 detectorVolume_log,
                                                                 0,0,true);
  // Liquid xenon volume
  double xenon_vol_half_height = innerCanHalfHeight-1./8.*2.54*cm;

  G4Tubs * liquid_xenon_tubs = new G4Tubs("liquid_xenon",
                                            0*cm,
                                            innerCanInnerRadius,
                                            xenon_vol_half_height,
                                            0.*deg,360.*deg);
  G4LogicalVolume * liquid_xenon_log = new G4LogicalVolume(liquid_xenon_tubs, BACCmaterials->LiquidXe(),"liquid_xenon");
  //liquid_xenon_log->SetVisAttributes( BACCmaterials->LiquidXeVis() ); 
  liquid_xenon_log->SetVisAttributes( BACCmaterials->VacuumVis() ); 
  BaccDetectorComponent * liquid_xenon = new BaccDetectorComponent(0,
                                                                 G4ThreeVector(0.,0.,0.),
                                                                 liquid_xenon_log,
                                                                 "liquid_xenon",
                                                                 inner_can_log,
                                                                 0,0,true);
  
  // Teflon piece - raw cylinder
  double teflonCentPos_Z = 0. - xenon_vol_half_height + teflonBlockHalfHeight;
  double teflonTopPos_Z = teflonCentPos_Z + teflonBlockHalfHeight;

  G4Tubs * teflon_block_tubs = new G4Tubs("teflon_block",
                                          teflonBlockInnerRadius,
                                          teflonBlockOuterRadius,
                                          teflonBlockHalfHeight,
                                          0.*deg,360.*deg);

  // Cathode ring
  double cathodeRingBotPos_Z = teflonTopPos_Z + 0.25*2.54*cm;
  
  G4Tubs * cathode_ring_tubs = new G4Tubs("cathode_ring",
                                         fieldRingInnerRadius,
                                         fieldRingOuterRadius,
                                         fieldGridHalfHeight,
                                         0.*deg,360.*deg);
  G4LogicalVolume * cathode_ring_log = new G4LogicalVolume(cathode_ring_tubs, BACCmaterials->Copper(),"cathode_ring");
  cathode_ring_log->SetVisAttributes( BACCmaterials->CopperVis() ); 
  BaccDetectorComponent * cathode_ring = new BaccDetectorComponent(0,
                                                                   G4ThreeVector(0.,0.,cathodeRingBotPos_Z+fieldGridHalfHeight),
                                                                   //G4ThreeVector(0.,0.,5.*cm),
                                                                   cathode_ring_log,
                                                                   "cathode_ring",
                                                                   liquid_xenon_log,
                                                                   0,0,true);


  // First field ring
  double fieldRingOffset_Z = 0.105*2.54*cm; 
  double fieldRing1Bot_Z = cathodeRingBotPos_Z + fieldGridHalfHeight*2. + fieldRingOffset_Z;
  
  G4Tubs * field_ring_1_tubs = new G4Tubs("field_ring_1",
                                         fieldRingInnerRadius,
                                         fieldRingOuterRadius,
                                         fieldRingHalfHeight,
                                         0.*deg,360.*deg);
  G4LogicalVolume * field_ring_1_log = new G4LogicalVolume(field_ring_1_tubs, BACCmaterials->Copper(),"field_ring_1");
  field_ring_1_log->SetVisAttributes( BACCmaterials->CopperVis() ); 
  BaccDetectorComponent * field_ring_1 = new BaccDetectorComponent(0,
                                                                   G4ThreeVector(0.,0.,fieldRing1Bot_Z + fieldRingHalfHeight),
                                                                   field_ring_1_log,
                                                                   "field_ring_1",
                                                                   liquid_xenon_log,
                                                                   0,0,true);

  // Second field ring
  double fieldRing2Bot_Z = fieldRing1Bot_Z + fieldRingHalfHeight*2. + fieldRingOffset_Z;
  
  G4Tubs * field_ring_2_tubs = new G4Tubs("field_ring_2",
                                         fieldRingInnerRadius,
                                         fieldRingOuterRadius,
                                         fieldRingHalfHeight,
                                         0.*deg,360.*deg);
  G4LogicalVolume * field_ring_2_log = new G4LogicalVolume(field_ring_2_tubs, BACCmaterials->Copper(),"field_ring_2");
  field_ring_2_log->SetVisAttributes( BACCmaterials->CopperVis() ); 
  BaccDetectorComponent * field_ring_2 = new BaccDetectorComponent(0,
                                                                   G4ThreeVector(0.,0.,fieldRing2Bot_Z + fieldRingHalfHeight),
                                                                   field_ring_2_log,
                                                                   "field_ring_2",
                                                                   liquid_xenon_log,
                                                                   0,0,true);
  
// Third field ring
  double fieldRing3Bot_Z = fieldRing2Bot_Z + fieldRingHalfHeight*2. + fieldRingOffset_Z;
  
  G4Tubs * field_ring_3_tubs = new G4Tubs("field_ring_3",
                                         fieldRingInnerRadius,
                                         fieldRingOuterRadius,
                                         fieldRingHalfHeight,
                                         0.*deg,360.*deg);
  G4LogicalVolume * field_ring_3_log = new G4LogicalVolume(field_ring_3_tubs, BACCmaterials->Copper(),"field_ring_3");
  field_ring_3_log->SetVisAttributes( BACCmaterials->CopperVis() ); 
  BaccDetectorComponent * field_ring_3 = new BaccDetectorComponent(0,
                                                                   G4ThreeVector(0.,0.,fieldRing3Bot_Z + fieldRingHalfHeight),
                                                                   field_ring_3_log,
                                                                   "field_ring_3",
                                                                   liquid_xenon_log,
                                                                   0,0,true);
  
// Extraction grid
  double extractionBotPos_Z = fieldRing3Bot_Z + fieldRingHalfHeight*2. + fieldRingOffset_Z;
  
  G4Tubs * extraction_grid_tubs = new G4Tubs("extraction_grid",
                                         fieldRingInnerRadius,
                                         fieldRingOuterRadius,
                                         fieldGridHalfHeight,
                                         0.*deg,360.*deg);
  G4LogicalVolume * extraction_grid_log = new G4LogicalVolume(extraction_grid_tubs, BACCmaterials->Copper(),"extraction_grid");
  extraction_grid_log->SetVisAttributes( BACCmaterials->CopperVis() ); 
  BaccDetectorComponent * extraction_grid = new BaccDetectorComponent(0,
                                                                   G4ThreeVector(0.,0.,extractionBotPos_Z + fieldGridHalfHeight),
                                                                   extraction_grid_log,
                                                                   "extraction_grid",
                                                                   liquid_xenon_log,
                                                                   0,0,true);
 
// Weir
  double weirPos_Z = extractionBotPos_Z + fieldGridHalfHeight*2. + 0.5*cm - weirHalfHeight;
  G4Tubs * weir_tubs = new G4Tubs("weir",
                                  weirInnerRadius,
                                  weirOuterRadius,
                                  weirHalfHeight,
                                  0.*deg - weirCutoutAngle/2., weirCutoutAngle); 
//                                  138.*deg,222.*deg);
//  G4LogicalVolume * weir_block_log = new G4LogicalVolume( weir_tubs, BACCmaterials->Peek(), "weir");

  G4Tubs * weir_hole_tubs = new G4Tubs("weir_hole",
                                       weirPocketInnerRadius,
                                       weirPocketOuterRadius,
                                       weirHalfHeight,// - 0.5*2.54*cm,
                                       0.*deg - weirPocketCutoutAngle/2., weirPocketCutoutAngle);
  G4LogicalVolume * weir_pocket_log = new G4LogicalVolume(weir_hole_tubs, BACCmaterials->LiquidXe(), "weir_pocket");

  G4SubtractionSolid * weir_solid = new G4SubtractionSolid("weir_solid",weir_tubs,weir_hole_tubs,0,G4ThreeVector( 0, 0, 0.5*2.54*cm));
  G4LogicalVolume * weir_log = new G4LogicalVolume(weir_solid, BACCmaterials->Peek(), "weir");
  weir_log->SetVisAttributes( BACCmaterials->PeekVis() ); 

  BaccDetectorComponent * weir = new BaccDetectorComponent( 0,
                                                            G4ThreeVector( 0., 0., weirPos_Z ),
                                                            weir_log,
                                                            "weir",
                                                            liquid_xenon_log,
                                                            0,0,true);

 // Teflon block - actual part
  G4Tubs * weir_sub_tubs = new G4Tubs("weir_subtraction_solid",
                                  weirInnerRadius,
                                  weirOuterRadius+1.*cm,
                                  weirHalfHeight,
                                  0.*deg - weirCutoutAngle/2., weirCutoutAngle);
 
  G4SubtractionSolid * teflon_block_minus_weir_solid = new G4SubtractionSolid("teflon_block_minus_weir",
                                                                              teflon_block_tubs,
                                                                              weir_sub_tubs,
                                                                              0, G4ThreeVector(0.,0.,((5.79-5.65)/2.+1.)*2.54*cm)); 

  G4LogicalVolume * teflon_block_minus_weir_log = new G4LogicalVolume(teflon_block_minus_weir_solid, BACCmaterials->Teflon(),"teflon_block_minus_weir");
  teflon_block_minus_weir_log->SetVisAttributes( BACCmaterials->TeflonVis() ); 
  BaccDetectorComponent * teflon_block_minus_weir = new BaccDetectorComponent(0,
                                                                   G4ThreeVector(0.,0.,teflonCentPos_Z),
                                                                   teflon_block_minus_weir_log,
                                                                   "teflon_block_minus_weir",
                                                                   liquid_xenon_log,
                                                                   0,0,true);

 // Gas volume in weir is modeled as upper half of weir.
  G4Tubs * gas_in_weir_tubs = new G4Tubs("gas_xenon_in_weir",
                                         weirPocketInnerRadius,
                                         weirPocketOuterRadius,
                                         weirHalfHeight/2.,
                                         0*deg - weirPocketCutoutAngle/2., weirPocketCutoutAngle);
  G4LogicalVolume * gas_in_weir_log = new G4LogicalVolume( gas_in_weir_tubs, BACCmaterials->GasXe(), "gas_xenon_in_weir");
  gas_in_weir_log->SetVisAttributes( BACCmaterials->VacuumVis() ); 
  BaccDetectorComponent * gas_in_weir = new BaccDetectorComponent(0,
                                                  G4ThreeVector(0.,0.,weirPos_Z + weirHalfHeight/2.),
                                                  gas_in_weir_log,
                                                  "gas_xenon_in_weir",
                                                  liquid_xenon_log,
                                                  0,0,true);
 // Three peek support rods
   double rod_half_length = (xenon_vol_half_height*2. - teflonBlockHalfHeight*2.)/2.;
   double rod_radius = 0.25/2. * 2.54 * cm;
   double rod_pos_r = (weirInnerRadius + weirOuterRadius)/2.;

   G4Tubs * peek_rod_tubs = new G4Tubs("peek_rod_tubs",
                                       0.*cm,
                                       rod_radius,
                                       rod_half_length,
                                       0.*deg,360.*deg);
   G4LogicalVolume * peek_rod_log = new G4LogicalVolume( peek_rod_tubs, BACCmaterials->Peek(), "peek_rod_log");
   peek_rod_log->SetVisAttributes( BACCmaterials->PeekVis() );
   BaccDetectorComponent * peek_rod_1 = new BaccDetectorComponent(0,
                                                   G4ThreeVector(rod_pos_r*cos(PI/3),rod_pos_r*sin(PI/3),teflonTopPos_Z+rod_half_length),
                                                   peek_rod_log,
                                                   "peek_rod_1",
                                                   liquid_xenon_log,
                                                   0,0,true);
   BaccDetectorComponent * peek_rod_2 = new BaccDetectorComponent(0,
                                                   G4ThreeVector(rod_pos_r*cos(PI),rod_pos_r*sin(PI),teflonTopPos_Z+rod_half_length),
                                                   peek_rod_log,
                                                   "peek_rod_2",
                                                   liquid_xenon_log,
                                                   0,0,true);
   BaccDetectorComponent * peek_rod_3 = new BaccDetectorComponent(0,
                                                   G4ThreeVector(rod_pos_r*cos(-PI/3),rod_pos_r*sin(-PI/3),teflonTopPos_Z+rod_half_length),
                                                   peek_rod_log,
                                                   "peek_rod_3",
                                                   liquid_xenon_log,
                                                   0,0,true);
 




 // Gas volume in upper part of detector
 double gas_volume_height = xenon_vol_half_height - (extractionBotPos_Z + fieldGridHalfHeight*2. + 0.5*cm);
 double gas_volume_Zpos = (extractionBotPos_Z + fieldGridHalfHeight * 2. + 0.5*cm + gas_volume_height/2.);
  G4Tubs * gas_xenon_tubs = new G4Tubs("gas_xenon",
                                   0.*cm,
                                   innerCanInnerRadius,
                                   gas_volume_height/2.,
                                   0.*deg, 360.*deg);

  G4SubtractionSolid * gas_xenon_tubs_rod_1 = new G4SubtractionSolid("gas_xenon_tubs_rod_1",
                                                   gas_xenon_tubs,
                                                   peek_rod_tubs,
                                                   0,
                                                   G4ThreeVector(rod_pos_r*cos(PI/3),rod_pos_r*sin(PI/3),0.));
  G4SubtractionSolid * gas_xenon_tubs_rod_2 = new G4SubtractionSolid("gas_xenon_tubs_rod_2",
                                                   gas_xenon_tubs_rod_1,
                                                   peek_rod_tubs,
                                                   0,
                                                   G4ThreeVector(rod_pos_r*cos(PI),rod_pos_r*sin(PI),0.));
  G4SubtractionSolid * gas_xenon_tubs_rod_3 = new G4SubtractionSolid("gas_xenon_tubs_rod_3",
                                                   gas_xenon_tubs_rod_2,
                                                   peek_rod_tubs,
                                                   0,
                                                   G4ThreeVector(rod_pos_r*cos(-PI/3),rod_pos_r*sin(-PI/3),0.));
        
  G4LogicalVolume * gas_xenon_log = new G4LogicalVolume( gas_xenon_tubs_rod_3, BACCmaterials->GasXe(), "gas_xenon");
  //gas_xenon_log->SetVisAttributes( BACCmaterials->GasXeVis() ); 
  gas_xenon_log->SetVisAttributes( BACCmaterials->VacuumVis() ); 
  BaccDetectorComponent * gas_xenon = new BaccDetectorComponent(0,
                                               G4ThreeVector(0.,0.,gas_volume_Zpos),
                                               gas_xenon_log,
                                               "gas_xenon",
                                               liquid_xenon_log,
                                               0,0,true);



 // Bottom PMT (vacuum void)
 double bottomPMT_Z = teflonTopPos_Z - bottomPMTHalfHeight - 0.25*2.54*cm;
 G4Tubs * bottom_pmt_tubs = new G4Tubs("bottom_pmt",
                                       0.*cm,
                                       bottomPMTRadius,
                                       bottomPMTHalfHeight,
                                       0.*deg,360.*deg);
 G4LogicalVolume * bottom_pmt_log = new G4LogicalVolume( bottom_pmt_tubs, BACCmaterials->Vacuum(), "bottom_pmt");
 bottom_pmt_log->SetVisAttributes( BACCmaterials->VacuumVis() ); 
 BaccDetectorComponent * bottom_pmt = new BaccDetectorComponent(0,
                                              G4ThreeVector( 0, 0, bottomPMT_Z ),
                                              bottom_pmt_log,
                                              "bottom_pmt",
                                              liquid_xenon_log,
                                              0,0,true); 
    
    
 
 // Aluminum box to hold PMTs 
 //
   double total_pmt_cage_height = 2.475 * 2.54 * cm;
   double pmt_cage_Z = -gas_volume_height/2. + total_pmt_cage_height/2. + 1. * cm; 

   XeNeu_PMTCage * pmtCage_obj = new XeNeu_PMTCage();
   BaccDetectorComponent * pmtCage = new BaccDetectorComponent(0, 
                                                        G4ThreeVector(0,0,pmt_cage_Z),
                                                        pmtCage_obj->GetLogicalVolume(),
                                                        "pmtCage",
                                                        gas_xenon_log,
                                                        0, 0, true);  
 

                                         
 // Liquid xenon in the target (drift) volume 
   double targetVolBot_Z = cathodeRingBotPos_Z + 0.05 * 2.54 * cm;
   double targetVolTop_Z = extractionBotPos_Z + 0.1 * 2.54 * cm;
   double targetVolHalfHeight = (targetVolTop_Z - targetVolBot_Z)/2.;

   G4Tubs * target_volume_tubs = new G4Tubs("target_volume_tubs",
                                         0. * cm,
                                         fieldRingInnerRadius,
                                         targetVolHalfHeight,
                                         0.*deg,360.*deg);
   G4LogicalVolume * target_volume_log = new G4LogicalVolume(target_volume_tubs, BACCmaterials->LiquidXe(),"liquid_xenon_target_log");
   target_volume_log->SetVisAttributes( BACCmaterials->LiquidXeVis() );
   BaccDetectorComponent * liquid_xenon_target = new BaccDetectorComponent(0,
                                         G4ThreeVector( 0., 0., targetVolBot_Z + targetVolHalfHeight ),
                                         target_volume_log,
                                         "liquid_xenon_target",
                                         liquid_xenon_log,
                                         0,0,true);

// Liquid xenon below the cathode
  double belowCathVolBot_Z = teflonTopPos_Z - 0.25 * 2.54 * cm;
  double belowCathVolTop_Z = cathodeRingBotPos_Z + 0.05 * 2.54 * cm;

  G4Tubs * below_cath_ring_vol = new G4Tubs("below_cath_ring_tubs",
                                        0.*cm,
                                        teflonBlockInnerRadius,
                                        (cathodeRingBotPos_Z - belowCathVolBot_Z)/2.,
                                        0.*deg,360.*deg);
  G4Tubs * below_cath_grid_vol = new G4Tubs("below_cath_grid_vol",
                                        0.*cm,
                                        fieldRingInnerRadius,
                                        (0.05 * 2.54 * cm)/2.,
                                        0.*deg,360.*deg);
  G4UnionSolid * below_cath_volume_total = new G4UnionSolid("below_cath_volume_total",
                                        below_cath_ring_vol,
                                        below_cath_grid_vol,
                                        0,
                                        G4ThreeVector(0.,0.,(cathodeRingBotPos_Z - belowCathVolBot_Z)/2. + (0.05 * 2.54 * cm)/2.));
  G4LogicalVolume * below_cath_log = new G4LogicalVolume( below_cath_volume_total,
                                        BACCmaterials->LiquidXe(),
                                        "below_cath_log");
  below_cath_log->SetVisAttributes( BACCmaterials->LiquidXeVis() );
  BaccDetectorComponent * liquid_xenon_below_cathode = new BaccDetectorComponent(0,
                                        G4ThreeVector(0.,0.,(cathodeRingBotPos_Z + belowCathVolBot_Z)/2.),
                                        below_cath_log,
                                        "liquid_xenon_below_cathode",
                                        liquid_xenon_log,
                                        0,0,true);

// Liquid xenon above the extraction grid,
  double aboveExtVolBot_Z = extractionBotPos_Z + 0.1 * 2.54 * cm;
  double aboveExtVolTop_Z = extractionBotPos_Z + fieldGridHalfHeight * 2. + 0.5*cm;
  
  G4Tubs * above_ext_ring_vol = new G4Tubs("above_ext_ring_tubs",
                                       0.*cm,
                                       fieldRingOuterRadius,
                                       0.5*cm/2.,
                                       0.*deg,360.*deg);
  G4Tubs * above_ext_grid_vol = new G4Tubs("above_ext_grid_vol",
                                       0.*cm,
                                       fieldRingInnerRadius,
                                       0.05 * 2.54 * cm / 2.,
                                       0.*deg, 360.*deg);
  G4UnionSolid * above_ext_total = new G4UnionSolid("above_ext_total",
                                       above_ext_ring_vol,
                                       above_ext_grid_vol,
                                       0,G4ThreeVector(0.,0.,-0.25*cm - 0.025 * 2.54 * cm));
  G4LogicalVolume * above_ext_log = new G4LogicalVolume( above_ext_total, BACCmaterials->LiquidXe(), "above_ext_log");
  above_ext_log->SetVisAttributes( BACCmaterials->LiquidXeVis() );
  BaccDetectorComponent * liquid_xenon_above_eg = new BaccDetectorComponent(0,
                                       G4ThreeVector(0.,0., aboveExtVolTop_Z - 0.25*cm),
                                       above_ext_log,
                                       "liquid_xenon_above_eg",
                                       liquid_xenon_log,
                                       0,0,true);
                                       


   

   
 


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_XeDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_XeDetector::~XeNeu_XeDetector(){}
