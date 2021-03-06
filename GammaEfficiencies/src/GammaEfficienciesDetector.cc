//////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define GammaEfficiencies geometry
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
//#include "G4Torus.hh"
//#include "G4CutTubs.hh"
//#include "G4Ellipsoid.hh"
//#include "G4Polyhedra.hh"
//#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
//#include "G4UnionSolid.hh"
//#include "G4IntersectionSolid.hh"
//#include "G4OpticalSurface.hh"
//#include "G4LogicalBorderSurface.hh"

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
#include "GammaEfficienciesDetector.hh"
#include "GammaEfficienciesMaterials.hh"
#include "GammaEfficienciesMessenger.hh"
#include "TestShieldMaterials.hh"

//
//		Definitions
//

//using namespace GammaEfficienciesDetectorParameters;
using namespace std;


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Constructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
GammaEfficienciesDetector::GammaEfficienciesDetector( G4String detName,
			G4String outVolName )
{

  cerr << "Inside GammaEfficienciesDetector constructor!!" << endl;
  detectorName = detName;
  outermostVolumeName = outVolName;
  
  GammaEfficienciesmessenger = new GammaEfficienciesMessenger( this );
  
  //***DO NOT*** create another instance of GammaEfficienciesMaterials anywhere else!
  //Just refer to it using GammaEfficienciesMaterials::GetInstance().
  GammaEfficienciesmaterials = GammaEfficienciesMaterials::GetInstance();
  if(!GammaEfficienciesmaterials) GammaEfficienciesmaterials = new GammaEfficienciesMaterials();
  //get the Bacc Materials and Nist materials
  NISTmaterials = G4NistManager::Instance();
  BACCmaterials = BaccMaterials::GetMaterials();
  TestShieldmaterials = TestShieldMaterials::GetInstance();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Destructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
GammaEfficienciesDetector::~GammaEfficienciesDetector()
{
  if(GammaEfficienciesmessenger) delete GammaEfficienciesmessenger;
  if(GammaEfficienciesmaterials) delete GammaEfficienciesmaterials;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              InitializeDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void GammaEfficienciesDetector::InitializeDetector(){
  
  //if you need to access BaccManager
  BaccManager *BACCmanager = BaccManager::GetManager();
  
  //you can add the detector-specific source catalog here
  
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              BuildDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void GammaEfficienciesDetector::BuildDetector(){

  cerr << "\nInside BuildDetector of GammaEfficienciesDetector...\n" << endl;

  InitializeDetector();

  //this is how you access your material definition
//  G4cout<<"GammaEfficienciesMaterials example"<<G4endl;
//  G4cout<<GammaEfficienciesmaterials->GetMaterialByName("liquidXe")<<G4endl<<G4endl;
 
  //Alternatively you can use the nist material database
//  G4cout<<"NistMaterials example"<<G4endl;
//  G4cout<< NISTmaterials->FindOrBuildMaterial("G4_lXe")<<G4endl<<G4endl;
  
  //another way is to use BaccMaterials
//  G4cout<<"BaccMaterials example"<<G4endl;
//  G4cout<<BACCmaterials->LiquidXe()<<G4endl<<G4endl;
 
  //  Remember that the outermost vessel is defined as a LUXSimDetector
  //	"logicalVolume".
  
  //build your detector here    
  
  //example box -- make it into a minimal working example
  G4Box * example_box = new G4Box("example_box", 5*m,5*m,5*m);
  logicalVolume  = new G4LogicalVolume(example_box, BACCmaterials->Vacuum(), "example_log");
  logicalVolume->SetVisAttributes( BACCmaterials->VacuumVis() );


  G4Box * lab_space_box = new G4Box("lab_space",4*m,4*m,4*m);
  G4LogicalVolume * lab_space_log = new G4LogicalVolume( lab_space_box, BACCmaterials->Vacuum(), "lab_space");
  lab_space_log->SetVisAttributes( BACCmaterials->VacuumVis() );
  BaccDetectorComponent* lab_space = new BaccDetectorComponent(0,
                                            G4ThreeVector(0,0,0),
                                            lab_space_log,
                                            "lab_space",
                                            logicalVolume,
                                            0,0,true);
                                            

 
//  G4Tubs * scintillator_tubs = new G4Tubs("scintillator_tubs",
//                                              0.*cm, 0.75 * 2.54 * cm,
//                                              1.25*2.54*cm,
//                                              0.*deg, 360.*deg);
//  G4LogicalVolume * scintillator_log = new G4LogicalVolume( scintillator_tubs, GammaEfficienciesmaterials->SodiumIodide(), "scintillator_log" );
//  scintillator_log->SetVisAttributes( BACCmaterials->TestBlueVis() );
//  BaccDetectorComponent * scintillator = new BaccDetectorComponent(0,
//                                              G4ThreeVector(0,0.75*2.54*cm,(-4 - 1.25*2.54)*cm),
//                                              scintillator_log,
//                                              "scintillator",
//                                              lab_space_log,
//                                              0,0,true);

//  G4Box * surface_box = new G4Box("surface_box",
//                                       20*cm,0.5*cm,20*cm);
//  G4LogicalVolume * surface_log = new G4LogicalVolume( surface_box, BACCmaterials->Polyethylene(),
//                                       "surface_log");
//  surface_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
//  BaccDetectorComponent * surface = new BaccDetectorComponent(0,
//                                       G4ThreeVector(0,-0.5*cm,0),
//                                       surface_log,
//                                       "surface", 
//                                       lab_space_log,
//                                       0,0,true);

  bool custom_1p1_1p1_1in_detector = false;
  bool ots_1p5_1p5_cylinder_detector = false;
  bool stanford_nai_detector = true;
  bool sg_sipm_detector = false;

  if( custom_1p1_1p1_1in_detector ) {
      G4Box * al_shell_box = new G4Box("al_shell_box",
                                       1./2. * 2.54 * cm,
                                       1.1/2. * 2.54 * cm,
                                       1.1/2. * 2.54 * cm );
      G4LogicalVolume * al_shell_log = new G4LogicalVolume( al_shell_box, BACCmaterials->Aluminum(), "al_shell_log" );
      al_shell_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
      BaccDetectorComponent * al_shell = new BaccDetectorComponent(0,  
                                                  G4ThreeVector(1.25*2.54*cm + 1./2.*2.54*cm,0,0),
                                                  al_shell_log,
                                                  "al_shell",
                                                  lab_space_log,
                                                  0,0,true);
    
    
      G4Box * scintillator_box = new G4Box("scintillator_box",
                                       1./2. * 2.54 * cm - 0.51 * mm,
                                       1.1/2. * 2.54 * cm - 0.51 * mm,
                                       1.1/2. * 2.54 * cm - 0.51 * mm);
      G4LogicalVolume * scintillator_log = new G4LogicalVolume( scintillator_box, GammaEfficienciesmaterials->LaBr3(), "scintillator_log" );
      scintillator_log->SetVisAttributes( BACCmaterials->TestBlueVis() );
      BaccDetectorComponent * scintillator = new BaccDetectorComponent(0,  
                                                  G4ThreeVector(0,0,0),
                                                  scintillator_log,
                                                  "scintillator",
                                                  al_shell_log,
                                                  0,0,true);
  } else if( ots_1p5_1p5_cylinder_detector ) {
      double detector_size = 1.5 * 2.54 * cm;
      G4Tubs * al_shell_tubs = new G4Tubs("al_shell_tubs",0.,detector_size/2.,
                                           detector_size/2., 0.*deg, 360.*deg );
      G4LogicalVolume * al_shell_log = new G4LogicalVolume( al_shell_tubs, BACCmaterials->Aluminum(), "al_shell_log" );
      al_shell_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
      G4RotationMatrix * rot = new G4RotationMatrix();
      rot->rotateY(90.*deg);
      BaccDetectorComponent * al_shell = new BaccDetectorComponent(rot,  
                                                  G4ThreeVector(1.25*2.54*cm + 1.5/2.*2.54*cm,0,0),
                                                  al_shell_log,
                                                  "al_shell",
                                                  lab_space_log,
                                                  0,0,true);
      
      G4Tubs * scintillator_tubs = new G4Tubs("scintillator_tubs",
                                              0., detector_size/2. - 0.51*mm,
                                              detector_size/2. - 0.51*mm,
                                              0.*deg, 360.*deg);
      G4LogicalVolume * scintillator_log = new G4LogicalVolume( scintillator_tubs, GammaEfficienciesmaterials->SodiumIodide(), "scintillator_log" );
      scintillator_log->SetVisAttributes( BACCmaterials->TestBlueVis() );
      BaccDetectorComponent * scintillator = new BaccDetectorComponent(0,  
                                                  G4ThreeVector(0,0,0),
                                                  scintillator_log,
                                                  "scintillator",
                                                  al_shell_log,
                                                  0,0,true);
      

  } else if( stanford_nai_detector ) {
    double detector_diameter = 1.5 * 2.54 *cm;
    double detector_length = 1.5 * 2.54 * cm;
    double detector_position_radius = 1.25 * 2.54 * cm;
    double detector_position_angle = 60.*deg;
    double detector_casing_thickness = 0.1 * 2.54 * cm;

    G4RotationMatrix * rot = new G4RotationMatrix();
    rot->rotateY(90.*deg);
    rot->rotateX(120.*deg);
    double xpos = (detector_position_radius + detector_length/2.)*cos(detector_position_angle);
    double ypos = (detector_position_radius + detector_length/2.)*sin(detector_position_angle);

    G4Tubs * detector_casing_tubs = new G4Tubs("casing_tubs",0.,(detector_diameter/2. + detector_casing_thickness),
                                           detector_length/2. + detector_casing_thickness,
                                           0.*deg,360.*deg);
    G4LogicalVolume * detector_casing_log = new G4LogicalVolume(detector_casing_tubs, BACCmaterials->Aluminum(), "detector_casing_log");
    BaccDetectorComponent * detector_casing = new BaccDetectorComponent(rot,
                                                   G4ThreeVector(xpos,ypos,0),
                                                   detector_casing_log,
                                                   "detector_casing",
                                                   lab_space_log,
                                                   0,0,true);



    G4Tubs * nai_tubs = new G4Tubs("nai_tubs",0.,detector_diameter/2.,
                                   detector_length/2.,0.*deg,360.*deg);
    G4LogicalVolume * scintillator_log = new G4LogicalVolume(nai_tubs, GammaEfficienciesmaterials->SodiumIodide(),"scintillator_log");
    scintillator_log->SetVisAttributes( BACCmaterials->TestBlueVis() );

    BaccDetectorComponent * scintillator = new BaccDetectorComponent(0,
                                                  G4ThreeVector(0,0,0),
                                                  scintillator_log,
                                                  "scintillator",
                                                  detector_casing_log,
                                                  0,0,true);

  } else if ( sg_sipm_detector ) {

    double detector_diameter = 1.5 * 2.54 *cm;
    double detector_length = 1.5 * 2.54 * cm;
    double detector_position_radius = 1.25 * 2.54 * cm;
    double detector_position_angle = 60.*deg;
    double detector_casing_thickness = 0.02 * 2.54 * cm;

    G4RotationMatrix * rot = new G4RotationMatrix();
    rot->rotateY(90.*deg);
    rot->rotateX(120.*deg);
    double xpos = (detector_position_radius + detector_length/2.)*cos(detector_position_angle);
    double ypos = (detector_position_radius + detector_length/2.)*sin(detector_position_angle);

    G4Tubs * detector_casing_tubs = new G4Tubs("casing_tubs",0.,(detector_diameter/2. + detector_casing_thickness),
                                           detector_length/2. + detector_casing_thickness,
                                           0.*deg,360.*deg);
    G4LogicalVolume * detector_casing_log = new G4LogicalVolume(detector_casing_tubs, BACCmaterials->Aluminum(), "detector_casing_log");
    BaccDetectorComponent * detector_casing = new BaccDetectorComponent(rot,
                                                   G4ThreeVector(xpos,ypos,0),
                                                   detector_casing_log,
                                                   "detector_casing",
                                                   lab_space_log,
                                                   0,0,true);



    G4Tubs * nai_tubs = new G4Tubs("nai_tubs",0.,detector_diameter/2.,
                                   detector_length/2.,0.*deg,360.*deg);
    G4LogicalVolume * scintillator_log = new G4LogicalVolume(nai_tubs, GammaEfficienciesmaterials->SodiumIodide(),"scintillator_log");
    scintillator_log->SetVisAttributes( BACCmaterials->TestBlueVis() );

    BaccDetectorComponent * scintillator = new BaccDetectorComponent(0,
                                                  G4ThreeVector(0,0,0),
                                                  scintillator_log,
                                                  "scintillator",
                                                  detector_casing_log,
                                                  0,0,true);

  }

//  double target_chamber_inner_height = 5.125 * 2.54 * cm;
//  double target_chamber_inner_radius = 6.025 * 2.54 * cm;
//  double target_chamber_radial_wall_thickness = 0.325 * 2.54 * cm;
//  double target_chamber_height_wall_thickness = 0.2 * 2.54 * cm;
//  G4Tubs * target_chamber_tubs = new G4Tubs("target_chamber_tubs",
//                                            0.,target_chamber_inner_radius+target_chamber_radial_wall_thickness,
//                                            (target_chamber_inner_height/2.+target_chamber_height_wall_thickness),
//                                            0.*deg,360.*deg);
//  G4Tubs * target_chamber_cavity_tubs = new G4Tubs("target_chamber_cavity_tubs",
//                                                   0.,target_chamber_inner_radius,
//                                                   target_chamber_inner_height/2.,
//                                                    0.*deg,360.*deg);
//  G4SubtractionSolid * target_chamber_sub = new G4SubtractionSolid("target_chamber_sub",target_chamber_tubs,target_chamber_cavity_tubs);
//  G4LogicalVolume * target_chamber_log = new G4LogicalVolume(target_chamber_sub, BACCmaterials->Aluminum(), "target_chamber_log");
//  target_chamber_log->SetVisAttributes( BACCmaterials->AluminumVis() );
//  BaccDetectorComponent * target_chamber = new BaccDetectorComponent(0,
//                                                     G4ThreeVector(0,0,0),
//                                                     target_chamber_log,
//                                                     "target_chamber",
//                                                     lab_space_log,
//                                                     0,0,true);
                                 

  double gas_cell_diameter = 0.5 * 2.54 * cm;
  G4Tubs * gas_cell_tubs = new G4Tubs("gas_cell_tubs",0.,gas_cell_diameter/2.,
                                      1.5 * 2.54 * cm, 0.*deg, 360.*deg);
  
  G4LogicalVolume * gas_in_cell_log = new G4LogicalVolume(gas_cell_tubs,BACCmaterials->GasXe(),"gas_in_cell_log");
  gas_in_cell_log->SetVisAttributes( BACCmaterials->TestRedVis() );
  BaccDetectorComponent * gas_in_cell = new BaccDetectorComponent(0,
                                                G4ThreeVector(0,0,0),
                                                gas_in_cell_log,
                                                "gas_in_cell",
                                                lab_space_log,
                                                0,0,true);


  G4Box * beam_hole = new G4Box("beam_hole_box",
                                1. * 2.54 * cm,
                                0.125 * 2.54 * cm,
                                0.125 * 2.54 * cm);
  G4SubtractionSolid * gas_cell_with_beam_hole = new G4SubtractionSolid("gas_cell_with_beam_hole_sub",
                                                                        gas_cell_tubs,
                                                                        beam_hole,
                                                                        0,G4ThreeVector(0.,0.,0.));
  G4Box * slice_1 = new G4Box("slice_1",
                              0.5*2.54*cm,
                              0.5*2.54*cm,
                              0.125*2.54*cm);
  G4SubtractionSolid * gas_cell_with_beam_hole_and_slice_1 = new G4SubtractionSolid("gas_cell_blah_blah",
                                                                 gas_cell_with_beam_hole,
                                                                 slice_1,
                                                                 0,G4ThreeVector((0.5+0.125/2.)*2.54*cm,-0.5*2.54*cm,0.));

  G4SubtractionSolid * full_gas_cell = new G4SubtractionSolid("full_gas_cell_sub",
                                                              gas_cell_with_beam_hole_and_slice_1,
                                                              slice_1,
                                                              0,G4ThreeVector( -(0.5+0.125/2.)*2.54*cm,-0.5*2.54*cm,0.));
  G4LogicalVolume * full_gas_cell_log = new G4LogicalVolume(full_gas_cell,BACCmaterials->Aluminum(),"full_gas_cell_log");
  BaccDetectorComponent * gas_cell = new BaccDetectorComponent(0,
                                             G4ThreeVector(0,0,0),
                                             full_gas_cell_log,
                                             "gas_cell",
                                             gas_in_cell_log,
                                             0,0,true);

 
 


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                                      SetOrigin()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4ThreeVector GammaEfficienciesDetector::SetOrigin(BaccDetectorComponent *ExpHall){
  //you need to implement this function here
  return G4ThreeVector(0,0,0);
}

