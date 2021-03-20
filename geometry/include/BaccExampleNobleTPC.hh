////////////////////////////////////////////////////////////////////////////////
/* BaccExampleSimpleDetector.hh

This is the header file to define an example dual-phase noble element detector. 
Within this default is the option to set the material to either argon or xenon,
with xenon being the default. The material can be changed via a messenger
command.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Kareem)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccExampleNobleTPC_HH
#define BaccExampleNobleTPC_HH 1

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
class G4LogicalVolume;
class BaccDetectorComponent;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccExampleNobleTPC : public BaccDetector
{
	public:
		BaccExampleNobleTPC( G4String, G4String );
		~BaccExampleNobleTPC();
        void BuildDetector();
        
	private:
        //  These variables have to be defined at the class level rather than
        //  the method or function level because they may be needed by multiple
        //  methods or functions.
        G4double pmtHeight;
        G4double pmtRadius;
    
        G4double spaceBelowBottomPMTs;
        G4double reverseFieldHeight;
        G4double activeTargetHeight;
        G4double gainVolumeHeight;
        G4double spaceAboveTopPMTs;
        G4double totalCentralVolumeHalfHeight;
        G4double totalCentralVolumeRadius;
    
        G4double reflectorThickness;
        G4double reflectorHalfHeight;
        G4double reflectorOuterRadius;
    
        G4double cryostatWallThickness;
        G4double cryostatHalfHeight;
        G4double cryostatOuterRadius;
    
        G4double vacuumSpaceThickness;
        G4double vacuumHalfHeight;
        G4double vacuumOuterRadius;
    
        G4double vacuumVesselThickness;
        G4double vacuumVesselOuterRadius;
        G4double vacuumVesselHalfHeight;
};

#endif
