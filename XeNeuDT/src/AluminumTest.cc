#include <sstream>
#include <string>
#include <cmath>


//      GEANT4 includes
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


//      LZSystemTest includes
//
#include "AluminumTest.hh"
//
//              Definitions
//
using namespace std;

#define PI 3.14159265358979312



AluminumTest::AluminumTest()
{

    //Get the BaccMaterials pointer
    BACCmaterials = BaccMaterials::GetMaterials();

      //example box -- make it into a minimal working example
        G4Box * detectorVolume_box = new G4Box("detector", 10. * cm, 10. * cm, 10. * cm);

	G4LogicalVolume * logicalVolume = new G4LogicalVolume(detectorVolume_box, BACCmaterials->Aluminum(),"logicalVolume");

	logicalVolume->SetVisAttributes(BACCmaterials->SteelVis());


	G4Box * vacuumbox = new G4Box("vacuumbox", 8. * cm, 8. * cm, 8. * cm);
    
	G4LogicalVolume * vacuumbox_log = new G4LogicalVolume(vacuumbox, BACCmaterials->Vacuum(),"vacuumbox_log"); 



	G4Box * measurementbox = new G4Box("measurementbox", 8. * cm, 8. * cm, 1 * mm);

	G4LogicalVolume * measurementbox_log = new G4LogicalVolume(measurementbox, BACCmaterials->Vacuum(),"measurementbox_log");

	measurementbox_log->SetVisAttributes(BACCmaterials->WaterVis());

	BaccDetectorComponent * measurementbox_obj = new BaccDetectorComponent(0,
									G4ThreeVector(0,0,0),
									measurementbox_log,
									"measurementbox_obj",
									vacuumbox_log,
									0,0,true); 




	BaccDetectorComponent * vacuumbox_obj = new BaccDetectorComponent(0,
									G4ThreeVector(0,0,0),
									vacuumbox_log,
									"vacuumbox_obj",
									logicalVolume,
									0,0,true);


}

//------++++++------++++++------++++++------++++++------++++++------++++++------
////                              ~Aluminum_Test
////------++++++------++++++------++++++------++++++------++++++------++++++------
AluminumTest::~AluminumTest(){}
 
