////////////////////////////////////////////////////////////////////////////////
/*	BaccExample8778PMT.hh

This is the header file to define the 8778 model PMT by Hamamatsu.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccExample8778PMT_HH
#define BaccExample8778PMT_HH 1

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
class BaccExample8778PMT
{
	public:
		BaccExample8778PMT();
		~BaccExample8778PMT();
		
		inline G4LogicalVolume *GetLogicalVolume() { return logicalVolume; };
		inline BaccDetectorComponent *GetPMTFlashing() 
			{ return PMTFlashing; };
		inline BaccDetectorComponent *GetPMTPhotocathode()
			{ return PMTPhotocathode; };
		inline BaccDetectorComponent *GetPMTVacuum() { return PMTvacuum; };
		inline BaccDetectorComponent *GetPMTWindow() { return PMTWindow; };
		inline G4double GetPMTHeight() { return height; };
		inline G4double GetPMTRadius() { return outerDiameter/2.; };	
		
	private:
		
		G4LogicalVolume *logicalVolume;
		BaccDetectorComponent *PMTWindow;
		BaccDetectorComponent *PMTPhotocathode;
		BaccDetectorComponent *PMTFlashing;
		BaccDetectorComponent *PMTvacuum;
		G4double height;
		G4double outerDiameter;
};

#endif
