/////////////////////////////////////////////////////
//                                                  //
//  XeNeu_DTShielding.cc                             //
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
*	2019-06-20 - Initial submission (Daniel Naim)
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
#include "XeNeu_DTShielding.hh"
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
XeNeu_DTShielding::XeNeu_DTShielding()
{

  double water_tank_radius = 93.98 * cm;
  double water_tank_half_height = water_tank_radius;

  double DT_Shift_y_Position = -30 * cm;

  double DT_container_half_height_z = 10.16/2 * cm;
  double DT_container_half_width_y = 38 * cm;
  double DT_container_half_length_x = 10.16/2* cm;

  double BPE_Thickness = 0. * cm; 
  double  Lead_Box_Thickness = 25.4 * cm; 

  double water_collimator_radius = 1.27 * cm;
  double water_collimator_height = ((119.38*cm - DT_container_half_length_x - Lead_Box_Thickness)/2.0); 
  double center_water_collimator = ((119.38*cm + DT_container_half_length_x + Lead_Box_Thickness)/2.0);
 

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

 G4Box * BPEShielding = new G4Box("BPEShielding", 15.24/2*cm, 22.86/2*cm, water_collimator_height);

 G4LogicalVolume * BPEShielding_log = new G4LogicalVolume(BPEShielding, XeNeumaterials->BPE30(),"BPEShielding_log");

BPEShielding_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());





//Create the collimator

// G4Tubs * Collimator = new G4Tubs("Collimator", 0, water_collimator_radius,water_collimator_height , 0, 2*PI);

  G4Box * Collimator = new G4Box("Collimator", 2.54/2 * cm, 2.54/2 * cm, water_collimator_height);

 G4LogicalVolume * Collimator_log = new G4LogicalVolume(Collimator ,BACCmaterials->Vacuum(),"Collimator_log");


 BaccDetectorComponent * Collimator_object = new BaccDetectorComponent(0,G4ThreeVector(0,0,0.), Collimator_log, "Collimator_object", BPEShielding_log,0,0,true);
 

 BaccDetectorComponent * BPEShielding_object = new BaccDetectorComponent(rm,G4ThreeVector(-center_water_collimator,DT_Shift_y_Position,0.),BPEShielding_log,"BPEShielding_object",Water_Tank_Shielding_log,0,0,true);


//BaccDetectorComponent * Collimator_object = new BaccDetectorComponent(rm,G4ThreeVector(-center_water_collimator,DT_Shift_y_Position,0.), Collimator_log, "Collimator_object", Water_Tank_Shielding_log,0,0,true);

//Create The Lead Shielding Surrounding the DT generator

 double DT_Tube_Radius = 10.16/2*cm;

 G4Box * Lead_DT_Box = new G4Box("Lead_DT_Box", 
					DT_Tube_Radius + Lead_Box_Thickness, 
					DT_Tube_Radius + Lead_Box_Thickness + 3.*2.54*cm, 
					DT_Tube_Radius + Lead_Box_Thickness);

 G4LogicalVolume * Lead_Box_log = new G4LogicalVolume(Lead_DT_Box, BACCmaterials->Lead(), "Lead_Box_log");


//Collimator for Lead Shielding around DT 
//
 
// G4Tubs * Lead_Collimator = new G4Tubs("Lead_Collimator", 0, water_collimator_radius, (DT_container_half_length_x - DT_Tube_Radius + 2*Lead_Box_Thickness)/2.0, 0, 2*PI);

 G4Box * Lead_Collimator = new G4Box("Lead_Collimator", 2.54/2 * cm, 2.54/2 * cm, (Lead_Box_Thickness)/2);

 G4LogicalVolume * Lead_Collimator_log = new G4LogicalVolume(Lead_Collimator, BACCmaterials->Vacuum(),"Lead_Collimator_log");

 Lead_Collimator_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());


 BaccDetectorComponent * Lead_Collimator_object = new BaccDetectorComponent( rm,
							G4ThreeVector(-(Lead_Box_Thickness +2*DT_Tube_Radius)/2, DT_Shift_y_Position,0.),
							Lead_Collimator_log,
							"Lead_Collimator_object",
							Lead_Box_log, 
							0,0,true);
 
 Lead_Box_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());
 
 //Create the vacuum that the DT generator will sit inside the lead shielding Box
  G4RotationMatrix* rm1 = new G4RotationMatrix();
  rm1->rotateX(90.*deg); 
/*
 //Box
 G4Box * DT_Box = new G4Box("DT_Box", DT_container_half_length_x, DT_container_half_width_y, DT_container_half_height_z);

 G4LogicalVolume * DT_Box_log = new G4LogicalVolume(DT_Box,BACCmaterials->Vacuum(),"DT_Box_log");

 DT_Box_log->SetVisAttributes(BACCmaterials->LeadVis());
 
 BaccDetectorComponent * DT_Box_object = new BaccDetectorComponent(rm,G4ThreeVector(0,0,0),DT_Box_log, "DT_Box_object", Lead_Box_log,0,0,true);
 */

 //Tube
 G4Tubs * DT_Tube = new G4Tubs("DT_Tube", 0, DT_Tube_Radius, (DT_container_half_width_y), 0,2*PI);

 G4LogicalVolume * DT_Tube_log = new G4LogicalVolume(DT_Tube,BACCmaterials->Vacuum(),"DT_Tube_log"); 

 DT_Tube_log->SetVisAttributes(BACCmaterials->SteelVis());

 BaccDetectorComponent * DT_Tube_obj = new BaccDetectorComponent(rm1,G4ThreeVector(0,0,0),DT_Tube_log, "DT_Tube_obj", Lead_Box_log,0,0,true);


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

G4Box * LeadOutside_Left = new G4Box("LeadOutside_Left", 2.54/2*cm,34.4/2*cm,22.62/2*cm);

G4LogicalVolume * LeadOutside_Left_log = new G4LogicalVolume(LeadOutside_Left, BACCmaterials->Lead(),"LeadOutside_Left_log");

LeadOutside_Left_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());

BaccDetectorComponent * LeadOutside_Left_object = new BaccDetectorComponent(0,G4ThreeVector(-119.38*cm - 2.54/2*cm,DT_Shift_y_Position - 34.4/2*cm - 2.54/2* cm,0),LeadOutside_Left_log,"LeadOutside_Left_object",full_shield_assembly_log,0,0,true); 


G4Box * LeadOutside_Right = new G4Box("LeadOutside_Right", 2.54/2*cm,24.4/2*cm,22.62/2*cm);

G4LogicalVolume * LeadOutside_Right_log = new G4LogicalVolume(LeadOutside_Right, BACCmaterials->Lead(),"LeadOutside_Right_log");

LeadOutside_Right_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());

BaccDetectorComponent * LeadOutside_Right_object = new BaccDetectorComponent(0,G4ThreeVector(-119.38*cm - 2.54/2*cm,DT_Shift_y_Position + 24.4/2*cm + 2.54/2* cm,0),LeadOutside_Right_log,"LeadOutside_Right_object",full_shield_assembly_log,0,0,true);


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_DTShielding
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_DTShielding::~XeNeu_DTShielding(){}
