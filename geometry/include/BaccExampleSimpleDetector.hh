////////////////////////////////////////////////////////////////////////////////
/*	BaccExampleSimpleDetector.hh

This is the header file to define an "simple" example detector.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccExampleSimpleDetector_HH
#define BaccExampleSimpleDetector_HH 1

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
class BaccExamplePMTBank;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccExampleSimpleDetector : public BaccDetector
{
	public:
		BaccExampleSimpleDetector( G4String, G4String );
		~BaccExampleSimpleDetector();
        void BuildDetector();
		
		inline BaccDetectorComponent *GetLXeTarget() {return liquidXeTarget;};
		inline BaccDetectorComponent *GetTopBank() { return topBank; };
		inline BaccDetectorComponent *GetBottomBank() { return bottomBank; };
		
	private:
		
		BaccDetectorComponent *liquidXeTarget;
		BaccExamplePMTBank *banks[2];
		BaccDetectorComponent *topBank;
		BaccDetectorComponent *bottomBank;
};

#endif
