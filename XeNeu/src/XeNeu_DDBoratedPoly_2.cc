//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_DDBoratedPoly_2.cc                             //
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

//
//      BACCARAT includes
//
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"

//
//	LZSystemTest includes
//
#include "XeNeu_DDBoratedPoly_2.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_DDBoratedPoly_2::XeNeu_DDBoratedPoly_2()
{


  double volume_half_width = (2 * 0.5 * cm + 2. * 1. * 2.54 * cm)/2.;
  double volume_half_height = 7. * 2.54 * cm;
  double volume_half_length = 31. * cm / 2.;

  double b_poly_sheet_half_width = 1. * 2.54 * cm / 2.;
  double b_poly_sheet_half_length = volume_half_length - 2.5 * cm;
  double b_poly_sheet_half_height = volume_half_height;

  double lead_sheet_half_width = 0.5/2. * cm;
  double lead_sheet_half_length = 31./2. * cm;
  double lead_sheet_half_height = volume_half_height;

    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

  //example box -- make it into a minimal working example
   G4Box * boratedPoly1Volume_box = new G4Box("shieldingVolume", 
                                             volume_half_width, 
                                             volume_half_length, 
                                             volume_half_height);
  boratedPoly2Volume_log = new G4LogicalVolume( boratedPoly1Volume_box, BACCmaterials->Vacuum(), "boratedPoly1Volume");
  boratedPoly2Volume_log->SetVisAttributes( BACCmaterials->VacuumVis() );


  // PMT cage aluminum box
  G4Box * b_poly_sheet_solid = new G4Box("b_poly_sheet_solid",
                                             b_poly_sheet_half_width, 
                                             b_poly_sheet_half_length, 
                                             b_poly_sheet_half_height);


  G4LogicalVolume * b_poly_log = new G4LogicalVolume( b_poly_sheet_solid, BACCmaterials->PolyethyleneBorated(),"b_poly_log");
  b_poly_log->SetVisAttributes( BACCmaterials->PolyethyleneBoratedVis() );  

  char sheet_name[50];
  double sheet_positionX = -volume_half_width + b_poly_sheet_half_width;
  double sheet_positionY = 0.;
  for(int i=0; i<2; i++) {
      sprintf(sheet_name,"b_poly_sheet_%d",i);
      b_poly_sheet[i] = new BaccDetectorComponent(0,
                                               G4ThreeVector(sheet_positionX,sheet_positionY,0.),
                                               b_poly_log,
                                               sheet_name,
                                               boratedPoly2Volume_log,
                                               0,0,true);
      sheet_positionX += b_poly_sheet_half_width*2.;
  }

  
  G4Box * lead_sheet_solid = new G4Box("lead_sheet_solid",
                                       lead_sheet_half_width,
                                       lead_sheet_half_length,
                                       lead_sheet_half_height);
  G4LogicalVolume * lead_sheet_log = new G4LogicalVolume( lead_sheet_solid, BACCmaterials->Lead(), "lead_sheet_log");
  lead_sheet_log->SetVisAttributes( BACCmaterials->LeadVis() );
  
  BaccDetectorComponent * lead_sheet_1 = new BaccDetectorComponent(0,
                                       G4ThreeVector(sheet_positionX-b_poly_sheet_half_width+lead_sheet_half_width,0.,0.),
                                       lead_sheet_log,
                                       "lead_sheet_1",
                                       boratedPoly2Volume_log,
                                       0,0,true);
  BaccDetectorComponent * lead_sheet_2 = new BaccDetectorComponent(0,
                                       G4ThreeVector(sheet_positionX-b_poly_sheet_half_width+3*lead_sheet_half_width,0.,0.),
                                       lead_sheet_log,
                                       "lead_sheet_2",
                                       boratedPoly2Volume_log,
                                       0,0,true);

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_DDBoratedPoly_2
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_DDBoratedPoly_2::~XeNeu_DDBoratedPoly_2(){}
