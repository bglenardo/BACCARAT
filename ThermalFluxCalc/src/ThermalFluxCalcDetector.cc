////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define ThermalFluxCalc geometry
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
//#include "G4SubtractionSolid.hh"
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
#include "ThermalFluxCalcDetector.hh"
#include "ThermalFluxCalcMaterials.hh"
#include "ThermalFluxCalcMessenger.hh"
//
//		Definitions
//

//using namespace ThermalFluxCalcDetectorParameters;
using namespace std;


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Constructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
ThermalFluxCalcDetector::ThermalFluxCalcDetector( G4String detName,
			G4String outVolName )
{

  cerr << "Inside ThermalFluxCalcDetector constructor!!" << endl;
  detectorName = detName;
  outermostVolumeName = outVolName;
  
  ThermalFluxCalcmessenger = new ThermalFluxCalcMessenger( this );
  
  //***DO NOT*** create another instance of ThermalFluxCalcMaterials anywhere else!
  //Just refer to it using ThermalFluxCalcMaterials::GetInstance().
  ThermalFluxCalcmaterials = ThermalFluxCalcMaterials::GetInstance();
  if(!ThermalFluxCalcmaterials) ThermalFluxCalcmaterials = new ThermalFluxCalcMaterials();
  //get the Bacc Materials and Nist materials
  NISTmaterials = G4NistManager::Instance();
  BACCmaterials = BaccMaterials::GetMaterials();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          Detector Destructor
//------++++++------++++++------++++++------++++++------++++++------++++++------
ThermalFluxCalcDetector::~ThermalFluxCalcDetector()
{
  if(ThermalFluxCalcmessenger) delete ThermalFluxCalcmessenger;
  if(ThermalFluxCalcmaterials) delete ThermalFluxCalcmaterials;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              InitializeDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void ThermalFluxCalcDetector::InitializeDetector(){
  
  //if you need to access BaccManager
  BaccManager *BACCmanager = BaccManager::GetManager();
  
  //you can add the detector-specific source catalog here
  
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              BuildDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void ThermalFluxCalcDetector::BuildDetector(){

  cerr << "\nInside BuildDetector of ThermalFluxCalcDetector...\n" << endl;

  InitializeDetector();

  //this is how you access your material definition
//  G4cout<<"ThermalFluxCalcMaterials example"<<G4endl;
//  G4cout<<ThermalFluxCalcmaterials->GetMaterialByName("liquidXe")<<G4endl<<G4endl;
 
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
                                            

  G4Tubs * shield_tubs = new G4Tubs( "shield_tubs",
                                            0. * cm, 24.*2.54*cm, // 24" radius
                                            20.*2.54*cm, // 40" tall
                                            0. * deg, 360. * deg);
  G4LogicalVolume * shield_log = new G4LogicalVolume( shield_tubs, BACCmaterials->Water(), "shield_log" );
  shield_log->SetVisAttributes( BACCmaterials->WaterVis() );
  BaccDetectorComponent* shield = new BaccDetectorComponent( 0,
                                            G4ThreeVector(0,0,0),
                                            shield_log,
                                            "shield",
                                            lab_space_log,
                                            0,0,true);


  G4Box * box1_box = new G4Box("box1",
                            5.*cm,
                            0.00001*cm,
                            5.*cm);
  G4LogicalVolume * box1_log = new G4LogicalVolume( box1_box, BACCmaterials->Water(), "box1_log");
  box1_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  BaccDetectorComponent* box1 = new BaccDetectorComponent( 0,
                                            G4ThreeVector(0.,10.*cm,0.),
                                            box1_log,
                                            "box1",
                                            shield_log,
                                            0,0,true);

  G4Box * box2_box = new G4Box("box2",
                            5.*cm,
                            0.00001*cm,
                            5.*cm);
  G4LogicalVolume * box2_log = new G4LogicalVolume( box2_box, BACCmaterials->Water(), "box2_log");
  box2_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  BaccDetectorComponent* box2 = new BaccDetectorComponent( 0,
                                            G4ThreeVector(0.,20.*cm,0.),
                                            box2_log,
                                            "box2",
                                            shield_log,
                                            0,0,true);

  G4Box * box3_box = new G4Box("box3",
                            5.*cm,
                            0.00001*cm,
                            5.*cm);
  G4LogicalVolume * box3_log = new G4LogicalVolume( box3_box, BACCmaterials->Water(), "box3_log");
  box3_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  BaccDetectorComponent* box3 = new BaccDetectorComponent( 0,
                                            G4ThreeVector(0.,30.*cm,0.),
                                            box3_log,
                                            "box3",
                                            shield_log,
                                            0,0,true);

  G4Box * box4_box = new G4Box("box4",
                            5.*cm,
                            0.00001*cm,
                            5.*cm);
  G4LogicalVolume * box4_log = new G4LogicalVolume( box4_box, BACCmaterials->Water(), "box4_log");
  box4_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  BaccDetectorComponent* box4 = new BaccDetectorComponent( 0,
                                            G4ThreeVector(0.,40.*cm,0.),
                                            box4_log,
                                            "box4",
                                            shield_log,
                                            0,0,true);

  G4Box * box5_box = new G4Box("box5",
                            5.*cm,
                            0.00001*cm,
                            5.*cm);
  G4LogicalVolume * box5_log = new G4LogicalVolume( box5_box, BACCmaterials->Water(), "box5_log");
  box5_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  BaccDetectorComponent* box5 = new BaccDetectorComponent( 0,
                                            G4ThreeVector(0.,50.*cm,0.),
                                            box5_log,
                                            "box5",
                                            shield_log,
                                            0,0,true);

  G4Box * box6_box = new G4Box("box6",
                            5.*cm,
                            0.00001*cm,
                            5.*cm);
  G4LogicalVolume * box6_log = new G4LogicalVolume( box6_box, BACCmaterials->Water(), "box6_log");
  box6_log->SetVisAttributes( BACCmaterials->TeflonVis() );
  BaccDetectorComponent* box6 = new BaccDetectorComponent( 0,
                                            G4ThreeVector(0.,5.*cm,0.),
                                            box6_log,
                                            "box6",
                                            shield_log,
                                            0,0,true);
  


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                                      SetOrigin()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4ThreeVector ThermalFluxCalcDetector::SetOrigin(BaccDetectorComponent *ExpHall){
  //you need to implement this function here
  return G4ThreeVector(0,0,0);
}


