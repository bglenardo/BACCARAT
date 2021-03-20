////////////////////////////////////////////////////////////////////////////////
/*	BaccExamplePMTBank.hh

* This is the header file to define an example bank of 8778-model PMTs.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccExamplePMTBank_HH
#define BaccExamplePMTBank_HH 1

//
//	GEANT4 includes
//
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccExample8778PMT.hh"

//
//	Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccExamplePMTBank
{
	public:
		BaccExamplePMTBank();
		~BaccExamplePMTBank();
		
		inline G4LogicalVolume *GetLogicalVolume() { return logicalVolume; };
		inline BaccDetectorComponent *GetPMT( G4int num )
				{ return PMTs[num]; };
		inline BaccExample8778PMT *GetPMTClass( G4int num )
				{ return PMT8778[num]; };
		
	private:
		
		G4LogicalVolume *logicalVolume;
		BaccExample8778PMT *PMT8778[3];
		BaccDetectorComponent *PMTs[3];
};

#endif
