////////////////////////////////////////////////////////////////////////////////
/*	BaccLBNLAlphaCounter.hh

This is the header file to define an "simple" example detector.
********************************************************************************
Change log
  2014/04/10 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccLBNLAlphaCounter_HH
#define BaccLBNLAlphaCounter_HH 1

//
//	GEANT4 includes
//
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccDetector.hh"

//
//	Class forwarding
//
class BaccDetectorComponent;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccLBNLAlphaCounter : public BaccDetector
{
	public:
		BaccLBNLAlphaCounter( G4String, G4String );
		~BaccLBNLAlphaCounter();
        void BuildDetector();
		
		inline BaccDetectorComponent *GetVacuumVolume()  {return VacuumVolume;};
		inline BaccDetectorComponent *GetDetectorVolume(){return DetectorVolume;};
		inline BaccDetectorComponent *GetHolderVolume()  {return HolderVolume;};
		
	private:
		BaccDetectorComponent *VacuumVolume;
		BaccDetectorComponent *DetectorVolume;
		BaccDetectorComponent *HolderVolume;
};

#endif
