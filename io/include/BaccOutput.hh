////////////////////////////////////////////////////////////////////////////////
/*	BaccOutput.hh

This is the header file to control the Bacc output. This output is solely to a 
general-purpose binary format, and should never be geared specifically toward 
either ROOT or Matlab. There will be separate projects to create ROOT- and 
Matlab-based readers for this binary format.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccOutput_HH
#define BaccOutput_HH 1

//
//	C/C++ includes
//
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//
//	GEANT4 includes
//
#include "globals.hh"

//
//	Class forwarding
//
class BaccManager;
class BaccDetectorComponent;

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccOutput
{
	public:
		BaccOutput();
		~BaccOutput();
		
	public:
		void RecordEventByVolume( BaccDetectorComponent*, G4int );
		void RecordInputHistory();	
	
	private:
		BaccManager *baccManager;
		
		G4String fName;
		ofstream fBaccOutput;

		G4int Size;
		G4int particleNameSize;
		G4int creatorProcessSize;
		G4int stepProcessSize;
		G4String GMT; // Time & Date
		G4String G4Ver; // G4 version & Date
		G4String SimVer; // SVN version
		G4String uname;  // Name of computer	
		G4String DetCompo;
		G4String commands;
		G4String differ;

		G4int recordLevel;
		G4int volume;
		G4int recordSize;  // Total number recorded for the volume/event 
		G4int numRecords;
		G4int randomSeed;
		G4int eventSeedIndex;
		G4long eventSeed1;
		G4long eventSeed2;

		G4String particleName;
		struct datalevel {
			G4int stepNumber;
			G4int particleID;
			G4int trackID;
			G4int parentID;
                        G4int creationVolumeID;
			G4double creationPosition[3];
			G4double particleEnergy;
			G4double particleDirection[3];
			G4double energyDeposition;
			G4double position[3];
			G4double stepTime;
			G4double charge;
		} data;
		G4String creatorProcess;
		G4String stepProcess;

		G4double totalVolumeEnergy;
		G4int optPhotRecordLevel;
		G4int totalOptPhotNumber;
		
		G4int thermElecRecordLevel;		
		G4int totalThermElecNumber;

		G4int    primaryParSize;
		G4String primaryParName;
		G4double primaryParEnergy_keV;
		G4double primaryParTime_ns;
		G4double primaryParPos_mm[3];
		G4double primaryParDir[3];
		G4int    primaryParAbsorptionVolume;
		G4double primaryParAbsorptionPos_mm[3];

};

#endif
