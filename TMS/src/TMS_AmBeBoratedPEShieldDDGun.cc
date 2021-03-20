//
//      C/C++ includes


//
#include <sstream>
#include <ctime>
#include <iostream>

//
//      GEANT4 includes
//
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

//
//      BACCARAT includes
//
#include "BaccManager.hh"
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"
#include "BaccSource.hh"
//#include "BaccPhysicsOpticalPhysics.hh"

//
//      Project includes
//
#include "TMSDetector.hh"
#include "TMSMaterials.hh"
#include "TMSMessenger.hh"
#include "TMS_AmBeBoratedPEShieldDDGun.hh"
//
//		Definitions
//

using namespace TMSDetectorParameters;
using namespace std;


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              BuildDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_AmBeBoratedPEShieldDDGun::TMS_AmBeBoratedPEShieldDDGun(){

  cerr << "\nInside TMS_AmbeBoratedPEShield()...\n" << endl;

  BACCmaterials = BaccMaterials::GetMaterials();
  TMSmaterials = TMSMaterials::GetInstance();
  
  //build your detector here    
  double paraffin_half_height = 5.25/2. * 2.54 * cm;
  double paraffin_half_length = 18.5/2. * 2.54 * cm;
  double bpe_half_height = 12. * 2.54 * cm;
  double bpe_half_width = 13. * 2.54 * cm;
  double bpe_half_depth = 13. * 2.54 * cm;
  double bpe_cavity_height = 12./2. * 2.54 * cm;
  double bpe_cavity_length = 8./2. * 2.54 * cm;
  double bpe_cavity_depth = 8./2. * 2.54 * cm;
  double beam_height_above_bottom = 10.5 * 2.54 * cm; 
  double total_volume_half_height = 19.5 * 2.54 * cm;

  
  // Outer  Box -- make it into a minimal working example
  G4Box * outerVolume_box = new G4Box("outer_volume_box", 14.*2.54*cm, 14.*2.54*cm, total_volume_half_height);
  outerVolume_log  = new G4LogicalVolume(outerVolume_box, BACCmaterials->Air(), "outer_volume");
  outerVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );


//  // Lead bricks box, which contains the scintillator dector and source.
//  G4Box * lead_bricks_box = new G4Box("lead_bricks_box",
//                                              2.*2.54*cm,
//                                              4.*2.54*cm,
//                                              2.5*2.54*cm);
//  G4LogicalVolume * lead_bricks_log = new G4LogicalVolume( lead_bricks_box, BACCmaterials->Lead(), "lead_bricks");
//  lead_bricks_log->SetVisAttributes( BACCmaterials->TeflonVis() );
//  BaccDetectorComponent * lead_bricks = new BaccDetectorComponent(0,
//                                              G4ThreeVector(0,0,0),
//                                              lead_bricks_log,
//                                              "lead_bricks",
//                                              outerVolume_log,
//                                              0,0,true);
// 
//  G4Tubs * center_hole_tubs = new G4Tubs("center_hole_tubs",
//                                              0.*cm, 0.7 * 2.54 * cm,
//                                              2.*2.54*cm,
//                                              0.*deg, 360.*deg);
//  G4LogicalVolume * center_hole_log = new G4LogicalVolume( center_hole_tubs, BACCmaterials->Air(), "center_hole_log" );
//  center_hole_log->SetVisAttributes( BACCmaterials->CopperVis() );
//  BaccDetectorComponent * center_hole = new BaccDetectorComponent(0,
//                                              G4ThreeVector(0,0,0.5*2.54*cm),
//                                              center_hole_log,
//                                              "center_hole",
//                                              lead_bricks_log,
//                                              0,0,true);
//
// 
//  G4Tubs * scint_hole_tubs = new G4Tubs("scint_hole_tubs",
//                                              0.*cm, 1.1 * 2.54 * cm,
//                                              2.*2.54*cm,
//                                              0.*deg, 360.*deg);
//  G4LogicalVolume * scint_hole_log = new G4LogicalVolume( scint_hole_tubs, BACCmaterials->Air(), "scint_hole_log" );
//  scint_hole_log->SetVisAttributes( BACCmaterials->CopperVis() );
//  BaccDetectorComponent * scint_hole = new BaccDetectorComponent(0,
//                                              G4ThreeVector(0,2.1*2.54*cm,0.5*2.54*cm),
//                                              scint_hole_log,
//                                              "scint_hole",
//                                              lead_bricks_log,
//                                              0,0,true);
//
// 
//  G4Tubs * scintillator_tubs = new G4Tubs("scintillator_tubs",
//                                              0.*cm, 0.75 * 2.54 * cm,
//                                              0.75*2.54*cm,
//                                              0.*deg, 360.*deg);
//  G4LogicalVolume * scintillator_log = new G4LogicalVolume( scintillator_tubs, TMSmaterials->SodiumIodide(), "scintillator_log" );
//  scintillator_log->SetVisAttributes( BACCmaterials->EJ200Vis() );
//  BaccDetectorComponent * scintillator = new BaccDetectorComponent(0,
//                                              G4ThreeVector(0,0,-0.95*2.54*cm),
//                                              scintillator_log,
//                                              "scintillator",
//                                              scint_hole_log,
//                                              0,0,true);
//  
//
//  G4Box * single_lead_brick_box = new G4Box("single_lead_brick_box",
//                                             1.*2.54*cm,
//                                             2.*2.54*cm,
//                                             4.*2.54*cm);
//  G4LogicalVolume * single_lead_brick_log = new G4LogicalVolume( single_lead_brick_box, BACCmaterials->Lead(), "single_lead_brick_log" );
//  single_lead_brick_log->SetVisAttributes( BACCmaterials->EJ200Vis() );
//
//  BaccDetectorComponent * single_lead_brick_1 = new BaccDetectorComponent(0,
//                                             G4ThreeVector(-3.*2.54*cm,2.*2.54*cm,1.5*2.54*cm),
//                                             single_lead_brick_log,
//                                             "single_lead_brick_1",
//                                             outerVolume_log,
//                                             0,0,true);
//  BaccDetectorComponent * single_lead_brick_2 = new BaccDetectorComponent(0,
//                                             G4ThreeVector(-3.*2.54*cm,-2.*2.54*cm,1.5*2.54*cm),
//                                             single_lead_brick_log,
//                                             "single_lead_brick_2",
//                                             outerVolume_log,
//                                             0,0,true);
//  
//
// G4Box * half_lead_brick_box = new G4Box("half_lead_brick_box",
//                                             1.*2.54*cm,
//                                             1.*2.54*cm,
//                                             4.*2.54*cm);
// G4LogicalVolume * half_lead_brick_log = new G4LogicalVolume( half_lead_brick_box, BACCmaterials->Lead(), "half_lead_brick_log" );
// half_lead_brick_log->SetVisAttributes( BACCmaterials->EJ200Vis() );
// 
//  BaccDetectorComponent * half_lead_brick_1 = new BaccDetectorComponent(0,
//                                             G4ThreeVector(3.*2.54*cm,3.*2.54*cm,1.5*2.54*cm),
//                                             half_lead_brick_log,
//                                             "half_lead_brick_1",
//                                             outerVolume_log,
//                                             0,0,true);
//  BaccDetectorComponent * half_lead_brick_2 = new BaccDetectorComponent(0,
//                                             G4ThreeVector(3.*2.54*cm,-3.*2.54*cm,1.5*2.54*cm),
//                                             half_lead_brick_log,
//                                             "half_lead_brick_2",
//                                             outerVolume_log,
//                                             0,0,true);

 G4Box * bpe_shield_box = new G4Box("bpe_shield_box",
                                             13.*2.54*cm,
                                             13.*2.54*cm,
                                             12.*2.54*cm);
 G4Box * bpe_shield_cavity_box = new G4Box("bpe_shield_cavity_box",
                                             9.*2.54*cm,
                                             2.5*2.54*cm,
                                             2.5*2.54*cm);
 G4Tubs * bpe_beam_pipe_tubs = new G4Tubs("bpe_beam_pipe_tubs",
                                             0.*cm,
                                             1.66/2.*2.54*cm,
                                             10.*2.54*cm, 
                                             0.*deg,360.*deg);  

 G4SubtractionSolid * bpe_shield_sub = new G4SubtractionSolid("bpe_shield_sub",
                                                              bpe_shield_box,
                                                              bpe_shield_cavity_box,
                                                              0,
                                                              G4ThreeVector(-4.*2.54*cm,0.,-1.5*2.54*cm) );

 G4RotationMatrix * collimator_rot = new G4RotationMatrix();
 collimator_rot->rotateY(90.*deg);
 
 G4SubtractionSolid * bpe_shield_collimator_sub = new G4SubtractionSolid("bpe_shield_collimator_sub",
                                                              bpe_shield_sub,
                                                              bpe_beam_pipe_tubs,
                                                              collimator_rot,
                                                              G4ThreeVector((10.)*2.54*cm,0.,-1.5*2.54*cm) );

 G4LogicalVolume * bpe_shield_log = new G4LogicalVolume( bpe_shield_collimator_sub, TMSmaterials->BoratedPE_LD(), "bpe_shield_log");
 bpe_shield_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
 BaccDetectorComponent * bpe_shield = new BaccDetectorComponent(0,
                                             G4ThreeVector(0.,0.,(4.-2.5)*2.54*cm),
                                             bpe_shield_log,
                                             "bpe_shield",
                                             outerVolume_log,
                                             0,0,true);
 
// G4Tubs * beam_pipe_pvc_tubs = new G4Tubs("beam_pipe_pvc_tubs",
//                                        1.38/2. * 2.54 * cm,
//                                        1.66/2. * 2.54 * cm,
//                                        4.5 * 2.54 * cm,
//                                        0.*deg, 360.*deg);
// G4LogicalVolume * beam_pipe_pvc_log = new G4LogicalVolume( beam_pipe_pvc_tubs, BACCmaterials->PVC(), "beam_pipe_pvc_log");
// BaccDetectorComponent * beam_pipe_pvc = new BaccDetectorComponent( collimator_rot,
//                                             G4ThreeVector(9.5*2.54*cm,0.,0.),
//                                             beam_pipe_pvc_log,
//                                             "beam_pipe_pvc",
//                                             outerVolume_log,
//                                             0,0,true);

//  G4Tubs * beam_pipe_narrow_pvc_tubs = new G4Tubs("beam_pipe_narrow_pvc_tubs",
//                                       1.049/2. * 2.54 * cm,
//                                       1.315/2. * 2.54 * cm,
//                                       4.5 * 2.54 * cm,
//                                       0.*deg, 360.*deg);
//  G4LogicalVolume * beam_pipe_narrow_pvc_log = new G4LogicalVolume( beam_pipe_narrow_pvc_tubs,
//                                                                    BACCmaterials->PVC(),
//                                                                    "beam_pipe_narrow_pvc_log");
//  BaccDetectorComponent * beam_pipe_narrow_pvc = new BaccDetectorComponent( collimator_rot,
//                                             G4ThreeVector(8.5*2.54*cm,0.,0.),
//                                             beam_pipe_narrow_pvc_log,
//                                             "beam_pipe_narrow_pvc",
//                                             outerVolume_log,
//                                             0,0,true);

 double paraffin_slab_z = (5.25/2. + 4. - 2.5 + 12.)*2.54 * cm;
 G4Box * paraffin_slab_box = new G4Box( "paraffin_slab_box",
                                        18.5/2. * 2.54 * cm,
                                        18.5/2. * 2.54 * cm,
                                        5.25/2. * 2.54 * cm );
 G4LogicalVolume * paraffin_slab_log = new G4LogicalVolume( paraffin_slab_box,TMSmaterials->Paraffin(), "paraffin_slab_log" );
 BaccDetectorComponent * paraffin_slab = new BaccDetectorComponent( 0,
                                               G4ThreeVector(0.,0.,paraffin_slab_z),
                                               paraffin_slab_log,
                                               "paraffin_slab",
                                               outerVolume_log,
                                               0,0,true); 



 G4Box * lead_sheets_box = new G4Box("lead_sheets_box",
                                     0.136*2.54*cm,
                                     12.*2.54*cm,
                                     12.*2.54*cm);
 G4SubtractionSolid * lead_sheets_w_hole_sub = new G4SubtractionSolid( "lead_sheets_w_hole_sub",
                                     lead_sheets_box,
                                     bpe_beam_pipe_tubs,
                                     collimator_rot,
                                     G4ThreeVector(0.,0.,-1.5*2.54*cm) );
 G4LogicalVolume * lead_sheets_log = new G4LogicalVolume( lead_sheets_w_hole_sub, BACCmaterials->Lead(), "lead_sheets_log");
 lead_sheets_log->SetVisAttributes( BACCmaterials->TestPurpleVis() );
 BaccDetectorComponent * lead_sheets = new BaccDetectorComponent(0,
                                     G4ThreeVector((13.+0.136)*2.54*cm,0.,1.5*2.54*cm),
                                     lead_sheets_log,
                                     "lead_sheets",
                                     outerVolume_log,
                                     0,0,true);

 G4Box * lead_sheet_12_by_12  = new G4Box("lead_sheets_three_12_by_12",
                                          3.*0.068*2.54*cm,
                                          6.*2.54*cm,
                                          6.*2.54*cm);
 G4Box * lead_sheet_4_by_12 = new G4Box("lead_sheet_4_by_12",
                                          0.068*2.54*cm,
                                          2.*2.54*cm,
                                          6.*2.54*cm);
 G4Box * lead_sheet_2_by_12 = new G4Box("lead_sheet_2_by_12",
                                          0.068*2.54*cm,
                                          1.*2.54*cm,
                                          6.*2.54*cm);
                                         
 G4UnionSolid * lead_sheet_4_and_2_layers = new G4UnionSolid("lead_sheet_4_and_2_layers",
                                                lead_sheet_12_by_12,
                                                lead_sheet_4_by_12,
                                                0,G4ThreeVector((2.*0.136)*2.54*cm,0.,0.));
 G4UnionSolid * lead_beam_pipe_cover_box = new G4UnionSolid("lead_beam_pipe_cover_box",
                                                lead_sheet_4_and_2_layers,
                                                lead_sheet_2_by_12,
                                                0,G4ThreeVector(3.*0.136*2.54*cm, 0.,0.));
 G4LogicalVolume * lead_beam_pipe_cover_log = new G4LogicalVolume( lead_beam_pipe_cover_box, 
                                                                   BACCmaterials->Lead(),
                                                                   "lead_beam_pipe_cover_log");
 lead_beam_pipe_cover_log->SetVisAttributes( BACCmaterials->TestPurpleVis() );
 BaccDetectorComponent * lead_beam_pipe_cover = new BaccDetectorComponent(0,
                                           G4ThreeVector((13.+0.5+0.068)*2.54*cm,0.,-4.5*2.54*cm),
                                           lead_beam_pipe_cover_log,
                                           "lead_beam_pipe_cover",
                                           outerVolume_log,
                                           0,0,true);


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Destructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_AmBeBoratedPEShieldDDGun::~TMS_AmBeBoratedPEShieldDDGun() {}
