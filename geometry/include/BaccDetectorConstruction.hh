////////////////////////////////////////////////////////////////////////////////
/*	BaccDetectorConstruction.hh

This is the header file to build the detector geometry within Bacc.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////


#ifndef BaccDetectorConstruction_HH
#define BaccDetectorConstruction_HH 1

//
//	GEANT4 includes
//
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "BaccGeometryMessenger.hh"

//
// C/C++ includes
//
#include <vector>

//
//	Class forwarding
//

class BaccManager;
class BaccDetectorComponent;
class BaccMaterials;

class BaccDetector;
class BaccExampleSimpleCryostat;
class BaccExampleComplexCryostat;
class BaccGeometryCatalog;
//class BaccGeometryMessenger;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccDetectorConstruction : public G4VUserDetectorConstruction
{
	public:

		BaccDetectorConstruction();
		~BaccDetectorConstruction();

        static BaccDetectorConstruction *GetDetectorConstruction();

		G4VPhysicalVolume *Construct();
		void UpdateGeometry();
		
		BaccDetectorComponent	*GetWorldVolume()	{ return experimentalHall;};
		BaccDetector 			*GetDetectorType()	{ return detectorType; };
		BaccDetectorComponent	*GetDetector()		{ return detector; };
		
		//	Materials methods
		void SetQuartzAbs(G4double );
		void SetLXeTeflonRefl( G4double );
        void SetLXeTeflonLinerRefl(G4double r);
        void SetLXeTeflonPMTRefl(G4double r);
        void SetLXeTeflonPlateRefl(G4double r);
        void SetLXeTeflonCablesRefl(G4double r);
        void SetLXeTeflonTrussRefl(G4double r);
        void SetLXeTeflonBskinPMTRefl(G4double r);
        void SetLXeTeflonBplateRefl(G4double r);
        void SetLXeTeflonTskinPMTRefl(G4double r);
        void SetLXeTeflonWeirRefl(G4double r);
        void SetLXeTeflonDomePMTRefl(G4double r);
		void SetLXeSteelRefl( G4double );
		void SetLXeAbs( G4double );
		void SetLXeRayleigh( G4double );
		void SetGXeTeflonRefl( G4double );
		void SetGXeSteelRefl( G4double );
		void SetGXeAbs( G4double );
		void SetGXeRayleigh( G4double );
		void SetAlUnoxQuartzRefl( G4double );
		
		//	Geometry methods
        void SetDetectorSelection( G4String det ) { detectorSelection = det; };
		void SetDetectorZ( G4double z );
		bool DetectorSpecified() { return detectorSpecified; };
        G4String DetectorSelection() {return detectorSelection;};

  void SetCheckVolumeOverlaps( G4bool val ) {
    checkVolumeOverlaps = val; }
  G4bool GetCheckVolumeOverlaps() { return checkVolumeOverlaps; };




	private:
        static BaccDetectorConstruction *baccDetectorConstruction;
        BaccGeometryMessenger *baccGeometryMessenger;
    
		//	Detector components
		//	Just the experimentalHall should be a G4VPhysicalVolume. The rest
		//	should be BaccDetectorComponents.
		BaccDetectorComponent *experimentalHall;
        BaccDetectorComponent *subVol;
		BaccDetector *detectorType;
		BaccDetectorComponent *detector;
        G4String detectorSelection;

		//	Support for internal workings
		BaccManager *baccManager;
		BaccMaterials *baccMaterials;
		G4VPhysicalVolume *ConstructDetector();
        BaccGeometryCatalog *baccGeometryCatalog;
		std::vector<BaccDetector*> detectorList;
		
		//	Detector position (default: 0 cm)
		G4double detX, detY, detZ;

		bool detectorSpecified;		

  G4bool checkVolumeOverlaps;

};

#endif

