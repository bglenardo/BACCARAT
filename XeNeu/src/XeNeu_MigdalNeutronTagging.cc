//////////////////////////////////////////////////////
//                                                  //
//  XeNeu_MigdalNeutronTagging.cc                             //
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
#include <cmath>

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
#include "XeNeu_MigdalNeutronTagging.hh"
#include "XeNeu_PMTCage.hh"
#include "XeNeu_XeDetectorParameters.hh"
//
//		Definitions
//
using namespace std;
using namespace XeNeuDetectorParameters;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_MigdalNeutronTagging::XeNeu_MigdalNeutronTagging()
{

    //	Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

  //example box -- make it into a minimal working example
//  G4Box * detectorVolume_box = new G4Box("detector", 10. * m, 10. * m, 10. * m);

  double detectorThickness = 1.5*2.54 * cm;
  double detectorHalfHeight = 4.*2.54*cm;

  // There is a 4.76mm lead sheet wrapped around the detector

  G4Tubs * taggingDetectorVolume_tubs = new G4Tubs("migdal_neutron_tag_tubs",
                                             outerCanOuterRadius + 0.476*cm,
                                             outerCanOuterRadius + detectorThickness + 0.476*cm,
                                             detectorHalfHeight,
                                             0.*deg,330.*deg);
  //taggingDetectorVolume_log = new G4LogicalVolume( taggingDetectorVolume_tubs, BACCmaterials->LiDopedPlasticScintillator(), "detector");
  taggingDetectorVolume_log = new G4LogicalVolume( taggingDetectorVolume_tubs, BACCmaterials->EJ254(), "detector");
  taggingDetectorVolume_log->SetVisAttributes( BACCmaterials->TestBlueVis() );

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeu_MigdalNeutronTagging
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeu_MigdalNeutronTagging::~XeNeu_MigdalNeutronTagging(){}
