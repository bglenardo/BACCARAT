////////////////////////////////////////////////////////////////////////////////
/*	BaccRootReader.cc

A reader which convert binary file into root file.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	ROOT includes
//
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TText.h"
#include "TPluginManager.h"
//
//	C/C++ includes
//
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
//
//	Definitions
//
#define DEBUGGING 0

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
int main( int argc, char** argv){
  
	gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo","*","TStreamerInfo","RIO","TStreamerInfo()");
	ifstream fin;
	char * filename = argv[1];
	fin.open(filename,ios::binary|ios::in);
	if(  !fin.is_open() ) {
		cout << "Couldn't find the file "<< filename << endl;
		exit( 0 );
	}

	TFile* fFile;
	TTree* fTree;
	TTree* fTree0;
	
	const int kMaxCharacter = 50;
//	const int kMaxCharacter = 1000;
	int cParticleNamePos, cCreatorProcessPos, cStepProcessPos;
	int iRecordSize;	
	int iEvtNb;
	int iVolume;
	char * cVolumeName;
	int iTotalOptPhotNumber=0;
	int iTotalThermElecNumber=0;

	int iPrimaryParNum;
	int cPrimaryParNamePos;
	char * cPrimaryParName;
	double * fPrimaryParEnergy_keV;
	double * fPrimaryParTime_ns;
	double * fPrimaryParPosX_mm;
	double * fPrimaryParPosY_mm;
	double * fPrimaryParPosZ_mm;
	double * fPrimaryParDirX;
	double * fPrimaryParDirY;
	double * fPrimaryParDirZ;

	int iNumRecords;
	char * productionTime;
	char * geant4Version;
	char * svnVersion;
	char * uName;
	char * InputCommands;
	char * diffs;
	char * DetCompo;
	int randomSeed;
	int numPrimaries;

	int arraySize=1;

	int * iStepNum;
	int * iParticleID;
	char * cParticleName;
	char * particleName;
	char * strName;
	char * cCreatorProcess;
	char * creatorProcessName;
	char * creatorProcName;
	char * cStepProcess;
	char * stepProcessName;
	char * stepProcName;
	
	int * iTrackID;
	int * iParentID;
	double * fKEnergy_keV;
	double * fOptPhoWaveLength_nm;
	double * fDirectionX;
	double * fDirectionY;
	double * fDirectionZ;
	double * fEnergyDep_keV;
	double * fPositionX_mm;
	double * fPositionY_mm;
	double * fPositionZ_mm;
	double * fStepTime;
	double * fCharge;
	double fTotEnergyDep_keV=0;

	string outfileString = filename;
	outfileString = outfileString.substr( 0, outfileString.find_last_of('.') );
	outfileString += ".root";
	fFile = TFile::Open(outfileString.c_str(), "RECREATE");
	fTree =  new TTree("tree", "tree");
	fTree0 =  new TTree("header", "header");

	fTree->Branch("iPrimaryParNum", &iPrimaryParNum, "iPrimaryParNum/I");
			// Total number of primary particles
	fTree->Branch("cPrimaryParName", cPrimaryParName, "cPrimaryParName/C");
			// The name of primary particles
	fTree->Branch("fPrimaryParEnergy_keV", fPrimaryParEnergy_keV,
			"fPrimaryParEnergy_keV[iPrimaryParNum]/D");
			// Kinetic Energy of primary particles
	fTree->Branch("fPrimaryParTime_ns", fPrimaryParTime_ns,
			"fPrimaryParTime_ns[iPrimaryParNum]/D");
			// Emission time of primary particles
	fTree->Branch("fPrimaryParPosX_mm", fPrimaryParPosX_mm,
			"fPrimaryParPosX_mm[iPrimaryParNum]/D");
			// PositionX of primary particles
	fTree->Branch("fPrimaryParPosY_mm", fPrimaryParPosY_mm,
			"fPrimaryParPosY_mm[iPrimaryParNum]/D");
			// PositionY of primary particles
	fTree->Branch("fPrimaryParPosZ_mm", fPrimaryParPosZ_mm,
			"fPrimaryParPosZ_mm[iPrimaryParNum]/D");
			// PositionZ of primary particles
	fTree->Branch("fPrimaryParDirX", fPrimaryParDirX,
			"fPrimaryParDirX[iPrimaryParNum]/D");
			// DirectionX of primary particles
	fTree->Branch("fPrimaryParDirY", fPrimaryParDirY,
			"fPrimaryParDirY[iPrimaryParNum]/D");
			// DirectionY of primary particles
	fTree->Branch("fPrimaryParDirZ", fPrimaryParDirZ,
			"fPrimaryParDirZ[iPrimaryParNum]/D");
			// DirectionZ of primary particles

	fTree->Branch("iRecordSize", &iRecordSize, "iRecordSize/I");
			// the total step recorded for the volume/event 
	fTree->Branch("iEvtN", &iEvtNb, "EvtN/I");  // Event Number: starts with 1 
	fTree->Branch("iV_name", &iVolume, "V_name/I"); // Volume name
	fTree->Branch("cVolumeName", &cVolumeName, "cVolumeName/C" ); // Volume name

	fTree->Branch("iStepNum", iStepNum, "iStepNum[iRecordSize]/I");
			// Step Number
	fTree->Branch("iParticleID", iParticleID, "iParticleID[iRecordSize]/I");
			// Particle ID
	fTree->Branch("cParticleName", &cParticleName, "cParticleName/C");
			// Particle Name

	fTree->Branch("iTrackID", iTrackID, "iTrackID[iRecordSize]/I"); // Track ID
	fTree->Branch("iParentID", iParentID, "iParentID[iRecordSize]/I");
			// Parent ID
	fTree->Branch("cCreatorProcess", &cCreatorProcess, "cCreatorProcess/C");
			// Creator process
        fTree->Branch("cStepProcess",&cStepProcess,"cStepProcess/C");
			// Step process
	fTree->Branch("fKEnergy_keV", fKEnergy_keV, "fKEnergy_keV[iRecordSize]/D");
			// Kinetic Energy keV
	fTree->Branch("fOptPhoWaveLength_nm", fOptPhoWaveLength_nm,
			"fOptPhoWaveLength_nm[iRecordSize]/D");
			// OptPhoton Wavelength nm
	fTree->Branch("fDirectionX", fDirectionX, "fDirectionX[iRecordSize]/D");
			// Particle Direction X
	fTree->Branch("fDirectionY", fDirectionY, "fDirectionY[iRecordSize]/D");
			// Particle Direction Y
	fTree->Branch("fDirectionZ", fDirectionZ, "fDirectionZ[iRecordSize]/D");
			// Particle Direction Z
	fTree->Branch("fEnergyDep_keV", fEnergyDep_keV,
			"fEnergyDep_keV[iRecordSize]/D");
			// Energy Deposition keV
	fTree->Branch("fPositionX_mm", fPositionX_mm,
			"fPositionX_mm[iRecordSize]/D");
			// Particle Position X_mm
	fTree->Branch("fPositionY_mm", fPositionY_mm,
			"fPositionY_mm[iRecordSize]/D");
			// Particle Position Y_mm
	fTree->Branch("fPositionZ_mm", fPositionZ_mm,
			"fPositionZ_mm[iRecordSize]/D");
			// Particle Position Z_mm
	fTree->Branch("fStepTime", fStepTime, "fStepTime[iRecordSize]/D");
			// Step Time
	fTree->Branch("fCharge", fCharge, "fCharge[iRecordSize]/D");
	                // Charge
	fTree->Branch("fTotEDep", &fTotEnergyDep_keV, "TotEDep/D");
			// Total energy deposition in the volume/event 
	fTree->Branch("iTotOptNum", &iTotalOptPhotNumber, "TotOptNum/I");
			// Total number of optical photons in the volume 
	fTree->Branch("iTotThermElecNum", &iTotalThermElecNumber, "TotThermElecNum/I");
			// Total number of thermal electrons in the volume

	struct datalevel {
		int stepNumber;
		int particleID;
		int trackID;
		int parentID;
		double particleEnergy;
		double particleDirection[3];
		double energyDeposition;
		double position[3];
		double stepTime;
	  double charge;
	} data;

	int recordLevel;
	int optPhotRecordLevel;
	int thermElecRecordLevel;
	int Size1, Size2, Size3;
	
	fin.read((char *)(&iNumRecords), sizeof(int));
	if( DEBUGGING ) cout<<"numRecords= "<<iNumRecords<<endl;

	fin.read((char *)(&Size1),sizeof(int));
	productionTime = new char [Size1+1];
	fin.read((char *)(productionTime),Size1);
	productionTime[Size1] = '\0';

	fin.read((char *)(&Size1),sizeof(int));
	geant4Version = new char [Size1+1];
	fin.read((char *)(geant4Version),Size1);
	geant4Version[Size1] = '\0';

	fin.read((char *)(&Size1),sizeof(int));
	svnVersion = new char [Size1+1];
	fin.read((char *)(svnVersion),Size1);
	svnVersion[Size1] = '\0';
	int svnVersion_int = atoi(svnVersion+10);
	//bool has_emission_time = (svnVersion_int > 606) ? true : false;

	fin.read((char *)(&Size1),sizeof(int));
	uName = new char [Size1+1];
	fin.read((char *)(uName),Size1);
	uName[Size1] = '\0';

    fin.read((char *)(&randomSeed),sizeof(int));
    fin.read((char *)(&numPrimaries),sizeof(int));	

	fin.read((char *)(&Size1),sizeof(int));
	InputCommands = new char [Size1+1];
	fin.read((char *)(InputCommands),Size1);
	InputCommands[Size1] = '\0';

	fin.read((char *)(&Size1),sizeof(int));
	diffs = new char [Size1+1];
	fin.read((char *)(diffs),Size1);
	diffs[Size1] = '\0';

	fin.read((char *)(&Size1),sizeof(int));
	DetCompo = new char [Size1+1];
	fin.read((char *)(DetCompo),Size1);
	DetCompo[Size1] = '\0';

	fTree0->Branch("iNumRecords", &iNumRecords, "iNumRecords/I");
			// number of Records
	fTree0->Branch("productionTime", productionTime, "sPT/C");
			// the time producting these data
	fTree0->Branch("geant4Version", geant4Version, "sG4V/C"); // Geant4 Version
	fTree0->Branch("svnVersion", svnVersion, "sSV/C");
			// SVN Version of simulation code 
	fTree0->Branch("uName", uName, "sUN/C"); // Name of the computer
    fTree0->Branch("randomSeed", &randomSeed, "iRandomSeed/I");
    fTree0->Branch("numPrimaries", &numPrimaries, "iNumPrimaries/I");
	fTree0->Branch("InputCommands", InputCommands, "sIC/C"); // Input Commands 
	fTree0->Branch("diffs", diffs, "sDI/C");
			// Code differences before/after modification 
	fTree0->Branch("DetCompo", DetCompo, "sDC/C");
			// Detector component lookup table 

	fTree0->Fill();
    
    //  Parse DetCompo to create a vector of volume names to act as a lookup
    //  table later in this converter
    string lookupTableStr = DetCompo;
    vector<string> componentLookupTable;
    string singleLine;
    int index = 1;
    while( lookupTableStr.length() ) {
        singleLine = lookupTableStr.substr( 0, lookupTableStr.find("\n") );
        singleLine = singleLine.substr( singleLine.find(": ")+2 );
        componentLookupTable.push_back( singleLine );
        lookupTableStr = lookupTableStr.substr( lookupTableStr.find("\n") + 1 );
    }
    

	if( DEBUGGING ) {
		cout<<"generationTime= "<<productionTime<<endl;
		cout<<"g4version= "<<geant4Version<<endl;
		cout<<"svnVersion= "<<svnVersion<<endl;
		cout<<"uName= "<<uName<<endl;
		cout<<"randomSeed= "<<randomSeed<<endl;
		cout<<"numPrimaries= "<<numPrimaries<<endl;
		cout<<"commands= "<<InputCommands<<endl;
		cout<<"diffs= "<<diffs<<endl;
		cout<<"component= "<<DetCompo<<endl;
	}
	delete[] productionTime;
	delete[] geant4Version;
	delete[] svnVersion;
	delete[] uName;
	delete[] InputCommands;
	delete[] diffs;
	delete[] DetCompo;

	for(int i=0; i<iNumRecords; i++) {
		fin.read((char *)(&iPrimaryParNum),sizeof(int));
		
		fPrimaryParEnergy_keV = new double[iPrimaryParNum];
		fPrimaryParTime_ns = new double[iPrimaryParNum];
		fPrimaryParPosX_mm = new double[iPrimaryParNum];	
		fPrimaryParPosY_mm = new double[iPrimaryParNum];
		fPrimaryParPosZ_mm = new double[iPrimaryParNum];
		fPrimaryParDirX = new double[iPrimaryParNum];
		fPrimaryParDirY = new double[iPrimaryParNum];
		fPrimaryParDirZ = new double[iPrimaryParNum];
		cPrimaryParName = new char[iPrimaryParNum*(kMaxCharacter +1) +2];
		cVolumeName = new char[kMaxCharacter+1];
		fTree->SetBranchAddress("fPrimaryParEnergy_keV", fPrimaryParEnergy_keV);
		fTree->SetBranchAddress("fPrimaryParTime_ns", fPrimaryParTime_ns);
		fTree->SetBranchAddress("fPrimaryParPosX_mm", fPrimaryParPosX_mm);
		fTree->SetBranchAddress("fPrimaryParPosY_mm", fPrimaryParPosY_mm);
		fTree->SetBranchAddress("fPrimaryParPosZ_mm", fPrimaryParPosZ_mm);
		fTree->SetBranchAddress("fPrimaryParDirX", fPrimaryParDirX);
		fTree->SetBranchAddress("fPrimaryParDirY", fPrimaryParDirY);
		fTree->SetBranchAddress("fPrimaryParDirZ", fPrimaryParDirZ);
		fTree->SetBranchAddress("cPrimaryParName", cPrimaryParName);
		fTree->SetBranchAddress("cVolumeName", cVolumeName);

		cPrimaryParNamePos = 0;
		for (int i = 0; i< iPrimaryParNum; i++){
			fin.read((char *)(&Size1),sizeof(int));
			strName = new char [Size1+1];
			particleName = new char [Size1+1];
			fin.read((char *)(particleName),Size1);
			strncpy(strName, particleName, Size1);
			if (Size1 > kMaxCharacter){
					cout << "WARNING: the particle name '" << particleName << "' has "
						 << "exceeded: " << kMaxCharacter << endl;
					cout<<"Truncating name...."<<endl;
					Size1 = kMaxCharacter;
			}
			particleName[Size1] = '&';
			strncpy(&(cPrimaryParName[cPrimaryParNamePos]), particleName,
					Size1+1);
			cPrimaryParNamePos += Size1+1;
			strName[Size1] = '\0';
			fin.read((char *)(&fPrimaryParEnergy_keV[i]),sizeof(double));
			fin.read((char *)(&fPrimaryParTime_ns[i]),sizeof(double));
			fin.read((char *)(&fPrimaryParPosX_mm[i]),sizeof(double));
			fin.read((char *)(&fPrimaryParPosY_mm[i]),sizeof(double));
			fin.read((char *)(&fPrimaryParPosZ_mm[i]),sizeof(double));
			fin.read((char *)(&fPrimaryParDirX[i]),sizeof(double));
			fin.read((char *)(&fPrimaryParDirY[i]),sizeof(double));
			fin.read((char *)(&fPrimaryParDirZ[i]),sizeof(double));
			if (DEBUGGING) {
				cout << "iPrimaryParNum, ParName, KEn, Time = "<< i <<", "<< strName<< ", "
				     << fPrimaryParEnergy_keV[i] << ", " << fPrimaryParTime_ns[i] << endl;
				cout << "Initial position, direction = (" << fPrimaryParPosX_mm[i] << ", "
				     << fPrimaryParPosY_mm[i] << ", " << fPrimaryParPosZ_mm[i] << "), ("
				     << fPrimaryParDirX[i] << ", " << fPrimaryParDirY[i] << ", "
				     << fPrimaryParDirZ[i] << ")" << endl;
            }
        }

		fin.read((char *)(&recordLevel),sizeof(int));
		fin.read((char *)(&optPhotRecordLevel),sizeof(int));
		fin.read((char *)(&thermElecRecordLevel),sizeof(int));
		fin.read((char *)(&iVolume),sizeof(int));
	        strncpy( cVolumeName, componentLookupTable[iVolume-1].c_str(),
		componentLookupTable[iVolume-1].length() );
		cVolumeName[componentLookupTable[iVolume-1].length()] = '\0';
		fin.read((char *)(&iEvtNb),sizeof(int));
		if (recordLevel>0) fin.read((char *)(&fTotEnergyDep_keV),
			sizeof(double));
		if (optPhotRecordLevel>0) fin.read((char *)(&iTotalOptPhotNumber),
			sizeof(int));
		if (thermElecRecordLevel>0) fin.read((char *)(&iTotalThermElecNumber),
			sizeof(int));
		fin.read((char *)(&iRecordSize),sizeof(int));
		iEvtNb += 1; //starts from 1
		iStepNum = new int[iRecordSize];
			iParticleID = new int[iRecordSize];
			iTrackID = new int[iRecordSize];
			iParentID = new int[iRecordSize];
			fKEnergy_keV = new double[iRecordSize]; 
			fOptPhoWaveLength_nm = new double[iRecordSize];
			fDirectionX = new double[iRecordSize]; 
			fDirectionY = new double[iRecordSize];
			fDirectionZ = new double[iRecordSize];
			fEnergyDep_keV = new double[iRecordSize];
			fPositionX_mm = new double[iRecordSize];
			fPositionY_mm = new double[iRecordSize];
			fPositionZ_mm = new double[iRecordSize];
			fStepTime = new double[iRecordSize];
			fCharge = new double[iRecordSize];
			cParticleName = new char[iRecordSize*(kMaxCharacter +1) +2];
			cCreatorProcess = new char[iRecordSize*(kMaxCharacter +1) +2];
			cStepProcess = new char[iRecordSize*(kMaxCharacter +1) +2];

			fTree->SetBranchAddress("iStepNum", iStepNum);
			fTree->SetBranchAddress("iParticleID",iParticleID);
			fTree->SetBranchAddress("iTrackID",iTrackID);
			fTree->SetBranchAddress("iParentID",iParentID);
			fTree->SetBranchAddress("fKEnergy_keV",fKEnergy_keV);
			fTree->SetBranchAddress("fOptPhoWaveLength_nm", fOptPhoWaveLength_nm);
			fTree->SetBranchAddress("fDirectionX",fDirectionX);
			fTree->SetBranchAddress("fDirectionY",fDirectionY);
			fTree->SetBranchAddress("fDirectionZ",fDirectionZ);
			fTree->SetBranchAddress("fEnergyDep_keV",fEnergyDep_keV);
			fTree->SetBranchAddress("fPositionX_mm",fPositionX_mm);
			fTree->SetBranchAddress("fPositionY_mm",fPositionY_mm);
			fTree->SetBranchAddress("fPositionZ_mm",fPositionZ_mm);
			fTree->SetBranchAddress("fStepTime",fStepTime);
			fTree->SetBranchAddress("fCharge",fCharge);
			fTree->SetBranchAddress("cParticleName",cParticleName);
			fTree->SetBranchAddress("cCreatorProcess",cCreatorProcess);
			fTree->SetBranchAddress("cStepProcess",cStepProcess);
	
		if( DEBUGGING ) {
			cout << "RecordLevel = " << recordLevel << endl;
			cout << "OptRecordLevel = " << optPhotRecordLevel << endl;
			cout << "ThermElecRecordLevel = " << thermElecRecordLevel << endl;
			cout << "volume, evtN, RecordSize = "<< iVolume << ", " << iEvtNb
				 << ", " << iRecordSize << endl;
			cout << "TotEdep, TotalOptPhotNumber, TotalThermElecNumber= " << fTotEnergyDep_keV 
				 <<", "<<iTotalOptPhotNumber<<", "<< iTotalThermElecNumber<< "\n" << endl;
		}

		cParticleNamePos = cCreatorProcessPos = cStepProcessPos = 0;	
		for( int i = 0; i<iRecordSize; i++ ) {
			fin.read((char *)(&Size1),sizeof(int));
		 	strName = new char [Size1+1];			
			particleName = new char [Size1+1];
			fin.read((char *)(particleName),Size1);
			strncpy(strName, particleName, Size1);
			if (Size1 > kMaxCharacter){
				cout << "WARNING: the particle name has exceeded: " << kMaxCharacter <<endl;
				cout<<"Truncating name...."<<endl;
				Size1 = kMaxCharacter;
			}
			particleName[Size1] = '&';
			strncpy(&(cParticleName[cParticleNamePos]), particleName, Size1+1);
			cParticleNamePos += Size1+1;
			strName[Size1] = '\0';
			
			fin.read((char *)(&Size2),sizeof(int));
			creatorProcName = new char[Size2+1];
			creatorProcessName = new char[Size2+1];
			fin.read((char *)(creatorProcessName), Size2 );
			strncpy(creatorProcName, creatorProcessName, Size2 );
			if( Size2 > kMaxCharacter ) {
				cout << "WARNING: the creator process name has exceeded: "
					 << kMaxCharacter << endl;
				cout << "Truncating process name..." << endl;
				Size2 = kMaxCharacter;
			}
			creatorProcessName[Size2] = '&';
			strncpy(&(cCreatorProcess[cCreatorProcessPos]), creatorProcessName,
					Size2+1);
			cCreatorProcessPos += Size2+1;
			creatorProcName[Size2] = '\0';

	            fin.read((char *)(&Size3),sizeof(int));
	            stepProcName = new char[Size3+1];
	            stepProcessName = new char[Size3+1];
	            fin.read((char *)(stepProcessName), Size3 );
	            strncpy(stepProcName, stepProcessName, Size3 );
	            if( Size3 > kMaxCharacter ) {
	                cout << "WARNING: the step process name has exceeded: "
	                << kMaxCharacter << endl;
	                cout << "Truncating process name..." << endl;
	                Size3 = kMaxCharacter;
	            }
	            stepProcessName[Size3] = '&';
	            strncpy(&(cStepProcess[cStepProcessPos]), stepProcessName,
	                    Size3+1);
	            cStepProcessPos += Size3+1;
	            stepProcName[Size3] = '\0';

			fin.read((char *)(&data),sizeof(data));
			iStepNum[i] = (data.stepNumber);
			iParticleID[i] = (data.particleID);
			iTrackID[i] = (data.trackID);
			iParentID[i] = (data.parentID);
			fKEnergy_keV[i] = (data.particleEnergy);
			if (string(strName) == "opticalphoton") {
				if (data.particleEnergy > 0 ){
					fOptPhoWaveLength_nm[i] = (1.24/data.particleEnergy);
				}
			}else{
				fOptPhoWaveLength_nm[i] = (0.);
			}
			fDirectionX[i] = (data.particleDirection[0]);
			fDirectionY[i] = (data.particleDirection[1]);
			fDirectionZ[i] = (data.particleDirection[2]);
			fEnergyDep_keV[i] = (data.energyDeposition);
			fPositionX_mm[i] = (10.*data.position[0]);
			fPositionY_mm[i] = (10.*data.position[1]);
			fPositionZ_mm[i] = (10.*data.position[2]);
			fStepTime[i] = (data.stepTime);
			fCharge[i] = (data.charge);
	
			if( DEBUGGING ) {
				cout<<"i = "<<i<<endl;
				cout<<"particleName, size= "<<strName<<"\t"<<Size1<<endl;
				cout<<"creatorProcessName, size= "<<creatorProcName<<"\t"<<Size2<<endl;
				cout<<"stepProcessName, size= "<<stepProcName<<"\t"<<Size2<<endl;
				cout<<"stepNumber= "<<iStepNum[i]<<endl;
				cout<<"particleID= "<<iParticleID[i]<<endl;
				cout<<"trackID= "<<iTrackID[i]<<endl;
				cout<<"parentID= "<<iParentID[i]<<endl;
				cout<<"particleEnergy= "<<fKEnergy_keV[i]<<endl;
				cout<<"fOptPhoWaveLength_nm= "<<fOptPhoWaveLength_nm[i]<<endl;
				cout<<"particleDirection= "<<fDirectionX[i];
				cout<<", "<<fDirectionY[i];
				cout<<", "<<fDirectionZ[i]<<endl;
				cout<<"fEnergyDep_keV= "<<fEnergyDep_keV[i]<<endl;
				cout<<"position= "<<fPositionX_mm[i];
				cout<<", "<<fPositionY_mm[i];
				cout<<", "<<fPositionZ_mm[i]<<endl;
				cout<<"stepTime = "<<fStepTime[i]<<endl;
				cout<<"charge = "<<fCharge[i]<<endl;
				cout<<endl<<endl;
			}

			delete[] particleName;
			delete[] strName;
			delete[] creatorProcessName;
			delete[] creatorProcName;
			delete[] stepProcessName;
			delete[] stepProcName;
		}	
		if (iRecordSize == 0) {
			cParticleName[cParticleNamePos] = ' ';
			cParticleNamePos++;
			cCreatorProcess[cCreatorProcessPos] = ' ';
			cCreatorProcessPos++;
			cStepProcess[cStepProcessPos] = ' ';
			cStepProcessPos++;
		}
		cPrimaryParName[cPrimaryParNamePos] = '\0';			
		cParticleName[cParticleNamePos]='\0';
		cCreatorProcess[cCreatorProcessPos]='\0';
		cStepProcess[cStepProcessPos]='\0';
		fTree->Fill();

		delete[] fPrimaryParEnergy_keV;
		delete[] fPrimaryParTime_ns;
		delete[] fPrimaryParPosX_mm;
		delete[] fPrimaryParPosY_mm;
		delete[] fPrimaryParPosZ_mm;
		delete[] fPrimaryParDirX;
		delete[] fPrimaryParDirY;
		delete[] fPrimaryParDirZ;
		delete[] cPrimaryParName; 
		delete[] cVolumeName;

		delete[] iStepNum;
		delete[] iParticleID;
		delete[] iTrackID;
		delete[] iParentID;
		delete[] fKEnergy_keV; 
		delete[] fOptPhoWaveLength_nm;
		delete[] fDirectionX; 
		delete[] fDirectionY;
		delete[] fDirectionZ;
		delete[] fEnergyDep_keV;
		delete[] fPositionX_mm;
		delete[] fPositionY_mm;
		delete[] fPositionZ_mm;
		delete[] fStepTime;
		delete[] fCharge;
		delete[] cParticleName;
		delete[] cCreatorProcess;
		delete[] cStepProcess;
	}
	
	fFile->cd();
	fTree->Write();
	fTree0->Write();
	fFile->Close();
	delete fFile;
	fin.close();
}