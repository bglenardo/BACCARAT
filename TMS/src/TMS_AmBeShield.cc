////////////////////////////////////////////////////////////////////////////////
//
// TMS_AmBeShield.cc
//
// This is the class implementation of the actual TMS
// cell used in the Stanford recoil experiments.
//
////////////////////////////////////////////////////////////////////////////////
/*
********************************************************************************
*
*      Change log
*      16  May 2018 - Initial coding. (Brian)
*
********************************************************************************
*/

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
//#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
//#include "G4Torus.hh"
//#include "G4CutTubs.hh"
//#include "G4Ellipsoid.hh"
//#include "G4Polyhedra.hh"
//#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

//
//      BACCARAT includes
//
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"
#include "BaccSource.hh"

//
//      Project includes
//
#include "TMSDetector.hh"
#include "TMSMaterials.hh"
#include "TMSMessenger.hh"
#include "TMS_AmBeShield.hh"
//
//		Definitions
//

using namespace TMSDetectorParameters;
using namespace std;



//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Constructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_AmBeShield::TMS_AmBeShield()
{
   cerr << "Building TMS_AmBeShield..." << endl; 
// variables:
  double waterTankDiam = 47. * 2.54 * cm;
  double waterTankHeight = 38. * 2.54 * cm;
  double innerVesselHeight = 22.5 * 2.54 * cm;
  double innerVesselWidth = 17. * 2.54 * cm;
  double innerVesselDepth = 17. * 2.54 * cm;
  double leadThickness = 2. * 2.54 * cm;
  double leadHeight = 8. * 2.54 * cm;
  double leadWidth = (8. + 2*2.) * 2.54 * cm;
  double leadDepth = 6. * 2.4 * cm;
  double paraffinBlockHeight = 8.*2.54 * cm;
  double paraffinBlockWidth = 17.*2.54 * cm;
  double paraffinBlockDepth = 17.*2.54*cm;


  BACCmaterials = BaccMaterials::GetMaterials();
  TMSmaterials = TMSMaterials::GetInstance();

  G4Tubs * outerVolume_tubs = new G4Tubs("outer_volume",
                                       0.*mm,
                                       waterTankDiam / 2.,
                                       (waterTankHeight)/2.,
                                       0.*deg,360.*deg);
  outerVolume_log = new G4LogicalVolume( outerVolume_tubs, BACCmaterials->Air(), "outerVolume");
  outerVolume_log->SetVisAttributes( BACCmaterials->AirVis() );


  // The outer water volume, which will contain all the rest of the stuff
  G4Tubs * waterVolume_tubs = new G4Tubs("water",
                                       0.*mm,
                                       waterTankDiam / 2.,
                                       (waterTankHeight)/2.,
                                       0.*deg,360.*deg);

  G4Tubs * collimator_tubs = new G4Tubs("collimator",
                                       0.*mm,
                                       1.9/2.*25.4*mm,
                                       10.*25.4*mm,
                                       0.*deg,360.*deg);

  G4RotationMatrix * collimator_rot = new G4RotationMatrix();
  collimator_rot->rotateY(90.*deg);


  G4SubtractionSolid * waterShield_solid = new G4SubtractionSolid("waterShield_solid",waterVolume_tubs,collimator_tubs,
                                                  collimator_rot,G4ThreeVector( 18.5*25.4*mm, 0, -0.5*25.4*mm ));

  G4LogicalVolume * waterVolume_log = new G4LogicalVolume( waterShield_solid, TMSmaterials->BoratedWater(), "waterVolume");
  waterVolume_log->SetVisAttributes( BACCmaterials->WaterVis() );

  BaccDetectorComponent * waterShield = new BaccDetectorComponent(0,
                                        G4ThreeVector(0,0,0),
                                        waterVolume_log,
                                        "water_volume",
                                        outerVolume_log,
                                        0,0,true);


   G4Tubs * collimator_pipe_tubs = new G4Tubs("collimator_pipe",
                                       1.61/2.*25.4*mm,
                                       1.9/2.*25.4*mm,
                                       7.*25.4*mm,
                                       0.*deg,360.*deg);
   G4LogicalVolume * collimatorPipe_log = new G4LogicalVolume( collimator_pipe_tubs, BACCmaterials->PVC(), "collimator_pipe");
   collimatorPipe_log->SetVisAttributes( BACCmaterials->PVCVis() );

   BaccDetectorComponent * collimator_tube = new BaccDetectorComponent(collimator_rot,
                                       G4ThreeVector( 15.5*25.4*mm, 0, -0.5*25.4*mm ),
                                       collimatorPipe_log,
                                       "collimator_pipe",
                                       outerVolume_log,
                                       0,0,true);


  // Inner air-filled vessel
  G4Box * innerVessel_box = new G4Box("innerVessel",
                                      innerVesselWidth/2.,
                                      innerVesselDepth/2.,
                                      innerVesselHeight/2.);
  G4LogicalVolume * innerVessel_log = new G4LogicalVolume( innerVessel_box, BACCmaterials->Air(), "innerVessel");
  innerVessel_log->SetVisAttributes( BACCmaterials->CopperVis() );

  double innerVesselZpos = 7.75 * 2.54 * cm;  

  BaccDetectorComponent * innerVessel = new BaccDetectorComponent(0,
                                      G4ThreeVector(0.,0.,innerVesselZpos),
                                      innerVessel_log,
                                      "inner_vessel",
                                      waterVolume_log,
                                      0,0,true);
  
  // Lead shielding around source. I'll create a box and then subtract out an inner volume.
  G4Box * leadBoxFull = new G4Box("leadBoxFull",
                                     leadDepth/2.,
                                     leadWidth/2.,
                                     leadHeight/2.);

  G4Box * leadBoxSubtraction = new G4Box("leadBoxSubtraction",
                                     4.*2.54*cm/2.,
                                     8.*2.54*cm/2.,
                                     4.*2.54*cm/2.);
  G4SubtractionSolid * leadBox_true = new G4SubtractionSolid("leadBox_true",leadBoxFull,leadBoxSubtraction,0,G4ThreeVector(1.*25.4*mm,0,0));
  G4LogicalVolume * leadBox_log = new G4LogicalVolume( leadBox_true, BACCmaterials->Lead(), "leadBox");
  leadBox_log->SetVisAttributes( BACCmaterials->TeflonVis() );
 

  BaccDetectorComponent * leadBox = new BaccDetectorComponent(0,
                                     G4ThreeVector(-1.*25.4*mm,0.,-innerVesselHeight/2.+4.*25.4*mm),
                                     leadBox_log,
                                     "lead_box",
                                     innerVessel_log,
                                     0,0,true);

  G4Box * leadBrick_box = new G4Box("leadBrick_box",
                                    4.*25.4*mm/2.,
                                    2.*25.4*mm/2.,
                                    8.*25.4*mm/2.);
  G4LogicalVolume * leadBrick_log = new G4LogicalVolume( leadBrick_box, BACCmaterials->Lead(), "leadBrick");
  leadBrick_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  
  BaccDetectorComponent * leadBrick_1 = new BaccDetectorComponent(0,
                                    G4ThreeVector(4.*25.4*mm, 3.*25.4*mm, -innerVesselHeight/2.+4.*25.4*mm),
                                    leadBrick_log,
                                    "lead_brick_1",
                                    innerVessel_log,
                                    0,0,true);
  BaccDetectorComponent * leadBrick_2 = new BaccDetectorComponent(0,
                                    G4ThreeVector(4.*25.4*mm, -3.*25.4*mm, -innerVesselHeight/2.+4.*25.4*mm),
                                    leadBrick_log,
                                    "lead_brick_2",
                                    innerVessel_log,
                                    0,0,true);
//
//  // Steel block for activation study
//  G4Box * steelBlock = new G4Box("steelBlock",
//                                4.*2.54*cm,
//                                3.*2.54*cm,
//                                1.*2.54*cm);
//  G4LogicalVolume * steelBlock_log = new G4LogicalVolume( steelBlock, BACCmaterials->Steel(), "steel_block");
//  steelBlock_log->SetVisAttributes( BACCmaterials->WaterVis() );
//
//  BaccDetectorComponent * steel_block = new BaccDetectorComponent(0,
//                                     G4ThreeVector(0.,0.,-8.5*2.54*cm),
//                                     steelBlock_log,
//                                     "steel_block",
//                                     innerVessel_log,
//                                     0,0,true);                              
//                                    
  // Paraffin wax shielding above the source
  G4Box * paraffinBlock_box = new G4Box("paraffinBlock",
                                      paraffinBlockWidth/2.,
                                      paraffinBlockDepth/2.,
                                      paraffinBlockHeight/2.);
  G4LogicalVolume *paraffinBlock_log = new G4LogicalVolume( paraffinBlock_box, TMSmaterials->Paraffin(), "paraffinBlock");

  double paraffinBlockZpos = innerVesselHeight/2. - paraffinBlockHeight/2.;

  BaccDetectorComponent * paraffinBlock = new BaccDetectorComponent(0,
                                      G4ThreeVector(0.,0.,paraffinBlockZpos),
                                      paraffinBlock_log,
                                      "paraffin_block",
                                      innerVessel_log,
                                      0,0,true);
//
                                    
  
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Destructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_AmBeShield::~TMS_AmBeShield(){}
