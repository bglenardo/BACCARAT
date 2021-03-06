////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define XeNeu geometry
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

//
//      C/C++ includes
//
#include <sstream>
#include <ctime>

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
#include "G4Torus.hh"
#include "G4CutTubs.hh"
#include "G4Ellipsoid.hh"
#include "G4Polyhedra.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

//
//      BACCARAT includes
//
#include "BaccManager.hh"
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"
#include "BaccSource.hh"
#include "BaccPhysicsOpticalPhysics.hh"
#include "BaccExample8778PMT.hh"

//
//      Project includes
//
#include "XeNeuDTDetector.hh"
#include "XeNeuDTMaterials.hh"
#include "XeNeuDTMessenger.hh"
//#include "XeNeu_XeDetectorParameters.hh"
#include "XeNeuDT_XeDetector.hh"
#include "XeNeu_PMTCage.hh"
#include "XeNeuDT_LSDetector.hh"
#include "XeNeu_DDCastle.hh"
#include "XeNeu_DDBoratedPoly_1.hh"
#include "XeNeu_DDBoratedPoly_2.hh"
#include "XeNeu_VacuumBox.hh"
#include "XeNeu_TUNLShielding.hh"
#include "XeNeu_DTShielding.hh"
//#include "XeNeu_MaterialTest.hh"
//#include "XeNeu_Sphere.hh"
#include "R8520PMT.hh"
//#include "AluminumTest.hh"
//
//		Definitions
//

//using namespace XeNeu_XeDetectorParameters;
using namespace std;


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Constructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuDTDetector::XeNeuDTDetector( G4String detName,
			G4String outVolName )
{
  detectorName = detName;
  outermostVolumeName = outVolName;
  
  XeNeumessenger = new XeNeuDTMessenger( this );
  
  //***DO NOT*** create another instance of XeNeuDTMaterials anywhere else!
  //Just refer to it using XeNeuDTMaterials::GetInstance().
  XeNeumaterials = XeNeuDTMaterials::GetInstance();
  if(!XeNeumaterials) XeNeumaterials = new XeNeuDTMaterials();
  //get the Bacc Materials and Nist materials
  NISTmaterials = G4NistManager::Instance();
  BACCmaterials = BaccMaterials::GetMaterials();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Destructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuDTDetector::~XeNeuDTDetector()
{
  if(XeNeumessenger) delete XeNeumessenger;
  if(XeNeumaterials) delete XeNeumaterials;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              InitializeDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void XeNeuDTDetector::InitializeDetector(){
  
  //if you need to access BaccManager
  BaccManager *BACCmanager = BaccManager::GetManager();
  
  //you can add the detector-specific source catalog here
  
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              BuildDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void XeNeuDTDetector::BuildDetector(){
  InitializeDetector();

//XeNeu_Sphere * Sphere_obj = new XeNeu_Sphere();
//logicalVolume = Sphere_obj->GetLogicalVolume();

//  R8520PMT * PMT_obj = new R8520PMT();
//  logicalVolume = PMT_obj->GetLogicalVolume(); 

//  XeNeu_PMTCage * PMTCage_Obj = new XeNeu_PMTCage();
//  logicalVolume = PMTCage_Obj->GetLogicalVolume(); 

//  XeNeu_XeDetector * detector_obj = new XeNeu_XeDetector();
//  logicalVolume = detector_obj->GetLogicalVolume();

//  AluminumTest * Aluminum_obj = new AluminumTest();
//  logicalVolume = Aluminum_obj->GetLogicalVolume();

//	XeNeu_DTShielding * dt_shield_obj = new XeNeu_DTShielding();
//	logicalVolume = dt_shield_obj->GetLogicalVolume();

//this is how you access your material definition
//  G4cout<<"XeNeuDTMaterials example"<<G4endl;
//  G4cout<<XeNeumaterials->GetMaterialByName("liquidXe")<<G4endl<<G4endl;
 
  //Alternatively you can use the nist material database
//  G4cout<<"NistMaterials example"<<G4endl;
//  G4cout<< NISTmaterials->FindOrBuildMaterial("G4_lXe")<<G4endl<<G4endl;
  
  //another way is to use BaccMaterials
//  G4cout<<"BaccMaterials example"<<G4endl;
//  G4cout<<BACCmaterials->LiquidXe()<<G4endl<<G4endl;
 
  //  Remember that the outermost vessel is defined as a LUXSimDetector
  //	"logicalVolume".
  //build your detector here    
/*
  //Create a Sphere Around the DT Shielding to isolate its effects
  G4Box * DTShielding_Measurement = new G4Box("DTShielding_Measurement", 4*m, 4*m, 4*m);
  G4LogicalVolume *  DTShielding_Measurement_log = new G4LogicalVolume(DTShielding_Measurement, BACCmaterials->Vacuum(), "DTShielding_Measurement_log");
  DTShielding_Measurement_log->SetVisAttributes(BACCmaterials->WaterVis());
  BaccDetectorComponent * DTShielding_Measurement_object = new BaccDetectorComponent(0,G4ThreeVector(0.,0.,0.),DTShielding_Measurement_log, "DTShielding_Measurement_object", logicalVolume, 0,0,true); 
*/
//Create Laboratory Space
  G4Box * laboratory = new G4Box("laboratory", 8 * m, 8 * m, 8 * m);
  logicalVolume  = new G4LogicalVolume(laboratory, BACCmaterials->Vacuum(), "laboratory");
  logicalVolume->SetVisAttributes( BACCmaterials->VacuumVis() );


// Create the DT Shielding object
if( dtShieldingOn ) {


 XeNeu_DTShielding * dt_shield_obj = new XeNeu_DTShielding();  
// ShieldingVolume = dt_shield_obj->GetLogicalVolume();
  
  BaccDetectorComponent * dt_shield = new BaccDetectorComponent(0,
                                            G4ThreeVector(1.8236*m,.3*m,0),
                                            dt_shield_obj->GetLogicalVolume(),
                                            "dt_shield",
                                            logicalVolume,
                                            0,0,true);
    
      // Build the liquid scintillator detectors
      // 180 deg is straight in front of the DT Beam 
      double d_LS1 =  54.53 * cm + 3.94 * 2.54 * cm + 4.5 * 2.54 *cm;
      double d_LS2 = 18.89*cm + 3.94 * 2.54 * cm + 4.5 * 2.54 *cm;//11.43 added (probably length of inner xenon to OD)
      double d_LS3 = 25.4*cm + 3.94 * 2.54 * cm + 4.5 * 2.54 *cm;//11.43 added (probably length of LS detector?)
      double d_LS4 = 13.66*cm +3.94 * 2.54 * cm + 4.5 * 2.54 *cm;//11.43 added (probably length of LS detector?)
      double d_LS5 =  7.315*cm + 3.94 * 2.54 * cm + 4.5 * 2.54 *cm;//11.43 added (probably length of LS detector?)
      double d_LS6 =  25.15*cm +3.94 * 2.54 * cm + 4.5 * 2.54 *cm;//11.43 added (probably length of LS detector?)
      double d_LS7 = 15.78*cm + 3.94 * 2.54 * cm + 4.5 * 2.54 *cm ;//11.43 added (probably length of LS detector?)
      double d_LS8 = 14.33*cm + 3.94 * 2.54 * cm + 4.5 * 2.54 *cm;//11.43 added (probably length of LS detector?)

      double backingDetectorAngle_1 = 17.*deg;
      double backingDetectorAngle_2 = 92.*deg;
      double backingDetectorAngle_3 = 50.*deg;
      double backingDetectorAngle_4 = 67.*deg;
      double backingDetectorAngle_5 = 115.*deg;
      double backingDetectorAngle_6 = 36.*deg;
      double backingDetectorAngle_7 = 140.*deg;
      double backingDetectorAngle_8 = 162.*deg;

      G4RotationMatrix * rotm_ls_1 = new G4RotationMatrix();
      rotm_ls_1->rotateX(90.*deg);
      rotm_ls_1->rotateY(-(-90.*deg-backingDetectorAngle_1));
    
      G4RotationMatrix * rotm_ls_2 = new G4RotationMatrix();
      rotm_ls_2->rotateX(90.*deg);
      rotm_ls_2->rotateY(-(-90.*deg - backingDetectorAngle_2));

      G4RotationMatrix * rotm_ls_3 = new G4RotationMatrix();
      rotm_ls_3->rotateX(90.*deg);
      rotm_ls_3->rotateY(-(-90.*deg - backingDetectorAngle_3));

      G4RotationMatrix * rotm_ls_4 = new G4RotationMatrix();
      rotm_ls_4->rotateX(90.*deg);
      rotm_ls_4->rotateY((-90.*deg - backingDetectorAngle_4));

      G4RotationMatrix * rotm_ls_5 = new G4RotationMatrix();
      rotm_ls_5->rotateX(90.*deg);
      rotm_ls_5->rotateY((-90.*deg - backingDetectorAngle_5));

      G4RotationMatrix * rotm_ls_6 = new G4RotationMatrix();
      rotm_ls_6->rotateX(90.*deg);
      rotm_ls_6->rotateY((-90.*deg - backingDetectorAngle_6));

      G4RotationMatrix * rotm_ls_7 = new G4RotationMatrix();
      rotm_ls_7->rotateX(90.*deg);
      rotm_ls_7->rotateY(-(-90.*deg - backingDetectorAngle_7));

      G4RotationMatrix * rotm_ls_8 = new G4RotationMatrix();
      rotm_ls_8->rotateX(90.*deg);
      rotm_ls_8->rotateY((-90.*deg - backingDetectorAngle_8));

      double PI = 3.1415927;

      double bdAngRad_1 = backingDetectorAngle_1/deg/180. * PI;
      double bdAngRad_2 = backingDetectorAngle_2/deg/180. * PI;
      double bdAngRad_3 = backingDetectorAngle_3/deg/180. * PI;
      double bdAngRad_4 = backingDetectorAngle_4/deg/180. * PI;
      double bdAngRad_5 = backingDetectorAngle_5/deg/180. * PI;
      double bdAngRad_6 = backingDetectorAngle_6/deg/180. * PI;
      double bdAngRad_7 = backingDetectorAngle_7/deg/180. * PI;
      double bdAngRad_8 = backingDetectorAngle_8/deg/180. * PI;
 
      XeNeuDT_LSDetector * ls_detector_obj = new XeNeuDT_LSDetector();
      BaccDetectorComponent * ls_detector_1 = new BaccDetectorComponent(rotm_ls_1,
                                                           G4ThreeVector(-d_LS1*cos(bdAngRad_1),-d_LS1*sin(bdAngRad_1),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_1",
                                                           logicalVolume,
                                                           0,0,true);
 
      BaccDetectorComponent * ls_detector_2 = new BaccDetectorComponent(rotm_ls_2,
                                                           G4ThreeVector(-d_LS2*cos(bdAngRad_2),-d_LS2*sin(bdAngRad_2),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_2",
                                                           logicalVolume,
                                                           0,0,true);

      BaccDetectorComponent * ls_detector_3 = new BaccDetectorComponent(rotm_ls_3,
                                                           G4ThreeVector(-d_LS3*cos(bdAngRad_3),-d_LS3*sin(bdAngRad_3),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_3",
                                                           logicalVolume,
                                                           0,0,true);
      BaccDetectorComponent * ls_detector_4 = new BaccDetectorComponent(rotm_ls_4,
                                                           G4ThreeVector(-d_LS4*cos(-bdAngRad_4),-d_LS4*sin(-bdAngRad_4),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_4",
                                                           logicalVolume,
                                                           0,0,true);


      BaccDetectorComponent * ls_detector_5 = new BaccDetectorComponent(rotm_ls_5,
                                                           G4ThreeVector(-d_LS5*cos(-bdAngRad_5),-d_LS5*sin(-bdAngRad_5),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_5",
                                                           logicalVolume,
                                                           0,0,true);

      BaccDetectorComponent * ls_detector_6 = new BaccDetectorComponent(rotm_ls_6,
                                                           G4ThreeVector(-d_LS6*cos(-bdAngRad_6),-d_LS6*sin(-bdAngRad_6),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_6",
                                                           logicalVolume,
                                                           0,0,true);

      BaccDetectorComponent * ls_detector_7 = new BaccDetectorComponent(rotm_ls_7,
                                                           G4ThreeVector(- d_LS7*cos(bdAngRad_7), -d_LS7*sin(bdAngRad_7),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_7",
                                                           logicalVolume,
                                                           0,0,true);

      BaccDetectorComponent * ls_detector_8 = new BaccDetectorComponent(rotm_ls_8,
                                                           G4ThreeVector(-(d_LS8)*cos(-bdAngRad_8),-(d_LS8)*sin(-bdAngRad_8),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_8",
                                                           logicalVolume,
                                                           0,0,true);


}
  G4RotationMatrix * det_rot = new G4RotationMatrix();
   det_rot->rotateZ(180.*deg);
  double detector_Z = 1.*cm;
// Build the liquid xenon detector and stuff
  XeNeuDT_XeDetector * detector_obj = new XeNeuDT_XeDetector();
  BaccDetectorComponent * detector = new BaccDetectorComponent(det_rot, 
                                                       G4ThreeVector(0,0,detector_Z),
                                                       detector_obj->GetLogicalVolume(),
                                                       "detector",
                                                       logicalVolume,
                                                        0, 0, true);


  // Build the vacuum box
  double detectorHalfHeight = 7. * 2.54 * cm; 
  double vacuumBoxHalfHeight = 3. * 2.54 * cm; 
  double vacuumBoxHole_Y = (11. - 5.)*2.54 * cm;

  XeNeu_VacuumBox * vacuum_box_obj = new XeNeu_VacuumBox();
  BaccDetectorComponent * vacuum_box = new BaccDetectorComponent(det_rot,
                                        G4ThreeVector(0.,
                                                      -vacuumBoxHole_Y,
                                                      detector_Z + detectorHalfHeight + vacuumBoxHalfHeight),
                                        vacuum_box_obj->GetLogicalVolume(),
                                        "vacuum_box",
                                        logicalVolume,
                                        0,0,true);







  // Build the aluminum base plate
  double baseplateHalfWidth = 15. * 2.54 * cm / 2.;
  double baseplateHalfLength = 26. * 2.54 * cm / 2.;
  double baseplateHalfHeight = 0.375 * 2.54 * cm / 2.;
  double baseplateHoleRadius = 5.5 * 2.54 * cm;

  G4Box * baseplate_solid = new G4Box("baseplate_solid",
                                      baseplateHalfWidth,
                                      baseplateHalfLength,
                                      baseplateHalfHeight);
  G4Tubs * baseplate_hole = new G4Tubs("baseplate_hole",
                                      0.*cm,
                                      baseplateHoleRadius,
                                      baseplateHalfHeight * 2.,
                                      0.*deg,360.*deg);
  G4SubtractionSolid * baseplate_total_solid = new G4SubtractionSolid("baseplate_total_solid",
                                      baseplate_solid,
                                      baseplate_hole,
                                      0,G4ThreeVector(0.,5.5*2.54*cm,0.));
  G4LogicalVolume * baseplate_log = new G4LogicalVolume( baseplate_total_solid, BACCmaterials->Aluminum(), "baseplate_log");
  BaccDetectorComponent * al_baseplate = new BaccDetectorComponent(0,
                                      G4ThreeVector(0.,-5.5*2.54*cm,detector_Z - detectorHalfHeight - baseplateHalfHeight - 2. * 2.54 * cm ),
                                       baseplate_log,
                                      "al_baseplate",
                                      logicalVolume,0,0,true);
 
 // Build the aluminum detector supports
 double supportRadius = 1.5/2. * 2.54 * cm;
 double supportHalfHeight = 16. * 2.54 * cm / 2.; 
 double sideSupportXOffset = baseplateHalfWidth - 0.75 * 2.54 * cm;
 double sideSupportYOffset = -(22. - 5.) * 2.54 * cm;
 double frontSupportYOffset  = (5.5 + 2. - 0.75) * 2.54 * cm;
  
 G4Tubs * al_detector_support = new G4Tubs("al_detector_suport",
                                      0.*cm,
                                      supportRadius,
                                      supportHalfHeight,
                                      0.*deg,360.*deg);
 G4LogicalVolume * al_support_log = new G4LogicalVolume( al_detector_support, BACCmaterials->Aluminum(), "al_support_log");
 BaccDetectorComponent * al_detector_support_1 = new BaccDetectorComponent(0,
                                      G4ThreeVector( 0., frontSupportYOffset, detector_Z - detectorHalfHeight - 2. * 2.54 * cm + supportHalfHeight),
                                      al_support_log,
                                      "al_detector_support_1",
                                      logicalVolume,
                                      0,0,true);
 BaccDetectorComponent * al_detector_support_2 = new BaccDetectorComponent(0,
                                      G4ThreeVector( sideSupportXOffset, sideSupportYOffset, detector_Z - detectorHalfHeight - 2. * 2.54 * cm + supportHalfHeight),
                                      al_support_log,
                                      "al_detector_support_2",
                                      logicalVolume,
                                      0,0,true);
 BaccDetectorComponent * al_detector_support_3 = new BaccDetectorComponent(0,
                                      G4ThreeVector( -sideSupportXOffset, sideSupportYOffset, detector_Z - detectorHalfHeight - 2. * 2.54 * cm + supportHalfHeight),
                                      al_support_log,
                                      "al_detector_support_3",
                                      logicalVolume,
                                      0,0,true);



  return;
/*
  if( ddShieldingOn ) {

      // Build the lead shielding around the detector 
      double detectorOuterRadius  = 4.* 2.54 * cm;
      double shieldThickness = 0.5 * cm;
      double openingAngle = 14.*deg;
    
      G4Tubs * pb_det_shield_1_tubs = new G4Tubs("pb_det_shield_1_tubs",
                                           detectorOuterRadius,
                                           detectorOuterRadius + shieldThickness,
                                           detectorHalfHeight,
                                           -180*deg + openingAngle/2.,
                                           180.*deg + backingDetectorAngle - openingAngle);
      G4LogicalVolume * pb_det_shield_1_log = new G4LogicalVolume(pb_det_shield_1_tubs,
                                           BACCmaterials->Lead(),
                                            "pb_det_shield_1_log");
      pb_det_shield_1_log->SetVisAttributes( BACCmaterials->LeadVis() );
      //pb_det_shield_1_log->SetVisAttributes( BACCmaterials->VacuumVis() );
      BaccDetectorComponent * pb_det_shield_1 = new BaccDetectorComponent(0,
                                           G4ThreeVector(0,0,detector_Z),
                                           pb_det_shield_1_log,
                                           "pb_det_shield_1",
                                           logicalVolume,
                                           0,0,true);
    
      G4Tubs * pb_det_shield_2_tubs = new G4Tubs("pb_det_shield_2_tubs",
                                           detectorOuterRadius,
                                           detectorOuterRadius + shieldThickness,
                                           detectorHalfHeight,
                                           backingDetectorAngle + openingAngle/2.,
                                           180.*deg - backingDetectorAngle - openingAngle);
      G4LogicalVolume * pb_det_shield_2_log = new G4LogicalVolume(pb_det_shield_2_tubs,
                                           BACCmaterials->Lead(),
                                            "pb_det_shield_1_log");
      pb_det_shield_2_log->SetVisAttributes( BACCmaterials->LeadVis() );
      //pb_det_shield_2_log->SetVisAttributes( BACCmaterials->VacuumVis() );
      BaccDetectorComponent * pb_det_shield_2 = new BaccDetectorComponent(0,
                                           G4ThreeVector(0,0,detector_Z),
                                           pb_det_shield_2_log,
                                           "pb_det_shield_2",
                                           logicalVolume,
                                           0,0,true);
    
    
      // Build the liquid scintillator detectors
      double d_LS = 44.*cm + 4.5 * 2.54 * cm;
    
      G4RotationMatrix * rotm_ls_1 = new G4RotationMatrix();
      rotm_ls_1->rotateX(90.*deg);
      rotm_ls_1->rotateY(-(90.*deg-backingDetectorAngle));
    
      G4RotationMatrix * rotm_ls_2 = new G4RotationMatrix();
      rotm_ls_2->rotateX(90.*deg);
      rotm_ls_2->rotateY(-90.*deg - backingDetectorAngle);
    
      double bdAngRad = backingDetectorAngle/deg/180. * PI;
    
      XeNeuDT_LSDetector * ls_detector_obj = new XeNeuDT_LSDetector();
      BaccDetectorComponent * ls_detector_1 = new BaccDetectorComponent(rotm_ls_1,
                                                           G4ThreeVector(d_LS*cos(bdAngRad),d_LS*sin(bdAngRad),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_1",
                                                           logicalVolume,
                                                           0,0,true);
    
      BaccDetectorComponent * ls_detector_2 = new BaccDetectorComponent(rotm_ls_2,
                                                           G4ThreeVector(d_LS*cos(-bdAngRad),d_LS*sin(-bdAngRad),0.),
                                                           ls_detector_obj->GetLogicalVolume(),
                                                           "ls_detector_2",
                                                           logicalVolume,
                                                           0,0,true);
      
    
      // Build the DD shielding castle setup
      XeNeu_DDCastle * dd_castle_obj = new XeNeu_DDCastle();
      BaccDetectorComponent * dd_castle = new BaccDetectorComponent(0,
                                                           G4ThreeVector(-42.*cm,-5.625 * 2.54 * cm,0.),
                                                           dd_castle_obj->GetLogicalVolume(),
                                                           "dd_castle",
                                                           logicalVolume,
                                                           0,0,true);
    
      XeNeu_DDBoratedPoly_1 * b_poly_1_obj = new XeNeu_DDBoratedPoly_1();
    
      double boratedPoly_1_X = -42.*cm + 8.*2.54*cm + (4*2.54*cm + 0.5*cm)/2. + 5.*cm;
      double boratedPoly_1_Y = -(27.*cm/2. + 3.75*2.54*cm);
      BaccDetectorComponent * b_poly_1 = new BaccDetectorComponent(0,
                                                             G4ThreeVector(boratedPoly_1_X,boratedPoly_1_Y,0.),
                                                             b_poly_1_obj->GetLogicalVolume(),
                                                             "b_poly_1",
                                                             logicalVolume,0,0,true);
    
    
    
      XeNeu_DDBoratedPoly_2 * b_poly_2_obj = new XeNeu_DDBoratedPoly_2();
    
      double borated_poly_2_X = -42.*cm + 8.*2.54*cm + (2*2.54*cm + 1.*cm)/2.;
      double borated_poly_2_Y = 0.5*2.54*cm + 31.*cm/2.;
      BaccDetectorComponent * b_poly_2 = new BaccDetectorComponent(0,
                                                             G4ThreeVector(borated_poly_2_X,borated_poly_2_Y,0.),
                                                             b_poly_2_obj->GetLogicalVolume(),
                                                             "b_poly_2",
                                                             logicalVolume,0,0,true);
                                                        
  }
*/
//-------------------------------------------------------------------------------------------------------------------//
//                                         	REFLECTIVITY TESTS						     //
//-------------------------------------------------------------------------------------------------------------------//                                         	
/*
  G4Box * detectorVolume_box = new G4Box("detector", 8.5 * cm, 8.5 * cm, 8.5 * cm);

  G4LogicalVolume * detectorVolume_log = new G4LogicalVolume(detectorVolume_box, BACCmaterials->GasXe(),"detectorVolume_log");

//  detectorVolume_log->SetVisAttributes(BACCmaterials->SteelVis());


  G4Box * vacuumbox = new G4Box("vacuumbox", 8. * cm, 8. * cm, 8. * cm);

  G4LogicalVolume * vacuumbox_log = new G4LogicalVolume(vacuumbox, BACCmaterials->LiquidXe(),"vacuumbox_log");
 
*/
/*
 G4Box * measurementbox = new G4Box("measurementbox", 8. * cm, 8. * cm, 1 * mm);
  G4LogicalVolume * measurementbox_log = new G4LogicalVolume(measurementbox, BACCmaterials->LiquidXe(),"measurementbox_log");

  measurementbox_log->SetVisAttributes(BACCmaterials->WaterVis());

  BaccDetectorComponent * measurementbox_obj = new BaccDetectorComponent(0,
                                                                  G4ThreeVector(0,0,0),
                                                                  measurementbox_log,
                                                                  "measurementbox_obj",
                                                                  vacuumbox_log,
                                                                  0,0,true);
*/
/*
  BaccDetectorComponent * vacuumbox_obj = new BaccDetectorComponent(0,
                                                                  G4ThreeVector(0,0,0),
                                                                  vacuumbox_log,
                                                                  "vacuumbox_obj",
                                                                  detectorVolume_log,
                                                                  0,0,true);


  BaccDetectorComponent * detectorVolume_obj = new BaccDetectorComponent(0,
  								G4ThreeVector(0,0,0),
  								detectorVolume_log,
  								"detectorVolume_obj",
  								logicalVolume,
  								0,0,true);
*/
/*
         G4LogicalBorderSurface *teflonLXeSurface;

         teflonLXeSurface = new G4LogicalBorderSurface(
         				                "teflonLXeSurface",
							 vacuumbox_obj,
							 detectorVolume_obj,
							 baccMaterials->LXeTeflonSurface() );
*/
//G4OpticalSurface * teflonLXeSurface = new G4OpticalSurface("teflonLXeSurface", detectorVolume_obj, vacuumbox_obj, dielectric_dielectric);
//teflonLXeSurface->SetMaterialPropertiesTable(baccMaterials->LXeTeflonSurface() );

}






//------++++++------++++++------++++++------++++++------++++++------++++++------
//                                      SetOrigin()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4ThreeVector XeNeuDTDetector::SetOrigin(BaccDetectorComponent *ExpHall){
  //you need to implement this function here
  return G4ThreeVector(0,0,0);
}
