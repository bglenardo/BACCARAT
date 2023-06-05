/////////////////////////////////////////////////////
//                                                  //
//  XeNeu_DDShieldingMigdal.cc                             //
//                                                  //
//  This is the class implementation for the XeNeu  //
//  xenon detector, for use in the LLNL xenon       //
//  recoil experiments                              //
//  Copied from the DT shielding model, with a few  //
//  modifications. 
//                                                  //
//////////////////////////////////////////////////////
/*
********************************************************************************
*
*	Change log
*	
*	2019-06-20 - Initial submission (Daniel Naim)
* 2022-10-06 - Copied from DTShielding
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
//#include "XeNeu_XeDetectorParameters.hh"
#include "XeNeu_DDShieldingMigdal.hh"
#include "XeNeuDTMaterials.hh"
#include "XeNeuDTMessenger.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_DDShieldingMigdal::XeNeu_DDShieldingMigdal()
{

  double water_tank_radius = 93.98 * cm;
  double water_tank_half_height = water_tank_radius;

  double DT_Shift_y_Position = -30 * cm;

  double DT_container_half_height_z = 10.16/2 * cm;
  double DT_container_half_width_y = 38 * cm;
  double DT_container_half_length_x = 10.16/2* cm;

  double BPE_Thickness = 0. * cm; 
  double  Lead_Box_Thickness = 25.4 * cm; 

  double bpe_collimator_radius = 1.27 * cm;
  double bpe_collimator_height = 4.5 * 2.54 * cm; 
  double center_bpe_collimator = -119.38 * cm + 9./2. * 2.54 * cm;
 

  double measurement_box_side = 10 * cm;

  double Dimension_of_World = water_tank_radius + 100*cm;

    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();
    XeNeumaterials = XeNeuDTMaterials::GetInstance();

 // Create the volume that the DT shielding will occupy (for now I have created a 5x5x5m box to represent the room)
  //G4Box * full_shield_assembly_box = new G4Box("full_shield_assembly_box",Dimension_of_World+50*cm,Dimension_of_World+50*cm,Dimension_of_World+50*cm);

   G4Box * full_shield_assembly_box = new G4Box("full_shield_assembly_box",119.38*cm + 2.54*cm,96.52*cm,83.82*cm);

   full_shield_assembly_log = new G4LogicalVolume( full_shield_assembly_box,
                                                   BACCmaterials->Vacuum(),
                                                   "full_shield_assembly_log");
  full_shield_assembly_log->SetVisAttributes( BACCmaterials->LeadVis() );

//Create the Water Tank
//
//First we attempted to create a tube, but found that a square design would be how to best approach the shielding design.
/*
  G4Tubs * water_tank_shielding = new G4Tubs("water_tank_shielding",
                                           0,
                                           water_tank_radius,
					   water_tank_half_height,
                                           0,
					   2*PI);


  G4LogicalVolume * Water_Tank_Shielding_log = new G4LogicalVolume(water_tank_shielding,XeNeumaterials->BoratedWater(), "Water_Tank_Shielding_log");
 
  Water_Tank_Shielding_log->SetVisAttributes(BACCmaterials->WaterVis());
*/
//Square Water Shielding
  G4Box * water_tank_shielding = new G4Box("water_tank_shielding",119.38*cm, 96.52*cm,83.82*cm);
  //G4Box * water_tank_shielding = new G4Box("water_tank_shielding",119.38*cm, 10*cm,83.82*cm);

   G4LogicalVolume * Water_Tank_Shielding_log = new G4LogicalVolume(water_tank_shielding,
                                                   XeNeumaterials->BoratedWater(),
                                                   "Water_Tank_Shielding_log");
   Water_Tank_Shielding_log->SetVisAttributes( BACCmaterials->WaterVis() );





  
  G4RotationMatrix* rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);

//Create the BPE30 that the collimator will sit in
//
G4Box * BPEShielding = new G4Box("BPEShielding", 
                                  9./2. * 2.54 * cm, 
                                  36./2 * 2.54 * cm, 
                                  6./2 * 2.54 * cm);
G4LogicalVolume * BPEShielding_log = new G4LogicalVolume(BPEShielding, XeNeumaterials->BPE30(),"BPEShielding_log");
BPEShielding_log->SetVisAttributes(BACCmaterials->TestRedVis());
BaccDetectorComponent * BPEShielding_object = new BaccDetectorComponent(0,
                              G4ThreeVector(center_bpe_collimator, 0., 0.5*2.54*cm),
                              BPEShielding_log,
                              "BPEShielding_object",
                              Water_Tank_Shielding_log,
                              0,0,true);
G4Box * BPE_5_slab = new G4Box("BPE_5_slab_box",
                               4.5 * 2.54 * cm,
                               18. * 2.54 * cm,
                               0.5 * 2.54 * cm);
G4LogicalVolume * BPE_5_slab_log = new G4LogicalVolume(BPE_5_slab, XeNeumaterials->BPE5(), "BPE_5_slab_log");
BPE_5_slab_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
BaccDetectorComponent * BPE_5_slab_object = new BaccDetectorComponent(0,
                              G4ThreeVector( 0, 0, -0.5*2.54*cm),
                              BPE_5_slab_log,
                              "BPE_5_slab",
                              BPEShielding_log,
                              0,0,true);

//Create the collimator
// G4Tubs * Collimator = new G4Tubs("Collimator", 0, bpe_collimator_radius,bpe_collimator_height , 0, 2*PI);
G4Box * Collimator = new G4Box("Collimator", 4.5*2.54*cm, 2.54/2*cm, 2.54/2*cm);
G4LogicalVolume * Collimator_log = new G4LogicalVolume(Collimator, BACCmaterials->Vacuum(), "Collimator_log");
BaccDetectorComponent * Collimator_object = new BaccDetectorComponent( 0,
                                           G4ThreeVector(0, DT_Shift_y_Position, 0.), 
                                           Collimator_log, 
                                           "Collimator_object", 
                                           BPE_5_slab_log,
                                           0,0,true );
 


//------------------------------------------------------------------------------------------------
// Create the box with the DD generator
double top_al_sheet_thickness = 3./8. * 2.54 *cm;
double bottom_al_sheet_thickness = 5./8. * 2.54 * cm;
double ddgen_box_x_offset = center_bpe_collimator + 9.*2.54*cm;

G4Box * top_al_sheet_box = new G4Box("top_al_sheet_box", 4.5*2.54*cm, 18.*2.54*cm, top_al_sheet_thickness/2.);
G4LogicalVolume * top_al_sheet_log = new G4LogicalVolume( top_al_sheet_box,
                                                          BACCmaterials->Aluminum(),
                                                          "top_al_sheet_log");
top_al_sheet_log->SetVisAttributes( BACCmaterials->AluminumVis() );
BaccDetectorComponent * top_al_sheet = new BaccDetectorComponent( 0, 
                                             G4ThreeVector(ddgen_box_x_offset, 0, 3.5*2.54*cm - top_al_sheet_thickness/2. ),
                                             top_al_sheet_log,
                                             "top_al_sheet",
                                             Water_Tank_Shielding_log,
                                             0,0,true);

G4Box * bottom_al_sheet_box = new G4Box("bottom_al_sheet_box", 4.5*2.54*cm, 18.*2.54*cm, bottom_al_sheet_thickness/2.);
G4LogicalVolume * bottom_al_sheet_log = new G4LogicalVolume( bottom_al_sheet_box,
                                                          BACCmaterials->Aluminum(),
                                                          "bottom_al_sheet_log");
bottom_al_sheet_log->SetVisAttributes( BACCmaterials->AluminumVis() );
BaccDetectorComponent * bottom_al_sheet = new BaccDetectorComponent( 0, 
                                             G4ThreeVector(ddgen_box_x_offset, 0, -2.5*2.54*cm + bottom_al_sheet_thickness/2. ),
                                             bottom_al_sheet_log,
                                             "bottom_al_sheet",
                                             Water_Tank_Shielding_log,
                                             0,0,true);

G4Box * dd_bpe_shield_box = new G4Box("dd_bpe_shield_box", 2.*2.54*cm, 18.*2.54*cm, 2.5*2.54*cm);
G4LogicalVolume * dd_bpe_shield_log = new G4LogicalVolume( dd_bpe_shield_box,
                                                          XeNeumaterials->BPE5(),
                                                          "dd_bpe_shield_log");
dd_bpe_shield_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
BaccDetectorComponent * dd_bpe_shield = new BaccDetectorComponent( 0, 
                                             G4ThreeVector(ddgen_box_x_offset-2.5*2.54*cm, 0, 0.625*2.54*cm),
                                             dd_bpe_shield_log,
                                             "dd_BPE_shield",
                                             Water_Tank_Shielding_log,
                                             0,0,true);

G4Box * dd_short_collimator_box = new G4Box("dd_short_collimator_box", 2.*2.54*cm, 0.5*2.54*cm, 1.*2.54*cm);
G4LogicalVolume * dd_short_collimator_log = new G4LogicalVolume( dd_short_collimator_box,
                                                          BACCmaterials->Vacuum(),
                                                          "dd_short_collimator_log");
// dd_short_collimator_log->SetVisAttributes( BACCmaterials->VacuumVis() );
BaccDetectorComponent * dd_short_collimator = new BaccDetectorComponent( 0, 
                                             G4ThreeVector(0, DT_Shift_y_Position, -0.625*2.54*cm),
                                             dd_short_collimator_log,
                                             "dd_short_collimator",
                                             dd_bpe_shield_log,
                                             0,0,true);

G4Box * dd_vacuum_space_box = new G4Box("dd_vacuum_space_box", 2.5*2.54*cm, 18.*2.54*cm, 2.5*2.54*cm);
G4LogicalVolume * dd_vacuum_space_log = new G4LogicalVolume( dd_vacuum_space_box,
                                             BACCmaterials->Vacuum(), "dd_vacuum_space_log");
BaccDetectorComponent * dd_vacuum_space = new BaccDetectorComponent( 0,
                                            G4ThreeVector( ddgen_box_x_offset + 2.*2.54*cm, 0, 0.625*2.54*cm),
                                            dd_vacuum_space_log,
                                            "dd_vacuum_space",
                                            Water_Tank_Shielding_log,
                                            0,0,true);





//Create The Lead Shielding Surrounding the DT generator

 double DD_Tube_Radius = 10.16/2*cm;

 G4Box * Lead_DT_Box = new G4Box("Lead_DT_Box", 
					DD_Tube_Radius + Lead_Box_Thickness, 
					DD_Tube_Radius + Lead_Box_Thickness + 3.*2.54*cm, 
					DD_Tube_Radius + Lead_Box_Thickness);

 G4LogicalVolume * Lead_Box_log = new G4LogicalVolume(Lead_DT_Box, BACCmaterials->Lead(), "Lead_Box_log");


//Collimator for Lead Shielding around DT 
//
 
// G4Tubs * Lead_Collimator = new G4Tubs("Lead_Collimator", 0, bpe_collimator_radius, (DT_container_half_length_x - DD_Tube_Radius + 2*Lead_Box_Thickness)/2.0, 0, 2*PI);

 G4Box * Lead_Collimator = new G4Box("Lead_Collimator", 2.54/2 * cm, 2.54/2 * cm, (Lead_Box_Thickness)/2);

 G4LogicalVolume * Lead_Collimator_log = new G4LogicalVolume(Lead_Collimator, BACCmaterials->Vacuum(),"Lead_Collimator_log");

 Lead_Collimator_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());


 BaccDetectorComponent * Lead_Collimator_object = new BaccDetectorComponent( rm,
							G4ThreeVector(-(Lead_Box_Thickness +2*DD_Tube_Radius)/2, DT_Shift_y_Position,0.),
							Lead_Collimator_log,
							"Lead_Collimator_object",
							Lead_Box_log, 
							0,0,true);
 
 Lead_Box_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());
 
 //Create the vacuum that the DT generator will sit inside the lead shielding Box
  G4RotationMatrix* rm1 = new G4RotationMatrix();
  rm1->rotateX(90.*deg); 

 //Tube
 G4Tubs * DD_Tube = new G4Tubs("DD_Tube", 0, DD_Tube_Radius, (DT_container_half_width_y), 0,2*PI);
 G4LogicalVolume * DD_Tube_log = new G4LogicalVolume(DD_Tube,BACCmaterials->Vacuum(),"DD_Tube_log"); 
 DD_Tube_log->SetVisAttributes(BACCmaterials->SteelVis());
 BaccDetectorComponent * DD_Tube_obj = new BaccDetectorComponent(rm1,
                                                  G4ThreeVector(0, 0,0),
                                                  DD_Tube_log, 
                                                  "DD_Tube_obj", 
                                                  dd_vacuum_space_log,
                                                  0,0,true);


//Finally Create the Lead Box
 BaccDetectorComponent * Lead_Box_object = new BaccDetectorComponent(0, G4ThreeVector(0,0,0), Lead_Box_log, "Lead_Box_object", Water_Tank_Shielding_log,0,0,true);


/*
//Create the BPE5 Shielding (5% boron concentration 5 cm thickness)

  G4Tubs * BPE5_Shielding = new G4Tubs("BPE5_Shielding", (water_tank_radius - BPE_Thickness), water_tank_radius, water_tank_half_height, 0 , 2*PI);
 
  G4LogicalVolume * BPE5_Shielding_log = new G4LogicalVolume(BPE5_Shielding, XeNeumaterials->BPE5(), "BPE5_Shielding_log");
 
  BaccDetectorComponent * BPE5_Shielding_object = new BaccDetectorComponent(0,
                                         G4ThreeVector(0., 0.,0.),
                                         BPE5_Shielding_log,
                                         "BPE5_Shielding_object",
                                         Water_Tank_Shielding_log,
                                         0,0,true);

*/


/*
//Create a layer of BPE5 inside the water tank 
double distance_of_layer_of_BPE5 = 50. *cm;

G4Tubs * Layer_of_BPE5 = new G4Tubs("Layer_of_BPE5", distance_of_layer_of_BPE5, distance_of_layer_of_BPE5 + BPE_Thickness,water_tank_half_height, 0, 2*PI);

G4LogicalVolume * Layer_of_BPE5_log = new G4LogicalVolume(Layer_of_BPE5, XeNeumaterials->BPE5(),"Layer_of_BPE5_log");

 
BaccDetectorComponent * Layer_of_BPE5_object = new BaccDetectorComponent(0,G4ThreeVector(0.,0.,0.), Layer_of_BPE5_log, "Layer_of_BPE5_object", subtract_log,0,0,true);

*/
/*
//Create the top and bottom BPE5 covers for the water tank
G4Tubs * BPE5_Top_Shielding = new G4Tubs("BPE5_Top_Shielding", 0, water_tank_radius,1*cm, 0 , 2*PI);

//G4Tubs * BPE5_Bottom_Shielding = new G4Tubs("BPE5_Bottom_Shielding", 0, water_tank_radius,BPE_Thickness/2.0, 0 , 2*PI);

G4LogicalVolume * BPE5_Top_Shielding_log = new G4LogicalVolume( BPE5_Top_Shielding, BACCmaterials->Vacuum(), "BPE5_Top_Shielding_log");

//G4LogicalVolume * BPE5_Bottom_Shielding_log = new G4LogicalVolume( BPE5_Bottom_Shielding, XeNeumaterials->BPE5(), "BPE5_Bottom_Shielding_log");


BPE5_Top_Shielding_log->SetVisAttributes(XeNeumaterials->BPE5Vis());
//BPE5_Bottom_Shielding_log->SetVisAttributes(XeNeumaterials->BPE5Vis());



BaccDetectorComponent * BPE5_Top_Shielding_object = new BaccDetectorComponent(0,G4ThreeVector(0.,0.,water_tank_half_height + 1*cm),BPE5_Top_Shielding_log,"BPE5_Top_Shielding_object", full_shield_assembly_log,0,0,true);

//BaccDetectorComponent * BPE5_Bottom_Shielding_object = new BaccDetectorComponent(0,G4ThreeVector(0.,0.,-(water_tank_half_height + (BPE_Thickness/2.))),BPE5_Bottom_Shielding_log,"BPE5_Bottom_Shielding_object", full_shield_assembly_log,0,0,true);
*/
/*
//Create thin sheet of Lead Shielding outside collimator
double Lead_Sheet_Thickness = 1.*cm;

G4Box * Lead_Sheet_Collimator = new G4Box("Lead_Sheet_Collimator", Lead_Sheet_Thickness/2., 10*cm, 10*cm);

G4LogicalVolume * Lead_Sheet_Collimator_log = new G4LogicalVolume(Lead_Sheet_Collimator, BACCmaterials->Lead(), "Lead_Sheet_Collimator_log");

Lead_Sheet_Collimator_log->SetVisAttributes(BACCmaterials->SteelVis());

BaccDetectorComponent * Lead_Sheet_Collimator_object = new BaccDetectorComponent(0,G4ThreeVector(-(water_tank_radius + BPE_Thickness + Lead_Sheet_Thickness ),0.,0.),Lead_Sheet_Collimator_log, "Lead_Sheet_Collimator_object", full_shield_assembly_log,0,0,true);





*/ 
//Create the volume that will measure and stop any particles/gammas entering its volume
/*
 double Measurement_Volume_Thickness = 0.5*cm;

 G4Box * Measurement_Volume = new G4Box("Measurement_Volume",Measurement_Volume_Thickness/2., water_tank_half_height, water_tank_radius);
 
 G4LogicalVolume * Measurement_Volume_log = new G4LogicalVolume( Measurement_Volume, BACCmaterials->Vacuum(), "Measurement_Volume_log");

 Measurement_Volume_log->SetVisAttributes(BACCmaterials->WaterVis());

 BaccDetectorComponent * Measurement_Volume_object = new BaccDetectorComponent(0, 
					 G4ThreeVector(-115.3*cm, 0.,0.), 
					 Measurement_Volume_log, 
					 "Measurement_Volume_object",
					 full_shield_assembly_log,
					 0,0,true);	
*/
//Finally Create the entire WaterTank Shielding
  BaccDetectorComponent * Water_Tank_object = new BaccDetectorComponent(0,G4ThreeVector(0,0,0),Water_Tank_Shielding_log,"Water_Tank_object",full_shield_assembly_log,0,0,true);
  
//Lead shielding outside collimator

G4Box * LeadOutside_Left = new G4Box("LeadOutside_Left", (2.54*0.275)/2*cm,34.4/2*cm,22.62/2*cm);

G4LogicalVolume * LeadOutside_Left_log = new G4LogicalVolume(LeadOutside_Left, BACCmaterials->Lead(),"LeadOutside_Left_log");

LeadOutside_Left_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());

BaccDetectorComponent * LeadOutside_Left_object = new BaccDetectorComponent(0,G4ThreeVector(-119.38*cm - 2.54/2*cm,DT_Shift_y_Position - 34.4/2*cm - 2.54/2* cm,0),LeadOutside_Left_log,"LeadOutside_Left_object",full_shield_assembly_log,0,0,true); 

// Commented out for DT Migdal measurements, August 2022
//G4Box * LeadOutside_Right = new G4Box("LeadOutside_Right", 2.54/2*cm,24.4/2*cm,22.62/2*cm);
G4Box * LeadOutside_Right = new G4Box("LeadOutside_Right", (2.54*0.275)/2*cm,24.4/2*cm,22.62/2*cm);

G4LogicalVolume * LeadOutside_Right_log = new G4LogicalVolume(LeadOutside_Right, BACCmaterials->Lead(),"LeadOutside_Right_log");

LeadOutside_Right_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());

BaccDetectorComponent * LeadOutside_Right_object = new BaccDetectorComponent(0,G4ThreeVector(-119.38*cm - 2.54/2*cm,DT_Shift_y_Position + 24.4/2*cm + 2.54/2* cm,0),LeadOutside_Right_log,"LeadOutside_Right_object",full_shield_assembly_log,0,0,true);


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_DDShieldingMigdal
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_DDShieldingMigdal::~XeNeu_DDShieldingMigdal(){}
