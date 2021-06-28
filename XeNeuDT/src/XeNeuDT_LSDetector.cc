//////////////////////////////////////////////////////
//                                                  //
//  XeNeuDT_LSDetector.cc                             //
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
#include "XeNeuDT_LSDetector.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				XeNeuDT_LSDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuDT_LSDetector::XeNeuDT_LSDetector()
{

  double detector_radius = 2. * 2.54 * cm;
  double detector_half_length = 1.5 * 2.54 * cm;
  double full_assembly_half_length = 4.5 * 2.54 * cm; 


    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

   G4Tubs * LSDetectorVolume_box = new G4Tubs("detector", 
                                             0.*cm, 
                                             detector_radius, 
                                             full_assembly_half_length,
                                             0.*deg,360.*deg);
  lsDetectorVolume_log = new G4LogicalVolume( LSDetectorVolume_box, BACCmaterials->Vacuum(), "lsDetectorVolume");
  lsDetectorVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );

  G4Tubs * detector_shell_tubs = new G4Tubs("detector_shell_tubs",
                                             0., 
                                             detector_radius, 
                                             full_assembly_half_length,
                                             0.*deg,360.*deg);
  G4Tubs * detector_shell_cutout_tubs = new G4Tubs("detector_shell_cutout_tubs",
                                             0.,
                                             detector_radius - 1./16. * 2.54 * cm,
                                             full_assembly_half_length - 1./16. * 2.54 * cm,
                                             0.*deg,360.*deg);
  G4SubtractionSolid * detector_shell_final_solid = new G4SubtractionSolid("detecto_shell_final_solid", 
                                             detector_shell_tubs,
                                             detector_shell_cutout_tubs,
                                             0,G4ThreeVector(0.,0.,0.));

  G4LogicalVolume * detector_shell_log = new G4LogicalVolume( detector_shell_final_solid, BACCmaterials->Aluminum(), "detector_shell_log");
  detector_shell_log->SetVisAttributes( BACCmaterials->WaterVis() );
  BaccDetectorComponent * detector_shell = new BaccDetectorComponent(0,
                                             G4ThreeVector(0.,0.,0.),
                                             detector_shell_log,
                                             "ls_detector_shell",
                                             lsDetectorVolume_log,
                                             0,0,true);

  double targetZ = -full_assembly_half_length + detector_half_length + 1/16.*2.54 * cm;

  G4Tubs * ls_target_volume = new G4Tubs( "ls_target_volume_tubs",
                                 0.*cm,
                                 detector_radius - 1./16. * 2.54 * cm,
                                 detector_half_length,
                                 0.*deg,360.*deg);
  G4LogicalVolume * ls_target_log = new G4LogicalVolume( ls_target_volume, BACCmaterials->EJ301(), "ls_target_log");
  ls_target_log->SetVisAttributes( BACCmaterials->EJ301Vis() );
  BaccDetectorComponent * ls_target = new BaccDetectorComponent(0,
                                            G4ThreeVector(0.,0.,targetZ),
                                            ls_target_log,
                                            "ls_target",
                                            lsDetectorVolume_log,
                                            0,0,true);

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeuDT_LSDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuDT_LSDetector::~XeNeuDT_LSDetector(){}
