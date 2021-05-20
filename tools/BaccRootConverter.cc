////////////////////////////////////////////////////////////////////////////////
/*	BaccRootConverter.cc
*
* A reader which converts a binary BACCARAT file into root file.
*
* This file was based on the original LUXRootReader.cc.
*
********************************************************************************
* Change log
*	20 Jan 2016 - Initial submission (Kareem Kazkaz)
*   23 Feb 2016 - Now #includes <algorithm> for compatability (Kareem)
*   14 Apr 2016 - Multiply the position by 10 to convert cm to mm (Kareem)
*   15 Aug 2016 - The primary particle positions did NOT need to be multiplied
*		  by 10. I put that back (Kareem)
*   13 Dec 2016 - Merged in final changes from LUXSim so that works with DER 
                  and LZAP: added photon charge dCharge and run number 
                  iRunNumber, sorting tracks, explicit TTree::Fill call, copy 
                  logic for updating event number (Jim Dobson)
    15 Dec 2016 - Reset iTotalOptPhotNumber, iTotalThermElecNumber, 
                  dTotalEnergyDep_keV for each event
*/
////////////////////////////////////////////////////////////////////////////////
 
//
//  Executable includes
//
#include "BaccRootConverterEvent.hh"

//
//	ROOT includes
//
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

//
//	C/C++ includes
//
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>

//
//	Definitions
//
#define DEBUGGING 0

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  GetStringFromInputFile()
//
//  Note that inputFile is declared here with global scope. That's to avoid
//  passing the pointer to GetStringFromInputFile every time we want to read in
//  a string from the input file.
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
ifstream *inputFile;
string GetStringFromInputFile()
{
    Int_t stringSize;
    
    inputFile->read( (Char_t *)(&stringSize), sizeof(Int_t) );
    Char_t *tmpChars;
    tmpChars = new Char_t[stringSize+1];
    inputFile->read( (Char_t *)(tmpChars), stringSize );
    tmpChars[stringSize] = '\0';
    
    string tmpString = tmpChars;
    
    delete [] tmpChars;
    
    return tmpString;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  GetDoubleFromInputFile()
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
Double_t GetDoubleFromInputFile()
{
    Double_t tmpDouble;
    inputFile->read( (Char_t *)(&tmpDouble), sizeof(Double_t) );
    return tmpDouble;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  GetIntFromInputFile()
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
Int_t GetIntFromInputFile()
{
    Int_t tmpInt;
    inputFile->read( (Char_t *)(&tmpInt), sizeof(Int_t) );
    return tmpInt;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  CompareSteps()
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
Bool_t CompareSteps( stepInfo a, stepInfo b )
{
    return ( a.iStepNumber < b.iStepNumber );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  CompareTracks()
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
Bool_t CompareTracks( trackInfo a, trackInfo b )
{
    return ( a.iTrackID < b.iTrackID );
}

int getRunNumber(std::string Name) //This is fragile to the condition that the number in the file name is separated from the rest of the filename by a non-numeric character
{
   
   Name = Name.substr( 0, Name.find_last_of('.') );//gets rid of extension
   //now iterate from the back to find the first non-numeric character
   std::string number;
   for(std::string::reverse_iterator rit=Name.rbegin(); rit!=Name.rend(); ++rit)
   {
      if(!isdigit(*rit))
	 break;
      else
	 number.insert(0,1,*rit);
   }
   return atoi(number.c_str());

}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  main()
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
int main( int argc, char** argv ){
	
    //
    //  Set up the input file and ROOT output file
    //
    
	Char_t *filename = argv[1];
    inputFile = new ifstream( filename, ios::binary|ios::in );
	if(  !inputFile->is_open() ) {
		cout << "Couldn't find the file "<< filename << endl;
		exit( 0 );
	}
    
    string outfileString = filename;
    Int_t runNumber = getRunNumber(outfileString);
    outfileString = outfileString.substr( 0, outfileString.find_last_of('.') );
    outfileString += ".root";
    TFile *outputROOTFile = TFile::Open( outfileString.c_str(), "RECREATE" );
    outputROOTFile->cd();
    
    //
    //  Set up the header tree
    //
    
    //
    
    Int_t numRecords;
    
    string productionTime;
    string geantVersion;
    string repoVersion;
    string uName;
    Int_t randomSeed;
    Int_t numPrimaries;
    string inputCommands;
    string diffs;
    string detectorComponents;
    vector<string> componentLookupTable;

    TTree* headerTree = new TTree( "HeaderTree", "Header tree" );
    
    headerTree->Branch( "iNumRecords", &numRecords, "iNumRecords/I" );
    headerTree->Branch( "sProductionTime", &productionTime);
    headerTree->Branch( "sGeantVersion", &geantVersion);
    headerTree->Branch( "sRepoVersion", &repoVersion);
    headerTree->Branch( "sUname", &uName);
    headerTree->Branch( "sInputCommands", &inputCommands);
    headerTree->Branch( "sDiffs", &diffs);
    headerTree->Branch( "sDetectorComponents", &detectorComponents);
    headerTree->Branch("componentLookupTable",&componentLookupTable);
//
    //  Read in the header information
    //
    
    numRecords = GetIntFromInputFile();
    productionTime = GetStringFromInputFile();
    geantVersion = GetStringFromInputFile();
    repoVersion = GetStringFromInputFile();
    uName = GetStringFromInputFile();
    randomSeed=GetIntFromInputFile();
    numPrimaries=GetIntFromInputFile();
    inputCommands = GetStringFromInputFile();
    diffs = GetStringFromInputFile();
    detectorComponents = GetStringFromInputFile();
    
    if( DEBUGGING ) {
        cout << "numRecords= " << numRecords << endl;
        cout << "productionTime = " << productionTime << endl;
        cout << "geantVersion = " << geantVersion << endl;
        cout << "repoVersion = " << repoVersion << endl;
        cout << "uName = " << uName << endl;
        cout << "inputCommands = " << inputCommands << endl;
        cout << "diffs = " << diffs << endl;
        cout << "detectorComponents = " << detectorComponents << endl;
    }
    
    
    //  Parse detectorComponents to create a vector of volume names to act as a
    //  lookup table later in this converter
    string singleLine;
    while( detectorComponents.length() ) {
        singleLine = detectorComponents.substr( 0,
                detectorComponents.find("\n") );
        singleLine = singleLine.substr( singleLine.find(": ")+2 );
        componentLookupTable.push_back( singleLine );
        detectorComponents =
                detectorComponents.substr( detectorComponents.find("\n") + 1 );
    }
    
    headerTree->Fill();
    headerTree->Write();
    
    //
    //  Create, read in, and store the main data
    //
    TTree* dataTree = new TTree( "DataTree", "Data tree" );
    
    BaccRootConverterEvent *anEvent = new BaccRootConverterEvent();
    dataTree->Branch( "Event", "An event", anEvent );
    
    Int_t currentEvent = 0;
    Bool_t recordedPrimaryParticles = false;

	for( int i=0; i<numRecords; i++ ) {
    
        if( !(i%5000) )
            cout << "Processing " << i << " of " << numRecords << endl;
        
        Int_t numberOfPrimaryParticles = GetIntFromInputFile();
        
        //
        //  Primary particles
        //
        vector<primaryParticleInfo> primaryParticles;
        primaryParticleInfo aPrimaryParticle;

        for( Int_t j=0; j<numberOfPrimaryParticles; j++ ) {
        
            aPrimaryParticle.sName = GetStringFromInputFile();
            aPrimaryParticle.dEnergy_keV = GetDoubleFromInputFile();
            aPrimaryParticle.dTime_ns = GetDoubleFromInputFile();
            for( Int_t k=0; k<3; k++ )
                aPrimaryParticle.dPosition_mm[k] = GetDoubleFromInputFile();
            for( Int_t k=0; k<3; k++ )
                aPrimaryParticle.dDirection[k] = GetDoubleFromInputFile();
            aPrimaryParticle.sVolumeName = "Not implemented yet";
            aPrimaryParticle.iVolumeID = -1;
            
            if( DEBUGGING ) {
                cout << "Primary particle " << j+1 << ":" << endl;
                cout << "\tname = " << aPrimaryParticle.sName << endl;
                cout << "\tenergy_keV = " << aPrimaryParticle.dEnergy_keV
                     << endl;
                cout << "\ttime_ns = " << aPrimaryParticle.dTime_ns << endl;
                cout << "\tposition_mm = ( "
                     << aPrimaryParticle.dPosition_mm[0] << ", "
                     << aPrimaryParticle.dPosition_mm[1] << ", "
                     << aPrimaryParticle.dPosition_mm[2] << " )" << endl;
                cout << "\tdirection = ( "
                     << aPrimaryParticle.dDirection[0] << ", "
                     << aPrimaryParticle.dDirection[1] << ", "
                     << aPrimaryParticle.dDirection[2] << " )" << endl;
                cout << "\tvolumeName = " << aPrimaryParticle.sVolumeName
                     << endl;
                cout << "\tvolumeID = " << aPrimaryParticle.iVolumeID << endl;
            }
            
            primaryParticles.push_back( aPrimaryParticle );
        }
        
        //
        //  The rest of the information
        //
        
        Int_t recordLevel = GetIntFromInputFile();
        Int_t optPhotRecordLevel = GetIntFromInputFile();
        Int_t thermElecRecordLevel = GetIntFromInputFile();
        
        Int_t volumeID = GetIntFromInputFile();
        string volumeName = componentLookupTable[volumeID-1];
        
        //  For this next line, note that the events start their numbering at 1
        Int_t eventNumber = GetIntFromInputFile();
        if( DEBUGGING )
            cout << "eventNumber, checking if still in same event " << eventNumber << endl;
        
        //  Check to see if we're in a new event. If so, record the previous
        //  event and clear everything out to get ready for this new event.
        Bool_t newEvent = true;
        if( currentEvent == eventNumber )
            newEvent = false;
        
        if( newEvent ) {
        
            //  Go through all the tracks and put the steps in number order
            for( Int_t j=0; j<anEvent->tracks.size(); j++ ) {
                stable_sort( anEvent->tracks[j].steps.begin(),
                        anEvent->tracks[j].steps.end(), CompareSteps );
            }
            
            //  Now sort the tracks in increasing Track ID
            stable_sort( anEvent->tracks.begin(), anEvent->tracks.end(),
                    CompareTracks );
            
            //  Record the data and reset what needs resetting
	    anEvent->iEventNumber=eventNumber-1;
	    anEvent->iRunNumber=runNumber;
            dataTree->Fill();
            anEvent->ClearEverything();
            recordedPrimaryParticles = false;
            currentEvent = eventNumber;
        }

        if( !recordedPrimaryParticles ) {
            for( Int_t j=0; j<primaryParticles.size(); j++ )
                anEvent->primaryParticles.push_back( primaryParticles[j] );
            recordedPrimaryParticles = true;
        }

        //  Process the volume
        volumeInfo aVolume;
        
        aVolume.sName = volumeName;
        aVolume.iVolumeID = volumeID;
        aVolume.dTotalEnergyDep_keV = 0.0;
        if( recordLevel > 0 )
            aVolume.dTotalEnergyDep_keV = GetDoubleFromInputFile();
        aVolume.iTotalOptPhotNumber = 0;
        if( optPhotRecordLevel  > 0 )
            aVolume.iTotalOptPhotNumber = GetIntFromInputFile();
        aVolume.iTotalThermElecNumber = 0;
        if( thermElecRecordLevel  > 0 )
            aVolume.iTotalThermElecNumber = GetIntFromInputFile();
        
        Bool_t foundVolume = false;
        for( Int_t j=0; j<anEvent->volumes.size(); j++ ) {
            if( anEvent->volumes[j].iVolumeID == aVolume.iVolumeID ) {
                foundVolume = true;
                j = anEvent->volumes.size();
            }
        }
        
        if( !foundVolume )
            anEvent->volumes.push_back( aVolume );

        if( DEBUGGING ) {
            cout << "recordLevel = " << recordLevel << endl;
            cout << "optPhotRecordLevel = " << optPhotRecordLevel << endl;
            cout << "thermElecRecordLevel = " << thermElecRecordLevel << endl;
            cout << "volumeID = " << volumeID << endl;
            cout << "volumeName = " << volumeName << endl;
            cout << "aVolume.dTotalEnergyDep_keV = "
                 << aVolume.dTotalEnergyDep_keV << endl;
            cout << "aVolume.iTotalOptPhotNumber = "
                 << aVolume.iTotalOptPhotNumber << endl;
            cout << "aVolume.iTotalThermElecNumber = "
                 << aVolume.iTotalThermElecNumber << endl;
        }
        
        //  Now process all the individual steps, if there are any.

        Int_t recordSize = GetIntFromInputFile();
        
        if( DEBUGGING )
            cout << "recordSize = " << recordSize << endl;

        trackInfo aTrack;

        for( Int_t j=0; j<recordSize; j++ ) {
            stepInfo aStep;
            
            aStep.iVolumeID = volumeID;
            aStep.sVolumeName = volumeName;
            
            aTrack.sParticleName = GetStringFromInputFile();
            aTrack.sCreatorProcess = GetStringFromInputFile();
            aStep.sProcess = GetStringFromInputFile();
            
            double size_read = 0.;
            aStep.iStepNumber = GetIntFromInputFile(); size_read += sizeof(Int_t);
            aTrack.iParticleID = GetIntFromInputFile(); size_read += sizeof(Int_t);
            aTrack.iTrackID = GetIntFromInputFile(); size_read += sizeof(Int_t);
            aTrack.iParentID = GetIntFromInputFile(); size_read += sizeof(Int_t);
            aTrack.iCreationVolumeID = GetIntFromInputFile(); size_read += sizeof(Int_t);
            // For reasons that I don't understand, the binary data has four extra bytes
            // in between the creationVolumeID and the particleEnergy members. So I have
            // to do an extra "GetInt" call in order for things to line up properly when reading.
            Int_t dummy = GetIntFromInputFile();
            for( Int_t k=0; k<3; k++ ) 
                { aTrack.dCreationPosition[k] = GetDoubleFromInputFile(); size_read += sizeof(Double_t); }
            aStep.dParticleEnergy_keV = GetDoubleFromInputFile(); size_read += sizeof(Double_t);
            aTrack.dWavelength_nm = -1; 
            if( aTrack.sParticleName == "opticalphoton" &&
                    aStep.dParticleEnergy_keV > 0 )
                aTrack.dWavelength_nm = 1.24/aStep.dParticleEnergy_keV;
            for( Int_t k=0; k<3; k++ )
                { aStep.dDirection[k] = GetDoubleFromInputFile(); size_read += sizeof(Double_t); }
            aStep.dEnergyDep_keV = GetDoubleFromInputFile(); size_read += sizeof(Double_t);
            for( Int_t k=0; k<3; k++ )
                { aStep.dPosition_mm[k] = GetDoubleFromInputFile(); size_read += sizeof(Double_t); }
            aStep.dTime_ns = GetDoubleFromInputFile(); size_read += sizeof(Double_t);
            aTrack.dCharge = GetDoubleFromInputFile(); size_read += sizeof(Double_t);

            
            if( DEBUGGING ) {
                cout << "READ OUT " << size_read << " BITS FROM THE data OBJECT" << endl;
		cout << "sizeof(Int_t)= " << sizeof(Int_t) << endl;
		cout << "sizeof(Double_t)= " << sizeof(Double_t) << endl;
                cout << "j = " << j << endl;
                cout << "aTrack.sParticleName = " << aTrack.sParticleName
                     << endl;
                cout << "aTrack.sCreatorProcess = " << aTrack.sCreatorProcess
                     << endl;
                cout << "aStep.sProcess = " << aStep.sProcess << endl;
                cout << "aStep.iStepNumber = " << aStep.iStepNumber << endl;
                cout << "aTrack.iParticleID = " << aTrack.iParticleID << endl;
                cout << "aTrack.iTrackID = " << aTrack.iTrackID << endl;
                cout << "aTrack.iParentID = " << aTrack.iParentID << endl;
                cout << "aTrack.iCreationVolumeID = " << aTrack.iCreationVolumeID << endl;
                cout << "aTrack.dCreationPosition = ( "
                     << aTrack.dCreationPosition[0] << ", "
                     << aTrack.dCreationPosition[1] << ", "
                     << aTrack.dCreationPosition[2] << " )" << endl;
                cout << "aStep.dParticleEnergy_keV = "
                     << aStep.dParticleEnergy_keV << endl;
                cout << "aStep.dDirection = ( "
                     << aStep.dDirection[0] << ", "
                     << aStep.dDirection[1] << ", "
                     << aStep.dDirection[2] << " )" << endl;
                cout << "aStep.dEnergyDep_keV = " << aStep.dEnergyDep_keV
                     << endl;
                cout << "aStep.dPosition_mm = ( "
                     << aStep.dPosition_mm[0] << ", "
                     << aStep.dPosition_mm[1] << ", "
                     << aStep.dPosition_mm[2] << " )" << endl;
                cout << "aStep.dTime_ns = " << aStep.dTime_ns << endl;
            }
            
            //  At this point, the step is completely read in. We need to make
            //  sure the step is added to the correct track struct, and create
            //  a new track entry in the event if it doesn't exist.
            Int_t trackIndex = -1;
            for( Int_t k=0; k<anEvent->tracks.size(); k++ ) {
                if( anEvent->tracks[k].iTrackID == aTrack.iTrackID ) {
                    trackIndex = k;
                    k = anEvent->tracks.size();
                }
            }
            //  If this is a new track, add it to the vector
            if( trackIndex == -1 ) {
                anEvent->tracks.push_back( aTrack );
                trackIndex = anEvent->tracks.size() - 1;
            }
            anEvent->tracks[trackIndex].steps.push_back( aStep );
        }
	if(i==numRecords-1) { 
	   
	   //  Go through all the tracks and put the steps in number order
	   for( Int_t j=0; j<anEvent->tracks.size(); j++ ) {
	      stable_sort( anEvent->tracks[j].steps.begin(),
			   anEvent->tracks[j].steps.end(), CompareSteps );
	   }
	   //  Now sort the tracks in increasing Track ID
	   stable_sort( anEvent->tracks.begin(), anEvent->tracks.end(),
			CompareTracks );
	   //  Record the data and reset what needs resetting
	   anEvent->iEventNumber=eventNumber;
	   anEvent->iRunNumber=runNumber;
	   dataTree->Fill();
	   anEvent->ClearEverything();
	   recordedPrimaryParticles = false;
	   currentEvent = eventNumber;
        }
    }
	
	dataTree->Write();
    
	outputROOTFile->Close();
	delete outputROOTFile;
	inputFile->close();
    delete inputFile;
}
