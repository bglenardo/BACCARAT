//////////////////////////////////////////////////////
//                                                  //
//  TMS_NaI_Detector_5in.cc                             //
//                                                  //
//  This is the class implementation for the TMS  //
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
#include "TMS_NaI_Detector_5in.hh"
#include "TMSMaterials.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				TMS_NaI_Detector_5in
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_NaI_Detector_5in::TMS_NaI_Detector_5in(string instance_num)
{

  double detector_radius = 2.375 * 2.54 * cm;
  double detector_half_length = 1.75 * 2.54 * cm;
  double full_assembly_half_length = 4.5 * 2.54 * cm; 

  stringstream namestream;


    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();
    TMSmaterials = TMSMaterials::GetInstance();

   G4Tubs * PSD_DetectorVolume_tubs = new G4Tubs("detector", 
                                             0.*cm, 
                                             detector_radius, 
                                             full_assembly_half_length,
                                             0.*deg,360.*deg);
  naiDetectorVolume_log = new G4LogicalVolume( PSD_DetectorVolume_tubs, BACCmaterials->Vacuum(), "naiDetectorVolume");
  naiDetectorVolume_log->SetVisAttributes( BACCmaterials->VacuumVis() );

//  G4Tubs * detector_shell_tubs = new G4Tubs("detector_shell_tubs",
//                                             0., 
//                                             detector_radius, 
//                                             full_assembly_half_length,
//                                             0.*deg,360.*deg);
//  G4Tubs * detector_shell_cutout_tubs = new G4Tubs("detector_shell_cutout_tubs",
//                                             0.,
//                                             detector_radius - 1./8. * 2.54 * cm,
//                                             full_assembly_half_length - 1./8. * 2.54 * cm,
//                                             0.*deg,360.*deg);
//  G4SubtractionSolid * detector_shell_final_solid = new G4SubtractionSolid("detecto_shell_final_solid", 
//                                             detector_shell_tubs,
//                                             detector_shell_cutout_tubs,
//                                             0,G4ThreeVector(0.,0.,0.));
//
//  G4LogicalVolume * detector_shell_log = new G4LogicalVolume( detector_shell_final_solid, BACCmaterials->Aluminum(), "detector_shell_log");
//  detector_shell_log->SetVisAttributes( BACCmaterials->WaterVis() );
//  
//  namestream.str("");
//  namestream << "nai_detector_shell_" << instance_num;
//
//  BaccDetectorComponent * detector_shell = new BaccDetectorComponent(0,
//                                             G4ThreeVector(0.,0.,0.),
//                                             detector_shell_log,
//                                             namestream.str().c_str(),
//                                             naiDetectorVolume_log,
//                                             0,0,true);

  namestream.str("");
  namestream << "nai_target_" << instance_num;

  double targetZ = -full_assembly_half_length + detector_half_length;

  G4Tubs * nai_target_volume = new G4Tubs( "nai_target_volume_tubs",
                                 0.*cm,
                                 detector_radius,
                                 detector_half_length,
                                 0.*deg,360.*deg);
  G4LogicalVolume * nai_target_log = new G4LogicalVolume( nai_target_volume, TMSmaterials->SodiumIodide(), "nai_target_log");
  nai_target_log->SetVisAttributes( BACCmaterials->TestBlueVis() );
  BaccDetectorComponent * nai_target = new BaccDetectorComponent(0,
                                            G4ThreeVector(0.,0.,targetZ),
                                            nai_target_log,
                                            namestream.str().c_str(),
                                            naiDetectorVolume_log,
                                            0,0,true);

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~TMS_NaI_Detector_5in
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMS_NaI_Detector_5in::~TMS_NaI_Detector_5in(){}
