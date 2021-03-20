////////////////////////////////////////////////////////////////////////////////
/*	BaccDetectorConstruction.cc

This is the code file to build the detector geometry within Bacc.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

//
//	GEANT4 includes
//
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

//
//	Bacc includes
//
#include "BaccDetectorConstruction.hh"
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"
#include "BaccManager.hh"
#include "BaccGeometryCatalog.hh"

#include "BaccDetector.hh"
#include "BaccExampleSimpleDetector.hh"
#include "BaccExampleComplexDetector.hh"
#include "BaccExampleNobleTPC.hh"
#include "BaccLBNLAlphaCounter.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccDetectorConstruction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccDetectorConstruction *BaccDetectorConstruction::baccDetectorConstruction = 0;
BaccDetectorConstruction::BaccDetectorConstruction()
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );
	
	baccDetectorConstruction = this;
	baccGeometryMessenger = new BaccGeometryMessenger( baccManager,
            baccDetectorConstruction);
	baccMaterials = BaccMaterials::GetMaterials();
    baccGeometryCatalog = BaccGeometryCatalog::GetCatalog();
	
	detectorSpecified = false;
	
	detX = detY = detZ = 0;
    
    detectorSelection = "Not yet set";
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccDetectorConstruction()
//------++++++------++++++------++++++------++++++------++++++------++++++------

BaccDetectorConstruction::~BaccDetectorConstruction()
{
	delete baccMaterials;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetDetectorConstruction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccDetectorConstruction *BaccDetectorConstruction::GetDetectorConstruction()
{
	return baccDetectorConstruction;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					Construct()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//	This method is required by the GEANT4 simulation
G4VPhysicalVolume *BaccDetectorConstruction::Construct()
{
	return ConstructDetector();
        std::cerr << "\n\nDetector successfully constructed...\n" << std::endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ConstructDetector()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//	This method is required by the GEANT4 simulation
G4VPhysicalVolume *BaccDetectorConstruction::ConstructDetector()
{
	//	Experimental hall. This is a box 100 km on a side.
	//G4double expHallHalfHeight = 50001.*m;
    G4double expHallHalfHeight = 50.*m;
	G4Box *experimentalHall_box = new G4Box( "expHall_box", expHallHalfHeight,
			expHallHalfHeight, expHallHalfHeight );
	G4LogicalVolume *experimentalHall_log = new G4LogicalVolume(
			experimentalHall_box, baccMaterials->Vacuum(), "expHall_log",
			0, 0, 0 );
	experimentalHall_log->SetVisAttributes( G4VisAttributes::Invisible );
	experimentalHall = new BaccDetectorComponent( 0, G4ThreeVector(),
			experimentalHall_log, "expHall", 0, 0, 0, false );
    
    //  Moveable sub-volume. This is so the center of the overall geometry can
    //  be placed at any absolute coordinate, within the restrictions of the
    //  size of the volume itself.
    //G4double subVolHalfHeight = 25000.*m;
    G4double subVolHalfHeight = 50.*m;
	G4Box *subVol_box = new G4Box( "subVol_box", subVolHalfHeight,
			subVolHalfHeight, subVolHalfHeight );
	G4LogicalVolume *subVol_log = new G4LogicalVolume(
			subVol_box, baccMaterials->Vacuum(), "subVol_log",
			0, 0, 0 );
	subVol_log->SetVisAttributes( G4VisAttributes::Invisible );
    subVol = new BaccDetectorComponent( 0, G4ThreeVector( detX, detY, detZ ),
            "subVol", subVol_log, experimentalHall, 0, 0, false );
	
    //  Now create and place the selected detector
    if( detectorSelection != "Not yet set" ) {
        G4bool alreadyFoundDetector = false;
        G4cout << "*****Creating detector: " << detectorSelection << ".*****" << G4endl;
        detectorList = baccGeometryCatalog->GetDetectors();
        for( G4int i=0; i<(G4int)detectorList.size(); i++ ) {
            if( detectorList[i]->GetName() == detectorSelection ){

                if( alreadyFoundDetector ) {
                    G4cout << G4endl << G4endl;
                    G4cout << "Looks like there are two detectors with the name "
                           << detectorType->GetName();
                    G4cout << "Look in the constructor of "
                           << "BaccDetectorConstruction, and look for two"
                           << G4endl;
                    G4cout << "push_back lines with the same first variable."
                           << G4endl;
                    G4cout << G4endl << G4endl;
                    exit(0);
                }
                
                detectorType = detectorList[i];
                detectorType->BuildDetector();
                detector = new BaccDetectorComponent(0, G4ThreeVector(0,0,0),
                                                     detectorType->GetVolumeName(),
                                                     detectorType->GetLogicalVolume(), 
                                                     subVol, 0, 0,false );
                
                G4ThreeVector offsetVector = detectorType->SetOrigin(experimentalHall);
                G4ThreeVector currentTranslation = subVol->GetTranslation();
                currentTranslation.setZ( currentTranslation.z() + offsetVector.z() );
                currentTranslation.setX( currentTranslation.x() + offsetVector.x() );
                currentTranslation.setY( currentTranslation.y() + offsetVector.y() );
                subVol->SetTranslation( currentTranslation );
                
                alreadyFoundDetector = true;
            }
        }
        if( !alreadyFoundDetector ) {
            G4cout << G4endl << G4endl;
            G4cout << "Could not find a detector with the name "
                   << detectorSelection << G4endl;
            G4cout << "Choices are" << G4endl;
            for( G4int i=0; i<(G4int)detectorList.size(); i++ )
                G4cout << "\t" << detectorList[i]->GetName() << G4endl;
            G4cout << G4endl << G4endl;
            exit(0);
        }
    }
    detectorSpecified = true;
    G4cout << "*****Done building detector: " << detectorSelection << ".*****" << G4endl;
	//
	//	All done! Return the detector geometry
	//
	return experimentalHall;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					UpdateGeometry()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//	This next method will be most useful for future use. If the geometry is
//	altered via a macro command, UpdateGeometry will have to be called to
//	instantiate the change.
void BaccDetectorConstruction::UpdateGeometry()
{
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
	G4LogicalSkinSurface::CleanSurfaceTable();
	G4LogicalBorderSurface::CleanSurfaceTable();
	G4RunManager::GetRunManager()->DefineWorldVolume( Construct() );
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	
	//	I have absolutely no fraking idea why this next line helps, but
	//	it seems to keep the visualization stack from spazzing out and 
	//	core dumping. Remove it at your own peril.
	G4cout << " " << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetQuartzAbs()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetQuartzAbs( G4double l )
{
	baccMaterials->SetQuartzAbs(l);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonRefl( G4double r )
{
	baccMaterials->SetLXeTeflonRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonPMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonPMTRefl( G4double r )
{
    baccMaterials->SetLXeTeflonPMTRefl(r);
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonPlateRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonPlateRefl( G4double r )
{
    baccMaterials->SetLXeTeflonPlateRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonCablesRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonCablesRefl( G4double r )
{
    baccMaterials->SetLXeTeflonCablesRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonTrussRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonTrussRefl( G4double r )
{
    baccMaterials->SetLXeTeflonTrussRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonBskinPMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonBskinPMTRefl( G4double r )
{
    baccMaterials->SetLXeTeflonBskinPMTRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonBplateRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonBplateRefl( G4double r )
{
    baccMaterials->SetLXeTeflonBplateRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonTskinPMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonTskinPMTRefl( G4double r )
{
    baccMaterials->SetLXeTeflonTskinPMTRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonWeirRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonWeirRefl( G4double r )
{
    baccMaterials->SetLXeTeflonWeirRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonDomePMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonDomePMTRefl( G4double r )
{
    baccMaterials->SetLXeTeflonDomePMTRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonLinerRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeTeflonLinerRefl( G4double r )
{
    baccMaterials->SetLXeTeflonLinerRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeSteelRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeSteelRefl( G4double r )
{
	baccMaterials->SetLXeSteelRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeAbs()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeAbs( G4double l )
{
	baccMaterials->SetLXeAbs(l);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeRayleigh()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetLXeRayleigh( G4double l )
{
	baccMaterials->SetLXeRayleigh(l);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeTeflonRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetGXeTeflonRefl( G4double r )
{
	baccMaterials->SetGXeTeflonRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeSteelRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetGXeSteelRefl( G4double r )
{
	baccMaterials->SetGXeSteelRefl(r);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeAbs()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetGXeAbs( G4double l )
{
	baccMaterials->SetGXeAbs(l);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeRayleigh()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetGXeRayleigh( G4double l )
{
	baccMaterials->SetGXeRayleigh(l);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetAlUnoxQuartzRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetAlUnoxQuartzRefl( G4double l )
{
	baccMaterials->SetAlUnoxQuartzRefl(l);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetDetectorZ()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorConstruction::SetDetectorZ( G4double z )
{
	detZ = z;
}


