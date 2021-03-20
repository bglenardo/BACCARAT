////////////////////////////////////////////////////////////////////////////////
//
// TMS_TPC.cc
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
#include <string>
#include <cmath>

//
//      GEANT4 includes
//
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
//#include "G4RotationMatrix.hh"
//#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
//#include "G4Torus.hh"
//#include "G4CutTubs.hh"
//#include "G4Ellipsoid.hh"
//#include "G4Polyhedra.hh"
#include "G4Cons.hh"
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
#include "TMS_TPC.hh"
//
//		Definitions
//

using namespace TMSDetectorParameters;
using namespace std;



//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Constructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_TPC::TMS_TPC()
{
   cerr << "Building TMS_TPC..." << endl; 
// variables:
    double  flangeDiam8in = 8. * 25.4 * mm;
    double  flangeThick8in = 0.88 * 25.4 * mm;
    double  flangeDiam275in = 2.75 * 25.4 * mm;
    double  flangeThick275in = 0.5 * 25.4 * mm;
    double  topFlangeHoleDiam = 1.63 * 25.4 * mm;
    double  tmsInnerDiam = 147.62 * mm;
    double  tmsCanOuterDiam = 152.4 * mm;
    double  tmsCanHeight = 28.2 * mm;
    double  tmsInnerHeight = 72.94 * mm;
    double  tmsActiveBottomHeight = 22. * mm;
    double  tmsActiveTopHeight = ( 22. + 15.51 ) * mm;
    double  tmsActiveSideLen = 70.33 * mm;
    double  tmsActiveHeight = 15.51 * mm;
    double tmsChannelSideLen = 9.*mm;
    double  cathodeBottomDistFromFlange = 20. * mm;
    double  cathodeThickness = 2.*mm;
    double  cathodeSideLength = 103.98*mm;

    double viewportWindowThick8in = 0.32 * 25.4 * mm;
    double viewportWindowDiam = 5.72 * 25.4 * mm;
    double viewportWindowInset = 0.435 * 25.4 * mm;
    double acrylicPMTHolderDiam = 5.*25.4 * mm;
    double acrylicPMTHolderHeight = 2.75 * 25.4 * mm;
    double aluminumRetainerOD = viewportWindowDiam;
    double aluminumRetainerID = acrylicPMTHolderDiam;
    double aluminumRetainerHeight = 0.625 * 25.4 * mm;
    double cherenkovPMTWallThickness = 0.125 * 25.4 * mm;
    double cherenkovPMTConeHeight = 1. * 25.4 * mm;
    double cherenkovPMTFaceDiam = 5. * 25.4 * mm;
    double cherenkovPMTBodyDiam = 3.5 * 25.4 * mm;

  BACCmaterials = BaccMaterials::GetMaterials();
  TMSmaterials = TMSMaterials::GetInstance();

  // The volume containing which contains the entire TPC structure
  G4Tubs * tpcVolume_tubs = new G4Tubs("tpc",
                                       0.*mm,
                                       flangeDiam8in / 2.,
                                       //( 4. * flangeThick8in + tmsCanHeight + 2 * flangeThick275in )/2.,
                                       ( 2. * flangeThick8in + tmsCanHeight/2. + acrylicPMTHolderHeight + cherenkovPMTConeHeight + 5.*25.4*mm),
                                       0.*deg,360.*deg);
  tpcVolume_log = new G4LogicalVolume( tpcVolume_tubs, BACCmaterials->Vacuum(), "tpcVolume");
  tpcVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );

//  // Bottom detector flange (blank)
//  G4Tubs * botFlange_tubs = new G4Tubs("bottom_flange",
//                                       0.*mm,
//                                       flangeDiam8in/2.,
//                                       flangeThick8in/2.,
//                                       0.*deg,360.*deg);
//  G4LogicalVolume * botFlange_log = new G4LogicalVolume( botFlange_tubs, BACCmaterials->Steel(), "botFlange");
//  botFlange_log->SetVisAttributes( BACCmaterials->SteelVis() );
//  
//  double botFlangeZpos = -tmsCanHeight/2. - flangeThick8in - flangeThick8in/2.;
//  BaccDetectorComponent * botFlange = new BaccDetectorComponent(0,
//                                        G4ThreeVector(0.,0.,botFlangeZpos),
//                                        botFlange_log,
//                                        "bottom_flange",
//                                        tpcVolume_log,
//                                        0,0,true);
  // Bottom detector flange (viewport)
  G4Tubs * botFlangeRing_tubs = new G4Tubs("bottom_flange",
                                           viewportWindowDiam/2.,
                                           flangeDiam8in/2.,
                                           flangeThick8in/2.,
                                           0.*deg, 360.*deg);
  G4LogicalVolume * botFlangeRing_log = new G4LogicalVolume( botFlangeRing_tubs, BACCmaterials->Steel(), "window_flange" );
  botFlangeRing_log->SetVisAttributes( BACCmaterials->SteelVis() );
  double botFlangeZpos = -tmsCanHeight/2. - flangeThick8in - flangeThick8in/2.;
  BaccDetectorComponent * botFlange = new BaccDetectorComponent(0,
                                           G4ThreeVector(0.,0.,botFlangeZpos),
                                           botFlangeRing_log,
                                           "window_flange",
                                           tpcVolume_log,
                                           0,0,true);
  
  G4Tubs * botFlangeWindow_tubs = new G4Tubs("window",
                                           0.,
                                           viewportWindowDiam/2.,
                                           viewportWindowThick8in/2.,
                                           0.*deg,360.*deg);
  G4LogicalVolume * botFlangeWindow_log = new G4LogicalVolume( botFlangeWindow_tubs, BACCmaterials->Quartz(), "window" );
  botFlangeWindow_log->SetVisAttributes( BACCmaterials->QuartzVis() );
  double windowZpos = botFlangeZpos + flangeThick8in/2. - viewportWindowInset - viewportWindowThick8in/2.;
  BaccDetectorComponent * botWindow = new BaccDetectorComponent(0,
                                           G4ThreeVector(0.,0.,windowZpos),
                                           botFlangeWindow_log,
                                           "window",
                                           tpcVolume_log,
                                           0,0,true);


  G4Tubs* tmsInsideWindowFlange_tubs = new G4Tubs("tms_inside_window_flange",
                                           0.,
                                           viewportWindowDiam/2.,
                                           viewportWindowInset/2.,
                                           0,360*deg);
  G4LogicalVolume * tmsInsideWindowFlange_log = new G4LogicalVolume( tmsInsideWindowFlange_tubs, TMSmaterials->TMS(), "tms_inside_window_flange");
  tmsInsideWindowFlange_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
  double tmsInsideWindowZpos = windowZpos + viewportWindowThick8in/2. + viewportWindowInset/2.;
  BaccDetectorComponent * tmsInsideWindowFlange = new BaccDetectorComponent(0,
                                           G4ThreeVector(0,0,tmsInsideWindowZpos),
                                           tmsInsideWindowFlange_log,
                                           "tms_inside_window",
                                           tpcVolume_log,
                                           0,0,true);




  G4Tubs * acrylicPMTHolder_tubs = new G4Tubs("acrylicPMTholder",
                                           0.,
                                           acrylicPMTHolderDiam/2.,
                                           acrylicPMTHolderHeight/2.,
                                           0*deg,360.*deg);
  G4Sphere* acrylicPMTcutout_sphere = new G4Sphere("acrylicPMTcutout",
                                           0.,
                                           acrylicPMTHolderDiam/2.,
                                           0.*deg,360.*deg,0.*deg,180.*deg);
  G4SubtractionSolid * acrylicPMTAdapter_sub = new G4SubtractionSolid("acrylicPMTHolder_sub",
                                                           acrylicPMTHolder_tubs,
                                                           acrylicPMTcutout_sphere,
                                                           0,G4ThreeVector(0,0,-acrylicPMTHolderHeight/2.));
  G4LogicalVolume * acrylicPMTHolder_log = new G4LogicalVolume( acrylicPMTAdapter_sub, BACCmaterials->Acrylic(), "pmt_holder" );
  acrylicPMTHolder_log->SetVisAttributes( BACCmaterials->AcrylicVis() );
  double pmtHolderZpos = windowZpos - viewportWindowThick8in/2. - acrylicPMTHolderHeight/2.;
  BaccDetectorComponent * pmtHolder = new BaccDetectorComponent(0,
                                           G4ThreeVector(0,0,pmtHolderZpos),
                                           acrylicPMTHolder_log,
                                           "cherenkov_pmt_holder",
                                           tpcVolume_log,
                                           0,0,true);
  
  G4Sphere * pmtFace_sphere = new G4Sphere( "pmtFace",
                                           cherenkovPMTFaceDiam/2. - cherenkovPMTWallThickness,
                                           cherenkovPMTFaceDiam/2.,
                                           0*deg,360*deg,0*deg,90*deg);
  G4Cons * pmtBody_cone = new G4Cons( "pmtBody_cone",
                                           cherenkovPMTBodyDiam/2. - cherenkovPMTWallThickness,
                                           cherenkovPMTBodyDiam/2.,
                                           cherenkovPMTFaceDiam/2. - cherenkovPMTWallThickness,
                                           cherenkovPMTFaceDiam/2.,
                                           cherenkovPMTConeHeight/2.,
                                           0*deg,360*deg);
  G4Tubs * pmtBody_tube = new G4Tubs( "pmtBody_tube",
                                           cherenkovPMTBodyDiam/2. - cherenkovPMTWallThickness,
                                           cherenkovPMTBodyDiam/2.,
                                           (5. * 25.4 * mm) /2.,
                                           0*deg,360*deg);
  G4UnionSolid * pmtFaceAndCone = new G4UnionSolid("pmtFaceAndCone",pmtFace_sphere,pmtBody_cone,
                                           0,G4ThreeVector(0,0,-cherenkovPMTConeHeight/2.));
  G4UnionSolid * pmtFull = new G4UnionSolid("pmtFull",pmtFaceAndCone,pmtBody_tube,
                                           0,G4ThreeVector(0,0,-cherenkovPMTConeHeight - (5. * 25.4 * mm)/2. ));
  G4LogicalVolume * cherenkovPMT_log = new G4LogicalVolume( pmtFull, BACCmaterials->Quartz(), "cherenkov_pmt" );
  cherenkovPMT_log->SetVisAttributes( BACCmaterials->QuartzVis() );
  double cherenkovpmtZpos = pmtHolderZpos - acrylicPMTHolderHeight/2.; //- cherenkovPMTFaceDiam/2.;
  BaccDetectorComponent* cherenkovPMT = new BaccDetectorComponent(0,
                                           G4ThreeVector(0,0,cherenkovpmtZpos),
                                           cherenkovPMT_log,
                                           "cherenkov_pmt",
                                           tpcVolume_log,
                                           0,0,true);
  
  G4Tubs * aluminumRetainingRing_tubs = new G4Tubs("al_retaining_ring",
                                           aluminumRetainerID/2.,
                                           aluminumRetainerOD/2.,
                                           aluminumRetainerHeight/2.,
                                           0*deg,360*deg);
  G4LogicalVolume* aluminumRetainingRing_log = new G4LogicalVolume( aluminumRetainingRing_tubs, BACCmaterials->Aluminum(), "aluminumRetainingRing_log");
  aluminumRetainingRing_log->SetVisAttributes( BACCmaterials->AluminumVis() );
  double retainingRingZpos = windowZpos - viewportWindowThick8in/2. - aluminumRetainerHeight/2.;
  BaccDetectorComponent* aluminumRetainingRing = new BaccDetectorComponent(0,
                                           G4ThreeVector(0,0,retainingRingZpos),
                                           aluminumRetainingRing_log,
                                           "aluminum_retaining_ring",
                                           tpcVolume_log,
                                           0,0,true);
  
  
  G4Tubs * teflonRetainingRing_tubs = new G4Tubs("ptfe_retaining_ring",
                                           cherenkovPMTBodyDiam/2.,
                                           (7.5 * 25.4 * mm)/2.,
                                           (0.25 * 25.4 * mm)/2.,
                                           0*deg,360*deg);
  G4LogicalVolume * teflonRetainingRing_log = new G4LogicalVolume( teflonRetainingRing_tubs,
                                                                   BACCmaterials->Teflon(),
                                                                   "teflonRetainingRing_log" );
  teflonRetainingRing_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  double teflonRetainingRingZpos = pmtHolderZpos - acrylicPMTHolderHeight/2. - cherenkovPMTConeHeight - (0.25 * 25.4 * mm)/2.;
  BaccDetectorComponent* teflonRetainingRing = new BaccDetectorComponent(0,
                                           G4ThreeVector(0,0,teflonRetainingRingZpos),
                                           teflonRetainingRing_log,
                                           "teflon_retaining_ring",
                                           tpcVolume_log,
                                           0,0,true);
 
  // Bottom nipple flange
  G4Tubs * botNipFlange_tubs = new G4Tubs("bottom_nipple_flange",
                                      tmsInnerDiam/2.,
                                      flangeDiam8in/2.,
                                      flangeThick8in/2.,
                                      0.*deg, 360.*deg);
  G4LogicalVolume * botNipFlange_log = new G4LogicalVolume( botNipFlange_tubs, BACCmaterials->Steel(), "bottom_nipple_flange");
  botNipFlange_log->SetVisAttributes( BACCmaterials->SteelVis() );

  double botNipFlangeZpos = -tmsCanHeight/2. - flangeThick8in/2.;
  BaccDetectorComponent * botNipFlange = new BaccDetectorComponent(0,
                                      G4ThreeVector(0.,0.,botNipFlangeZpos),
                                      botNipFlange_log,
                                      "bottom_nipple_flange",
                                      tpcVolume_log,
                                      0,0,true);

  // Can
  G4Tubs * detectorCan_tubs = new G4Tubs("detector_can",
                                      tmsInnerDiam/2.,
                                      tmsCanOuterDiam/2.,
                                      tmsCanHeight/2.,
                                      0.*deg,360.*deg);
  G4LogicalVolume * detectorCan_log = new G4LogicalVolume( detectorCan_tubs, BACCmaterials->Steel(), "detector_can");
 
  BaccDetectorComponent * detectorCan = new BaccDetectorComponent(0,
                                      G4ThreeVector(0.,0.,0.),
                                      detectorCan_log,
                                      "detector_can",
                                      tpcVolume_log,
                                      0,0,true);

  // Top Nipple Flange
  G4Tubs * topNipFlange_tubs = new G4Tubs("top_nipple_flange",
                                      tmsInnerDiam/2.,
                                      flangeDiam8in/2.,
                                      flangeThick8in/2.,
                                      0.*deg, 360.*deg);
  G4LogicalVolume * topNipFlange_log = new G4LogicalVolume( topNipFlange_tubs, BACCmaterials->Steel(), "top_nipple_flange");
  topNipFlange_log->SetVisAttributes( BACCmaterials->SteelVis() );
 
  BaccDetectorComponent * topNipFlange = new BaccDetectorComponent(0,
                                      G4ThreeVector(0.,0.,tmsCanHeight/2.+flangeThick8in/2.),
                                      topNipFlange_log,
                                      "top_nipple_flange",
                                      tpcVolume_log,
                                      0,0,true);

  // Top Flange
  G4Tubs * topFlange_tubs = new G4Tubs("top_flange",
                                       0.*mm,
                                       flangeDiam8in/2.,
                                       flangeThick8in/2.,
                                       0.*deg,360.*deg);
  
  double topFlangeZpos = tmsCanHeight/2. + flangeThick8in + flangeThick8in/2.;

  // Top flange holes
  G4Tubs * topFlangeHole_tubs = new G4Tubs("top_flange_hole",
                                       0.*mm,
                                       topFlangeHoleDiam/2.,
                                       flangeThick8in/2. + 1.*mm,
                                       0.*deg,360.*deg);

   G4SubtractionSolid * topFlange_onehole = new G4SubtractionSolid("topFlange_onehole",topFlange_tubs,topFlangeHole_tubs,0,
                                                               G4ThreeVector(50.3*mm/2.,50.3*sqrt(3.)/2.*mm,0.));
   G4SubtractionSolid * topFlange_twohole = new G4SubtractionSolid("topFlange_twohole",topFlange_onehole,topFlangeHole_tubs,0,
                                                               G4ThreeVector(-50.3*mm*sqrt(3.)/2.,50.3*mm/2.,0.));
   G4SubtractionSolid * topFlange_threehole = new G4SubtractionSolid("topFlange_threehole",topFlange_twohole,topFlangeHole_tubs,0,
                                                               G4ThreeVector(-50.3*mm/2.,-50.3*mm*sqrt(3.)/2.,0.));
   G4SubtractionSolid * topFlange_fourhole = new G4SubtractionSolid("topFlange_fourhole",topFlange_threehole,topFlangeHole_tubs,0,
                                                               G4ThreeVector(50.3*mm*sqrt(3.)/2.,-50.3*mm/2.,0.));
  
  G4LogicalVolume * topFlange_log = new G4LogicalVolume( topFlange_fourhole, BACCmaterials->Steel(), "topFlange");
  topFlange_log->SetVisAttributes( BACCmaterials->SteelVis() );
                                       


  BaccDetectorComponent * topFlange = new BaccDetectorComponent(0,
                                        G4ThreeVector(0.,0.,topFlangeZpos),
                                        topFlange_log,
                                        "top_flange",
                                        tpcVolume_log,
                                        0,0,true);
 
  // TMS volume
  G4Tubs * tmsVolume_tubs = new G4Tubs("tms_volume",
                                       0.*mm,
                                       tmsInnerDiam/2.,
                                       flangeThick8in + tmsCanHeight/2.,
                                       0.*deg,360.*deg);
  G4LogicalVolume * tmsVolume_log = new G4LogicalVolume( tmsVolume_tubs, TMSmaterials->TMS(), "tms_volume");
  tmsVolume_log->SetVisAttributes( BACCmaterials->WaterVis() );
  BaccDetectorComponent * tmsVolume = new BaccDetectorComponent(0,
                                       G4ThreeVector(0.,0.,0.),
                                       tmsVolume_log,
                                       "tms_volume",
                                       tpcVolume_log,
                                       0,0,true); 

   
  // Active TMS volume
  double activeVolZpos = -tmsCanHeight/2. - flangeThick8in + tmsActiveBottomHeight + tmsActiveHeight/2.;
  G4Box * tmsActiveVol_tubs = new G4Box("active_tms_volume",
                                         tmsActiveSideLen/2.,
                                         tmsActiveSideLen/2.,
                                         tmsActiveHeight/2.);
  G4LogicalVolume * tmsActiveVol_log = new G4LogicalVolume( tmsActiveVol_tubs, TMSmaterials->TMS(), "active_tms_volume");
  tmsActiveVol_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  BaccDetectorComponent * tmsActiveVol = new BaccDetectorComponent(0,
                                         G4ThreeVector(0.,0.,activeVolZpos),
                                         tmsActiveVol_log,
                                         "active_tms_volume",
                                         tmsVolume_log,
                                         0,0,true); 

  // Creating individual channels in the TMS detector
  G4Box * tmsSingleChannel_box = new G4Box("single_channel_box",
                                         tmsChannelSideLen/2.,
                                         tmsActiveSideLen/2.,
                                         tmsActiveHeight/2.);
  G4LogicalVolume * tmsSingleChannel_log = new G4LogicalVolume( tmsSingleChannel_box, TMSmaterials->TMS(), "single_tms_channel");
  tmsSingleChannel_log->SetVisAttributes( BACCmaterials->TestGreenVis() );

  BaccDetectorComponent * tmsChannels[3];
  char channel_name[100];
  for(int i=0; i<3; i++){  
     double channel_x_offset = -tmsActiveSideLen/2. + (i + 1./2.)*tmsChannelSideLen;
     sprintf(channel_name,"tms_channel_%d",i);
     tmsChannels[i]  =  new BaccDetectorComponent(0,
                                         G4ThreeVector(channel_x_offset,0.,0.),
                                         tmsSingleChannel_log,
                                         channel_name,
                                         tmsActiveVol_log,
                                         0,0,true);
  } 

  
//  // Gas pocket if we were to drain out some of the liquid
//  G4Tubs * gas_pocket_disk = new G4Tubs("gas_pocket_disk",
//					0.,tmsInnerDiam/2.,
//                                        tmsActiveHeight/2.,
//                                        0.,360.);
//  G4Box * gas_pocket_subtraction_block = new G4Box("gas_pocket_sub_block",
//                                        tmsInnerDiam/2.,tmsInnerDiam/2.,tmsActiveHeight/2. + 1.*cm);
//
//  G4SubtractionSolid * gas_pocket_solid = new G4SubtractionSolid("gas_pocket",
//                                                           gas_pocket_disk,
//                                                           gas_pocket_subtraction_block,
//                                                           0,G4ThreeVector(0.,-2.*cm,0.));
//
//  G4LogicalVolume * gas_pocket_log = new G4LogicalVolume(gas_pocket_solid,BACCmaterials->Air(),"gas_pocket");
//  gas_pocket_log->SetVisAttributes( BACCmaterials->TestPurpleVis() );
//  BaccDetectorComponent * gas_pocket = new BaccDetectorComponent(0,
//                                         G4ThreeVector(0.,0.,activeVolZpos),
//                                         gas_pocket_log,
//                                         "gas_pocket",
//                                         tmsVolume_log,
//                                         0,0,true);
 
  
  // First 2.75" flange
  double smallFlangeZpos = tmsCanHeight/2. + 2. * flangeThick8in + flangeThick275in/2.;

  G4Tubs * smallFlange_tubs = new G4Tubs("small_flange",
                                         0.*mm,
                                         flangeDiam275in/2.,
                                         flangeThick275in/2.,
                                         0.*deg,360.*deg);
  G4LogicalVolume * smallFlange_log = new G4LogicalVolume( smallFlange_tubs, BACCmaterials->Steel(), "small_flange" );
  smallFlange_log->SetVisAttributes( BACCmaterials->SteelVis() );
  BaccDetectorComponent * smallFlange_1 = new BaccDetectorComponent(0,
                                         G4ThreeVector(50.3*mm/2.,50.3*sqrt(3.)/2.*mm,smallFlangeZpos),
                                         smallFlange_log,
                                         "small_flange_1",
                                         tpcVolume_log,
                                         0,0,true);
  BaccDetectorComponent * smallFlange_2 = new BaccDetectorComponent(0,
                                         G4ThreeVector(-50.3*mm*sqrt(3.)/2.,50.3*mm/2.,smallFlangeZpos),
                                         smallFlange_log,
                                         "small_flange_2",
                                         tpcVolume_log,
                                         0,0,true);
  BaccDetectorComponent * smallFlange_3 = new BaccDetectorComponent(0,
                                         G4ThreeVector(-50.3*mm/2.,-50.3*mm*sqrt(3.)/2.,smallFlangeZpos),
                                         smallFlange_log,
                                         "small_flange_3",
                                         tpcVolume_log,
                                         0,0,true);
  BaccDetectorComponent * smallFlange_4 = new BaccDetectorComponent(0,
                                         G4ThreeVector(50.3*mm*sqrt(3.)/2.,-50.3*mm/2.,smallFlangeZpos),
                                         smallFlange_log,
                                         "small_flange_4",
                                         tpcVolume_log,
                                         0,0,true);

  double cathodeZpos = -tmsCanHeight/2. - flangeThick8in + cathodeBottomDistFromFlange + cathodeThickness/2.;
  
  // Copper cathode
  G4Box * cathode_box = new G4Box("cathode_box",
                                  cathodeSideLength/2.,
                                  cathodeSideLength/2.,
                                  cathodeThickness/2.);
  G4LogicalVolume * cathode_log = new G4LogicalVolume( cathode_box, BACCmaterials->Copper(), "cathode");
  cathode_log->SetVisAttributes( BACCmaterials->CopperVis() );
  BaccDetectorComponent * cathode = new BaccDetectorComponent(0,
                                  G4ThreeVector(0.,0.,cathodeZpos),
                                  cathode_log,
                                  "cathode",
                                  tmsVolume_log,
                                  0,0,true); 
                                    
 
  // Make bolts sticking into space between flanges
  double bolt_rad = 7.13/2.*25.4*mm;
  double bolt_ang = 360./20. * 3.1415926/180.;
  double bolt_height = 3./8. * 25.4 * mm;
  double bolt_diam = 5./16. * 25.4 * mm;
  G4Tubs * bolt_tail_tubs = new G4Tubs("bolt_tail_tubs",
                                  0.*mm,
                                  bolt_diam/2.,
                                  bolt_height/2.,
                                  0.*deg,360.*deg);
  G4LogicalVolume * bolt_tail_log = new G4LogicalVolume( bolt_tail_tubs, BACCmaterials->Steel(), "bolt_tail");
  bolt_tail_log->SetVisAttributes( BACCmaterials->SteelVis() );
  BaccDetectorComponent * bolt_tails[40];
  stringstream bolt_tail_name;
  for(int bb=0; bb<20.; bb++){
     bolt_tail_name.str("");
     bolt_tail_name << "bolt_upper_" << bb;
     bolt_tails[bb] = new BaccDetectorComponent(0,
                              G4ThreeVector(bolt_rad*sin(bb*bolt_ang),bolt_rad*cos(bb*bolt_ang),tmsCanHeight/2.-bolt_height/2.),
                              bolt_tail_log,
                              bolt_tail_name.str().c_str(),
                              tpcVolume_log,
                              0,0,true); 
  }
  for(int bb=20; bb<40; bb++){
     bolt_tail_name.str("");
     bolt_tail_name << "bolt_lower_" << bb;
     bolt_tails[bb] = new BaccDetectorComponent(0,
                              G4ThreeVector(bolt_rad*sin(bb*bolt_ang),bolt_rad*cos(bb*bolt_ang),-tmsCanHeight/2.+bolt_height/2.),
                              bolt_tail_log,
                              bolt_tail_name.str().c_str(),
                              tpcVolume_log,
                              0,0,true); 
  }
 
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Destructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_TPC::~TMS_TPC(){}
