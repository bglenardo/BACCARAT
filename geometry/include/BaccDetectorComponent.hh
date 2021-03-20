////////////////////////////////////////////////////////////////////////////////
/*	BaccDetectorComponent.hh

This is the header file to define the detector component class. The detector 
components inherit from G4VPhysicalVolumes, but they contain additional 
information and methods related to event generation and output recording.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)

  2014/10/05 - Added method declarations for calculating and retrieving the
               volume and mass of the component (Brian, from LUXSim)
  2015/03/03 - Added Set methods for the mass and volume (Brian, from LUXSim)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccDetectorComponent_HH
#define BaccDetectorComponent_HH 1

//
//	C/C++ includes
//
#include <vector>

//
//	GEANT4 includes
//
#include "G4PVPlacement.hh"
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccManager.hh"
#include "BaccMaterials.hh"

//
//	Class forwarding
//
class G4GeneralParticleSource;
class G4Event;
class G4PVPlacement;
class G4Navigator;

class BaccSource;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccDetectorComponent : public G4PVPlacement
{
	public:
		struct source {
			BaccSource *type;
			G4double activity;
			G4double ratio;
            G4int particleMass; //single decay
            G4int particleNumber;
            G4String parentIsotope; //decay chain
            G4double sourceAge;//in seconds
            G4String particleName; //single particle
            G4double particleEnergy;//doubles for singleparticle and wimpMass
			//G4double wimpMass; //Wimp
	    G4bool pointSource;//is this a point source?  
	    G4ThreeVector posSource;//position of point source
		};

	public:

		BaccDetectorComponent( G4RotationMatrix *pRot,
								 const G4ThreeVector &tlate,
								 G4LogicalVolume *pCurrentLogical,
								 const G4String &pName,
								 G4LogicalVolume *pMotherLogical,
								 G4bool pMany,
								 G4int pCopyNo,
								 G4bool pSurfChk=false );
		
		BaccDetectorComponent( const G4Transform3D &Transform3D,
								 G4LogicalVolume *pCurrentLogical,
								 const G4String &pName,
								 G4LogicalVolume *pMotherLogical,
								 G4bool pMany,
								 G4int pCopyNo,
								 G4bool pSurfChk=false );

		BaccDetectorComponent( G4RotationMatrix *pRot,
								 const G4ThreeVector &tlate,
								 const G4String &pName,
								 G4LogicalVolume *pLogical,
								 G4VPhysicalVolume *pMother,
								 G4bool pMany,
								 G4int pCopyNo,
								 G4bool pSurfChk=false );
		
		BaccDetectorComponent( const G4Transform3D &Transform3D,
								 const G4String &pName,
								 G4LogicalVolume *pLogical,
								 G4VPhysicalVolume *pMother,
								 G4bool pMany,
								 G4int pCopyNo,
								 G4bool pSurfChk=false );
								 
		~BaccDetectorComponent();
		
		void Initialize();
	
		inline G4int GetRecordLevel() { return recordLevel; };
		inline void SetRecordLevel( G4int level ) { recordLevel = level; };
		
		inline G4int GetRecordLevelOptPhot() { return recordLevelOptPhot; };
		inline void SetRecordLevelOptPhot( G4int level )
				{ recordLevelOptPhot = level; };
		
		inline G4int GetRecordLevelThermElec() { return recordLevelThermElec; };
		inline void SetRecordLevelThermElec( G4int level )
				{ recordLevelThermElec = level; };
		
		void AddDeposition( BaccManager::stepRecord aStepRecord )
				{ eventRecord.push_back(aStepRecord);};
		void ClearRecord() { eventRecord.clear(); };
		std::vector<BaccManager::stepRecord> GetEventRecord()
				{ return eventRecord; };
		
		void SetID( G4int ID ) { compID = ID; };
		G4int GetID() { return compID; };
		
		void AddSource( BaccSource*, G4double, G4int, G4int, G4String, 
				G4double, G4String, G4double, G4bool, G4ThreeVector);
		void ResetSources() {sources.clear(); totalActivity=0;};
		std::vector<source> GetSources() { return sources; };
		void CalculateRatios();
		G4double GetTotalActivity() { return totalActivity; };
		void GenerateFromEventList( G4GeneralParticleSource*, G4Event*, decayNode* );
        void GenerateEventList(G4int);
		void DetermineCenterAndExtent( G4PVPlacement* );
		G4ThreeVector GetGlobalCenter() { return globalCenter; };
        G4ThreeVector GetMinXYZ() { return minXYZ; };
        G4ThreeVector GetMaxXYZ() { return maxXYZ; };
	
    
                void SetVolumePrecision( G4double );
                G4double CalculateVolume( G4bool takeOutDaughters = true );
                G4double CalculateMass();
                G4double GetVolume();
                G4double GetMass();
                void SetVolume( G4double );
                void SetMass( G4double );

	
		G4bool GetCapturePhotons() { return capturePhotons; };
		
	private:
		G4ThreeVector GetEventLocation();

	private:

		G4int recordLevel;
		G4int recordLevelOptPhot;
		G4int recordLevelThermElec;
		std::vector<BaccManager::stepRecord> eventRecord;
		G4int compID;
		
		std::vector<source> sources;
		G4double totalActivity;
		G4double maxX, minX;
		G4double maxY, minY;
		G4double maxZ, minZ;
        G4ThreeVector minXYZ;
        G4ThreeVector maxXYZ;
		G4ThreeVector globalCenter;
		G4RotationMatrix globalOrientation;
		G4double xPos, yPos, zPos;

                G4double componentVolume;
                G4double componentMass;
                G4int volumePrecision;

		G4bool capturePhotons;
		
		BaccManager *baccManager;
    BaccMaterials *baccMaterials;
		G4Navigator *navigator;
};

#endif
