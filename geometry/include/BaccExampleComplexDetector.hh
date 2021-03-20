////////////////////////////////////////////////////////////////////////////////
/*	BaccExampleComplexDetector.hh

This is the header file to define the example "complex" detector.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccExampleComplexDetector_HH
#define BaccExampleComplexDetector_HH 1

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
class BaccExampleComplexDetector : public BaccDetector
{
	public:
		BaccExampleComplexDetector( G4String, G4String );
		~BaccExampleComplexDetector();
        void BuildDetector();
		
		inline BaccDetectorComponent *GetInnerCryostat()
				{ return innerCryostat; };
		inline BaccDetectorComponent *GetLXeTarget() {return liquidXeTarget;};
		inline BaccDetectorComponent *GetTopBank() { return topBank; };
		inline BaccDetectorComponent *GetBottomBank() { return bottomBank; };
		inline BaccDetectorComponent *GetVacuum() {return cryostatVacuum; };
    
	private:
		BaccDetectorComponent *innerCryostat;
		BaccDetectorComponent *liquidXeTarget;
		BaccExamplePMTBank *banks[2];
		BaccDetectorComponent *topBank;
		BaccDetectorComponent *bottomBank;
		BaccDetectorComponent *cryostatVacuum;
};

#endif
