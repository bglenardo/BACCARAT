////////////////////////////////////////////////////////////////////////////////
/*	BaccOutput.cc

This is the code file to control the Bacc output. This output is solely to a 
general-purpose binary format, and should never be geared specifically toward 
either ROOT or Matlab. There will be separate projects to create ROOT- and 
Matlab-based readers for this binary format.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/09/08 - Time stamp now records in local time rather than GMT (Brian)
  2016/02/09 - Records a string if not under Git or SVN control so that the
               reader doesn't get confused (Kareem)
  2017/02/21 - Added clauses for record level 5 (TD & AC)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	C/C++ includes
//
#include <sstream>

//
//	Bacc includes
//
#include "BaccOutput.hh"
#include "BaccManager.hh"
#include "BaccDetectorComponent.hh"
#include "G4Version.hh"
#include "G4SystemOfUnits.hh"

#define DEBUGGING 0
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccOutput()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccOutput::BaccOutput()
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );
	
	std::stringstream RandSeed, TimeDate;
	stringstream TempName1;
	G4String SeedStr, TempName, TempNameTmp, OutDir, TempName2,
			 TempName3;
	char* OutName, * OutNameTmp;

	OutDir = baccManager->GetOutputDir();	 //get output directory
	if( OutDir.substr( OutDir.length() - 1, 1 ) == "/" )
	  OutDir = OutDir.substr( 0, OutDir.length() - 1 );
	struct stat st;
	if ( stat(OutDir.c_str(), &st) == -1 ) mkdir(OutDir.c_str(), 0777);	//	check, if not exist,
													//	create the folder
	
	// Create file name with the random number in it
	RandSeed << baccManager->GetRandomSeed();
	SeedStr = RandSeed.str();

	if( (baccManager->GetOutputName().length() > 0) &&
			(baccManager->GetOutputName() != "0") )
		TempName = OutDir + "/" + baccManager->GetOutputName() + SeedStr +".bin";
	else 
		TempName = OutDir + "/BaccOut" + SeedStr + ".bin";

	TempNameTmp = TempName + ".tmp"; // set name 
	OutName = new char[TempName.length()+1];
	TempName.copy(OutName,TempName.length(),0);
	OutName[TempName.length()]='\0';
	OutNameTmp = new char[TempNameTmp.length()+1];
	TempNameTmp.copy(OutNameTmp,TempNameTmp.length(),0);
	OutNameTmp[TempNameTmp.length()]='\0';
	
	// Set global file name
	fName = TempName;
	
	fBaccOutput.open(OutNameTmp, ios::out | ios::binary);
	delete[] OutName;
	
	// Set record size placeholder
	int placeholder = 0;
	fBaccOutput.write((char *)(&placeholder), sizeof(int));

	struct tm *gm;
	time_t t;
	char timeBuffer[20];
	t = time(NULL);
	gm = localtime(&t);						   //find production time
	strftime( timeBuffer, 20, "%Z", gm );
	TimeDate << asctime(gm);
	G4String gmt_head = timeBuffer;
	gmt_head += ": ";
	GMT = gmt_head + TimeDate.str();
	Size = GMT.length();
	fBaccOutput.write((char *)(&Size), sizeof(int));
	fBaccOutput.write((char *)(GMT.c_str()), Size);	

	G4Ver = G4Version;								// find G4 Version
	G4Ver = G4Ver.substr( G4Ver.find("Name:") + 6 );
	G4Ver = G4Ver.substr( 0, G4Ver.find(" $") );
	Size = G4Ver.length();
	fBaccOutput.write((char *)(&Size), sizeof(int));
	fBaccOutput.write((char *)(G4Ver.c_str()), Size);

	char * temp1;
	char * temp2;
	ifstream is;

	if ( baccManager->GetIsSVNRepo() ) {	
          TempName = "/tmp/BaccInfo_" + SeedStr + ".txt";
          TempName1 << "svn info " << baccManager->GetCompilationDirectory() << " > "
          		 << TempName;
      //  TempName1 = "svn info > " + TempName;
          system(TempName1.str().c_str());
          is.open(TempName.c_str(), ios::binary );
          is.seekg (0, ios::end);
          Size = is.tellg();
          is.seekg (0, ios::beg);
          temp1 = new char [Size];
          is.read(temp1, Size);
          SimVer = temp1;
          SimVer = SimVer.substr(SimVer.find("Revision:"));  // find revision number 
          SimVer = SimVer.substr(0,13);
          Size = SimVer.length();
          fBaccOutput.write((char *)(&Size), sizeof(int));
          fBaccOutput.write((char *)(SimVer.c_str()), Size);	
          is.close();
          delete[] temp1;
	} else if ( baccManager->GetIsGitRepo() ) { 
          TempName = "/tmp/BaccInfo_" + SeedStr + ".txt";
          TempName1 << "git rev-parse HEAD " << " > "
          		 << TempName;
      //  TempName1 = "svn info > " + TempName;
          system(TempName1.str().c_str());
          is.open(TempName.c_str(), ios::binary );
          is.seekg (0, ios::end);
          Size = is.tellg();
          is.seekg (0, ios::beg);
          temp1 = new char [Size];
          is.read(temp1, Size);
          SimVer = temp1;
//          SimVer = SimVer.substr(SimVer.find("Revision:"));  // find revision number 
//          SimVer = SimVer.substr(0,13);
          Size = SimVer.length();
          fBaccOutput.write((char *)(&Size), sizeof(int));
          fBaccOutput.write((char *)(SimVer.c_str()), Size);	
          is.close();
          delete[] temp1;
    } else {
        SimVer = "Not under Git or SVN control";
        Size = SimVer.length();
        fBaccOutput.write((char *)(&Size), sizeof(int));
        fBaccOutput.write((char *)(SimVer.c_str()), Size);
    }


	TempName2 = "uname -n > " + TempName;
	system(TempName2.c_str());	  // find name of computer
	is.open(TempName, ios::binary);
	is.seekg (0, ios::end);
	Size = is.tellg();
	is.seekg (0, ios::beg);
	temp2 = new char [Size+1];
	is.read(temp2, Size);
	temp2[Size] = '\0';
	is.close();
	uname = temp2;
	Size = uname.length();
	fBaccOutput.write((char *)(&Size), sizeof(int));
	fBaccOutput.write((char *)(uname.c_str()), Size);
	delete[] temp2;
	TempName3 = "rm -f " + TempName;
	system(TempName3.c_str());

    //  Write out the random seed as an integer
    G4int randomSeed = baccManager->GetRandomSeed();
    fBaccOutput.write((char *)(&randomSeed), sizeof(int));
    
    //  Write out the number of primary particles
    G4int numPrimaries = baccManager->GetNumEvents();
    fBaccOutput.write((char *)(&numPrimaries), sizeof(int));


	numRecords = 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccOutput()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccOutput::~BaccOutput()
{
	fBaccOutput.seekp(0, std::ios_base::beg);
	fBaccOutput.write((char *)(&numRecords), sizeof(int));
	
	fBaccOutput.close();
	
	// We're done writing to the file -- remove the .tmp suffix if the run
	// ended cleanly.
	if( baccManager->GetRunEndedCleanly() ) {
		G4String OutDir = baccManager->GetOutputDir();
		G4String command = "mv " + fName + ".tmp " + fName + " ";
		system( command.c_str() );
		
		G4cout << "\nOutput saved to " << fName << G4endl << G4endl;
	} else
		G4cout << "\nRun did not end cleanly, file name remains " << fName
			   << ".tmp" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                                      RecordInputHistory()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccOutput::RecordInputHistory()
{
		// this part should be done before the beamOn
                commands = baccManager->GetInputCommands();
                Size = commands.length();
                fBaccOutput.write((char *)(&Size), sizeof(int));
                fBaccOutput.write((char *)(commands.c_str()), Size);
                fBaccOutput.flush();

                differ = baccManager->GetDiffs();
                Size = differ.length();
                fBaccOutput.write((char *)(&Size), sizeof(int));
                fBaccOutput.write((char *)(differ.c_str()), Size);
                fBaccOutput.flush();

                DetCompo = baccManager->GetDetectorComponentLookupTable();
                Size = DetCompo.length();
                fBaccOutput.write((char *)(&Size), sizeof(int));
                fBaccOutput.write((char *)(DetCompo.c_str()), Size);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					RecordEventByVolume()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccOutput::RecordEventByVolume( BaccDetectorComponent* component,
		G4int eventNum )
{
	//	Record level definitions
	//	0 - Do not record energy depositions (default)
	//	1 - Record only the total energy in the volume for each event, and allow full propagation
	//	2 - Record only the steps where energy was deposited, and allow full propagation
	//	3 - Record all steps, including steps where no energy was deposited, and allow full propagation
	//	4 - Track killed after first step, which is recorded if in the volume (includes creation step (step 0)). N.B. total energy in volume limited to total of all first steps
	//		The processing for this step is done within UserSteppingAction, but
	//		the record level still needs to be taken into account here.
        //      5 - Record only the total energy in the volume for each event AND creation step (step 0) of each particle AND allow full propagation.

	//	Optical photons level definitions
	//	0 - Don't record any info (default)
	//	1 - Record only the total number of optical photons entering the 
	//		volume AND kill the track after first step in the volume so the photons don't propagate
	//	2 - Record only the total number of optical photons entering the volume, 
	//		and allow full propagation
	//	3 - Record only the first step in the volume (with step0 if step=1),
	//		then kill the track. 
 	//	4 - Record all the info on the optical photons, 
	//		and allow full propagtion
        //      5 - Record only the total number of optical photons entering the volume AND creation step (step 0) of each optical photon AND
        //              allow full propagation.

        //      Thermal electrons level definitions
        //      0 - Don't record any info (default)
        //      1 - Record only the total number of thermal electrons entering the 
        //              volume AND kill the track after first step in volume so the electron don't propagate
        //      2 - Record only the total number of thermal electrons entering the volume, 
        //              and allow full propagation
        //      3 - Record only the first step in the volume (with step0 if step=1),
        //              then kill the track
        //      4 - Record all the info on the thermal electrons, but don't kill the 
        //              and allow full propagation
        //      5 - Record only the total number of thermal electrons entering the volume AND creation step (step 0) of each thermal electron AND
        //              allow full propagation.

	///////////////////////calculate record size

        std::vector<BaccManager::stepRecord> eventRecord =
                        component->GetEventRecord();

        std::vector<BaccManager::primaryParticleInfo> primaryPar =
                        baccManager->GetPrimaryParticles();

	optPhotRecordLevel = component->GetRecordLevelOptPhot();
        thermElecRecordLevel = component->GetRecordLevelThermElec();
        recordLevel = component->GetRecordLevel();

        totalVolumeEnergy = 0.;
        totalOptPhotNumber = 0;
	totalThermElecNumber = 0;
        G4int recordsize2 = 0;
        G4int recordsize3 = 0;
	G4int recordsize5 = 0;
	G4int optphotRecordsize5 = 0;
	G4int thermelecRecordsize5 = 0;
        for( G4int i=0; i<(G4int)eventRecord.size(); i++ ){
                totalVolumeEnergy += eventRecord[i].energyDeposition;
		
		if (eventRecord[i].particleName == "opticalphoton"){
		  totalOptPhotNumber ++;
		  if ( optPhotRecordLevel == 5 && eventRecord[i].stepNumber < 1 ) optphotRecordsize5 ++;
		
                }else if(eventRecord[i].particleName == "thermalelectron"){
		  totalThermElecNumber ++;
		  if ( thermElecRecordLevel == 5 && eventRecord[i].stepNumber < 1 ) thermelecRecordsize5 ++; 
			
		}else{
                        if ( eventRecord[i].energyDeposition > 0. ){
                                recordsize2 ++;
                        }
                        recordsize3 ++;
			if ( recordLevel == 5 && eventRecord[i].stepNumber < 1 ) recordsize5 ++;
                }
        }

        //      if primary record set to false and there is no energy depositon
        //      in the interested volume, no primary information recorded.
   if ( totalVolumeEnergy > 0 || component->GetRecordLevel() > 2 
				|| baccManager->GetAlwaysRecordPrimary()){	
        ++numRecords;
	////  Primary particle information
	primaryParSize = (int) primaryPar.size();

        fBaccOutput.write((char *)(&primaryParSize),sizeof(int));
	if ( DEBUGGING ) G4cout<< "\n primaryParSize = "<< primaryParSize <<G4endl;
	for (int idx = 0; idx < primaryParSize; idx++ ) {
		Size = primaryPar[idx].id.length();
		fBaccOutput.write((char *)(&Size),sizeof(int));
		primaryParName = primaryPar[idx].id;
		fBaccOutput.write((char *)(primaryParName.c_str()),Size);
		primaryParEnergy_keV = primaryPar[idx].energy / keV;
		fBaccOutput.write((char *)(&primaryParEnergy_keV),sizeof(double));		
		primaryParTime_ns = primaryPar[idx].time / ns;
		fBaccOutput.write((char *)(&primaryParTime_ns),sizeof(double));
		primaryParPos_mm[0] = primaryPar[idx].position[0] / mm;
		fBaccOutput.write((char *)(&primaryParPos_mm[0]),sizeof(double));	
		primaryParPos_mm[1] = primaryPar[idx].position[1] / mm;
		fBaccOutput.write((char *)(&primaryParPos_mm[1]),sizeof(double));
		primaryParPos_mm[2] = primaryPar[idx].position[2] / mm;
		fBaccOutput.write((char *)(&primaryParPos_mm[2]),sizeof(double));
		primaryParDir[0] = primaryPar[idx].direction[0];
		fBaccOutput.write((char *)(&primaryParDir[0]),sizeof(double));
		primaryParDir[1] = primaryPar[idx].direction[1];
		fBaccOutput.write((char *)(&primaryParDir[1]),sizeof(double));
		primaryParDir[2] = primaryPar[idx].direction[2];
		fBaccOutput.write((char *)(&primaryParDir[2]),sizeof(double));

		if( DEBUGGING ) {
			G4cout<<"primary_ID = "<< primaryParName <<G4endl;
			G4cout<<"primary_energy = "<< primaryParEnergy_keV <<" keV"
				  <<G4endl;
			G4cout<<"primary_time = "<< primaryParTime_ns<<" ns" <<G4endl;
			G4cout<<"primary_positionX = "<< primaryParPos_mm[0] <<" mm"
				  <<G4endl;
			G4cout<<"primary_positionY = "<< primaryParPos_mm[1] <<" mm"
				  <<G4endl;
			G4cout<<"primary_positionZ = "<< primaryParPos_mm[2] <<" mm"
				  <<G4endl;
			G4cout<<"primary_directionX = "<< primaryParDir[0] <<G4endl;
			G4cout<<"primary_directionY = "<< primaryParDir[1] <<G4endl;
			G4cout<<"primary_directionZ = "<< primaryParDir[2] <<G4endl;
		}

		// This block grabs the initial point of the highest track number whose parent is the
		// primary particle. The intent is to get the point where the primary absorbs
		std::vector<BaccDetectorComponent*> BaccComponents = baccManager->GetComponentsVec();
		G4int tempTrackID=0;
		for(G4int j=0; j<(G4int)BaccComponents.size(); j++){
			std::vector<BaccManager::stepRecord> tempEventRecord = BaccComponents[j]->GetEventRecord();
			for(G4int k=0; k<(G4int)tempEventRecord.size(); k++){
				if( tempEventRecord[k].parentID == 1 &&
					tempEventRecord[k].stepNumber == 0 && 
					tempEventRecord[k].trackID > tempTrackID ){
					primaryParAbsorptionVolume = (G4int)BaccComponents[j]->GetID();
						for(G4int l=0; l<3; l++) 
							primaryParAbsorptionPos_mm[l] = tempEventRecord[k].position[l];
						tempTrackID = tempEventRecord[k].trackID;
				}
			}
		}
	
		fBaccOutput.write((char *)(&primaryParAbsorptionVolume),sizeof(int));
		fBaccOutput.write((char *)(&primaryParAbsorptionPos_mm[0]),sizeof(double));
		fBaccOutput.write((char *)(&primaryParAbsorptionPos_mm[1]),sizeof(double));
		fBaccOutput.write((char *)(&primaryParAbsorptionPos_mm[2]),sizeof(double));

	}
	
	//	First handle information recording that is independent of the specific
	//	record level.
	fBaccOutput.write((char *)(&recordLevel),sizeof(int));
	fBaccOutput.write((char *)(&optPhotRecordLevel),sizeof(int));
        fBaccOutput.write((char *)(&thermElecRecordLevel),sizeof(int));
	volume = component->GetID();
	fBaccOutput.write((char *)(&volume),sizeof(int));
	fBaccOutput.write((char *)(&eventNum),sizeof(int));

	//	record steping information according to the specified record level
	//
	if( recordLevel>0) fBaccOutput.write((char *)(&totalVolumeEnergy),
			sizeof(double));
	if( optPhotRecordLevel >0) fBaccOutput.write((char *)(&totalOptPhotNumber),
			sizeof(int));
        if( thermElecRecordLevel >0) fBaccOutput.write((char *)(&totalThermElecNumber),
                        sizeof(int));
	fBaccOutput.flush();
	if( DEBUGGING ) {
		G4cout << G4endl;
		G4cout << "OpticalLevel, thermElecLevel, recordLevel, volume, evtN, Edep, NOptPho, NthermEle= "
			   << optPhotRecordLevel<<", "<<thermElecRecordLevel<<", "<<recordLevel << ", "
			   <<volume << ", " << eventNum << ", " << totalVolumeEnergy
			   <<", "<<totalOptPhotNumber<<", "<<totalThermElecNumber <<G4endl;
	}
	//	Record Level 1
	recordSize = 0;
	//	Record Level 2
	if( recordLevel == 2){
	  recordSize = recordsize2;
	} else if ( recordLevel == 5) {
	  recordSize = recordsize5;
	} else if( recordLevel > 2 ){
	//	  Record Level 3
		recordSize = recordsize3;
	}

	if (optPhotRecordLevel == 5) { 
	  recordSize += optphotRecordsize5;
	} else if (optPhotRecordLevel > 2 ) recordSize += totalOptPhotNumber;
	
	if (thermElecRecordLevel == 5 ){
	  recordSize += thermelecRecordsize5;
	} else if (thermElecRecordLevel > 2 ) recordSize += totalThermElecNumber;

	fBaccOutput.write((char *)(&recordSize),sizeof(int));
	fBaccOutput.flush();



	if( recordSize > 0 ) {
		for( G4int i=0; i<(G4int)eventRecord.size(); i++ ) {
			if( ( (eventRecord[i].particleName != "opticalphoton") && 
				(eventRecord[i].particleName != "thermalelectron") &&
				  ( (eventRecord[i].energyDeposition > 0 && recordLevel == 2) ||
				    recordLevel == 3 || 
                                    recordLevel == 4 || 
                                    (recordLevel == 5 && eventRecord[i].stepNumber < 1) ) ) ||
			    ( (optPhotRecordLevel == 3 || optPhotRecordLevel == 4 || 
                                (optPhotRecordLevel == 5 && eventRecord[i].stepNumber < 1)) &&
						eventRecord[i].particleName == "opticalphoton") ||
			    ((thermElecRecordLevel == 3 || thermElecRecordLevel == 4 
                               || (thermElecRecordLevel == 5 && eventRecord[i].stepNumber < 1)) &&
                                                eventRecord[i].particleName == "thermalelectron") ) {


				particleName = eventRecord[i].particleName;
				particleNameSize = particleName.length();
				fBaccOutput.write((char *)(&particleNameSize), sizeof(int));
				fBaccOutput.write((char *)(particleName.c_str()),
						particleNameSize);

				creatorProcess = eventRecord[i].creatorProcess;
				creatorProcessSize = creatorProcess.length();
				fBaccOutput.write((char *)(&creatorProcessSize), sizeof(int));
				fBaccOutput.write((char *)(creatorProcess.c_str()),
						creatorProcessSize);
				
				stepProcess = eventRecord[i].stepProcess;
				stepProcessSize = stepProcess.length();
				fBaccOutput.write((char *)(&stepProcessSize), sizeof(int));
				fBaccOutput.write((char *)(stepProcess.c_str()),
				stepProcessSize);
		
				data.stepNumber = eventRecord[i].stepNumber;
				data.particleID = eventRecord[i].particleID;
				data.trackID = eventRecord[i].trackID;
				data.parentID = eventRecord[i].parentID;
//				for(G4int j=0; j<(G4int)eventRecord.size(); j++){
//					if (eventRecord[j].trackID == data.trackID &&
//						eventRecord[j].stepNumber==0 ){
//						data.creationVolumeID = eventRecord[j].creationVolumeID;
//					}
//				}
				std::vector<BaccDetectorComponent*> BaccComponents = baccManager->GetComponentsVec();
				G4bool foundTrackStart=false;
				for(G4int j=0; j<(G4int)BaccComponents.size(); j++){
					std::vector<BaccManager::stepRecord> tempEventRecord = BaccComponents[j]->GetEventRecord();
					for(G4int k=0; k<(G4int)tempEventRecord.size(); k++){
						if( tempEventRecord[k].trackID == data.trackID &&
							tempEventRecord[k].stepNumber == 0 ){
							data.creationVolumeID = (G4int)BaccComponents[j]->GetID();
							for(G4int l=0; l<3; l++) data.creationPosition[l] = tempEventRecord[k].position[l];
							foundTrackStart = true;
						}
						if(foundTrackStart) break;	
					}
					if(foundTrackStart) break;
				}
				//data.creationVolumeID = eventRecord[i].creationVolumeID;
				data.particleEnergy = eventRecord[i].particleEnergy;
				data.particleDirection[0]=eventRecord[i].particleDirection[0];
				data.particleDirection[1]=eventRecord[i].particleDirection[1];
				data.particleDirection[2]=eventRecord[i].particleDirection[2];
				data.energyDeposition = eventRecord[i].energyDeposition;
				data.position[0]=eventRecord[i].position[0];
				data.position[1]=eventRecord[i].position[1];
				data.position[2]=eventRecord[i].position[2];
				data.stepTime= eventRecord[i].stepTime;
				data.charge = eventRecord[i].charge;
				
				fBaccOutput.write((char *)(&data),sizeof(data));


				fBaccOutput.flush();

				if (DEBUGGING) {
					G4cout << "sizeof(data) = " << sizeof(data) << G4endl;
					G4cout << "particleName, Size= " << particleName << ", "
						   << particleNameSize << G4endl;
					G4cout << "data.stepNumber= " << data.stepNumber << G4endl;
					G4cout << "data.particleID= " << data.particleID << G4endl;
					G4cout << "data.trackID= " << data.trackID << G4endl;
					G4cout << "data.parentID= " << data.parentID << G4endl;
					G4cout << "data.creationVolumeID= " << data.creationVolumeID << G4endl;
					G4cout << "data.creationPosition= "
						   << data.creationPosition[0] << ", "
						   << data.creationPosition[1] << ", "
						   << data.creationPosition[2] << G4endl;
					G4cout << "data.particleEnergy= " << data.particleEnergy
						   << G4endl;
					G4cout << "data.particleDirection= "
						   << data.particleDirection[0] << ", "
						   << data.particleDirection[1] << ", "
						   << data.particleDirection[2] << G4endl;
					G4cout << "data.energyDeposition= " << data.energyDeposition
						   << G4endl;
					G4cout << "data.position= " << data.position[0] << ", "
						   << data.position[1] << ", " << data.position[2]
						   << G4endl;
					G4cout << "data.stepTime= " << data.stepTime << G4endl;
					G4cout << "creatorProcess, Size= " << creatorProcess << ", "
						   << creatorProcessSize << G4endl;
					G4cout << G4endl << G4endl;
				}	
			}
		}
	}
	}
}
