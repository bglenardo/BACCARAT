////////////////////////////////////////////////////////////////////////////////
/*	Bacc7081PMT.hh

This is the header file to define the 7081 model PMT by Hamamatsu.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef Bacc7081PMT_HH
#define Bacc7081PMT_HH 1

//
//	GEANT4 includes
//
#include "globals.hh"

//
//	Class forwarding
//
class G4LogicalVolume;
class BaccDetectorComponent;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class Bacc7081PMT
{
		
	public:
		Bacc7081PMT();
		~Bacc7081PMT();
		
		inline G4LogicalVolume *GetLogicalVolume() { return PMT7081_log; };
		inline BaccDetectorComponent *GetPMTWindow() { return PMT7081window; };
		
	private:
		
		G4LogicalVolume *PMT7081_log;
		BaccDetectorComponent *PMT7081window;
};

#endif
