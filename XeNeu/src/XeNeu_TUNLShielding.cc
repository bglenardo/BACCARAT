/////////////////////////////////////////////////////
//                                                  //
//  XeNeu_TUNLShielding.cc                             //
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
#include "G4Trd.hh"
#include "G4Ellipsoid.hh"

//
//      BACCARAT includes
//
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"

//
//	LZSystemTest includes
//
#include "XeNeu_TUNLShielding.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_TUNLShielding::XeNeu_TUNLShielding()
{


  double collimator_height = 6. * 2.54 * cm;
  double collimator_width = 6. * 2.54 * cm;
  double collimator_inset_depth = 6. * 2.54 * cm;
  double hdpe_block_height = 20. * 2.54 * cm;
  double hdpe_block_width = 20.*2.54*cm;
  double hdpe_block_length = 30.*cm + collimator_inset_depth; 
  double bpe_shield_thickness = 4. * 2.54 * cm;
  double lead_sheet_thickness = 1. * 2.54 * cm;
  double lead_sheet_length = 29. * 2.54 * cm;
  double lead_sheet_width = 4. * 2.54 * cm;
  int num_lead_sheets = 4;
  double unistrut_spacing = 1.5 * 2.54 * cm;

  double full_x_half_length = (hdpe_block_length + bpe_shield_thickness + 
                               num_lead_sheets*lead_sheet_thickness)/2.;

    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

   G4Box * full_shield_assembly_box = new G4Box("full_shield_assembly_box",
                                                 ( hdpe_block_length +
                                                   bpe_shield_thickness +
                                                   num_lead_sheets*lead_sheet_thickness )/2.,
                                                 ( bpe_shield_thickness*2. +
                                                   hdpe_block_width )/2.,
                                                 ( bpe_shield_thickness*2. +
                                                   hdpe_block_height )/2. );
   full_shield_assembly_log = new G4LogicalVolume( full_shield_assembly_box, 
                                                   BACCmaterials->Vacuum(), 
                                                   "full_shield_assembly_log");
  full_shield_assembly_log->SetVisAttributes( BACCmaterials->LeadVis() );
                                     
  G4Box * bpe_outer_shield_box = new G4Box("bpe_outer_shield_box",
                                           (hdpe_block_length + bpe_shield_thickness)/2.,
                                           (hdpe_block_height + 2.*bpe_shield_thickness)/2.,
                                           (hdpe_block_width + 2.*bpe_shield_thickness)/2.);
  G4LogicalVolume * bpe_outer_shield_log = new G4LogicalVolume( bpe_outer_shield_box,
                                                                BACCmaterials->PolyethyleneBorated(),
                                                                "bpe_outer_shield_log");
  bpe_outer_shield_log->SetVisAttributes( BACCmaterials->PolyethyleneBoratedVis() );
  BaccDetectorComponent * bpe_outer_shield = new BaccDetectorComponent( 0,
                                           G4ThreeVector(-full_x_half_length+(hdpe_block_length + bpe_shield_thickness)/2.,
                                                         0,0),
                                           bpe_outer_shield_log,
                                           "bpe_outer_shield",
                                           full_shield_assembly_log,
                                           0,0,true);

  //example box -- make it into a minimal working example
   G4Box * hdpe_block_box = new G4Box("hdpe_block_box", 
                                             hdpe_block_length/2., 
                                             hdpe_block_width/2., 
                                             hdpe_block_height/2.);
  G4LogicalVolume * hdpe_block_log = new G4LogicalVolume( hdpe_block_box, BACCmaterials->Polyethylene(), "hdpe_block_log");
  hdpe_block_log->SetVisAttributes( BACCmaterials->TeflonVis() );

  BaccDetectorComponent * hdpe_block = new BaccDetectorComponent(0,
                                             G4ThreeVector(-bpe_shield_thickness/2.,0.,0.),
                                             hdpe_block_log,
                                             "hdpe_block",
                                             bpe_outer_shield_log,
                                             0,0,true);

  // Create the empty space for the target inset. 
  G4Box * target_inset_box = new G4Box("target_inset_box",
                                   collimator_inset_depth/2.,
                                   collimator_inset_depth/2.,
                                   collimator_inset_depth/2.);
  G4LogicalVolume * target_inset_log = new G4LogicalVolume( target_inset_box,
                                                            BACCmaterials->Air(), "target_inset_log");
  target_inset_log->SetVisAttributes( BACCmaterials->CopperVis() );
  BaccDetectorComponent * target_inset = new BaccDetectorComponent(0,
                                             G4ThreeVector((-hdpe_block_length+collimator_inset_depth)/2.,0,0),
                                             target_inset_log,
                                             "target_inset",
                                             hdpe_block_log,
                                             0,0,true);

  // Create the block for the collimator
  G4Box * collimator_block_box = new G4Box("collimator_block_box",
                                     15.*cm,
                                     collimator_width/2.,
                                     collimator_height/2.);
  G4LogicalVolume * collimator_block_log = new G4LogicalVolume( collimator_block_box,
                                                                BACCmaterials->PolyethyleneBorated(),
                                                                "collimator_block_log");
  collimator_block_log->SetVisAttributes( BACCmaterials->PolyethyleneBoratedVis() );
  BaccDetectorComponent * collimator_block = new BaccDetectorComponent(0,
                                                   G4ThreeVector(collimator_inset_depth/2.,0,0),
                                                   collimator_block_log,
                                                   "collimator_block",
                                                   hdpe_block_log,
                                                   0,0,true);
  
  // Create the trapezoidal collimator pipe
  G4Trd * collimator_trd = new G4Trd("collimator_trd",
                                     1.5*2.54*cm/2.,
                                     0.875*2.54*cm/2.,
                                     1.5*2.54*cm/2.,
                                     0.875*2.54*cm/2.,
                                     15.*cm);
  G4LogicalVolume * collimator_pipe_log = new G4LogicalVolume( collimator_trd,
                                                               BACCmaterials->Air(),
                                                               "collimator_pipe_log");
  collimator_pipe_log->SetVisAttributes( BACCmaterials->CopperVis() );

  G4RotationMatrix * rot = new G4RotationMatrix();
  rot->rotateY(-90.*deg);
  
  BaccDetectorComponent * collimator_pipe = new BaccDetectorComponent(rot,
                                      G4ThreeVector(0,0,0),
                                      collimator_pipe_log,
                                      "collimator_pipe",
                                      collimator_block_log,
                                      0,0,true);

  // Create the beam port in the borated poly shield
  G4Box * beam_port_box = new G4Box("beam_port_box",
                                bpe_shield_thickness/2.,
                                1.*2.54*cm,
                                1.*2.54*cm);
  G4Tubs * beam_port_tubs = new G4Tubs("beam_port_tubs",
                                0.*cm,
                                0.625*2.54*cm,
                                2.*2.54*cm,
                                0.*deg,360.*deg); 
  G4LogicalVolume * beam_port_log = new G4LogicalVolume( beam_port_tubs, BACCmaterials->Air(), "beam_port_log");
  beam_port_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
  BaccDetectorComponent * beam_port = new BaccDetectorComponent(rot,
                                G4ThreeVector(hdpe_block_length/2.,0,0),
                                beam_port_log,
                                "beam_port",
                                bpe_outer_shield_log,
                                0,0,true);



  // Create the lead wall
  G4Box * lead_wall_box = new G4Box("lead_wall_box",
                                      2.*2.54*cm,
                                      (bpe_shield_thickness*2. + hdpe_block_width)/2.,
                                      (bpe_shield_thickness*2. + hdpe_block_width)/2.);

  G4Box * lead_wall_beam_port = new G4Box("lead_wall_beam_port",
                                      2.*2.54*cm,
                                      3.8*cm/2.,
                                      1.*2.54*cm);

  G4SubtractionSolid * lead_wall_sub = new G4SubtractionSolid("lead_wall_sub",lead_wall_box,lead_wall_beam_port);



  G4LogicalVolume * lead_wall_log = new G4LogicalVolume(lead_wall_sub,BACCmaterials->Lead(),
                                                               "lead_wall_log");
  lead_wall_log->SetVisAttributes( BACCmaterials->TestPurpleVis() );
  
  BaccDetectorComponent * lead_wall_near = new BaccDetectorComponent(0,
                                      G4ThreeVector(full_x_half_length-(2.)*2.54*cm,0,0),
                                      lead_wall_log,
                                      "lead_wall_near",
                                      full_shield_assembly_log,
                                      0,0,true);

                       

//  BaccDetectorComponent * lead_sheet_far = new BaccDetectorComponent(0,
//                                      G4ThreeVector(full_x_half_length-(1.)*2.54*cm,0,0),
//                                      lead_sheet_log,
//                                      "lead_sheet_far",
//                                      full_shield_assembly_log,
//                                      0,0,true);





}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_TUNLShielding
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_TUNLShielding::~XeNeu_TUNLShielding(){}
