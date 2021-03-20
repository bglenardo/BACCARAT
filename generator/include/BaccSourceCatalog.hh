////////////////////////////////////////////////////////////////////////////////
/*	BaccSourceCatalog.hh

This is the header file to handle the various event generators. This catalog 
object is essentially a sub-manager for the source generators.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccSourceCatalog_HH
#define BaccSourceCatalog_HH 1

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
class BaccSource;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccSourceCatalog
{
	public:
		BaccSourceCatalog();
		~BaccSourceCatalog();

	public:
		G4int GetNumSourceTypes() { return sources.size(); };
		BaccSource *GetSourceType( G4int i ) { return sources[i]; };
                void AddToSourceCatalog(BaccSource *val){ sources.push_back(val); };

	private:
		BaccManager *baccManager;
		
		std::vector<BaccSource*> sources;
};

#endif
