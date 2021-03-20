//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_DDCastle.cc                             //
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
//	C/C++ includes:1
//
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
#include "G4SubtractionSolid.hh"

//
//      BACCARAT includes
//
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"

//
//	LZSystemTest includes
//
#include "XeNeu_DDCastle.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				XeNeu_DDCastle
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_DDCastle::XeNeu_DDCastle()
{

  double total_volume_half_height = 8 * 2.54 * cm;
  double total_volume_half_width = 8 * 2.54 * cm;
  double total_volume_half_length = 25./2. * 2.54 * cm;

  double poly_block_half_height = 8. * 2.54 * cm; 
  double poly_block_half_width = 8. * 2.54 * cm; 
  double poly_block_half_length = 2.125 * 2.54 * cm / 2.;
  double poly_block_hole_radius = 8.25/2. * 2.54 * cm;
   
  double small_poly_block_half_width = 8.375/2. * 2.54 * cm;
  double small_poly_block_half_height = 5.625/2. * 2.54 * cm;
  double small_poly_block_half_length = 1./2. * 2.54 * cm;

  double lead_ring_half_length = (4.*2.125 - 3.)/2. * 2.54 * cm;
  double lead_ring_outer_radius = 4. * 2.54 * cm;
  double lead_ring_inner_radius = 2.5 * 2.54 * cm;

  double dd_generator_half_length = 10. * 2.54 * cm;
  double dd_generator_outer_radius = 2. * 2.54 * cm;


    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

   G4Box * ddCastleVolume_box = new G4Box("detector", 
                                             total_volume_half_width, 
                                             total_volume_half_length, 
                                             total_volume_half_height);
  ddCastleVolume_log = new G4LogicalVolume( ddCastleVolume_box, BACCmaterials->Vacuum(), "ddCastleVolume");
  ddCastleVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );

  G4Box * poly_block_box = new G4Box("poly_block_box",
                                             poly_block_half_width,
                                             poly_block_half_length,
                                             poly_block_half_height);
  G4Tubs * poly_block_cutout = new G4Tubs("poly_block_cutout",
                                             0.*cm,
                                             poly_block_hole_radius,
                                             poly_block_half_length + 1.*cm,
                                             0.*deg,360.*deg);
  G4RotationMatrix * poly_hole_rotm = new G4RotationMatrix();
  poly_hole_rotm->rotateX(90.*deg);
  G4SubtractionSolid * poly_block_final_solid = new G4SubtractionSolid("poly_block_final_solid",
                                            poly_block_box,
                                            poly_block_cutout,
                                            poly_hole_rotm, G4ThreeVector(0.,0.,0.));
  G4LogicalVolume * poly_block_log = new G4LogicalVolume( poly_block_final_solid, 
                                                          BACCmaterials->Polyethylene(),
                                                          "poly_block_log" );
  poly_block_log->SetVisAttributes( BACCmaterials->PolyethyleneVis() );


  G4Box * small_poly_block_box = new G4Box("small_poly_block_box",
                                            small_poly_block_half_width,
                                            small_poly_block_half_length,
                                            small_poly_block_half_height);
  G4LogicalVolume * small_poly_block_log = new G4LogicalVolume( small_poly_block_box,
                                            BACCmaterials->Polyethylene(),
                                            "small_poly_block_log");
  small_poly_block_log->SetVisAttributes( BACCmaterials->PolyethyleneVis() );

  double block_positionY = total_volume_half_length - poly_block_half_length;
  double dd_plane_positionY;
  char block_name[50];

  for(int i=0; i<7; i++) {  
     sprintf(block_name,"poly_block_%d",i);
     poly_blocks[i] = new BaccDetectorComponent(0,
                                            G4ThreeVector(0.,block_positionY,0.),
                                            poly_block_log,
                                            block_name,
                                            ddCastleVolume_log,
                                            0,0,true);                                              
     if( i == 2 ) {
          block_positionY -= (small_poly_block_half_length + poly_block_half_length);
          dd_plane_positionY = block_positionY;
          BaccDetectorComponent * small_poly_block_1 = new BaccDetectorComponent(0,
                                            G4ThreeVector(poly_block_half_width - small_poly_block_half_width,
                                                          block_positionY,
                                                          poly_block_half_height - small_poly_block_half_height),
                                            small_poly_block_log,
                                            "sm_poly_block_1",
                                            ddCastleVolume_log,
                                            0,0,true);
          BaccDetectorComponent * small_poly_block_2 = new BaccDetectorComponent(0,
                                            G4ThreeVector(poly_block_half_width - small_poly_block_half_width,
                                                          block_positionY,
                                                          -(poly_block_half_height - small_poly_block_half_height)),
                                            small_poly_block_log,
                                            "sm_poly_block_2",
                                            ddCastleVolume_log,
                                            0,0,true);
         block_positionY -= (small_poly_block_half_length + poly_block_half_length);
         continue;
     }
     block_positionY -= poly_block_half_length * 2.;                                             
  }

  G4Tubs * lead_ring_tubs = new G4Tubs("lead_ring_tubs",
                                           lead_ring_inner_radius,
                                           lead_ring_outer_radius,
                                           lead_ring_half_length,
                                           0.*deg,360.*deg);
  G4LogicalVolume * lead_ring_log = new G4LogicalVolume(lead_ring_tubs,BACCmaterials->Lead(),"lead_ring_log");
  lead_ring_log->SetVisAttributes( BACCmaterials->LeadVis() );

  G4RotationMatrix * lead_ring_rotm = new G4RotationMatrix();
  lead_ring_rotm->rotateX(90.*deg);  

  BaccDetectorComponent * lead_ring_1 = new BaccDetectorComponent(lead_ring_rotm,
                                           G4ThreeVector(0.,dd_plane_positionY - (small_poly_block_half_length+lead_ring_half_length),0.),
                                           lead_ring_log,
                                           "lead_ring_1",
                                           ddCastleVolume_log,
                                           0,0,true);
  BaccDetectorComponent * lead_ring_2 = new BaccDetectorComponent(lead_ring_rotm,
                                           G4ThreeVector(0.,dd_plane_positionY + (small_poly_block_half_length+lead_ring_half_length),0.),
                                           lead_ring_log,
                                           "lead_ring_2",
                                           ddCastleVolume_log,
                                           0,0,true);

  
  G4Tubs * dd_generator_tubs = new G4Tubs("dd_generator_tubs",
                                          0.*cm,
                                          dd_generator_outer_radius,
                                          dd_generator_half_length,
                                          0.*deg,360.*deg);
  G4Tubs * dd_generator_cutout_tubs = new G4Tubs("dd_generator_cutout_tubs",
                                          0.*cm,
                                          dd_generator_outer_radius-0.065*2.54*cm,
                                          dd_generator_half_length-0.065*2.54*cm,
                                          0.*deg,360.*deg);
  G4SubtractionSolid * dd_generator_final_tubs = new G4SubtractionSolid("dd_generator_final_tubs",
                                          dd_generator_tubs,
                                          dd_generator_cutout_tubs,
                                          0,G4ThreeVector(0.,0.,0.));
  G4LogicalVolume * dd_generator_log = new G4LogicalVolume( dd_generator_final_tubs,
                                          BACCmaterials->Aluminum(),
                                          "dd_generator_log");
  dd_generator_log->SetVisAttributes( BACCmaterials->AluminumVis() );

  G4RotationMatrix * dd_gen_rotm = lead_ring_rotm;
  BaccDetectorComponent * dd_generator = new BaccDetectorComponent(lead_ring_rotm,
                                          G4ThreeVector(0.,-( total_volume_half_length-dd_generator_half_length),0.),
                                          dd_generator_log,
                                          "dd_generator",
                                          ddCastleVolume_log,
                                          0,0,true);

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_DDCastle
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_DDCastle::~XeNeu_DDCastle(){}
