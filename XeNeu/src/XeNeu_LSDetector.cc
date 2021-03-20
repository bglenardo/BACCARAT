//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_LSDetector.cc                             //
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
#include "XeNeu_LSDetector.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				XeNeu_LSDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_LSDetector::XeNeu_LSDetector(int det_num)
{


  double detector_radius = 2. * 2.54 * cm;
  double detector_half_length = 1.5 * 2.54 * cm;
  double full_assembly_half_length = 4.5 * 2.54 * cm; 


    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

   G4Tubs * LSDetectorVolume_box = new G4Tubs("detector", 
                                             0.*cm, 
                                             detector_radius+2.4*mm, 
                                             full_assembly_half_length+2.4*mm,
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
  G4SubtractionSolid * detector_shell_final_solid = new G4SubtractionSolid("detector_shell_final_solid", 
                                             detector_shell_tubs,
                                             detector_shell_cutout_tubs,
                                             0,G4ThreeVector(0.,0.,0.));

  char shell_name[100];
  sprintf(shell_name,"ls_detector_shell_%d",det_num);
  G4LogicalVolume * detector_shell_log = new G4LogicalVolume( detector_shell_final_solid, BACCmaterials->Aluminum(), "detector_shell_log");
  detector_shell_log->SetVisAttributes( BACCmaterials->WaterVis() );
  BaccDetectorComponent * detector_shell = new BaccDetectorComponent(0,
                                             G4ThreeVector(0.,0.,0.),
                                             detector_shell_log,
                                             shell_name,
                                             lsDetectorVolume_log,
                                             0,0,true);

  double targetZ = -full_assembly_half_length + detector_half_length + 1/16.*2.54 * cm;

  char targ_name[100];
  sprintf(targ_name,"ls_target_%d",det_num);
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
                                            targ_name,
                                            lsDetectorVolume_log,
                                            0,0,true);


  char lead_sheath_name[100];
  sprintf(lead_sheath_name,"lead_sheath_%d",det_num);
  G4Tubs * lead_sheath_tubs = new G4Tubs( "lead_sheath_tubs",
                                           detector_radius,
                                           detector_radius + 2.4*mm,
                                           detector_half_length,
                                           0.*deg,360.*deg);
  G4LogicalVolume * lead_sheath_log = new G4LogicalVolume( lead_sheath_tubs, BACCmaterials->Lead(), "lead_sheath_log");
  lead_sheath_log->SetVisAttributes( BACCmaterials->TestPurpleVis() );
  BaccDetectorComponent * lead_sheath = new BaccDetectorComponent(0,
                                            G4ThreeVector(0.,0.,targetZ),
                                            lead_sheath_log,
                                            lead_sheath_name,
                                            lsDetectorVolume_log,
                                            0,0,true);
  
  char lead_face_name[100];
  sprintf(lead_face_name,"lead_face_%d",det_num);
  G4Tubs * lead_face_tubs = new G4Tubs("lead_face_tubs",
                                        0.,detector_radius+2.4*mm,
                                        2.4*mm/2.,
                                        0.*deg,360.*deg);
  G4LogicalVolume * lead_face_log = new G4LogicalVolume( lead_face_tubs, BACCmaterials->Lead(), "lead_face_log");
  lead_face_log->SetVisAttributes( BACCmaterials->TestPurpleVis() );
  BaccDetectorComponent * lead_face = new BaccDetectorComponent(0,
                                        G4ThreeVector(0.,0.,-full_assembly_half_length - 1.2*mm),
                                        lead_face_log,
                                        lead_face_name,
                                        lsDetectorVolume_log,
                                        0,0,true);  


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_LSDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_LSDetector::~XeNeu_LSDetector(){}
