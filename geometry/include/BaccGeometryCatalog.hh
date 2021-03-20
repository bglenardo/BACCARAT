////////////////////////////////////////////////////////////////////////////////
/*	BaccGeometryCatalog.hh

This is the header file to handle the various event generators. This catalog 
object is essentially a sub-manager for the geometry.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2015/01/14 - Add BuildCatalog() to isolate codes that rely on projects. Jingke
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeometryCatalog_HH
#define BaccGeometryCatalog_HH 1

//
//	C/C++ includes
//
#include <vector>

//
//	GEANT4 includes
//
#include "globals.hh"

//
//	Class forwarding
//
class BaccManager;
class BaccDetector;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeometryCatalog
{
	public:
		BaccGeometryCatalog();
		~BaccGeometryCatalog();

		static BaccGeometryCatalog *GetCatalog();

	public:
		G4int GetNumGeometryTypes() { return geometry.size(); };
		BaccDetector *GetGeometryType( G4int i ) { return geometry[i]; };
        std::vector<BaccDetector*> GetDetectors() { return geometry; };

	private:
		BaccManager *baccManager;

		static BaccGeometryCatalog *baccGeometryCatalog;
		
		std::vector<BaccDetector*> geometry;

  //this function builds the catalog by scanning over the project dirs
  void BuildCatalog();
};

#endif
