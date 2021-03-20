////////////////////////////////////////////////////////////////////////////////
/* BaccGeometryCatalog.hh

This is the code file to handle the various detector geometries. This catalog 
object is essentially a sub-manager for the detector geometries.  This file is 
edited by the setup script for each project.  Please don't alter the commets 
stating where the #include's and push_back's go because they are they serve as
markers to tell the setup scripts where to add the salient lines.
********************************************************************************
Change log
  2013/09/11 - Initial submission (Vic)
  2015/07/07 - Added hooks for project-specific setup scripts (Vic)
  2015/01/14 - Add BuildCatalog() to isolate codes that rely on projects. 
               The function BuildCatalog will be created by make (Jingke)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccGeometryCatalog.hh"
#include "BaccManager.hh"

#include "Bacc7081PMT.hh"
#include "BaccDetectorConstruction.hh"
#include "BaccExampleComplexDetector.hh"
#include "BaccDetector.hh"
#include "BaccExamplePMTBank.hh"
#include "BaccGeometryCatalog.hh"
#include "BaccMaterials.hh"
#include "BaccDetectorComponent.hh"
#include "BaccExample8778PMT.hh"
#include "BaccExampleSimpleDetector.hh"

// Project related #include's will go here!
//#include "LZDetector.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeometryCatalog()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeometryCatalog *BaccGeometryCatalog::baccGeometryCatalog = 0;
BaccGeometryCatalog::BaccGeometryCatalog(){
	baccGeometryCatalog = this;
	
	baccManager = BaccManager::GetManager();
	baccManager->Register( baccGeometryCatalog );
	
	geometry.clear();
    
	BuildCatalog();
	//    geometry.push_back( new BaccExampleSimpleDetector( "simple", "cryostat" ));
	//    geometry.push_back( new BaccExampleSimpleDetector( "complex", "cryostat" ));
// Project related push_back's will go here!
//	geometry.push_back( new LZDetector( "LZDetector", "WaterTank" ) );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetCatalog()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeometryCatalog *BaccGeometryCatalog::GetCatalog()
{
	return baccGeometryCatalog;
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeometryCatalog()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeometryCatalog::~BaccGeometryCatalog()
{
	geometry.clear();
}
